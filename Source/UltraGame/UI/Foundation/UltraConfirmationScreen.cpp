// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraConfirmationScreen.h"

#if WITH_EDITOR
#include "Editor/WidgetCompilerLog.h"
#endif

#include "CommonBorder.h"
#include "CommonRichTextBlock.h"
#include "CommonTextBlock.h"
#include "Components/DynamicEntryBox.h"
#include "ICommonInputModule.h"
#include "Input/Reply.h"
#include "CommonButtonBase.h"
#include "UltraButtonBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraConfirmationScreen)

void UUltraConfirmationScreen::SetupDialog(UCommonGameDialogDescriptor* Descriptor, FCommonMessagingResultDelegate ResultCallback)
{
	Super::SetupDialog(Descriptor, ResultCallback);

	Text_Title->SetText(Descriptor->Header);
	RichText_Description->SetText(Descriptor->Body);

	EntryBox_Buttons->Reset<UUltraButtonBase>([](UUltraButtonBase& Button)
	{
		Button.OnClicked().Clear();
	});

	for (const FConfirmationDialogAction& Action : Descriptor->ButtonActions)
	{
		FDataTableRowHandle ActionRow;

		switch(Action.Result)
		{
			case ECommonMessagingResult::Confirmed:
				ActionRow = ICommonInputModule::GetSettings().GetDefaultClickAction();
				break;
			case ECommonMessagingResult::Declined:
				ActionRow = ICommonInputModule::GetSettings().GetDefaultBackAction();
				break;
			case ECommonMessagingResult::Cancelled:
				ActionRow = CancelAction;
				break;
			default:
				ensure(false);
				continue;
		}

		UUltraButtonBase* Button = EntryBox_Buttons->CreateEntry<UUltraButtonBase>();
		Button->SetTriggeringInputAction(ActionRow);
		Button->OnClicked().AddUObject(this, &ThisClass::CloseConfirmationWindow, Action.Result);
		Button->SetButtonText(Action.OptionalDisplayText);
	}

	OnResultCallback = ResultCallback;
}

void UUltraConfirmationScreen::KillDialog()
{
	Super::KillDialog();
}

void UUltraConfirmationScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Border_TapToCloseZone->OnMouseButtonDownEvent.BindDynamic(this, &UUltraConfirmationScreen::HandleTapToCloseZoneMouseButtonDown);
}

void UUltraConfirmationScreen::CloseConfirmationWindow(ECommonMessagingResult Result)
{
	DeactivateWidget();
	OnResultCallback.ExecuteIfBound(Result);
}

FEventReply UUltraConfirmationScreen::HandleTapToCloseZoneMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = FReply::Unhandled();

	if (MouseEvent.IsTouchEvent() || MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		CloseConfirmationWindow(ECommonMessagingResult::Declined);
		Reply.NativeReply = FReply::Handled();
	}

	return Reply;
}

#if WITH_EDITOR
void UUltraConfirmationScreen::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const
{
	if (CancelAction.IsNull())
	{
		CompileLog.Error(FText::Format(FText::FromString(TEXT("{0} has unset property: CancelAction.")), FText::FromString(GetName())));
	}
}
#endif
