// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/WAttributeSet.h"

UWAttributeSet::UWAttributeSet()
{
    InitCurrentHealth(1.f);
    InitMaxHealth(1.f);
    InitCurrentRage(1.f);
    InitMaxRage(1.f);
    InitAttackPower(1.f);
    InitDefensePower(1.f);
}