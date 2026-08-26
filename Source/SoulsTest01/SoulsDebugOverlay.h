#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SoulsDebugOverlay.generated.h"

UCLASS()
class SOULSTEST01_API ASoulsDebugOverlay : public AHUD
{
	GENERATED_BODY()

public:

	ASoulsDebugOverlay();

	virtual void DrawHUD() override;

	UFUNCTION(BlueprintCallable, Category = "Debug")
	void ToggleDebug();

private:

	bool bDebugVisible = false;

	float SmoothedFPS = 0.0f;
	float SmoothedFrameTime = 0.0f;

	void UpdatePerformanceStats();

	void DrawDebugOverlay();

	void DrawTextLine(
		const FString& Text,
		float X,
		float Y,
		float Scale = 1.0f
	);
};