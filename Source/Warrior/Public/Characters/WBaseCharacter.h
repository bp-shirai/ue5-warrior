// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WBaseCharacter.generated.h"

class UWAbilitySystemComponent;
class UWAttributeSet;

UCLASS(Abstract)
class WARRIOR_API AWBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AWBaseCharacter();

	//~ Begin IAbilitySystemInterface Interface.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;
	//~ End IAbilitySystemInterface Interface.

protected:
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	TObjectPtr<UWAbilitySystemComponent> WAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	TObjectPtr<UWAttributeSet> WAttributeSet;

public:
	FORCEINLINE UWAbilitySystemComponent* GetWAbilitySystemComponent() const { return WAbilitySystemComponent; }

	FORCEINLINE UWAttributeSet* GetWAttributeSet() const { return WAttributeSet; }
};
