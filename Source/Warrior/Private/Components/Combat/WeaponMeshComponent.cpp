// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/WeaponMeshComponent.h"
#include "Components/BoxComponent.h"
#include "WFunctionLibrary.h"


UWeaponMeshComponent::UWeaponMeshComponent()
{
    WeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>("WeaponCollisionBox");
    WeaponCollisionBox->SetupAttachment(this);
    WeaponCollisionBox->SetBoxExtent(FVector(20.f));
	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxBeginOverlap);
	WeaponCollisionBox->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxEndOverlap);
}


void UWeaponMeshComponent::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void UWeaponMeshComponent::OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
