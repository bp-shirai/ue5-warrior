// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/WGameplayAbility.h"
#include "AbilitySystem/WAbilitySystemComponent.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

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
	if (!ensure(CurrentActorInfo) || !ensure(CurrentActorInfo->AvatarActor.IsValid())) { return nullptr; }

	return CurrentActorInfo->AvatarActor.Get()->FindComponentByClass<UPawnCombatComponent>();
}

UWAbilitySystemComponent* UWGameplayAbility::GetWAbilitySystemComponentFromActorInfo() const
{
	UAbilitySystemComponent* AbilitySystemComponent = CurrentActorInfo ? CurrentActorInfo->AbilitySystemComponent.Get() : nullptr;
	ensure(AbilitySystemComponent);

	return Cast<UWAbilitySystemComponent>(AbilitySystemComponent);
}

FActiveGameplayEffectHandle UWGameplayAbility::NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle)
{
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	check(SourceASC && TargetASC && InSpecHandle.IsValid());

	return SourceASC->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data, TargetASC);
}

FActiveGameplayEffectHandle UWGameplayAbility::BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EWSuccessType& OutSuccessType)
{
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, InSpecHandle);

	OutSuccessType = ActiveGameplayEffectHandle.WasSuccessfullyApplied() ? EWSuccessType::Successful : EWSuccessType::Failed;
	
	return ActiveGameplayEffectHandle;
}