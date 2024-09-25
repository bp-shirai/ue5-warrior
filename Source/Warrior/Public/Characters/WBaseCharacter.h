// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemInterface.h"
#include "Interfaces/PawnCombatInterface.h"
#include "GameFramework/Character.h"
#include "WBaseCharacter.generated.h"

class UWAbilitySystemComponent;
class UWAttributeSet;
class UDataAsset_StartUpDataBase;

UCLASS(Abstract)
class WARRIOR_API AWBaseCharacter : public ACharacter, public IAbilitySystemInterface, public IPawnCombatInterface
{
	GENERATED_BODY()

public:
	AWBaseCharacter();

	//~ Begin IAbilitySystemInterface Interface.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;
	//~ End IAbilitySystemInterface Interface.

	//~ Begin IPawnCombatInterface Interface.
	virtual UPawnCombatComponent* GetPawnCombatComponent() const;
	//~ End IPawnCombatInterface Interface.

protected:
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Warrior|AbilitySystem")
	TObjectPtr<UWAbilitySystemComponent> WAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Warrior|AbilitySystem")
	TObjectPtr<UWAttributeSet> WAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Warrior|CharacterData")
	TSoftObjectPtr<UDataAsset_StartUpDataBase> CharacterStartUpData;

public:
	FORCEINLINE UWAbilitySystemComponent* GetWAbilitySystemComponent() const { return WAbilitySystemComponent; }

	FORCEINLINE UWAttributeSet* GetWAttributeSet() const { return WAttributeSet; }
};
