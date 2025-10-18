// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/PlayerMixComponent.h"
#include "Characters/PlayerCharacters/PlayerCharacter.h"
#include "PlayerControllers/WorldPlayerController.h"
#include "HUD/WorldHUD.h"
#include "Actors/Mix/MixGasStove.h"
#include "Actors/Mix/HintBook.h"

UPlayerMixComponent::UPlayerMixComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerMixComponent::BeginInteraction_Implementation(APlayerCharacter* Player)
{
	
	AHintBook* Hint = Cast< AHintBook>(GetOwner());
	if (Hint)
	{
		Super::BeginInteraction_Implementation(Player);

		if (IsOpen)
		{
			if (AWorldPlayerController* PC = Cast<AWorldPlayerController>(Player->GetController()))
			{
				PC->AddGameIMC();

				if (Cast<AWorldHUD>(PC->GetHUD()))
				{
					Cast<AWorldHUD>(PC->GetHUD())->SetHintBookVisible(false);
				}
			}
			IsOpen = false;
			EndInteraction_Implementation();
		}
		else
		{
			if (AWorldPlayerController* PC = Cast<AWorldPlayerController>(Player->GetController()))
			{
				PC->RemoveGameIMC();

				if (Cast<AWorldHUD>(PC->GetHUD()))
				{
					Cast<AWorldHUD>(PC->GetHUD())->SetHintBookVisible(true);
				}
			}
			IsOpen = true;
			EndInteraction_Implementation();
			Player->Keeping(this);
		}
	}//��Ʈå ����

	if (AMixGasStove::FindMedicine < 3)return; //���� ��ǰ�� �� ��������� ����������

	Super::BeginInteraction_Implementation(Player);

	AMixGasStove* CorrectStove = Cast<AMixGasStove>(GetOwner());



	if (IsOpen)//�ͽ� UI �ݱ�
	{
		if (AWorldPlayerController* PC = Cast<AWorldPlayerController>(Player->GetController()))
		{
			if (CorrectStove)
			{
				PC->AddGameIMC();

				if (Cast<AWorldHUD>(PC->GetHUD()))
				{
					Cast<AWorldHUD>(PC->GetHUD())->SetMedicineMixVisible(false);
					if (AMixGasStove::FindMedicine > 3)
					{
						CorrectStove->EndMix();
					}
			
				}
			}
		}
		IsOpen = false;
		EndInteraction_Implementation();
	}
	else//�ͽ� UI ����
	{
		if (AWorldPlayerController* PC = Cast<AWorldPlayerController>(Player->GetController()))
		{
			if (CorrectStove)
			{
				PC->RemoveGameIMC();

				if (Cast<AWorldHUD>(PC->GetHUD()))
				{
					Cast<AWorldHUD>(PC->GetHUD())->SetMedicineMixVisible(true);
					Cast<AWorldHUD>(PC->GetHUD())->SetMedicineCorrectAnswer(CorrectStove);
				}
			}
		
		}
		IsOpen = true;
		EndInteraction_Implementation();
		Player->Keeping(this);
	}//�����ϱ�
}
