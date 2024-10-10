// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/HeroGameplayAbility_TargetLock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Characters/WHeroCharacter.h"
#include "Controllers/WHeroController.h"
#include "Widgets/WUserWidgetBase.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/SizeBox.h"
#include "WFunctionLibrary.h"
#include "WGameplayTags.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"

#include "WDebugHelper.h"

void UHeroGameplayAbility_TargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	TryLockOnTarget();
	InitTargetLockMovement();
	InitTargetLockMappingContext();

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UHeroGameplayAbility_TargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ResetTargetLockMovement();
	ResetTargetLockMappingContext();
	CleanUp();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroGameplayAbility_TargetLock::TryLockOnTarget()
{
	GetAvailableActorsToLock();

	if (AvailableActorsToLock.IsEmpty())
	{
		CancelTargetLockAbility();
		return;
	}

	CurrentLockedActor = GetNearestTargetFromAvailableActors(AvailableActorsToLock);
	if (CurrentLockedActor.IsValid())
	{
		DrawTargetLockWidget();

		SetTargetLockeWidgetPosition();
	}
	else
	{
		CancelTargetLockAbility();
	}
}

void UHeroGameplayAbility_TargetLock::GetAvailableActorsToLock()
{
	AvailableActorsToLock.Empty();

	const AWHeroCharacter* Hero = GetHeroCharacterFromActorInfo();
	const FVector Forward		= Hero->GetActorForwardVector();
	const FVector Start			= Hero->GetActorLocation();
	const FVector End			= Start + Forward * BoxTraceDistance;
	const FVector HalfSize		= TraceBoxSize / 2.f;
	const FRotator Orientation	= Forward.ToOrientationRotator();
	const TArray<AActor*> ActorsToIgnore;
	const EDrawDebugTrace::Type DrawDebugType = bShowPersistentDebugShape ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
	TArray<FHitResult> BoxTraceHits;

	UKismetSystemLibrary::BoxTraceMultiForObjects(Hero, Start, End, HalfSize, Orientation, BoxTraceChannel, false, ActorsToIgnore, DrawDebugType, BoxTraceHits, true);

	for (const FHitResult& TraceHit : BoxTraceHits)
	{
		if (AActor* HitActor = TraceHit.GetActor())
		{
			if (HitActor != Hero)
			{
				AvailableActorsToLock.AddUnique(HitActor);
				// Debug::Print(HitActor->GetActorNameOrLabel());
			}
		}
	}
}

void UHeroGameplayAbility_TargetLock::CancelTargetLockAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}

void UHeroGameplayAbility_TargetLock::CleanUp()
{
	AvailableActorsToLock.Empty();

	CurrentLockedActor.Reset();

	if (DrawnTargetLockWidget)
	{
		DrawnTargetLockWidget->RemoveFromParent();
		DrawnTargetLockWidget = nullptr;
		TargetLockWidgetSize  = FVector2D::ZeroVector;
	}
}

AActor* UHeroGameplayAbility_TargetLock::GetNearestTargetFromAvailableActors(const TArray<AActor*>& InAvailableActors)
{
	const AWHeroCharacter* Hero = GetHeroCharacterFromActorInfo();
	float ClosestDistance		= 0.f;

	return UGameplayStatics::FindNearestActor(Hero->GetActorLocation(), InAvailableActors, ClosestDistance);
}

void UHeroGameplayAbility_TargetLock::DrawTargetLockWidget()
{
	if (!DrawnTargetLockWidget)
	{
		checkf(TargetLockWidgetClass, TEXT("Forgot to assign a valid widget class in Blueprint"));
		DrawnTargetLockWidget = CreateWidget<UWUserWidgetBase>(GetHeroControllerFromActorInfo(), TargetLockWidgetClass);
		DrawnTargetLockWidget->AddToViewport();
	}
}

void UHeroGameplayAbility_TargetLock::SetTargetLockeWidgetPosition()
{
	if (!DrawnTargetLockWidget || !CurrentLockedActor.IsValid())
	{
		CancelTargetLockAbility();
		return;
	}

	FVector2D ScreenPosition;
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(GetHeroControllerFromActorInfo(), CurrentLockedActor->GetActorLocation(), ScreenPosition, true);

	if (TargetLockWidgetSize == FVector2D::ZeroVector)
	{
		DrawnTargetLockWidget->WidgetTree->ForEachWidget(
			[this](UWidget* FoundWidget) {
				if (USizeBox* FoundSizeBox = Cast<USizeBox>(FoundWidget))
				{
					TargetLockWidgetSize.X = FoundSizeBox->GetWidthOverride();
					TargetLockWidgetSize.Y = FoundSizeBox->GetHeightOverride();
				}
			});
	}

	// ScreenPosition -= TargetLockWidgetSize / 2.f;
	ScreenPosition -= TargetLockWidgetSize * 0.5f;

	DrawnTargetLockWidget->SetPositionInViewport(ScreenPosition, false);
}

