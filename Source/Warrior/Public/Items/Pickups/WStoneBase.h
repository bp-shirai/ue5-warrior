// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Pickups/WPickupBase.h"
#include "WStoneBase.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

/**
 *
 */
UCLASS(Abstract)
class WARRIOR_API AWStoneBase : public AWPickupBase
{
	GENERATED_BODY()

public:

	void Consume(UAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel);

protected:

	virtual void OnCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Stone Consumed"))
	void BP_OnStoneConsumed();

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TSubclassOf<UGameplayEffect> StoneGameplayEffectClass;
};
