// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimInstances/Hero/WHeroLinkedAnimLayer.h"
#include "AnimInstances/Hero/WHeroAnimInstance.h"

UWHeroAnimInstance* UWHeroLinkedAnimLayer::GetHeroAnimInstance() const
{
	/// cast checked
	return Cast<UWHeroAnimInstance>(GetOwningComponent()->GetAnimInstance());
}