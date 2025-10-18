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
			PC->AddGameIMC();							// ���� IMC �ٽ� �־��ֱ�
			Player->SetActorHiddenInGame(false);		// ĳ���� ���� Ǯ���ֱ�
			Player->OverlapSizeUp(true);				// ��ȣ�ۿ� ������ ���� ũ��� �ٿ��ֱ�
			PC->SetViewTargetWithBlend(Player, 1.0f);	// �÷��̾� ī�޶�� ���󺹱�
			
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
		//��ȣ�ۿ� ���� �� �̵� �Է� ����
		if (AWorldPlayerController* PC = Cast<AWorldPlayerController>(Player->GetController()))
		{
			PC->RemoveGameIMC();							// ���� IMC ����
			Player->SetActorHiddenInGame(true);				// ���ڷ� ���ε� ��, ĳ���� �����
			Player->OverlapSizeUp(false);					// ��ȣ�ۿ� ������ ũ�� Ű���
			PC->SetViewTargetWithBlend(GetOwner(), 1.0f);	// ���� ī�޶�� ���� �ٲٱ�

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
