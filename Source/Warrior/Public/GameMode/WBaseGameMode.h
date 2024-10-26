// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WTypes/WEnumTypes.h"
#include "WBaseGameMode.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class WARRIOR_API AWBaseGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AWBaseGameMode();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Game Settings")
	EWGameDifficulty CurrentGameDifficulty;

public:
	FORCEINLINE EWGameDifficulty GetCurrentGameDifficulty() const { return CurrentGameDifficulty; }
};
