// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Combat/EnemyCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "WGameplayTags.h"

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (OverlappedActors.Contains(HitActor)) return;

	OverlappedActors.AddUnique(HitActor);

	// TODO: Implement block check
	bool bIsValidBlock = false;

	const bool bIsPlayerBlocking	  = false;
	const bool bIsMyAttackUnblockable = false;

	if (bIsPlayerBlocking && !bIsMyAttackUnblockable)
	{
		// TODO: check if the block is valid
	}

	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target	 = HitActor;

	if (bIsValidBlock)
	{
		// TODO:  Handle successful block
	}
	else
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(), WTags::Shared_Event_MeleeHit, EventData);
	}
}

void UEnemyCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
}