// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AnimInstances/WCharacterAnimInstance.h"
#include "CoreMinimal.h"
#include "WHeroAnimInstance.generated.h"

class AWHeroCharacter;
/**
 *
 */
UCLASS()
class WARRIOR_API UWHeroAnimInstance : public UWCharacterAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|References")
	TObjectPtr<AWHeroCharacter> OwningHeroCharacter;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bShouldEnterRelaxState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float EnterRelaxStateThreshold = 5.f;

	float IdleElapsedTime;
};
