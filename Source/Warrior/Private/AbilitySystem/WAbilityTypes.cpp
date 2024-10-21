// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/WAbilityTypes.h"
#include "AbilitySystem/WAbilitySystemComponent.h"
#include "Characters/WHeroCharacter.h"
#include "Characters/WEnemyCharacter.h"
#include "Controllers/WHeroController.h"
#include "Controllers/WAIController.h"
// #include "Components/Combat/PawnCombatComponent.h"

void FWGameplayAbilityActorInfo::InitFromActor(AActor* InOwnerActor, AActor* InAvatarActor, UAbilitySystemComponent* InAbilitySystemComponent)
{
	Super::InitFromActor(InOwnerActor, InAvatarActor, InAbilitySystemComponent);

	WAbilitySystemComponent = Cast<UWAbilitySystemComponent>(InAbilitySystemComponent);

	EnemyCharacter = Cast<AWEnemyCharacter>(InAvatarActor);
	HeroCharacter  = Cast<AWHeroCharacter>(InAvatarActor);

	if (PlayerController.IsValid())
	{
		HeroController = Cast<AWHeroController>(PlayerController.Get());
		// HeroState = PlayerController->GetPlayerState<AWHeroState>();
	}
	else if (EnemyCharacter.IsValid())
	{
		AIController = EnemyCharacter->GetController<AWAIController>();
	}
}

void FWGameplayAbilityActorInfo::SetAvatarActor(AActor* InAvatarActor)
{
	Super::SetAvatarActor(InAvatarActor);
}

void FWGameplayAbilityActorInfo::ClearActorInfo()
{
	Super::ClearActorInfo();

	EnemyCharacter = nullptr;
	HeroCharacter  = nullptr;
	// HeroState = nullptr;
	HeroController			= nullptr;
	AIController			= nullptr;
	WAbilitySystemComponent = nullptr;
}

class UWAbilitySystemComponent* FWGameplayAbilityActorInfo::GetWAbilitySystemComponent() const
{
	return WAbilitySystemComponent.IsValid() ? WAbilitySystemComponent.Get() : nullptr;
}

class AWEnemyCharacter* FWGameplayAbilityActorInfo::GetEnemyCharacter() const
{
	return EnemyCharacter.IsValid() ? EnemyCharacter.Get() : nullptr;
}

class AWHeroCharacter* FWGameplayAbilityActorInfo::GetHeroCharacter() const
{
	return HeroCharacter.IsValid() ? HeroCharacter.Get() : nullptr;
}

class AWHeroController* FWGameplayAbilityActorInfo::GetHeroController() const
{
	return HeroController.IsValid() ? HeroController.Get() : nullptr;
}

class AWAIController* FWGameplayAbilityActorInfo::GetAIController() const
{
	return AIController.IsValid() ? AIController.Get() : nullptr;
}

// class AWHeroState* FWGameplayAbilityActorInfo::GetHeroState() const
// {
//     return HeroState.IsValid() ? HeroState.Get() : nullptr;
// }

// class AWWeapon* FWGameplayAbilityActorInfo::GetWeapon() const
// {
//     return GetHeroCharacter() ? GetHeroCharacter()->GetCurrentWeapon() : nullptr;
// }

// class AWWeaponRanged* FWGameplayAbilityActorInfo::GetWeaponRanged() const
// {
//     return GetHeroCharacter() ? Cast<AWWeaponRanged>(GetHeroCharacter()->GetCurrentWeapon()) : nullptr;
// }