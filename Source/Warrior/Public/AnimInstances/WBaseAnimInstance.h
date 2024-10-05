// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "GameplayTagContainer.h"
#include "WBaseAnimInstance.generated.h"

/**
 *
 */
UCLASS(Abstract)
class WARRIOR_API UWBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	bool DoesOwningHaveTag(FGameplayTag TagToCheck) const;
};
