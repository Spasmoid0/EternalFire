#include "SoulsDebugOverlay.h"

#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

ASoulsDebugOverlay::ASoulsDebugOverlay()
{
	bDebugVisible = false;
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

	DrawDebugOverlay();
}

void ASoulsDebugOverlay::DrawDebugOverlay()
{
	if (!Canvas)
	{
		return;
	}

	float Y = 30.0f;

	DrawTextLine(
		TEXT("SOULSTEST01 DEBUG"),
		30.0f,
		Y,
		1.3f
	);

	Y += 35.0f;

	const float DeltaTime = FApp::GetDeltaTime();

	const float FPS = DeltaTime > 0.0f
		? 1.0f / DeltaTime
		: 0.0f;

	const float FrameTime = DeltaTime * 1000.0f;

	DrawTextLine(
		FString::Printf(
			TEXT("FPS: %.1f"),
			FPS
		),
		30.0f,
		Y
	);

	Y += 22.0f;

	DrawTextLine(
		FString::Printf(
			TEXT("Frame Time: %.2f ms"),
			FrameTime
		),
		30.0f,
		Y
	);

	Y += 35.0f;

	APlayerController* PC = GetOwningPlayerController();

	if (!PC)
	{
		return;
	}

	APawn* Pawn = PC->GetPawn();

	if (!Pawn)
	{
		DrawTextLine(
			TEXT("PLAYER: NONE"),
			30.0f,
			Y
		);

		return;
	}

	const FVector Location = Pawn->GetActorLocation();
	const FVector Velocity = Pawn->GetVelocity();

	DrawTextLine(
		TEXT("PLAYER"),
		30.0f,
		Y
	);

	Y += 22.0f;

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