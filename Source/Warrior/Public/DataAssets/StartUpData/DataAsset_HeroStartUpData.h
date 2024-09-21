// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "WTypes/WStructTypes.h"
#include "DataAsset_HeroStartUpData.generated.h"


/**
 *
 */
UCLASS()
class WARRIOR_API UDataAsset_HeroStartUpData : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UWAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData", meta = (TitleProperty="InputTag"))
	TArray<FWHeroAbilitySet> HeroStartUpAbilitySets;
};
