// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Weapons/WHeroWeapon.h"

void AWHeroWeapon::AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles)
{
	//
	GrantedAbilitySpecHandles = InSpecHandles;
}

TArray<FGameplayAbilitySpecHandle> AWHeroWeapon::GetGrantedAbilitySpecHandles() const
{
	//
	return GrantedAbilitySpecHandles;
}