// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/WHeroGameplayAbility.h"
#include "Characters/WHeroCharacter.h"
#include "Controllers/WHeroController.h"
#include "Components/Combat/HeroCombatComponent.h"

AWHeroCharacter* UWHeroGameplayAbility::GetHeroCharacterFromActorInfo()
{
	if (!CachedHeroCharacter.IsValid() && CurrentActorInfo)
	{
		// Cached pointer
		CachedHeroCharacter = Cast<AWHeroCharacter>(CurrentActorInfo->AvatarActor);
	}
	return CachedHeroCharacter.IsValid() ? CachedHeroCharacter.Get() : nullptr;
}

AWHeroController* UWHeroGameplayAbility::GetHeroControllerFromActorInfo()
{
	if (!CachedHeroController.IsValid() && CurrentActorInfo)
	{
		//
		CachedHeroController = Cast<AWHeroController>(CurrentActorInfo->PlayerController);
	}
	return CachedHeroController.IsValid() ? CachedHeroController.Get() : nullptr;
}

UHeroCombatComponent* UWHeroGameplayAbility::GetHeroCombatComponentFromActorInfo()
{
    AWHeroCharacter* HeroCharacter =  GetHeroCharacterFromActorInfo();

    return HeroCharacter ? HeroCharacter->GetHeroCombatComponent() : nullptr;
}