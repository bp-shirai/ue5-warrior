// Fill out your copyright notice in the Description page of Project Settings.

#include "WGameplayTags.h"

namespace WTags
{
	//** Input Tags */
	UE_DEFINE_GAMEPLAY_TAG(Input_Move, "Input.Move");
	UE_DEFINE_GAMEPLAY_TAG(Input_Look, "Input.Look");
	UE_DEFINE_GAMEPLAY_TAG(Input_EquipAxe, "Input.EquipAxe");
	UE_DEFINE_GAMEPLAY_TAG(Input_UnequipAxe, "Input.UnequipAxe");

	//** Player Tags */
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Axe, "Player.Weapon.Axe");

}