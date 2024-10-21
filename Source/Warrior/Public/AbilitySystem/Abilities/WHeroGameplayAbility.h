// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystem/Abilities/WGameplayAbility.h"

#include "WHeroGameplayAbility.generated.h"

class AWHeroCharacter;
class AWHeroController;
class UHeroCombatComponent;
class UHeroUIComponent;

/**
 *
 */
UCLASS()
class WARRIOR_API UWHeroGameplayAbility : public UWGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	AWHeroCharacter* GetHeroCharacterFromActorInfo() const;

	UFUNCTION(BlueprintPure)
	AWHeroController* GetHeroControllerFromActorInfo() const;

	UFUNCTION(BlueprintPure)
	UHeroCombatComponent* GetHeroCombatComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure)
	UHeroUIComponent* GetHeroUIComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure)
	FGameplayEffectSpecHandle MakeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount) const;

	UFUNCTION(BlueprintPure)
	float GetCurrentEquippedWeaponDamageAtAbilityLevel() const;

	UFUNCTION(BlueprintCallable)
	bool GetAbilityRemainingCooldownByTag(FGameplayTag InCooldownTag, float& TotalCooldownTime, float& RemainingCooldownTime) const;
};
