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

	if (WAbilitySystemComponent) { WAbilitySystemComponent->InitAbilityActorInfo(this, this); }
}

UAbilitySystemComponent* AWBaseCharacter::GetAbilitySystemComponent() const { return GetWAbilitySystemComponent(); }