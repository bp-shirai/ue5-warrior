// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/PawnExtensionComponentBase.h"

#include "GameplayTagContainer.h"
#include "PawnCombatComponent.generated.h"

class AWWeaponBase;

/**
 *
 */
UCLASS()
class WARRIOR_API UPawnCombatComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	void RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, AWWeaponBase* InWeaponToRegister, bool bResisterAsEquippedWeapon = false);

	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	AWWeaponBase* GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const;

	UPROPERTY(BlueprintReadWrite, Category = "Warrior|Combat")
	FGameplayTag CurrentEquippedWeaponTag;

	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	AWWeaponBase* GetCharacterCurrentEquippedWeapon() const;

private:
	TMap<FGameplayTag, AWWeaponBase*> CharacterCarriedWeaponMap;
};
