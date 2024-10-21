// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/WGameplayAbility.h"
#include "AbilitySystem/WAbilitySystemComponent.h"
#include "AbilitySystem/WAbilityTypes.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "Interfaces/PawnCombatInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "WFunctionLibrary.h"
#include "WGameplayTags.h"

void UWGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (AbilityActivationPolicy == EWAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo && !Spec.IsActive())
		{
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void UWGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (AbilityActivationPolicy == EWAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo)
		{
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}

UPawnCombatComponent* UWGameplayAbility::GetPawnCombatComponentFromActorInfo() const
{
	if (!ensure(CurrentActorInfo) || !ensure(CurrentActorInfo->AvatarActor.IsValid()))
	{
		return nullptr;
	}
	// return CurrentActorInfo->AvatarActor.Get()->FindComponentByClass<UPawnCombatComponent>();

	const auto* CombatInterface = Cast<IPawnCombatInterface>(CurrentActorInfo->AvatarActor.Get());
	return CombatInterface ? CombatInterface->GetPawnCombatComponent() : nullptr;
}

UWAbilitySystemComponent* UWGameplayAbility::GetWAbilitySystemComponentFromActorInfo() const
{
	//	UAbilitySystemComponent* AbilitySystemComponent = CurrentActorInfo ? CurrentActorInfo->AbilitySystemComponent.Get() : nullptr;
	//	ensure(AbilitySystemComponent);
	//	return Cast<UWAbilitySystemComponent>(AbilitySystemComponent);

	const FWGameplayAbilityActorInfo* ActorInfo = GetWActorInfo(CurrentActorInfo);

	return ActorInfo ? ActorInfo->WAbilitySystemComponent.Get() : nullptr;
}

FActiveGameplayEffectHandle UWGameplayAbility::ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle)
{
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);
	// UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	check(SourceASC && TargetASC && InSpecHandle.IsValid());

	return SourceASC->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data, TargetASC);
}

FActiveGameplayEffectHandle UWGameplayAbility::BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EWSuccessType& OutSuccessType)
{
	const FActiveGameplayEffectHandle ActiveEffectHandle = ApplyEffectSpecHandleToTarget(TargetActor, InSpecHandle);

	OutSuccessType = ActiveEffectHandle.WasSuccessfullyApplied() ? EWSuccessType::Successful : EWSuccessType::Failed;

	return ActiveEffectHandle;
}

const FWGameplayAbilityActorInfo* UWGameplayAbility::GetWActorInfo(const FGameplayAbilityActorInfo* InInfo) const
{
	return static_cast<const FWGameplayAbilityActorInfo*>(InInfo);
}

void UWGameplayAbility::ApplyEffectSpecHandleToHitResult(const FGameplayEffectSpecHandle& InSpecHandle, const TArray<FHitResult>& InHitResults)
{
	if (InHitResults.IsEmpty()) return;

	const APawn* OwningPawn = Cast<APawn>(GetAvatarActorFromActorInfo());

	for (const FHitResult& Hit : InHitResults)
	{
		if (APawn* HitPawn = Cast<APawn>(Hit.GetActor()))
		{
			if (UWFunctionLibrary::IsTargetPawnHostile(OwningPawn, HitPawn))
			{
				const FActiveGameplayEffectHandle ActiveEffectHandle = ApplyEffectSpecHandleToTarget(HitPawn, InSpecHandle);
				if (ActiveEffectHandle.WasSuccessfullyApplied())
				{
					FGameplayEventData Data;
					Data.Instigator = OwningPawn;
					Data.Target		= HitPawn;

					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitPawn, WTags::Shared_Event_HitReact, Data);
				}
			}
		}
	}
}