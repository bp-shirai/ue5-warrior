// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/WEnemyGameplayAbility.h"
#include "Characters/WEnemyCharacter.h"


AWEnemyCharacter* UWEnemyGameplayAbility::GetEnemyCharacterFromActorInfo()
{
    if (!CachedEnemyCharacter.IsValid())
    {
        CachedEnemyCharacter = Cast<AWEnemyCharacter>(CurrentActorInfo->AvatarActor);
    }
    return CachedEnemyCharacter.IsValid() ? CachedEnemyCharacter.Get() : nullptr;
}

UEnemyCombatComponent* UWEnemyGameplayAbility::GetEnemyCombatComponentFromActorInfo()
{
    return GetEnemyCharacterFromActorInfo()->GetEnemyCombatComponent(); 
}
