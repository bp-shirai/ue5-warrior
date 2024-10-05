// Fill out your copyright notice in the Description page of Project Settings.

#include "WFunctionLibrary.h"
#include "AbilitySystem/WAbilitySystemComponent.h"
// #include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "Interfaces/PawnCombatInterface.h"
#include "Controllers/WHeroController.h"

UWAbilitySystemComponent* UWFunctionLibrary::NativeGetWarriorASCFromActor(const AActor* InActor)
{
	check(InActor);

	// return CastChecked<UWAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
	return CastChecked<UWAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(InActor));
}

void UWFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	UWAbilitySystemComponent* ASC = NativeGetWarriorASCFromActor(InActor);

	if (ASC->HasMatchingGameplayTag(TagToAdd) == false)
	{
		ASC->AddLooseGameplayTag(TagToAdd);
	}
}

void UWFunctionLibrary::RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	UWAbilitySystemComponent* ASC = NativeGetWarriorASCFromActor(InActor);

	if (ASC->HasMatchingGameplayTag(TagToRemove))
	{
		ASC->RemoveLooseGameplayTag(TagToRemove);
	}
}

bool UWFunctionLibrary::NativeDoesActorHaveTag(const AActor* InActor, FGameplayTag TagToCheck)
{
	const UWAbilitySystemComponent* ASC = NativeGetWarriorASCFromActor(InActor);

	return ASC->HasMatchingGameplayTag(TagToCheck);
}

void UWFunctionLibrary::BP_DoesActorHaveTag(const AActor* InActor, FGameplayTag TagToCheck, EWConfirmType& OutConfirmType)
{
	OutConfirmType = NativeDoesActorHaveTag(InActor, TagToCheck) ? EWConfirmType::Yes : EWConfirmType::No;
}

UPawnCombatComponent* UWFunctionLibrary::NativeGetPawnCombatComponent(const AActor* InActor)
{
	check(InActor);

	if (const IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
	{
		return PawnCombatInterface->GetPawnCombatComponent();
	}

	return nullptr;
}

UPawnCombatComponent* UWFunctionLibrary::BP_GetPawnCombatComponentFromActor(const AActor* InActor, EWValidType& OutValidType)
{
	UPawnCombatComponent* CombatComponent = NativeGetPawnCombatComponent(InActor);

	OutValidType = CombatComponent ? EWValidType::Valid : EWValidType::Invalid;

	return CombatComponent;
}

bool UWFunctionLibrary::IsTargetPawnHostile(const APawn* QueryPawn, const APawn* TargetPawn)
{
	check(QueryPawn && TargetPawn);

	const IGenericTeamAgentInterface* QueryTramAgent  = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
	const IGenericTeamAgentInterface* TargetTramAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

	if (QueryTramAgent && TargetTramAgent)
	{
		return QueryTramAgent->GetGenericTeamId() != TargetTramAgent->GetGenericTeamId();
	}

	return false;
}

FVector UWFunctionLibrary::GetForwardVector(const AActor* Actor)
{
	const FRotator BaseRotation = Actor->GetActorRotation();
	const FMatrix RotMatrix = FRotationMatrix(BaseRotation);
	const FVector ForwardVector = RotMatrix.GetScaledAxis(EAxis::X);

	return ForwardVector;
}
