#include "SoulsWorldTimeSubsystem.h"
#include "Tickable.h"
#include "Engine/World.h"

void USoulsWorldTimeSubsystem::Initialize(
	FSubsystemCollectionBase& Collection
)
{
	Super::Initialize(Collection);

	TimeOfDay = 8.0f;
	DayNumber = 1;
	TimeScale = 1.0f;
	DayLengthSeconds = 1200.0f;
}

void USoulsWorldTimeSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void USoulsWorldTimeSubsystem::Tick(float DeltaTime)
{
	AdvanceTime(DeltaTime);
}

bool USoulsWorldTimeSubsystem::IsTickable() const
{
	return true;
}

TStatId USoulsWorldTimeSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(
		USoulsWorldTimeSubsystem,
		STATGROUP_Tickables
	);
}

void USoulsWorldTimeSubsystem::AdvanceTime(float DeltaTime)
{
	if (DayLengthSeconds <= 0.0f)
	{
		return;
	}

	const float HoursPerSecond =
		24.0f / DayLengthSeconds;

	TimeOfDay +=
		DeltaTime *
		TimeScale *
		HoursPerSecond;

	while (TimeOfDay >= 24.0f)
	{
		TimeOfDay -= 24.0f;
		DayNumber++;
	}

	while (TimeOfDay < 0.0f)
	{
		TimeOfDay += 24.0f;
		DayNumber--;
	}
}

float USoulsWorldTimeSubsystem::GetTimeOfDay() const
{
	return TimeOfDay;
}

int32 USoulsWorldTimeSubsystem::GetDayNumber() const
{
	return DayNumber;
}

float USoulsWorldTimeSubsystem::GetTimeScale() const
{
	return TimeScale;
}

void USoulsWorldTimeSubsystem::SetTimeOfDay(float NewTime)
{
	TimeOfDay = FMath::Fmod(NewTime, 24.0f);

	if (TimeOfDay < 0.0f)
	{
		TimeOfDay += 24.0f;
	}
}

void USoulsWorldTimeSubsystem::SetTimeScale(float NewTimeScale)
{
	TimeScale = FMath::Max(0.0f, NewTimeScale);
}

void USoulsWorldTimeSubsystem::SetDayLength(
	float NewDayLengthSeconds
)
{
	DayLengthSeconds =
		FMath::Max(1.0f, NewDayLengthSeconds);
}

bool USoulsWorldTimeSubsystem::IsDay() const
{
	return TimeOfDay >= 6.0f &&
		TimeOfDay < 18.0f;
}

bool USoulsWorldTimeSubsystem::IsNight() const
{
	return !IsDay();
}

//EXPOSE TO BLUEPRINT
USoulsWorldTimeSubsystem*
USoulsWorldTimeSubsystem::GetSoulsWorldTimeSubsystem(
	UObject* WorldContextObject
)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}

	UWorld* World = WorldContextObject->GetWorld();

	if (!World)
	{
		return nullptr;
	}

	return World->GetSubsystem<USoulsWorldTimeSubsystem>();
}