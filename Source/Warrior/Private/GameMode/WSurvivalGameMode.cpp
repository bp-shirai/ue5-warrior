// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode/WSurvivalGameMode.h"
#include "Characters/WEnemyCharacter.h"
#include "Engine/AssetManager.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"

#include "WDebugHelper.h"

void AWSurvivalGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (!ensureMsgf(EnemyWaveSpawnerDataTable, TEXT("Forgot to assign a valid data table in survival game mode blueprint"))) return;

	SetCurrentSurvivalGameModeState(EWSurvivalGameModeState::WaitSpawnNewWave);

	TotalWaveSpawn = EnemyWaveSpawnerDataTable->GetRowNames().Num();

	PreLoadNextWaveEnemies();
}

void AWSurvivalGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!EnemyWaveSpawnerDataTable) return;

	if (CurrentGameModeState == EWSurvivalGameModeState::WaitSpawnNewWave)
	{
		TimePassedSinceStart += DeltaTime;

		if (TimePassedSinceStart >= SpawnNewWaveWaitTime)
		{
			TimePassedSinceStart = 0.f;
			SetCurrentSurvivalGameModeState(EWSurvivalGameModeState::SpawningNewWave);
		}
	}

	if (CurrentGameModeState == EWSurvivalGameModeState::SpawningNewWave)
	{
		TimePassedSinceStart += DeltaTime;

		if (TimePassedSinceStart >= SpawnEnemiesDelayTime)
		{
			// TODO: Handle spawn new enemies
			CurrentSpawnedEnemiesCounter += TrySpawnWaveEnemies();

			TimePassedSinceStart = 0.f;
			SetCurrentSurvivalGameModeState(EWSurvivalGameModeState::InProgress);
		}
	}

	if (CurrentGameModeState == EWSurvivalGameModeState::WaveCompleted)
	{
		TimePassedSinceStart += DeltaTime;

		if (TimePassedSinceStart >= WaveCompletedWaitTime)
		{
			TimePassedSinceStart = 0.f;

			CurrentWaveCount++;

			if (HasFinishedAllWaves())
			{
				SetCurrentSurvivalGameModeState(EWSurvivalGameModeState::AllWavesDone);
			}
			else
			{
				SetCurrentSurvivalGameModeState(EWSurvivalGameModeState::WaitSpawnNewWave);

				PreLoadNextWaveEnemies();
			}
		}
	}
}

void AWSurvivalGameMode::SetCurrentSurvivalGameModeState(EWSurvivalGameModeState InState)
{
	CurrentGameModeState = InState;

	OnSurvivalGameModeStateChanged.Broadcast(CurrentGameModeState);
}

bool AWSurvivalGameMode::HasFinishedAllWaves() const
{
	return CurrentWaveCount > TotalWaveSpawn;
}

void AWSurvivalGameMode::PreLoadNextWaveEnemies()
{
	if (HasFinishedAllWaves()) return;

	PreLoadedEnemyClassMap.Empty();

	for (const FWEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnerDefinitions)
	{
		if (SpawnerInfo.EnemyClassToSpawn.IsNull()) continue;

		UAssetManager::GetStreamableManager().RequestAsyncLoad(
			SpawnerInfo.EnemyClassToSpawn.ToSoftObjectPath(),
			FStreamableDelegate::CreateLambda(
				[SpawnerInfo, this]() {
					if (UClass* LoadedEnemyClass = SpawnerInfo.EnemyClassToSpawn.Get())
					{
						PreLoadedEnemyClassMap.Emplace(SpawnerInfo.EnemyClassToSpawn, LoadedEnemyClass);
						// Debug::Print(LoadedEnemyClass->GetName() + TEXT(" is loaded"));
					}
				}));
	}
}

