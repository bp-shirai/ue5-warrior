// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemComponent.h"
#include "WTypes/WStructTypes.h"
#include "WAbilitySystemComponent.generated.h"



/**
 *
 */
UCLASS()
class WARRIOR_API UWAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void OnAbilityInputPressed(const FGameplayTag& InInputTag);
	void OnAbilityInputReleased(const FGameplayTag& InInputTag);

	UFUNCTION(BlueprintCallable, meta = (ApplyLevel = "1"))
	void GrantHeroWeaponAbilities(const TArray<FWHeroAbilitySet>& InDefaultWeaponAbilities, const TArray<FWHeroSpecialAbilitySet>& InSpecialWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles);

	UFUNCTION(BlueprintCallable)
	void RemovedGrantedHeroWeaponAbilities(UPARAM(ref) TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove);

	UFUNCTION(BlueprintCallable)
	bool TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate);

	UFUNCTION(BlueprintCallable)
	bool TryActivateAbilityByTagExact(FGameplayTag AbilityTagToActivate);


	FGameplayAbilitySpec* FindAbilitySpecFromTag(FGameplayTag Tag) const;
};
