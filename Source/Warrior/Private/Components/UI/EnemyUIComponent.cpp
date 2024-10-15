// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/UI/EnemyUIComponent.h"
#include "Widgets/WUserWidgetBase.h"

void UEnemyUIComponent::RegisterEnemyDrawnWidget(UWUserWidgetBase* InWidgetToRegister)
{
	EnemyDrawnWidgets.Add(InWidgetToRegister);
}

void UEnemyUIComponent::RemoveEnemyDrawnWidgetIfAny()
{
	if (EnemyDrawnWidgets.IsEmpty()) return;

	for (auto DrawnWidget : EnemyDrawnWidgets)
	{
		if (DrawnWidget.IsValid())
		{
			DrawnWidget->RemoveFromParent();
		}
	}

	EnemyDrawnWidgets.Empty();
}
