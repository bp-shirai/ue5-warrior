// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimInstances/WBaseAnimInstance.h"
#include "WFunctionLibrary.h"

bool UWBaseAnimInstance::DoesOwningHaveTag(FGameplayTag TagToCheck) const
{
	if (const APawn* OwningPawn = TryGetPawnOwner())
	{
        return UWFunctionLibrary::DoesActorHaveTag(OwningPawn, TagToCheck);
	}

	return false;
}