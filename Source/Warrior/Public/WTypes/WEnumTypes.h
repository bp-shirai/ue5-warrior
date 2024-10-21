#pragma once

UENUM()
enum class EWConfirmType : uint8
{
	Yes,
	No,
};

UENUM()
enum class EWValidType : uint8
{
	Valid,
	Invalid,
};

UENUM()
enum class EWSuccessType : uint8
{
	Successful,
	Failed,
};

UENUM()
enum class EWCountdownActionInput : uint8
{
	Start,
	Cancel,
};

UENUM()
enum class EWCountdownActionOutput : uint8
{
	Updated,
	Completed,
	Canceled,
};