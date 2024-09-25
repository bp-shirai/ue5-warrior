// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

namespace WTags
{
	//** Input Tags */
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Move);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Look);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_EquipAxe);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_UnequipAxe);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_LightAttack_Axe);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_HeavyAttack_Axe);

	//** Player Tags */
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Equip_Axe);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Unequip_Axe);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Light_Axe);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Heavy_Axe);


	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Weapon_Axe);

	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Equip_Axe);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Unequip_Axe);

	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_JumpToFinisher);

	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_SetByCaller_AttackType_Light);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_SetByCaller_AttackType_Heavy);

	//** Enemy Tags */
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Weapon);

	//** Shared Tags */
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_MeleeHit);

	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_SetByCaller_BaseDamage);
}
