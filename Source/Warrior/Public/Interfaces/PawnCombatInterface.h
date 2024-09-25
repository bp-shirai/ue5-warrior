// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PawnCombatInterface.generated.h"

class UPawnCombatComponent;


UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UPawnCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WARRIOR_API IPawnCombatInterface
{
	GENERATED_BODY()

public:
	virtual UPawnCombatComponent* GetPawnCombatComponent() const = 0;
};
