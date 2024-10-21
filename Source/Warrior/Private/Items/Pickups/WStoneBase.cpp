// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Pickups/WStoneBase.h"
#include "Characters/WHeroCharacter.h"
#include "AbilitySystem/WAbilitySystemComponent.h"
#include "WGameplayTags.h"

void AWStoneBase::OnCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AWHeroCharacter* OverlappedHeroCharacter = Cast<AWHeroCharacter>(OtherActor))
	{
		UWAbilitySystemComponent* TargetASC = OverlappedHeroCharacter->GetWAbilitySystemComponent();

		//TargetASC->TryActivateAbilityByTag(WTags::Player_Ability_Pickup_Stones);
		TargetASC->TryActivateAbilityByTagExact(WTags::Player_Ability_Pickup_Stones);
	}
}

void AWStoneBase::Consume(UAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel)
{
	if (!ensure(StoneGameplayEffectClass)) return;

	const UGameplayEffect* EffectCDO = StoneGameplayEffectClass->GetDefaultObject<UGameplayEffect>();

	AbilitySystemComponent->ApplyGameplayEffectToSelf(EffectCDO, ApplyLevel, AbilitySystemComponent->MakeEffectContext());

	BP_OnStoneConsumed();
}
