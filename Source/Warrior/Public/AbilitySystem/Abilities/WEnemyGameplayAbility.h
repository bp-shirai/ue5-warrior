// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WGameplayAbility.h"
#include "WEnemyGameplayAbility.generated.h"

class AWEnemyCharacter;
class UEnemyCombatComponent;

/**
 *
 */
UCLASS()
class WARRIOR_API UWEnemyGameplayAbility : public UWGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	AWEnemyCharacter* GetEnemyCharacterFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	UEnemyCombatComponent* GetEnemyCombatComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	FGameplayEffectSpecHandle MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalableFloat) const;

private:
	// TWeakObjectPtr<AWEnemyCharacter> CachedEnemyCharacter;
};
