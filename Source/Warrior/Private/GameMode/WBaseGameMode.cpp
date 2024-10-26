// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/WBaseGameMode.h"

AWBaseGameMode::AWBaseGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;
}