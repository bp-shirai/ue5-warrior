// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Combat/HeroCombatComponent.h"
#include "Items/Weapons/WHeroWeapon.h"

AWHeroWeapon* UHeroCombatComponent::GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{
	//
	return Cast<AWHeroWeapon>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}