const FWEnemyWaveSpawnerTableRow* AWSurvivalGameMode::GetCurrentWaveSpawnerTableRow() const
{
	const FName RowName = FName(TEXT("Wave") + FString::FromInt(CurrentWaveCount));

	const FWEnemyWaveSpawnerTableRow* FoundRow = EnemyWaveSpawnerDataTable->FindRow<FWEnemyWaveSpawnerTableRow>(RowName, FString());

	checkf(FoundRow, TEXT("Could not find a valid row under the name %s in the data table"), *RowName.ToString());

	return FoundRow;
}

int32 AWSurvivalGameMode::TrySpawnWaveEnemies()
{
	if (TargetPointsArray.IsEmpty())
	{
		UGameplayStatics::GetAllActorsOfClass(this, ATargetPoint::StaticClass(), TargetPointsArray);
	}

	if (!ensureMsgf(!TargetPointsArray.IsEmpty(), TEXT("No valid target point found in level : %s for spawning enemies"), *GetWorld()->GetName())) return 0;

	uint32 EnemiesSpawnedThisTime = 0;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (const FWEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnerDefinitions)
	{
		if (SpawnerInfo.EnemyClassToSpawn.IsNull()) continue;

		const int32 NumToSpawn = FMath::RandRange(SpawnerInfo.MinPerSpawnCount, SpawnerInfo.MaxPerSpawnCount);

		UClass* LoadedEnemyClass = PreLoadedEnemyClassMap.FindChecked(SpawnerInfo.EnemyClassToSpawn);

		for (int32 i = 0; i < NumToSpawn; i++)
		{
			const int32 RandomTargetPointIndex = FMath::RandRange(0, TargetPointsArray.Num() - 1);
			const AActor* TargetPoint		   = TargetPointsArray[RandomTargetPointIndex];
			const FVector SpawnOrigin		   = TargetPoint->GetActorLocation();
			const FRotator SpawnRotation	   = TargetPoint->GetActorForwardVector().ToOrientationRotator();

			FVector RandomLocation;
			UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(this, SpawnOrigin, RandomLocation, 400.f);

			RandomLocation += FVector(0.f, 0.f, 150.f);

			AWEnemyCharacter* SpawnedEnemy = GetWorld()->SpawnActor<AWEnemyCharacter>(LoadedEnemyClass, RandomLocation, SpawnRotation, SpawnParams);

			if (SpawnedEnemy)
			{
                SpawnedEnemy->OnDestroyed.AddUniqueDynamic(this, &ThisClass::OnEnemyDestroyed);

				EnemiesSpawnedThisTime++;
				TotalSpawnedEnemiesThisWaveCounter++;
			}

			if (!ShouldKeepSpawnEnemies())
			{
				return EnemiesSpawnedThisTime;
			}
		}
	}

	return EnemiesSpawnedThisTime;
}

bool AWSurvivalGameMode::ShouldKeepSpawnEnemies() const
{
	return TotalSpawnedEnemiesThisWaveCounter < GetCurrentWaveSpawnerTableRow()->TotalEnemyToSpawnThisWave;
}

void AWSurvivalGameMode::OnEnemyDestroyed(AActor* DestroyedActor)
{
    CurrentSpawnedEnemiesCounter--;

    if (ShouldKeepSpawnEnemies())
    {
        CurrentSpawnedEnemiesCounter += TrySpawnWaveEnemies();
    }
    else if (CurrentSpawnedEnemiesCounter == 0)
    {
        TotalSpawnedEnemiesThisWaveCounter = 0;
        CurrentSpawnedEnemiesCounter = 0;

        SetCurrentSurvivalGameModeState(EWSurvivalGameModeState::WaveCompleted);
    }
}

void AWSurvivalGameMode::RegisterSpawnedEnemies(const TArray<AWEnemyCharacter*>& InEnemiesToRegister)
{
	for (AWEnemyCharacter* SpawnedEnemy : InEnemiesToRegister)
	{
		if (SpawnedEnemy)
		{
			CurrentSpawnedEnemiesCounter++;

			SpawnedEnemy->OnDestroyed.AddUniqueDynamic(this, &ThisClass::OnEnemyDestroyed);
		}
	}
}