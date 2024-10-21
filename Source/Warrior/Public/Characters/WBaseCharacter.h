// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemInterface.h"

#include "Interfaces/PawnCombatInterface.h"
#include "Interfaces/PawnUIInterface.h"

#include "GameFramework/Character.h"
#include "WBaseCharacter.generated.h"

class UWAbilitySystemComponent;
class UWAttributeSet;
class UDataAsset_StartUpDataBase;
class UMotionWarpingComponent;

UCLASS(Abstract)
class WARRIOR_API AWBaseCharacter : public ACharacter, public IAbilitySystemInterface, public IPawnCombatInterface, public IPawnUIInterface
{
	GENERATED_BODY()

public:
	AWBaseCharacter();

	//~ Begin IAbilitySystemInterface Interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface Interface

	//~ Begin IPawnCombatInterface Interface
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	//~ End IPawnCombatInterface Interface

	//~ Begin IPawnUIInterface Interface
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	//~ End IPawnUIInterface Interface

protected:
	//~ Begin APawn Interface
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default|AbilitySystem")
	TObjectPtr<UWAbilitySystemComponent> WAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default|AbilitySystem")
	TObjectPtr<UWAttributeSet> WAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default|CharacterData")
	TSoftObjectPtr<UDataAsset_StartUpDataBase> CharacterStartUpData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default|MotionWarping")
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;

public:
	FORCEINLINE UWAbilitySystemComponent* GetWAbilitySystemComponent() const { return WAbilitySystemComponent; }

	FORCEINLINE UWAttributeSet* GetWAttributeSet() const { return WAttributeSet; }
};
