// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/StartUpData/DataAsset_EnemyStartUp.h"
#include "AbilitySystem/Abilities/WEnemyGameplayAbility.h"
#include "AbilitySystem/WAbilitySystemComponent.h"


void UDataAsset_EnemyStartUp::GiveToAbilitySystemComponent(UWAbilitySystemComponent* InASC, int32 ApplyLevel)
{
    Super::GiveToAbilitySystemComponent(InASC, ApplyLevel);

    if (!EnemyCombatAbilities.IsEmpty())
    {
        for(const TSubclassOf<UWEnemyGameplayAbility>& AbilityClass : EnemyCombatAbilities)
        {
            if (!AbilityClass) continue;

            FGameplayAbilitySpec AbilitySpec(AbilityClass);
            AbilitySpec.SourceObject = InASC->GetAvatarActor();
            AbilitySpec.Level = ApplyLevel;

            InASC->GiveAbility(AbilitySpec);
        }
    }
}