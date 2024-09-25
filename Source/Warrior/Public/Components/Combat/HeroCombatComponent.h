// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Combat/PawnCombatComponent.h"
#include "HeroCombatComponent.generated.h"

class AWHeroWeapon;
/**
 *
 */
UCLASS()
class WARRIOR_API UHeroCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	AWHeroWeapon* GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag) const;

	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	AWHeroWeapon* GetHeroCurrentEquippedWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	float GetHeroCurrentEquipWeaponDamageAtLevel(float InLevel) const;

	virtual void OnHitTargetActor(AActor* HitActor) override;

	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor) override;
};
