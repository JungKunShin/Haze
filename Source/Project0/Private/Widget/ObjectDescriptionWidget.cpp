
#include "Widget/ObjectDescriptionWidget.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Components/Widget.h"
#include "Animation/WidgetAnimation.h"

void UObjectDescriptionWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    //초기 상태: 힌트만 보이고 본문은 감춤

    SetIsFocusable(false);
    SetInspectHUDVisible(true);
    SetObjectPanelVisible(false);
}

void UObjectDescriptionWidget::SetTexts(const FText& InTitle, const FText& InBody)
{
    if (ObjectTitle) ObjectTitle->SetText(InTitle);
    if (ObjectText) ObjectText->SetText(InBody);
}

void UObjectDescriptionWidget::SetIntroTexts(const FText& InTitle, const FText& InDesc, const FText& InHint)
{
    if (ObjectTitle)       ObjectTitle->SetText(InTitle);
    if (ObjectDescription) ObjectDescription->SetText(InDesc);
    if (HintText)          HintText->SetText(InHint);
}

static void SetPanelVisible(UWidget* Panel, bool bVisible)
{
    if (!Panel) return;
    Panel->SetRenderOpacity(bVisible ? 1.f : 0.f);
    Panel->SetVisibility(bVisible ? ESlateVisibility::Visible
        : ESlateVisibility::HitTestInvisible);
}

void UObjectDescriptionWidget::SetInspectHUDVisible(bool bVisible)
{
    SetPanelVisible(InspectHUDPanel, bVisible);
}

void UObjectDescriptionWidget::SetObjectPanelVisible(bool bVisible)
{
    SetPanelVisible(ObjectCanvasPanel, bVisible);
}




