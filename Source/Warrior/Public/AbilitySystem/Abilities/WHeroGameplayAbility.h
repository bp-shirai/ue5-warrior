// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystem/Abilities/WGameplayAbility.h"

#include "WHeroGameplayAbility.generated.h"

class AWHeroCharacter;
class AWHeroController;
class UHeroCombatComponent;

/**
 *
 */
UCLASS()
class WARRIOR_API UWHeroGameplayAbility : public UWGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	AWHeroCharacter* GetHeroCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	AWHeroController* GetHeroControllerFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	UHeroCombatComponent* GetHeroCombatComponentFromActorInfo();

private:
	TWeakObjectPtr<AWHeroCharacter> CachedHeroCharacter;
	TWeakObjectPtr<AWHeroController> CachedHeroController;
};
