// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "WAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API UWAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	//~ Begin UAbilitySystemGlobals Interface
	virtual FGameplayAbilityActorInfo* AllocAbilityActorInfo() const;
	//~ End	UAbilitySystemGlobals Interface
};
