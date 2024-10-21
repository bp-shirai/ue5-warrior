// Fill out your copyright notice in the Description page of Project Settings.

#include "WFunctionLibrary.h"
#include "AbilitySystem/WAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Interfaces/PawnCombatInterface.h"
#include "Controllers/WHeroController.h"
#include "Kismet/KismetMathLibrary.h"
#include "WGameplayTags.h"
#include "WTypes/WCountdownAction.h"

#include "WDebugHelper.h"

UWAbilitySystemComponent* UWFunctionLibrary::GetWarriorASCFromActor(const AActor* InActor)
{
	return Cast<UWAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(InActor));
}

void UWFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(InActor);
	ensure(ASC);

	if (ASC && ASC->HasMatchingGameplayTag(TagToAdd) == false)
	{
		ASC->AddLooseGameplayTag(TagToAdd);
	}
}

void UWFunctionLibrary::RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(InActor);
	ensure(ASC);

	if (ASC && ASC->HasMatchingGameplayTag(TagToRemove))
	{
		ASC->RemoveLooseGameplayTag(TagToRemove);
	}
}

bool UWFunctionLibrary::DoesActorHaveTag(const AActor* InActor, FGameplayTag TagToCheck)
{
	const UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(InActor);
	ensure(ASC);

	return ASC ? ASC->HasMatchingGameplayTag(TagToCheck) : false;
}

void UWFunctionLibrary::BP_DoesActorHaveTag(const AActor* InActor, FGameplayTag TagToCheck, EWConfirmType& OutConfirmType)
{
	if (!ensure(InActor))
	{
		OutConfirmType = EWConfirmType::No;
		return;
	}
	OutConfirmType = DoesActorHaveTag(InActor, TagToCheck) ? EWConfirmType::Yes : EWConfirmType::No;
}

UPawnCombatComponent* UWFunctionLibrary::GetPawnCombatComponent(const AActor* InActor)
{
	ensure(InActor);

	if (const IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
	{
		return PawnCombatInterface->GetPawnCombatComponent();
	}

	return nullptr;
}


UPawnCombatComponent* UWFunctionLibrary::BP_GetPawnCombatComponentFromActor(const AActor* InActor, EWValidType& OutValidType)
{
	UPawnCombatComponent* CombatComponent = GetPawnCombatComponent(InActor);

	OutValidType = CombatComponent ? EWValidType::Valid : EWValidType::Invalid;

	return CombatComponent;
}

bool UWFunctionLibrary::IsTargetPawnHostile(const APawn* QueryPawn, const APawn* TargetPawn)
{
	if (!ensure(QueryPawn && TargetPawn)) return false;

	const IGenericTeamAgentInterface* QueryTramAgent  = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
	const IGenericTeamAgentInterface* TargetTramAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

	if (QueryTramAgent && TargetTramAgent)
	{
		return QueryTramAgent->GetGenericTeamId() != TargetTramAgent->GetGenericTeamId();
	}

	return false;
}

float UWFunctionLibrary::GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevel)
{
	return InScalableFloat.GetValueAtLevel(InLevel);
}

FGameplayTag UWFunctionLibrary::ComputeHitReactDirectionTag(const AActor* InAttacker, const AActor* InVictim, float& OutAngleDifference)
{
	if (!ensure(InAttacker && InVictim)) return FGameplayTag::EmptyTag;

	const FVector VictimForward				= InVictim->GetActorForwardVector();
	const FVector VictimToAttackerNormalize = (InAttacker->GetActorLocation() - InVictim->GetActorLocation()).GetSafeNormal();

	const float DotResult = FVector::DotProduct(VictimForward, VictimToAttackerNormalize);
	OutAngleDifference	  = UKismetMathLibrary::DegAcos(DotResult);

	const FVector CrossResult = FVector::CrossProduct(VictimForward, VictimToAttackerNormalize);

	if (CrossResult.Z < 0.f)
	{
		OutAngleDifference *= -1.f;
	}

	if (OutAngleDifference >= -45.f && OutAngleDifference <= 45.f) { return WTags::Shared_Status_HitReact_Front; }
	else if (OutAngleDifference < -45.f && OutAngleDifference >= -135.f) { return WTags::Shared_Status_HitReact_Left; }
	else if (OutAngleDifference < -135.f || OutAngleDifference > 135.f) { return WTags::Shared_Status_HitReact_Back; }
	else if (OutAngleDifference > 45.f && OutAngleDifference <= 135.f) { return WTags::Shared_Status_HitReact_Right; }

	return WTags::Shared_Status_HitReact_Front;
}

bool UWFunctionLibrary::IsValidBlock(const AActor* InAttacker, const AActor* InDefender)
{
	if (!ensure(InAttacker && InDefender)) return false;

	const float DotResult = FVector::DotProduct(InAttacker->GetActorForwardVector(), InDefender->GetActorForwardVector());

	// const FString DebugString = FString::Printf(TEXT("Dot Result: %f %s"), DotResult, DotResult < -0.1f ? TEXT("Valid Block") : TEXT("Invalid Block"));
	// Debug::Print(DebugString, DotResult < -0.1f ? FColor::Green : FColor::Red);

	return DotResult < -0.1f;
}

bool UWFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator, AActor* InTargetActor, const FGameplayEffectSpecHandle& InSpecHandle)
{
	UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(InInstigator);
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(InTargetActor);

	FActiveGameplayEffectHandle ActiveHandle = SourceASC->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data, TargetASC);

	return ActiveHandle.WasSuccessfullyApplied();
}

void UWFunctionLibrary::CountDown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval, float& OutRemainingTime, EWCountdownActionInput CountdownInput, UPARAM(DisplayName = "Output") EWCountdownActionOutput& CountdownOutput, FLatentActionInfo LatentInfo)
{
	UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	if (!World) return;

	FLatentActionManager& LatentActionManager = World->GetLatentActionManager();

	FWCountdownAction* FoundAction = LatentActionManager.FindExistingAction<FWCountdownAction>(LatentInfo.CallbackTarget, LatentInfo.UUID);

	if (CountdownInput == EWCountdownActionInput::Start)
	{
		if (!FoundAction)
		{
			LatentActionManager.AddNewAction(
				LatentInfo.CallbackTarget,
				LatentInfo.UUID,
				new FWCountdownAction(TotalTime, UpdateInterval, OutRemainingTime, CountdownOutput, LatentInfo));
		}
	}

	if (CountdownInput == EWCountdownActionInput::Cancel)
	{
		if (FoundAction)
		{
			FoundAction->CancelAction();
		}
	}
}