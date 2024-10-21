// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/WAbilitySystemGlobals.h"
#include "AbilitySystem/WAbilityTypes.h"

FGameplayAbilityActorInfo* UWAbilitySystemGlobals::AllocAbilityActorInfo() const
{
    return new FWGameplayAbilityActorInfo();
}