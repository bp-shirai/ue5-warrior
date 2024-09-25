// Fill out your copyright notice in the Description page of Project Settings.

#include "WTypes/WStructTypes.h"
#include "AbilitySystem/Abilities/WGameplayAbility.h"

bool FWHeroAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}
