// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PawnUIInterface.generated.h"

class UPawnUIComponent;
class UHeroUIComponent;
class UEnemyUIComponent;

UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UPawnUIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class WARRIOR_API IPawnUIInterface
{
	GENERATED_BODY()

public:
	virtual UPawnUIComponent* GetPawnUIComponent() const = 0;

	virtual UHeroUIComponent* GetHeroUIComponent() const;

	virtual UEnemyUIComponent* GetEnemyUIComponent() const;
};
