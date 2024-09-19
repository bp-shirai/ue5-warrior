// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AnimInstances/WBaseAnimInstance.h"

#include "WCharacterAnimInstance.generated.h"

class AWBaseCharacter;
class UCharacterMovementComponent;

/**
 *
 */
UCLASS()
class WARRIOR_API UWCharacterAnimInstance : public UWBaseAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	TObjectPtr<AWBaseCharacter> OwningCharacter;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> OwningMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float GroundSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bHasAcceleration;
};
