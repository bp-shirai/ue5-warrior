// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimInstances/WBaseAnimInstance.h"
#include "WFunctionLibrary.h"

bool UWBaseAnimInstance::DoesOwningHaveTag(FGameplayTag TagToCheck) const
{
	if (APawn* OwningPawn = TryGetPawnOwner())
	{
        return UWFunctionLibrary::NativeDoesActorHaveTag(OwningPawn, TagToCheck);
	}

	return false;
}