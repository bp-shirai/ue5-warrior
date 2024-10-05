// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/WBaseCharacter.h"
#include "AbilitySystem/WAbilitySystemComponent.h"
#include "AbilitySystem/WAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "MotionWarpingComponent.h"

// Sets default values
AWBaseCharacter::AWBaseCharacter()
{
	PrimaryActorTick.bCanEverTick		   = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	

	WAbilitySystemComponent = CreateDefaultSubobject<UWAbilitySystemComponent>("WAbilitySystemComponent");

	WAttributeSet = CreateDefaultSubobject<UWAttributeSet>("WAttributeSet");

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarpingComponent");
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