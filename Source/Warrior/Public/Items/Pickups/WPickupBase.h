// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WPickupBase.generated.h"

class USphereComponent;


UCLASS(Abstract)
class WARRIOR_API AWPickupBase : public AActor
{
	GENERATED_BODY()
	
public:
	AWPickupBase();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PickUp Interaction")
	TObjectPtr<USphereComponent> PickupCollisionSphere;

	UFUNCTION()
	virtual void OnCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
