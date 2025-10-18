// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/PicturePuzzleWidget.h"
#include "Actors/Pictures/BasePictures.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UPicturePuzzleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibleLeftAndRight();
}

void UPicturePuzzleWidget::SetVisibleLeftAndRight()
{
	if (ABasePictures::IsComplete)
	{
		TEXT_Left->SetVisibility(ESlateVisibility::Hidden);
		TEXT_Right->SetVisibility(ESlateVisibility::Hidden);
		LeftImage->SetVisibility(ESlateVisibility::Hidden);
		RightImage->SetVisibility(ESlateVisibility::Hidden);
	}
}
