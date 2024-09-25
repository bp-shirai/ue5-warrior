// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"


class UWGameplayAbility;
class UWAbilitySystemComponent;
class UGameplayEffect;
/**
 *
 */
UCLASS()
class WARRIOR_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UWAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UWGameplayAbility>> ActivateOnGivenAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UWGameplayAbility>> ReactiveAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UGameplayEffect>> StartUpGameplayEffects;


	void GrantAbilities(const TArray<TSubclassOf<UWGameplayAbility>>& InAbilitiesToGive, UWAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);
};
