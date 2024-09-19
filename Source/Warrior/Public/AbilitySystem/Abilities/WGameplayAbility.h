// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/GameplayAbility.h"

#include "WGameplayAbility.generated.h"

class UPawnCombatComponent;
class UWAbilitySystemComponent;

UENUM(BlueprintType)
enum class EWAbilityActivationPolicy : uint8
{
	OnTriggered,
	OnGiven,
};

/**
 *
 */
UCLASS(Abstract)
class WARRIOR_API UWGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	//~ Begin UGameplayAbility Interface.
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface

	UPROPERTY(EditDefaultsOnly, Category = "Warrior|Ability")
	EWAbilityActivationPolicy AbilityActivationPolicy = EWAbilityActivationPolicy::OnTriggered;

	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	UPawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	UWAbilitySystemComponent* GetWAbilitySystemComponentFromActorInfo() const;
};
