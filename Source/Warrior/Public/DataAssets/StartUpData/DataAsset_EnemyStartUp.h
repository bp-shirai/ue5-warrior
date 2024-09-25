// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "DataAsset_EnemyStartUp.generated.h"


class UWEnemyGameplayAbility;

/**
 *
 */
UCLASS()
class WARRIOR_API UDataAsset_EnemyStartUp : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UWAbilitySystemComponent* InASC, int32 ApplyLevel = 1) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UWEnemyGameplayAbility>> EnemyCombatAbilities;
};
