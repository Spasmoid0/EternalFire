#include "SoulsTest01Editor.h"

#include "ToolMenus.h"
#include "Widgets/SWindow.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"
#include "Framework/Application/SlateApplication.h"
#include "Logging/LogMacros.h"

#include "ToolMenu.h"






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
	TSharedRef<SWindow> Window =
		SNew(SWindow)
		.Title(
			LOCTEXT(
				"TickAuditTitle",
				"SoulsTest01 - Tick Audit"
			)
		)
		.ClientSize(
			FVector2D(700.0f, 500.0f)
		)
		.SupportsMaximize(true)
		.SupportsMinimize(false);

	Window->SetContent(
		SNew(SBox)
		.Padding(20.0f)
		[
			SNew(STextBlock)
			.Text(
				LOCTEXT(
					"TickAuditPlaceholder",
					"SoulsTest01 Tick Audit\n\n"
					"Scanner coming next..."
				)
			)
		]
	);

	FSlateApplication::Get().AddWindow(Window);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(
	FSoulsTest01EditorModule,
	SoulsTest01Editor
)