#pragma once

#include "CoreMinimal.h"

class UWorld;
class AActor;

struct FSoulsTickAuditEntry
{
	FString ActorName;
	FString ActorClass;
	bool bActorTicks = false;
	int32 TickingComponentCount = 0;
	float TickInterval = 0.0f;

	TWeakObjectPtr<AActor> Actor;
};

class FSoulsTickAudit
{
public:

	static void ScanWorld(
		UWorld* World,
		TArray<FSoulsTickAuditEntry>& OutEntries
	);

	static int32 CountTickingActors(
		const TArray<FSoulsTickAuditEntry>& Entries
	);

	static int32 CountTickingComponents(
		const TArray<FSoulsTickAuditEntry>& Entries
	);
};