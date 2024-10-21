// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayAbilitySpecHandle.h"
#include "Items/Weapons/WWeaponBase.h"
#include "WTypes/WStructTypes.h"
#include "WHeroWeapon.generated.h"

/**
 *
 */
UCLASS()
class WARRIOR_API AWHeroWeapon : public AWWeaponBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapons")
	FWHeroWeaponData HeroWeaponData;

	UFUNCTION(BlueprintCallable)
	void AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles);

	UFUNCTION(BlueprintPure)
	TArray<FGameplayAbilitySpecHandle>  GetGrantedAbilitySpecHandles() const;

private:
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;
};
