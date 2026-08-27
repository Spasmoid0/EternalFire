#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "SoulsTest01Editor/SoulsTickAudit.h"

class FSoulsTest01EditorModule : public IModuleInterface
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;


private:
	
	TSharedRef<SWidget> BuildActorRow(
	const FSoulsTickAuditEntry& Entry
);

	void RegisterMenus();

	void OpenTickAudit(const FToolMenuContext& Context);

	void RefreshTickAudit();

	TSharedRef<SWidget> BuildTickAuditWidget();
	
	TSharedRef<SWidget> BuildTickAuditHeader();
	
	void PopulateTickAuditList();

	TArray<FSoulsTickAuditEntry> TickEntries;

	TSharedPtr<STextBlock> ActorCountText;
	TSharedPtr<STextBlock> TickingActorCountText;
	TSharedPtr<STextBlock> TickingComponentCountText;

	TSharedPtr<SVerticalBox> ActorList;

	TWeakObjectPtr<UWorld> TickAuditWorld;
};