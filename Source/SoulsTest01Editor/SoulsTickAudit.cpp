#include "SoulsTickAudit.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "EngineUtils.h"

void FSoulsTickAudit::ScanWorld(
	UWorld* World,
	TArray<FSoulsTickAuditEntry>& OutEntries
)
{
	OutEntries.Reset();

	if (!World)
	{
		return;
	}

	for (TActorIterator<AActor> It(World); It; ++It)
	{
		AActor* Actor = *It;

		if (!IsValid(Actor))
		{
			continue;
		}

		const bool bActorTicks =
			Actor->IsActorTickEnabled();

		int32 TickingComponents = 0;

		for (UActorComponent* Component : Actor->GetComponents())
		{
			if (!IsValid(Component))
			{
				continue;
			}

			if (Component->IsComponentTickEnabled())
			{
				TickingComponents++;
			}
		}

		if (!bActorTicks && TickingComponents == 0)
		{
			continue;
		}

		FSoulsTickAuditEntry Entry;

		Entry.ActorName = Actor->GetActorLabel();
		Entry.ActorClass = Actor->GetClass()->GetName();
		Entry.bActorTicks = bActorTicks;
		Entry.TickingComponentCount = TickingComponents;
		Entry.TickInterval = Actor->PrimaryActorTick.TickInterval;
		Entry.Actor = Actor;

		OutEntries.Add(MoveTemp(Entry));
	}
}

int32 FSoulsTickAudit::CountTickingActors(
	const TArray<FSoulsTickAuditEntry>& Entries
)
{
	int32 Count = 0;

	for (const FSoulsTickAuditEntry& Entry : Entries)
	{
		if (Entry.bActorTicks)
		{
			Count++;
		}
	}

	return Count;
}

int32 FSoulsTickAudit::CountTickingComponents(
	const TArray<FSoulsTickAuditEntry>& Entries
)
{
	int32 Count = 0;

	for (const FSoulsTickAuditEntry& Entry : Entries)
	{
		Count += Entry.TickingComponentCount;
	}

	return Count;
}