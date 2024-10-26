// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/WEnemyCharacter.h"
#include "Components/Combat/EnemyCombatComponent.h"
#include "Components/UI/EnemyUIComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "DataAssets/StartUpData/DataAsset_EnemyStartUp.h"
#include "Engine/AssetManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Widgets/WUserWidgetBase.h"
#include "WFunctionLibrary.h"
#include "GameMode/WBaseGameMode.h"

// #include "Components/Combat/WeaponMeshComponent.h"

#include "WDebugHelper.h"

AWEnemyCharacter::AWEnemyCharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw	= false;
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

	LeftHandCollisionBox = CreateDefaultSubobject<UBoxComponent>("LeftHandCollisionBox");
	LeftHandCollisionBox->SetupAttachment(GetMesh());
	LeftHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxBeginOverlap);

	RightHandCollisionBox = CreateDefaultSubobject<UBoxComponent>("RightHandCollisionBox");
	RightHandCollisionBox->SetupAttachment(GetMesh());
	RightHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxBeginOverlap);

	//	EnemyWeaponMeshComponent = CreateDefaultSubobject<UWeaponMeshComponent>("EnemyWeaponMesh");
	//	EnemyWeaponMeshComponent->SetupAttachment(GetMesh());
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

	int32 AbilityApplyLevel = 1;

	if (const AWBaseGameMode* BaseGameMode = GetWorld()->GetAuthGameMode<AWBaseGameMode>())
	{
		switch (BaseGameMode->GetCurrentGameDifficulty())
		{
		case EWGameDifficulty::Easy: AbilityApplyLevel = 1; break;
		case EWGameDifficulty::Normal: AbilityApplyLevel = 2; break;
		case EWGameDifficulty::Hard: AbilityApplyLevel = 3; break;
		case EWGameDifficulty::VeryHard: AbilityApplyLevel = 4; break;
		}
	}

	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		CharacterStartUpData.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[this, AbilityApplyLevel]() {
				if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.Get())
				{
					LoadedData->GiveToAbilitySystemComponent(WAbilitySystemComponent, AbilityApplyLevel);
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

void AWEnemyCharacter::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (UWFunctionLibrary::IsTargetPawnHostile(this, HitPawn))
		{
			EnemyCombatComponent->OnHitTargetActor(HitPawn);
		}
	}
}

#if WITH_EDITOR
void AWEnemyCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, LeftHandCollisionBoxAttachBoneName))
	{
		LeftHandCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, LeftHandCollisionBoxAttachBoneName);
	}

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, RightHandCollisionBoxAttachBoneName))
	{
		RightHandCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, RightHandCollisionBoxAttachBoneName);
	}
}
#endif
