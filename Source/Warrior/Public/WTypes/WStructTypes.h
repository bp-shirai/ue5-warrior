// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "WStructTypes.generated.h"

class UWHeroLinkedAnimLayer;
class UWGameplayAbility;
class UInputMappingContext;

USTRUCT(BlueprintType)
struct FWHeroAbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Input"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Input"))
	TSubclassOf<UWGameplayAbility> AbilityToGrant;

	bool IsValid() const; //{ return InputTag.IsValid() && AbilityToGrant; }
};

USTRUCT(BlueprintType)
struct FWHeroSpecialAbilitySet : public FWHeroAbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UMaterialInterface> AbilityIconMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="Player.Cooldown"))
	FGameplayTag AbilityCooldownTag;
};

USTRUCT(BlueprintType)
struct FWHeroWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWHeroLinkedAnimLayer> WeaponAnimLayerToLink;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputMappingContext> WeaponInputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FWHeroAbilitySet> DefaultWeaponAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FWHeroSpecialAbilitySet> SpecialWeaponAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat WeaponBaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> WeaponIconTexture;
};
