// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/WBaseCharacter.h"
#include "WEnemyCharacter.generated.h"

class UEnemyCombatComponent;

/**
 *
 */
UCLASS()
class WARRIOR_API AWEnemyCharacter : public AWBaseCharacter
{
	GENERATED_BODY()

public:
	AWEnemyCharacter();

	//~ Begin IPawnCombatInterface Interface.
	virtual UPawnCombatComponent* GetPawnCombatComponent() const;
	//~ End IPawnCombatInterface Interface.

protected:
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Warrior|Combat")
	TObjectPtr<UEnemyCombatComponent> EnemyCombatComponent;

private:
	void InitEnemyStartUpData();

public:
	FORCEINLINE UEnemyCombatComponent* GetEnemyCombatComponent() const { return EnemyCombatComponent; }
};
