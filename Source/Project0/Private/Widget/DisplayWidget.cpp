// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/DisplayWidget.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/ConfigCacheIni.h"

static const TCHAR* SectionGUS = TEXT("/Script/Engine.GameUserSettings");
static const TCHAR* KeyBrightness = TEXT("BrightnessGamma");

void UDisplayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Combo_WindowMode && Combo_WindowMode->GetOptionCount() == 0)
	{
		Combo_WindowMode->AddOption(TEXT("Fullscreen"));
		Combo_WindowMode->AddOption(TEXT("Windowed"));
	}
	if (Combo_Resolution && Combo_Resolution->GetOptionCount() == 0)
	{
		
		Combo_Resolution->AddOption(TEXT("1920x1080"));
		Combo_Resolution->AddOption(TEXT("1280x720"));
	}

	if (Combo_WindowMode)
		Combo_WindowMode->OnSelectionChanged.AddDynamic(this, &UDisplayWidget::OnWindowModeChanged);
	if (Combo_Resolution)
		Combo_Resolution->OnSelectionChanged.AddDynamic(this, &UDisplayWidget::OnResolutionChanged);
	if (Slider_Brightness)
		Slider_Brightness->OnValueChanged.AddDynamic(this, &UDisplayWidget::OnBrightnessChanged);

	LoadInitialSettingsToUI();
}

void UDisplayWidget::LoadInitialSettingsToUI()
{
	if (!GEngine) return;
	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	if (!Settings) return;

	// 창 모드
	const EWindowMode::Type WM = Settings->GetFullscreenMode();
	const bool bFullscreen = (WM == EWindowMode::Fullscreen || WM == EWindowMode::WindowedFullscreen);
	if (Combo_WindowMode)
	{
		Combo_WindowMode->SetSelectedOption(bFullscreen ? TEXT("Fullscreen") : TEXT("Windowed"));
	}

	// 해상도
	const FIntPoint Res = Settings->GetScreenResolution();
	const FString ResStr = ToResString(Res);
	if (Combo_Resolution)
	{
		if (Combo_Resolution->FindOptionIndex(ResStr) == INDEX_NONE)
		{
			Combo_Resolution->AddOption(ResStr);
		}
		Combo_Resolution->SetSelectedOption(ResStr);
	}

	const float SavedGamma = LoadBrightnessFromConfig(); 
	const int32 BrightnessLevel = GammaToBrightness(SavedGamma); 
	if (Text_BrightnessValue)
		Text_BrightnessValue->SetText(FText::AsNumber(BrightnessLevel));

	if (Slider_Brightness)
	{
		
		const float Normalized = (BrightnessLevel - 1) / 9.0f;
		Slider_Brightness->SetValue(Normalized);
	}

	
	ApplyBrightnessLevel(BrightnessLevel);
}

void UDisplayWidget::OnWindowModeChanged(FString Selected, ESelectInfo::Type SelectionType)
{
	ApplyWindowModeAndResolution();
}

void UDisplayWidget::OnResolutionChanged(FString Selected, ESelectInfo::Type SelectionType)
{
	ApplyWindowModeAndResolution();
}

void UDisplayWidget::ApplyWindowModeAndResolution()
{
	if (!GEngine) return;
	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	if (!Settings) return;


	const bool bFullscreen = (Combo_WindowMode && Combo_WindowMode->GetSelectedOption() == TEXT("Fullscreen"));
	Settings->SetFullscreenMode(bFullscreen ? EWindowMode::Fullscreen : EWindowMode::Windowed);

	
	if (Combo_Resolution)
	{
		const FIntPoint Res = ParseRes(Combo_Resolution->GetSelectedOption());
		if (Res.X > 0 && Res.Y > 0)
		{
			Settings->SetScreenResolution(Res);
		}
	}

	// 즉시 적용 + 저장
	Settings->ApplySettings(false);
	Settings->SaveSettings();
}

void UDisplayWidget::OnBrightnessChanged(float Normalized)
{
	int32 Level = FMath::Clamp(FMath::RoundToInt(Normalized * 9.0f) + 1, 1, 10);
	if (Text_BrightnessValue)
		Text_BrightnessValue->SetText(FText::AsNumber(Level));

	ApplyBrightnessLevel(Level);
}

void UDisplayWidget::ApplyBrightnessLevel(int32 Level01to10)
{
	const float Gamma = BrightnessToGamma(Level01to10);

	
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		PC->ConsoleCommand(FString::Printf(TEXT("r.TonemapperGamma %.2f"), Gamma), false);
	}

	SaveBrightnessToConfig(Gamma);
}

FIntPoint UDisplayWidget::ParseRes(const FString& In)
{
	FIntPoint Out(0, 0);
	TArray<FString> Parts;
	In.ParseIntoArray(Parts, TEXT("x"));
	if (Parts.Num() == 2)
	{
		Out.X = FCString::Atoi(*Parts[0]);
		Out.Y = FCString::Atoi(*Parts[1]);
	}
	return Out;
}

FString UDisplayWidget::ToResString(const FIntPoint& In)
{
	return FString::Printf(TEXT("%dx%d"), In.X, In.Y);
}

float UDisplayWidget::BrightnessToGamma(int32 Level01to10)
{
	return FMath::GetMappedRangeValueClamped(FVector2D(1, 10), FVector2D(1.8f, 2.6f), Level01to10);
}

int32 UDisplayWidget::GammaToBrightness(float Gamma)
{
	const float L = FMath::GetMappedRangeValueClamped(FVector2D(1.8f, 2.6f), FVector2D(1, 10), Gamma);
	return FMath::Clamp(FMath::RoundToInt(L), 1, 10);
}

void UDisplayWidget::SaveBrightnessToConfig(float Gamma)
{
	GConfig->SetFloat(SectionGUS, KeyBrightness, Gamma, GGameUserSettingsIni);
	GConfig->Flush(false, GGameUserSettingsIni);
}

float UDisplayWidget::LoadBrightnessFromConfig() const
{
	float Gamma = 2.2f; 
	GConfig->GetFloat(SectionGUS, KeyBrightness, Gamma, GGameUserSettingsIni);
	return Gamma;
}
