// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/WEnemyGameplayAbility.h"
#include "Characters/WEnemyCharacter.h"
#include "AbilitySystem/WAbilitySystemComponent.h"
#include "WGameplayTags.h"

AWEnemyCharacter* UWEnemyGameplayAbility::GetEnemyCharacterFromActorInfo() const
{
	// if (!CachedEnemyCharacter.IsValid())
	// {
	//     CachedEnemyCharacter = Cast<AWEnemyCharacter>(CurrentActorInfo->AvatarActor);
	// }
	// return CachedEnemyCharacter.IsValid() ? CachedEnemyCharacter.Get() : nullptr;
	if (!ensure(CurrentActorInfo))
	{
		return nullptr;
	}

	return CastChecked<AWEnemyCharacter>(CurrentActorInfo->AvatarActor.Get());
}

UEnemyCombatComponent* UWEnemyGameplayAbility::GetEnemyCombatComponentFromActorInfo() const
{
	if (!ensure(CurrentActorInfo))
	{
		return nullptr;
	}

	return GetEnemyCharacterFromActorInfo()->GetEnemyCombatComponent();
}

FGameplayEffectSpecHandle UWEnemyGameplayAbility::MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalableFloat) const
{
	check(EffectClass);

	UWAbilitySystemComponent* ASC = GetWAbilitySystemComponentFromActorInfo();
	AActor* AvatarActor			  = GetAvatarActorFromActorInfo();

	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(AvatarActor);
	ContextHandle.AddInstigator(AvatarActor, AvatarActor);

	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(EffectClass, GetAbilityLevel(), ContextHandle);

	EffectSpecHandle.Data->SetSetByCallerMagnitude(WTags::Shared_SetByCaller_BaseDamage, InDamageScalableFloat.GetValueAtLevel(GetAbilityLevel()));

	return EffectSpecHandle;
}