#include "SoulsDebugOverlay.h"

#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "RHI.h"

ASoulsDebugOverlay::ASoulsDebugOverlay()
{
	bDebugVisible = false;
	SmoothedFPS = 0.0f;
	SmoothedFrameTime = 0.0f;
}

void ASoulsDebugOverlay::ToggleDebug()
{
	bDebugVisible = !bDebugVisible;
}

void ASoulsDebugOverlay::DrawHUD()
{
	Super::DrawHUD();

	if (!bDebugVisible)
	{
		return;
	}

	UpdatePerformanceStats();
	DrawDebugOverlay();
}

void ASoulsDebugOverlay::UpdatePerformanceStats()
{
	const float DeltaTime = GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.0f;

	if (DeltaTime <= 0.0f)
	{
		return;
	}

	const float CurrentFPS = 1.0f / DeltaTime;
	const float CurrentFrameTime = DeltaTime * 1000.0f;

	constexpr float Smoothing = 0.1f;

	if (SmoothedFPS <= 0.0f)
	{
		SmoothedFPS = CurrentFPS;
		SmoothedFrameTime = CurrentFrameTime;
	}
	else
	{
		SmoothedFPS = FMath::Lerp(
			SmoothedFPS,
			CurrentFPS,
			Smoothing
		);

		SmoothedFrameTime = FMath::Lerp(
			SmoothedFrameTime,
			CurrentFrameTime,
			Smoothing
		);
	}
}

void ASoulsDebugOverlay::DrawDebugOverlay()
{
	if (!Canvas)
	{
		return;
	}

	float Y = 30.0f;

	int32 SizeX = 0;
	int32 SizeY = 0;

	APlayerController* PC = GetOwningPlayerController();

	if (PC)
	{
		PC->GetViewportSize(SizeX, SizeY);
	}

	// -------------------------------------------------
	// Header
	// -------------------------------------------------

	DrawTextLine(
		TEXT("Souls Debug Overlay"),
		30.0f,
		Y,
		1.3f
	);

	Y += 35.0f;

	// -------------------------------------------------
	// Performance
	// -------------------------------------------------

	DrawTextLine(
		TEXT("PERFORMANCE"),
		30.0f,
		Y
	);

	Y += 22.0f;

	DrawTextLine(
		FString::Printf(
			TEXT("FPS: %.1f"),
			SmoothedFPS
		),
		30.0f,
		Y
	);

	Y += 22.0f;

	DrawTextLine(
		FString::Printf(
			TEXT("Frame Time: %.2f ms"),
			SmoothedFrameTime
		),
		30.0f,
		Y
	);

	Y += 35.0f;

	// -------------------------------------------------
	// Engine
	// -------------------------------------------------

	DrawTextLine(
		TEXT("ENGINE"),
		30.0f,
		Y
	);

	Y += 22.0f;

	FString RHIName = TEXT("Unknown");

	if (GDynamicRHI)
	{
		RHIName = GDynamicRHI->GetName();
	}

	DrawTextLine(
		FString::Printf(
			TEXT("RHI: %s"),
			*RHIName
		),
		30.0f,
		Y
	);

	Y += 22.0f;

	DrawTextLine(
		FString::Printf(
			TEXT("Resolution: %d x %d"),
			SizeX,
			SizeY
		),
		30.0f,
		Y
	);

	Y += 35.0f;

	// -------------------------------------------------
	// Player
	// -------------------------------------------------

	DrawTextLine(
		TEXT("PLAYER"),
		30.0f,
		Y
	);

	Y += 22.0f;

	if (!PC)
	{
		DrawTextLine(
			TEXT("Player Controller: NONE"),
			30.0f,
			Y
		);

		return;
	}

	APawn* Pawn = PC->GetPawn();

	if (!Pawn)
	{
		DrawTextLine(
			TEXT("Pawn: NONE"),
			30.0f,
			Y
		);

		return;
	}

	const FVector Location = Pawn->GetActorLocation();
	const FVector Velocity = Pawn->GetVelocity();

	DrawTextLine(
		FString::Printf(
			TEXT("Location: X %.1f  Y %.1f  Z %.1f"),
			Location.X,
			Location.Y,
			Location.Z
		),
		30.0f,
		Y
	);

	Y += 22.0f;

	DrawTextLine(
		FString::Printf(
			TEXT("Velocity: %.1f cm/s"),
			Velocity.Size()
		),
		30.0f,
		Y
	);
}

void ASoulsDebugOverlay::DrawTextLine(
	const FString& Text,
	float X,
	float Y,
	float Scale
)
{
	if (!Canvas || !GEngine)
	{
		return;
	}

	FCanvasTextItem TextItem(
		FVector2D(X, Y),
		FText::FromString(Text),
		GEngine->GetMediumFont(),
		FLinearColor::White
	);

	TextItem.Scale = FVector2D(Scale, Scale);

	Canvas->DrawItem(TextItem);
}