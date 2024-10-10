#pragma once

#include "CoreMinimal.h"

// エディタ時のみ有効にする
#if WITH_EDITOR
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"
#endif

#define CheckNull(x)              \
	{                             \
		if (x == nullptr) return; \
	}
#define CheckNullResult(x, y)       \
	{                               \
		if (x == nullptr) return y; \
	}

#if WITH_EDITOR
// エディタ時のみ有効にする
#define CheckEd(expression)                                           \
	{                                                          \
		if (!expression)                                              \
		{                                                      \
			ensure(expression);                                       \
			GUnrealEd->PlayWorld->bDebugPauseExecution = true; \
			GUnrealEd->PlaySessionPaused();                    \
		}                                                      \
	}
#else
// それ以外のときはcheckマクロで止める
#define CheckEd(expression) check(expression);
#endif

namespace Debug
{
	static void Print(const FString& Msg, const FColor& Color = FColor::MakeRandomColor(), int32 InKey = -1)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(InKey, 7.f, Color, Msg);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
		}
	}

	static void Print(const FString& FloatTitle, float FloatValueToPrint, int32 InKey = -1, const FColor& Color = FColor::MakeRandomColor())
	{
		if (GEngine)
		{
			FString FinalMsg = FloatTitle + TEXT(": ") + FString::SanitizeFloat(FloatValueToPrint);
			GEngine->AddOnScreenDebugMessage(InKey, 7.f, Color, FinalMsg);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *FinalMsg);
		}
	}
}
