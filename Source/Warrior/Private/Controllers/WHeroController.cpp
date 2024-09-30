// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/WHeroController.h"

AWHeroController::AWHeroController()
{
    HeroTeamID = FGenericTeamId(0);
}

FGenericTeamId AWHeroController::GetGenericTeamId() const
{
	return HeroTeamID;
}