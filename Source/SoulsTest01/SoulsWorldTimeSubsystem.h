#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Tickable.h"
#include "SoulsWorldTimeSubsystem.generated.h"


UCLASS()
class SOULSTEST01_API USoulsWorldTimeSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:

	// Current time of day, represented as hours from 0.0 to 24.0.
	UPROPERTY(BlueprintReadOnly, Category = "World Time")
	float TimeOfDay = 8.0f;

	// Current in-game day.
	UPROPERTY(BlueprintReadOnly, Category = "World Time")
	int32 DayNumber = 1;

	// How quickly the world clock advances.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Time")
	float TimeScale = 1.0f;

	// How many real seconds are required for one full in-game day.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Time")
	float DayLengthSeconds = 1200.0f;
	
	//OBJECT EXPOSED FOR BLUEPRINTS
	UFUNCTION(
		BlueprintPure,
		Category = "World Time",
		meta = (WorldContext = "WorldContextObject")
	)
	static USoulsWorldTimeSubsystem* GetSoulsWorldTimeSubsystem(
		UObject* WorldContextObject
	);

public:

	virtual void Initialize(
		FSubsystemCollectionBase& Collection
	) override;

	virtual void Deinitialize() override;

	virtual void Tick(float DeltaTime) override;

	virtual bool IsTickable() const override;

	virtual TStatId GetStatId() const override;

	UFUNCTION(BlueprintPure, Category = "World Time")
	float GetTimeOfDay() const;

	UFUNCTION(BlueprintPure, Category = "World Time")
	int32 GetDayNumber() const;

	UFUNCTION(BlueprintPure, Category = "World Time")
	float GetTimeScale() const;

	UFUNCTION(BlueprintCallable, Category = "World Time")
	void SetTimeOfDay(float NewTime);

	UFUNCTION(BlueprintCallable, Category = "World Time")
	void SetTimeScale(float NewTimeScale);

	UFUNCTION(BlueprintCallable, Category = "World Time")
	void SetDayLength(float NewDayLengthSeconds);

	UFUNCTION(BlueprintPure, Category = "World Time")
	bool IsDay() const;

	UFUNCTION(BlueprintPure, Category = "World Time")
	bool IsNight() const;

private:

	void AdvanceTime(float DeltaTime);
};