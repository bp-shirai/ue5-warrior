// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/WHeroCharacter.h"

#include "AbilitySystem/WAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "Components/Input/WInputComponent.h"
#include "Components/UI/HeroUIComponent.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "DataAssets/StartUpData/DataAsset_HeroStartUpData.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "WGameplayTags.h"
#include "GameMode/WBaseGameMode.h"

#include "AbilitySystemBlueprintLibrary.h"

#include "WDebugHelper.h"

AWHeroCharacter::AWHeroCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw	= false;
	bUseControllerRotationRoll	= false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength			= 200.f;
	CameraBoom->SocketOffset			= FVector(0.f, 55.f, 65.f);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement  = true;
	GetCharacterMovement()->RotationRate			   = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->BrakingDecelerationWalking = 400.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	HeroCombatComponent = CreateDefaultSubobject<UHeroCombatComponent>("HeroCombatComponent");

	HeroUIComponent = CreateDefaultSubobject<UHeroUIComponent>("HeroUIComponent");
}

void AWHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!CharacterStartUpData.IsNull())
	{
		if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.LoadSynchronous())
		{
			int32 AbilityApplyLevel = 1;

			if (const AWBaseGameMode* BaseGameMode = GetWorld()->GetAuthGameMode<AWBaseGameMode>())
			{
				switch (BaseGameMode->GetCurrentGameDifficulty())
				{
				case EWGameDifficulty::Easy: AbilityApplyLevel = 4; break;
				case EWGameDifficulty::Normal: AbilityApplyLevel = 3; break;
				case EWGameDifficulty::Hard: AbilityApplyLevel = 2; break;
				case EWGameDifficulty::VeryHard: AbilityApplyLevel = 1; break;
				}
			}

			// Grants an Abilities
			LoadedData->GiveToAbilitySystemComponent(WAbilitySystemComponent, AbilityApplyLevel);
		}
	}
}

void AWHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	checkf(InputConfigDataAsset, TEXT("Forgot to assign a valid data asset as input config"));

	const ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem);

	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);

	UWInputComponent* WInputComponent = CastChecked<UWInputComponent>(PlayerInputComponent);

	WInputComponent->BindNativeInputAction(InputConfigDataAsset, WTags::Input_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	WInputComponent->BindNativeInputAction(InputConfigDataAsset, WTags::Input_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);

	WInputComponent->BindNativeInputAction(InputConfigDataAsset, WTags::Input_SwitchTarget, ETriggerEvent::Triggered, this, &ThisClass::Input_SwitchTargetTriggered);
	WInputComponent->BindNativeInputAction(InputConfigDataAsset, WTags::Input_SwitchTarget, ETriggerEvent::Completed, this, &ThisClass::Input_SwitchTargetCompleted);

	WInputComponent->BindNativeInputAction(InputConfigDataAsset, WTags::Input_Pickup_Stones, ETriggerEvent::Started, this, &ThisClass::Input_PickupStonesStarted);

	WInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
}

void AWHeroCharacter::BeginPlay() { Super::BeginPlay(); }

void AWHeroCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

	if (MovementVector.Y != 0.f || MovementVector.X != 0.f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		const FVector RightDirection   = MovementRotation.RotateVector(FVector::RightVector);

		const FVector MovementDirection = (ForwardDirection * MovementVector.Y) + (RightDirection * MovementVector.X);
		// AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(MovementDirection, 1.f);
	}
}

void AWHeroCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (LookAxisVector.X != 0.f || LookAxisVector.Y != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AWHeroCharacter::Input_AbilityInputPressed(FGameplayTag InInputTag)
{
	// Debug::Print(InInputTag.ToString());
	if (!ensure(WAbilitySystemComponent)) return;
	WAbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}

void AWHeroCharacter::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
	if (!ensure(WAbilitySystemComponent)) return;
	WAbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}

void AWHeroCharacter::Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue)
{
	SwitchDirection = InputActionValue.Get<FVector2D>();
}

void AWHeroCharacter::Input_SwitchTargetCompleted(const FInputActionValue& InputActionValue)
{
	FGameplayEventData Data;
	FGameplayTag EventTag = SwitchDirection.X > 0.f ? WTags::Player_Event_SwitchTarget_Right : WTags::Player_Event_SwitchTarget_Left;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, EventTag, Data);
}

void AWHeroCharacter::Input_PickupStonesStarted(const FInputActionValue& InputActionValue)
{
	FGameplayEventData Data;
	FGameplayTag EventTag = WTags::Player_Event_ConsumeStones;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, EventTag, Data);
}

UPawnCombatComponent* AWHeroCharacter::GetPawnCombatComponent() const
{
	return HeroCombatComponent;
}

UPawnUIComponent* AWHeroCharacter::GetPawnUIComponent() const
{
	return HeroUIComponent;
}

UHeroUIComponent* AWHeroCharacter::GetHeroUIComponent() const
{
	return HeroUIComponent;
}