// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/WEnemyGameplayAbility.h"
#include "AbilitySystem/WAbilitySystemComponent.h"
#include "AbilitySystem/WAbilityTypes.h"
#include "Characters/WEnemyCharacter.h"
#include "WGameplayTags.h"

AWEnemyCharacter* UWEnemyGameplayAbility::GetEnemyCharacterFromActorInfo() const
{
	const FWGameplayAbilityActorInfo* ActorInfo = GetWActorInfo(CurrentActorInfo);

	return ActorInfo ? ActorInfo->EnemyCharacter.Get() : nullptr;
}

UEnemyCombatComponent* UWEnemyGameplayAbility::GetEnemyCombatComponentFromActorInfo() const
{
	const AWEnemyCharacter* EnemyCharacter = GetEnemyCharacterFromActorInfo();
	return EnemyCharacter ? EnemyCharacter->GetEnemyCombatComponent() : nullptr;
}

FGameplayEffectSpecHandle UWEnemyGameplayAbility::MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalableFloat) const
{
	ensure(EffectClass);

	const UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	AActor* AvatarActor			 = GetAvatarActorFromActorInfo();

	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(AvatarActor);
	ContextHandle.AddInstigator(AvatarActor, AvatarActor);

	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(EffectClass, GetAbilityLevel(), ContextHandle);
	if (EffectSpecHandle.IsValid())
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude(WTags::Shared_SetByCaller_BaseDamage, InDamageScalableFloat.GetValueAtLevel(GetAbilityLevel()));
	}
	return EffectSpecHandle;
}