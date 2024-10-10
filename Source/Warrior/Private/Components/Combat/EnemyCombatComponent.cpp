// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Combat/EnemyCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "WGameplayTags.h"
#include "WFunctionLibrary.h"

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (OverlappedActors.Contains(HitActor)) return;

	OverlappedActors.AddUnique(HitActor);

	// TODO: Implement block check
	bool bIsValidBlock = false;

	const bool bIsPlayerBlocking	  = UWFunctionLibrary::DoesActorHaveTag(HitActor, WTags::Player_Status_Blocking);
	const bool bIsMyAttackUnblockable = false;

	if (bIsPlayerBlocking && !bIsMyAttackUnblockable)
	{
		// TODO: check if the block is valid
		bIsValidBlock = UWFunctionLibrary::IsValidBlock(GetOwningPawn(), HitActor);
	}

	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target	 = HitActor;

	if (bIsValidBlock)
	{
		// TODO:  Handle successful block
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, WTags::Player_Event_SuccessfulBlock, EventData);
	}
	else
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(), WTags::Shared_Event_MeleeHit, EventData);
	}
}

void UEnemyCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
}