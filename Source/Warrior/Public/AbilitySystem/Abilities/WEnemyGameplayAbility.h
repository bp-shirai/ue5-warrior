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
	UFUNCTION(BlueprintPure, Category="Warrior|Ability")
	AWEnemyCharacter* GetEnemyCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category="Warrior|Ability")
	UEnemyCombatComponent* GetEnemyCombatComponentFromActorInfo();

private:
	TWeakObjectPtr<AWEnemyCharacter> CachedEnemyCharacter;
};
