// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AnimInstances/WBaseAnimInstance.h"
#include "CoreMinimal.h"
#include "WHeroLinkedAnimLayer.generated.h"

class UWHeroAnimInstance;
/**
 *
 */
UCLASS(Abstract)
class WARRIOR_API UWHeroLinkedAnimLayer : public UWBaseAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	UWHeroAnimInstance* GetHeroAnimInstance() const;
};
