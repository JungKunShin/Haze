// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/PlayerRotationComponent.h"
#include "Characters/PlayerCharacters/PlayerCharacter.h"
#include "PlayerControllers/WorldPlayerController.h"
#include "Actors/Pictures/BasePictures.h"
#include "HUD/WorldHUD.h"
#include "Kismet/GameplayStatics.h"

UPlayerRotationComponent::UPlayerRotationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerRotationComponent::BeginInteraction_Implementation(APlayerCharacter* Player)
{
	Super::BeginInteraction_Implementation(Player);

	if (IsZoom)
	{
		if (AWorldPlayerController* PC = Cast<AWorldPlayerController>(Player->GetController()))
		{
			PC->AddGameIMC();							// 게임 IMC 다시 넣어주기
			Player->SetActorHiddenInGame(false);		// 캐릭터 숨김 풀어주기
			Player->OverlapSizeUp(true);				// 상호작용 오버랩 원래 크기로 줄여주기
			PC->SetViewTargetWithBlend(Player, 1.0f);	// 플레이어 카메라로 원상복구
			
			if (Cast<AWorldHUD>(PC->GetHUD()))
			{
				Cast<AWorldHUD>(PC->GetHUD())->SetPicturePuzzleVisible(false);
			}
		}
		IsZoom = false;
		EndInteraction_Implementation();
	}
	else
	{
		//상호작용 시작 때 이동 입력 끄기
		if (AWorldPlayerController* PC = Cast<AWorldPlayerController>(Player->GetController()))
		{
			PC->RemoveGameIMC();							// 게임 IMC 삭제
			Player->SetActorHiddenInGame(true);				// 액자로 줌인될 때, 캐릭터 숨기기
			Player->OverlapSizeUp(false);					// 상호작용 오버랩 크기 키우기
			PC->SetViewTargetWithBlend(GetOwner(), 1.0f);	// 액자 카메라로 시점 바꾸기

			if (Cast<AWorldHUD>(PC->GetHUD()))
			{
				Cast<AWorldHUD>(PC->GetHUD())->SetPicturePuzzleVisible(true);
			}
		}
		IsZoom = true;
		EndInteraction_Implementation();
		Player->Keeping(this);
	}
}

void UPlayerRotationComponent::LeftRotationPicture()
{
	if (ABasePictures::IsComplete)return;

	if (ABasePictures* Picture = Cast< ABasePictures>(GetOwner()))
	{
		Picture->LeftRotatePicture();
	}
}

void UPlayerRotationComponent::RightRotationPicture()
{
	if (ABasePictures::IsComplete)return;

	if (ABasePictures* Picture = Cast< ABasePictures>(GetOwner()))
	{
		Picture->RightRotatePicture();
	}
}

void UPlayerRotationComponent::ResetRotComp()
{
	if (IsZoom)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (AWorldPlayerController* WPC = Cast<AWorldPlayerController>(PC))
		{
			WPC->AddGameIMC();
			WPC->GetCharacter()->SetActorHiddenInGame(false);
			WPC->SetViewTargetWithBlend(WPC->GetCharacter(), 1.0f);
			EndInteraction_Implementation();

			if (Cast<AWorldHUD>(WPC->GetHUD()))
			{
				Cast<AWorldHUD>(WPC->GetHUD())->SetPicturePuzzleVisible(false);
			}
		}
	}

	IsZoom = false;

}
