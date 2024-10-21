// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/GameplayAbilityTypes.h"

#include "WAbilityTypes.generated.h"

class UWAbilitySystemComponent;
class AWEnemyCharacter;
class AWHeroCharacter;
class AWHeroController;
class AWAIController;
// class UPawnCombatComponent;

USTRUCT(BlueprintType)
struct FWGameplayAbilityActorInfo : public FGameplayAbilityActorInfo
{
	GENERATED_USTRUCT_BODY()

	typedef Super FGameplayAbilityActorInfo;

	virtual ~FWGameplayAbilityActorInfo()
	{
	}

	// Our Warrior AbilitySystemComponent. Should NEVER be null.
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UWAbilitySystemComponent> WAbilitySystemComponent;

	// Our Warrior Enemy Pawn. Often nullptr.
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AWEnemyCharacter> EnemyCharacter;

	// Our Warrior Player Pawn. Often nullptr
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AWHeroCharacter> HeroCharacter;

	// Our Warrior Player Controller. Often nullptr.
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AWHeroController> HeroController;

	// Our Warrior Player State. Often nullptr.
	// UPROPERTY(BlueprintReadOnly, Category = "ActorInfo")
	// TWeakObjectPtr<class AWPlayerState> WPlayerState;

	// Our Warrior Player Controller. Often nullptr.
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AWAIController> AIController;

	// UPROPERTY(BlueprintReadOnly, Category = "ActorInfo")
	// TWeakObjectPtr<UPawnCombatComponent> PawnCombatComponent;

	virtual void InitFromActor(AActor* OwnerActor, AActor* AvatarActor, UAbilitySystemComponent* InAbilitySystemComponent) override;

	virtual void SetAvatarActor(AActor* AvatarActor) override;

	virtual void ClearActorInfo() override;

	UWAbilitySystemComponent* GetWAbilitySystemComponent() const;

	// Gets the Enemy Pawn. This is often nullptr.
	AWEnemyCharacter* GetEnemyCharacter() const;

	// Gets the Hero Pawn. This is often nullptr.
	AWHeroCharacter* GetHeroCharacter() const;

	// Gets the Hero Controller. This is often nullptr.
	AWHeroController* GetHeroController() const;

	// Gets the Hero Controller. This is often nullptr.
	AWAIController* GetAIController() const;

	// Gets the Player State. This is often nullptr.
	// class AWHeroState* GetHeroState() const;

	// UPawnCombatComponent* GetPawnCombatComponent() const;
};