// Fill out your copyright notice in the Description page of Project Settings.

#include "WFunctionLibrary.h"
#include "AbilitySystem/WAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Interfaces/PawnCombatInterface.h"

UWAbilitySystemComponent* UWFunctionLibrary::NativeGetWarriorASCFromActor(AActor* InActor)
{
	check(InActor);

	return CastChecked<UWAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

void UWFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	UWAbilitySystemComponent* ASC = NativeGetWarriorASCFromActor(InActor);

	if (!ASC->HasMatchingGameplayTag(TagToAdd))
	{
		ASC->AddLooseGameplayTag(TagToAdd);
	}
}

void UWFunctionLibrary::RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	UWAbilitySystemComponent* ASC = NativeGetWarriorASCFromActor(InActor);

	if (!ASC->HasMatchingGameplayTag(TagToRemove))
	{
		ASC->RemoveLooseGameplayTag(TagToRemove);
	}
}

bool UWFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
	UWAbilitySystemComponent* ASC = NativeGetWarriorASCFromActor(InActor);

	return ASC->HasMatchingGameplayTag(TagToCheck);
}

void UWFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EWConfirmType& OutConfirmType)
{
	OutConfirmType = NativeDoesActorHaveTag(InActor, TagToCheck) ? EWConfirmType::Yes : EWConfirmType::No;
}

UPawnCombatComponent* UWFunctionLibrary::NativeGetPawnCombatComponent(AActor* InActor)
{
	check(InActor);

	if (IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
	{
		return PawnCombatInterface->GetPawnCombatComponent();
	}

	return nullptr;
}

UPawnCombatComponent* UWFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor, EWValidType& OutValidType)
{
	UPawnCombatComponent* CombatComponent = NativeGetPawnCombatComponent(InActor);

	OutValidType = CombatComponent ? EWValidType::Valid : EWValidType::Invalid;

	return CombatComponent;
}
