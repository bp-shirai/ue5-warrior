// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/AbilityTasks/AbilityTask_WaitSpawnEnemies.h"
#include "AbilitySystemComponent.h"
#include "Engine/AssetManager.h"
#include "NavigationSystem.h"
#include "Characters/WEnemyCharacter.h"
#include "Components/CapsuleComponent.h"

#include "WDebugHelper.h"

UAbilityTask_WaitSpawnEnemies* UAbilityTask_WaitSpawnEnemies::WaitSpawnEnemies(UGameplayAbility* OwningAbility, FGameplayTag EventTag, TSoftClassPtr<AWEnemyCharacter> SoftEnemyClassToSpawn, int32 NumToSpawn, const FVector& SpawnOrigin, float RandomSpawnRadius)
{
	UAbilityTask_WaitSpawnEnemies* Node = NewAbilityTask<UAbilityTask_WaitSpawnEnemies>(OwningAbility);
	Node->EventTag						= EventTag;
	Node->SoftEnemyClassToSpawn			= SoftEnemyClassToSpawn;
	Node->NumToSpawn					= NumToSpawn;
	Node->SpawnOrigin					= SpawnOrigin;
	Node->RandomSpawnRadius				= RandomSpawnRadius;

	return Node;
}

void UAbilityTask_WaitSpawnEnemies::Activate()
{
	AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(EventTag).AddUObject(this, &ThisClass::OnGameplayEventReceived);
}

void UAbilityTask_WaitSpawnEnemies::OnDestroy(bool bInOwnerFinished)
{
	if (DelegateHandle.IsValid())
	{
		AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(EventTag).Remove(DelegateHandle);
	}

	Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_WaitSpawnEnemies::OnGameplayEventReceived(const FGameplayEventData* InPayload)
{
	if (ensure(!SoftEnemyClassToSpawn.IsNull()))
	{
		UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
			SoftEnemyClassToSpawn.ToSoftObjectPath(),
			FStreamableDelegate::CreateUObject(this, &ThisClass::OnEnemyClassLoaded));
	}
	else
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			DidNotSpawn.Broadcast(TArray<AWEnemyCharacter*>());
		}

		EndTask();
	}
}

void UAbilityTask_WaitSpawnEnemies::OnEnemyClassLoaded()
{
	UClass* LoadedClass = SoftEnemyClassToSpawn.Get();
	UWorld* World		= GetWorld();

	if (!LoadedClass || !World)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			DidNotSpawn.Broadcast(TArray<AWEnemyCharacter*>());
		}
		EndTask();
		return;
	}

	TArray<AWEnemyCharacter*> SpawnedEnemies;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	const FRotator SpawnFacingRotation = AbilitySystemComponent->GetAvatarActor()->GetActorForwardVector().ToOrientationRotator();

	for (int32 i = 0; i < NumToSpawn; i++)
	{
		FVector RandomLocation;
		UNavigationSystemV1::K2_GetRandomReachablePointInRadius(this, SpawnOrigin, RandomLocation, RandomSpawnRadius);

		float HalfHeight = 150.f;
		if (LoadedClass->GetDefaultObject<AWEnemyCharacter>())
		{
			HalfHeight = LoadedClass->GetDefaultObject<AWEnemyCharacter>()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		}

		RandomLocation += FVector(0.f, 0.f, HalfHeight);

		AWEnemyCharacter* SpawnedEnemy = World->SpawnActor<AWEnemyCharacter>(LoadedClass, RandomLocation, SpawnFacingRotation, SpawnParams);

		if (SpawnedEnemy)
		{
			//SpawnedEnemy->SetHidden(true);
			SpawnedEnemies.Add(SpawnedEnemy);
		}
	}

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		if (!SpawnedEnemies.IsEmpty())
		{
			OnSpawnFinished.Broadcast(SpawnedEnemies);
		}
		else
		{
			DidNotSpawn.Broadcast(TArray<AWEnemyCharacter*>());
		}
	}

	EndTask();
}