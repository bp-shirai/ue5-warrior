// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/WHeroGameplayAbility.h"
#include "AbilitySystem/WAbilitySystemComponent.h"
#include "AbilitySystem/WAbilityTypes.h"
#include "Characters/WHeroCharacter.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "Components/UI/HeroUIComponent.h"
#include "Controllers/WHeroController.h"

#include "WGameplayTags.h"

AWHeroCharacter* UWHeroGameplayAbility::GetHeroCharacterFromActorInfo() const
{
	const FWGameplayAbilityActorInfo* ActorInfo = GetWActorInfo(CurrentActorInfo);

	return ActorInfo ? ActorInfo->HeroCharacter.Get() : nullptr;
}

AWHeroController* UWHeroGameplayAbility::GetHeroControllerFromActorInfo() const
{
	const FWGameplayAbilityActorInfo* ActorInfo = GetWActorInfo(CurrentActorInfo);

	return ActorInfo ? ActorInfo->HeroController.Get() : nullptr;
}

UHeroCombatComponent* UWHeroGameplayAbility::GetHeroCombatComponentFromActorInfo() const
{
	const AWHeroCharacter* HeroCharacter = GetHeroCharacterFromActorInfo();

	return HeroCharacter ? HeroCharacter->GetHeroCombatComponent() : nullptr;
}

UHeroUIComponent* UWHeroGameplayAbility::GetHeroUIComponentFromActorInfo() const
{
	const AWHeroCharacter* HeroCharacter = GetHeroCharacterFromActorInfo();

	return HeroCharacter ? HeroCharacter->GetHeroUIComponent() : nullptr;
}

FGameplayEffectSpecHandle UWHeroGameplayAbility::MakeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount) const
{
	ensure(EffectClass);

	const UWAbilitySystemComponent* ASC = GetWAbilitySystemComponentFromActorInfo();
	AActor* AvatarActor					= GetAvatarActorFromActorInfo();

	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(AvatarActor);
	ContextHandle.AddInstigator(AvatarActor, AvatarActor);

	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(EffectClass, GetAbilityLevel(), ContextHandle);
	if (EffectSpecHandle.IsValid())
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude(WTags::Shared_SetByCaller_BaseDamage, InWeaponBaseDamage);

		if (InCurrentAttackTypeTag.IsValid())
		{
			EffectSpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttackTypeTag, InUsedComboCount);
		}
	}

	return EffectSpecHandle;
}

float UWHeroGameplayAbility::GetCurrentEquippedWeaponDamageAtAbilityLevel() const
{
	const UHeroCombatComponent* HeroCombat = GetHeroCombatComponentFromActorInfo();
	if (HeroCombat)
	{
		return HeroCombat->GetHeroCurrentEquipWeaponDamageAtLevel(GetAbilityLevel());
	}
	return 0.0f;
}

bool UWHeroGameplayAbility::GetAbilityRemainingCooldownByTag(FGameplayTag InCooldownTag, float& TotalCooldownTime, float& RemainingCooldownTime) const
{
	if (!ensure(InCooldownTag.IsValid())) return false;

	const UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

	const FGameplayEffectQuery CooldownQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(InCooldownTag.GetSingleTagContainer());

	const TArray<TPair<float, float>> TimeRemainingAndDuration = ASC->GetActiveEffectsTimeRemainingAndDuration(CooldownQuery);

	if (!TimeRemainingAndDuration.IsEmpty())
	{
		RemainingCooldownTime = TimeRemainingAndDuration[0].Key;
		TotalCooldownTime	  = TimeRemainingAndDuration[0].Value;
	}

	return RemainingCooldownTime > 0.f;
}