void UHeroGameplayAbility_TargetLock::OnTargetLockTick(float DeltaTime)
{
	AWHeroCharacter* Hero	  = GetHeroCharacterFromActorInfo();
	const AActor* LockedActor = CurrentLockedActor.Get();

	if (!LockedActor ||
		UWFunctionLibrary::DoesActorHaveTag(LockedActor, WTags::Shared_Status_Dead) ||
		UWFunctionLibrary::DoesActorHaveTag(Hero, WTags::Shared_Status_Dead))
	{
		CancelTargetLockAbility();
		return;
	}

	SetTargetLockeWidgetPosition();

	const bool bShouldOverrideRotation =
		!UWFunctionLibrary::DoesActorHaveTag(Hero, WTags::Player_Status_Rolling) &&
		!UWFunctionLibrary::DoesActorHaveTag(Hero, WTags::Player_Status_Blocking);

	if (bShouldOverrideRotation)
	{
		AWHeroController* HeroController = GetHeroControllerFromActorInfo();

		FRotator LookAtRot				 = UKismetMathLibrary::FindLookAtRotation(Hero->GetActorLocation(), LockedActor->GetActorLocation());
		const FRotator CurrentControlRot = HeroController->GetControlRotation();
		const FRotator TargetRot		 = FMath::RInterpTo(CurrentControlRot, LookAtRot, DeltaTime, TargetLockRotationInterpSpeed);

		LookAtRot -= FRotator(TargetLockCameraOffsetDistance, 0.f, 0.f);

		HeroController->SetControlRotation(FRotator(TargetRot.Pitch, TargetRot.Yaw, 0.f));
		Hero->SetActorRotation(FRotator(0.f, TargetRot.Yaw, 0.f));
	}
}

void UHeroGameplayAbility_TargetLock::InitTargetLockMovement()
{
	UCharacterMovementComponent* CMC = GetHeroCharacterFromActorInfo()->GetCharacterMovement();

	CachedDefaultMaxWalkSpeed = CMC->MaxWalkSpeed;
	CMC->MaxWalkSpeed		  = TargetLockMaxWalkSpeed;
}

void UHeroGameplayAbility_TargetLock::ResetTargetLockMovement()
{
	if (CachedDefaultMaxWalkSpeed > 0.f)
	{
		GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = CachedDefaultMaxWalkSpeed;
		CachedDefaultMaxWalkSpeed											  = 0.f;
	}
}

void UHeroGameplayAbility_TargetLock::InitTargetLockMappingContext()
{
	if (!GetHeroControllerFromActorInfo()) return;

	const ULocalPlayer* LocalPlayer				  = GetHeroControllerFromActorInfo()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(Subsystem)

		Subsystem->AddMappingContext(TargetLockMappingContext, 3);
}

void UHeroGameplayAbility_TargetLock::ResetTargetLockMappingContext()
{
	if (!GetHeroControllerFromActorInfo()) return;

	const ULocalPlayer* LocalPlayer				  = GetHeroControllerFromActorInfo()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(Subsystem)

		Subsystem->RemoveMappingContext(TargetLockMappingContext);
}

void UHeroGameplayAbility_TargetLock::SwitchTarget(const FGameplayTag& InSwitchDirectionTag)
{
	GetAvailableActorsToLock();

	TArray<AActor*> ActorsOnLeft;
	TArray<AActor*> ActorsOnRight;
	AActor* NewTargetToLock = nullptr;

	GetAvailableActorsAroundTarget(ActorsOnLeft, ActorsOnRight);

	if (InSwitchDirectionTag == WTags::Player_Event_SwitchTarget_Left)
	{
		NewTargetToLock = GetNearestTargetFromAvailableActors(ActorsOnLeft);
	}
	else if (InSwitchDirectionTag == WTags::Player_Event_SwitchTarget_Right)
	{
		NewTargetToLock = GetNearestTargetFromAvailableActors(ActorsOnRight);
	}

	if (NewTargetToLock)
	{
		CurrentLockedActor = NewTargetToLock;
	}
}

void UHeroGameplayAbility_TargetLock::GetAvailableActorsAroundTarget(TArray<AActor*>& OutActorsOnLeft, TArray<AActor*>& OutActorsRight)
{
	if (!CurrentLockedActor.IsValid() || AvailableActorsToLock.IsEmpty())
	{
		CancelTargetLockAbility();
		return;
	}

	const AActor* LockedActor				= CurrentLockedActor.Get();
	const FVector PlayerLocation			= GetHeroCharacterFromActorInfo()->GetActorLocation();
	const FVector PlayerToCurrentNormalized = (LockedActor->GetActorLocation() - PlayerLocation).GetSafeNormal();

	for (AActor* AvailableActor : AvailableActorsToLock)
	{
		if (!AvailableActor || AvailableActor == LockedActor) continue;

		const FVector PlayerToAvailableNormalized = (AvailableActor->GetActorLocation() - PlayerLocation).GetSafeNormal();

		const FVector CrossResult = FVector::CrossProduct(PlayerToCurrentNormalized, PlayerToAvailableNormalized);

		if (CrossResult.Z > 0.f)
		{
			OutActorsRight.AddUnique(AvailableActor);
		}
		else
		{
			OutActorsOnLeft.AddUnique(AvailableActor);
		}
	}
}