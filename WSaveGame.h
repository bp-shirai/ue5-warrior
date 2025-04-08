// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "WTypes/WEnumTypes.h"
#include "WSaveGame.generated.h"

/**
 *
 */
UCLASS()
class WARRIOR_API UWSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	EWGameDifficulty SavedCurrentGameDifficulty;
};