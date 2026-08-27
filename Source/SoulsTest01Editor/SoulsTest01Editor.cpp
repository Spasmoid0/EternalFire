#include "SoulsTest01Editor.h"

#include "ToolMenus.h"
#include "Widgets/SWindow.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"
#include "Framework/Application/SlateApplication.h"
#include "Logging/LogMacros.h"

#include "ToolMenu.h"
#include "SoulsTickAudit.h"
#include "Editor.h"
#include "Widgets/SWidget.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SScrollBox.h"






#define LOCTEXT_NAMESPACE "FSoulsTest01EditorModule"
DEFINE_LOG_CATEGORY_STATIC(LogSoulsTest01Editor, Log, All);

void FSoulsTest01EditorModule::StartupModule()
{
	UE_LOG(
		LogSoulsTest01Editor,
		Warning,
		TEXT("SoulsTest01Editor module started!")
	);

	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateRaw(
			this,
			&FSoulsTest01EditorModule::RegisterMenus
		)
	);
}

void FSoulsTest01EditorModule::ShutdownModule()
{
	UToolMenus::UnregisterOwner(this);
}

void FSoulsTest01EditorModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);

	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu(
		"LevelEditor.MainMenu.Tools"
	);

	FToolMenuSection& Section = Menu->FindOrAddSection(
		"SoulsTest01"
	);

	FNewToolMenuChoice MakeSubMenu =
		FNewToolMenuDelegate::CreateLambda(
			[this](UToolMenu* SubMenu)
			{
				FToolMenuSection& SubSection =
					SubMenu->FindOrAddSection(
						"SoulsTest01Tools"
					);

				SubSection.AddMenuEntry(
					"TickAudit",
					LOCTEXT(
						"TickAuditLabel",
						"Tick Audit"
					),
					LOCTEXT(
						"TickAuditTooltip",
						"Analyze actors and components using Tick"
					),
					FSlateIcon(),
					FToolMenuExecuteAction::CreateRaw(
						this,
						&FSoulsTest01EditorModule::OpenTickAudit
					)
				);
			}
		);

	Section.AddSubMenu(
		"SoulsTest01",
		LOCTEXT(
			"SoulsTest01Label",
			"SoulsTest01"
		),
		LOCTEXT(
			"SoulsTest01Tooltip",
			"SoulsTest01 developer tools"
		),
		MakeSubMenu
	);
}

void FSoulsTest01EditorModule::OpenTickAudit(
	const FToolMenuContext& Context
)
{
	RefreshTickAudit();

	TSharedRef<SWindow> Window =
		SNew(SWindow)
		.Title(
			LOCTEXT(
				"TickAuditTitle",
				"SoulsTest01 - Tick Audit"
			)
		)
		.ClientSize(
			FVector2D(850.0f, 650.0f)
		)
		.SupportsMaximize(true)
		.SupportsMinimize(false);

	Window->SetContent(
		BuildTickAuditWidget()
	);

	FSlateApplication::Get().AddWindow(Window);
}


void FSoulsTest01EditorModule::RefreshTickAudit()
{
	TickEntries.Reset();

	UWorld* World = nullptr;

	if (GEditor)
	{
		World = GEditor->GetEditorWorldContext().World();
	}

	TickAuditWorld = World;

	if (!World)
	{
		return;
	}

	FSoulsTickAudit::ScanWorld(
		World,
		TickEntries
	);
}

TSharedRef<SWidget> FSoulsTest01EditorModule::BuildTickAuditWidget()
{
	const int32 ActorCount =
		FSoulsTickAudit::CountActors(
			TickAuditWorld.Get()
		);

	const int32 TickingActors =
		FSoulsTickAudit::CountTickingActors(
			TickEntries
		);

	const int32 TickingComponents =
		FSoulsTickAudit::CountTickingComponents(
			TickEntries
		);

	return
		SNew(SVerticalBox)

		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10.0f)
		[
			SNew(STextBlock)
			.Text(
				LOCTEXT(
					"TickAuditHeader",
					"SOULSTEST01 — TICK AUDIT"
				)
			)
		]

		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10.0f)
		[
			SNew(STextBlock)
			.Text(
				FText::Format(
					LOCTEXT(
						"ActorCount",
						"Actors: {0}"
					),
					ActorCount
				)
			)
		]

		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10.0f)
		[
			SNew(STextBlock)
			.Text(
				FText::Format(
					LOCTEXT(
						"TickingActorCount",
						"Actors with Tick: {0}"
					),
					TickingActors
				)
			)
		]

		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10.0f)
		[
			SNew(STextBlock)
			.Text(
				FText::Format(
					LOCTEXT(
						"TickingComponentCount",
						"Components with Tick: {0}"
					),
					TickingComponents
				)
			)
		]

		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		.Padding(10.0f)
		[
			SNew(SScrollBox)

			+ SScrollBox::Slot()
			[
				SAssignNew(ActorList, SVerticalBox)
			]
		]

		+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Right)
		.Padding(10.0f)
		[
			SNew(SButton)
			.Text(
				LOCTEXT(
					"RefreshButton",
					"Refresh"
				)
			)
			.OnClicked_Lambda(
				[this]()
				{
					RefreshTickAudit();

					return FReply::Handled();
				}
			)
		];
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(
	FSoulsTest01EditorModule,
	SoulsTest01Editor
)