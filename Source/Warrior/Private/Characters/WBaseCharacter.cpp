// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/WBaseCharacter.h"
#include "AbilitySystem/WAbilitySystemComponent.h"
#include "AbilitySystem/WAttributeSet.h"

// Sets default values
AWBaseCharacter::AWBaseCharacter()
{
	PrimaryActorTick.bCanEverTick		   = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;

	WAbilitySystemComponent = CreateDefaultSubobject<UWAbilitySystemComponent>("WAbilitySystemComponent");

	WAttributeSet = CreateDefaultSubobject<UWAttributeSet>("WAttributeSet");
}

void AWBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (WAbilitySystemComponent)
	{
		WAbilitySystemComponent->InitAbilityActorInfo(this, this);

		ensureMsgf(!CharacterStartUpData.IsNull(), TEXT("Forgot to assign start up data to %s"), *GetName());
	}
}

UAbilitySystemComponent* AWBaseCharacter::GetAbilitySystemComponent() const 
{ 
	return GetWAbilitySystemComponent(); 
}

UPawnCombatComponent* AWBaseCharacter::GetPawnCombatComponent() const
{
	return nullptr;
}

UPawnUIComponent* AWBaseCharacter::GetPawnUIComponent() const
{
	return nullptr;
}