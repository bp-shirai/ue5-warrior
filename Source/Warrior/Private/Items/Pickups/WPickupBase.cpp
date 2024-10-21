// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Pickups/WPickupBase.h"
#include "Components/SphereComponent.h"

AWPickupBase::AWPickupBase()
{
	PrimaryActorTick.bCanEverTick = false;

	PickupCollisionSphere = CreateDefaultSubobject<USphereComponent>("PickupCollisionSphere");
	SetRootComponent(PickupCollisionSphere);
	PickupCollisionSphere->InitSphereRadius(50.f);
	PickupCollisionSphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionSphereBeginOverlap);
}

void AWPickupBase::OnCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}