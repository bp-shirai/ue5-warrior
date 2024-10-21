// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/HeroGameplayAbility_PickupStones.h"
#include "Characters/WHeroCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Items/Pickups/WStoneBase.h"
#include "Components/UI/HeroUIComponent.h"

void UHeroGameplayAbility_PickupStones::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	GetHeroUIComponentFromActorInfo()->OnStoneInteracted.Broadcast(true);

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UHeroGameplayAbility_PickupStones::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	GetHeroUIComponentFromActorInfo()->OnStoneInteracted.Broadcast(false);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroGameplayAbility_PickupStones::CollectStones()
{
	CollectedStones.Empty();

	const AWHeroCharacter* Hero				  = GetHeroCharacterFromActorInfo();
	const FVector Down						  = -Hero->GetActorUpVector();
	const FVector Start						  = Hero->GetActorLocation();
	const FVector End						  = Start + Down * BoxTraceDistance;
	const TArray<AActor*> ActorsToIgnore	  = TArray<AActor*>();
	const EDrawDebugTrace::Type DrawDebugType = bDrawDebugShape ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;
	TArray<FHitResult> TraceHits;

	UKismetSystemLibrary::BoxTraceMultiForObjects(Hero, Start, End, TraceBoxSize * 0.5f, Down.ToOrientationRotator(), StoneTraceChannel, false, ActorsToIgnore, DrawDebugType, TraceHits, true);

	for (const FHitResult& TraceHit : TraceHits)
	{
		if (AWStoneBase* FoundStone = Cast<AWStoneBase>(TraceHit.GetActor()))
		{
			CollectedStones.AddUnique(FoundStone);
		}
	}

	if (CollectedStones.IsEmpty())
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
	}
}

void UHeroGameplayAbility_PickupStones::ConsumeStones()
{
	if (CollectedStones.IsEmpty())
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
		return;
	}

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	const int32 AbilityLevel	 = GetAbilityLevel();

	for (AWStoneBase* CollectedStone : CollectedStones)
	{
		if (CollectedStone)
		{
			CollectedStone->Consume(ASC, AbilityLevel);
		}
	}
}