// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/WGameplayAbility.h"
#include "AbilitySystem/WAbilitySystemComponent.h"

void UWGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (AbilityActivationPolicy == EWAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo && !Spec.IsActive()) { ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle); }
	}
}

void UWGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (AbilityActivationPolicy == EWAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo) { ActorInfo->AbilitySystemComponent->ClearAbility(Handle); }
	}
}