// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/WEnemyCharacter.h"
#include "Components/Combat/EnemyCombatComponent.h"
#include "Components/UI/EnemyUIComponent.h"
#include "Components/WidgetComponent.h"
#include "DataAssets/StartUpData/DataAsset_EnemyStartUp.h"
#include "Engine/AssetManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Widgets/WUserWidgetBase.h"

#include "WDebugHelper.h"

AWEnemyCharacter::AWEnemyCharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll	= false;
	bUseControllerRotationRoll	= false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement	  = true;
	GetCharacterMovement()->RotationRate				  = FRotator(0.f, 180.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed				  = 300.f;
	GetCharacterMovement()->BrakingDecelerationWalking	  = 1000.f;

	EnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>("EnemyCombatComponent");

	EnemyUIComponent = CreateDefaultSubobject<UEnemyUIComponent>("EnemyUIComponent");

	EnemyHealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("EnemyHealthWidgetComponent");
	EnemyHealthWidgetComponent->SetupAttachment(GetMesh());
}

void AWEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (UWUserWidgetBase* HealthWidget = Cast<UWUserWidgetBase>(EnemyHealthWidgetComponent->GetUserWidgetObject()))
	{
		HealthWidget->InitEnemyCreatedWidget(this);
	}
}

void AWEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitEnemyStartUpData();
}

void AWEnemyCharacter::InitEnemyStartUpData()
{
	if (CharacterStartUpData.IsNull()) return;

	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		CharacterStartUpData.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda([this]() {
			if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.Get())
			{
				LoadedData->GiveToAbilitySystemComponent(WAbilitySystemComponent);
				// Debug::Print(TEXT("Enemy Start Up Data Loaded"), FColor::Green);
			}
		}));
}

UPawnCombatComponent* AWEnemyCharacter::GetPawnCombatComponent() const
{
	return EnemyCombatComponent;
}

UPawnUIComponent* AWEnemyCharacter::GetPawnUIComponent() const
{
	return EnemyUIComponent;
}