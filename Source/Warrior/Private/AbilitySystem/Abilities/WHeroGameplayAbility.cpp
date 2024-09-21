// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/WHeroGameplayAbility.h"
#include "Characters/WHeroCharacter.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "Controllers/WHeroController.h"

AWHeroCharacter* UWHeroGameplayAbility::GetHeroCharacterFromActorInfo()
{
	ensure(CurrentActorInfo);
	if (!CachedHeroCharacter.IsValid())
	{
		// Cached pointer
		CachedHeroCharacter = Cast<AWHeroCharacter>(CurrentActorInfo->AvatarActor);
	}
	return CachedHeroCharacter.IsValid() ? CachedHeroCharacter.Get() : nullptr;
}

AWHeroController* UWHeroGameplayAbility::GetHeroControllerFromActorInfo()
{
	ensure(CurrentActorInfo);
	if (!CachedHeroController.IsValid()) { CachedHeroController = Cast<AWHeroController>(CurrentActorInfo->PlayerController); }
	return CachedHeroController.IsValid() ? CachedHeroController.Get() : nullptr;
}

UHeroCombatComponent* UWHeroGameplayAbility::GetHeroCombatComponentFromActorInfo()
{
	AWHeroCharacter* HeroCharacter = GetHeroCharacterFromActorInfo();

	return HeroCharacter ? HeroCharacter->GetHeroCombatComponent() : nullptr;
}