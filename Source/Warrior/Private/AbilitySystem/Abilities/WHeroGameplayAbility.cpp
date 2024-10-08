// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/WHeroGameplayAbility.h"
#include "AbilitySystem/WAbilitySystemComponent.h"
#include "Characters/WHeroCharacter.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "Controllers/WHeroController.h"

#include "WGameplayTags.h"

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
	if (!CachedHeroController.IsValid())
	{
		CachedHeroController = Cast<AWHeroController>(CurrentActorInfo->PlayerController);
	}
	return CachedHeroController.IsValid() ? CachedHeroController.Get() : nullptr;
}

UHeroCombatComponent* UWHeroGameplayAbility::GetHeroCombatComponentFromActorInfo()
{
	AWHeroCharacter* HeroCharacter = GetHeroCharacterFromActorInfo();

	return HeroCharacter ? HeroCharacter->GetHeroCombatComponent() : nullptr;
}

FGameplayEffectSpecHandle UWHeroGameplayAbility::MakeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount)
{
	check(EffectClass);

	UWAbilitySystemComponent* ASC = GetWAbilitySystemComponentFromActorInfo();
	AActor* AvatarActor			  = GetAvatarActorFromActorInfo();

	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(AvatarActor);
	ContextHandle.AddInstigator(AvatarActor, AvatarActor);

	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(EffectClass, GetAbilityLevel(), ContextHandle);

	EffectSpecHandle.Data->SetSetByCallerMagnitude(WTags::Shared_SetByCaller_BaseDamage, InWeaponBaseDamage);

	if (InCurrentAttackTypeTag.IsValid())
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttackTypeTag, InUsedComboCount);
	}

	return EffectSpecHandle;
}