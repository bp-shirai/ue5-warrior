// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/PawnExtensionComponentBase.h"

#include "GameplayTagContainer.h"
#include "PawnCombatComponent.generated.h"

class AWWeaponBase;

UENUM(BlueprintType)
enum class EToggleDamageType : uint8
{
	CurrentEquippedWeapon,
	LeftHand,
	RightHand,
};

/**
 *
 */
UCLASS()
class WARRIOR_API UPawnCombatComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	void RegisterSpawnedWeapon(FGameplayTag InWeaponTag, AWWeaponBase* InWeapon, bool bResisterAsEquippedWeapon = false);

	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	AWWeaponBase* GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTag) const;

	UPROPERTY(BlueprintReadWrite, Category = "Warrior|Combat")
	FGameplayTag CurrentEquippedWeaponTag;

	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	AWWeaponBase* GetCharacterCurrentEquippedWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	void ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType = EToggleDamageType::CurrentEquippedWeapon);

	virtual void OnHitTargetActor(AActor* HitActor);

	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Warrior|Combat")
	bool bShowDebug;

protected:
	TArray<AActor*> OverlappedActors;


private:
	TMap<FGameplayTag, TObjectPtr<AWWeaponBase>> CharacterCarriedWeaponMap;
};
