// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/PlayerInteractableCharacterComp.h"

#include "GameFramework/Character.h"

void UPlayerInteractableCharacterComp::BeginFocus_Implementation()
{
	Super::BeginFocus_Implementation();

	ACharacter* OwnerCharacter = GetOwner<ACharacter>();
	if (OwnerCharacter) {
		// ������ �������� ��Ƽ������ �����ֱ� ���ؼ� MaxDrawDistance�� ������(�ϴ� 5000)���� �ø���
		OwnerCharacter->GetMesh()->SetOverlayMaterialMaxDrawDistance(5000.0f);
	}
}

void UPlayerInteractableCharacterComp::EndFocus_Implementation()
{
	Super::EndFocus_Implementation();

	ACharacter* OwnerCharacter = GetOwner<ACharacter>();
	if (OwnerCharacter) {
		// ������ �������� ��Ƽ������ ���� ���ؼ� MaxDrawDistance�� ������(�ϴ� 0.1)���� ���̱�
		OwnerCharacter->GetMesh()->SetOverlayMaterialMaxDrawDistance(0.1f);
	}
}

void UPlayerInteractableCharacterComp::BeginInteraction_Implementation(APlayerCharacter* Player)
{

}

void UPlayerInteractableCharacterComp::EndInteraction_Implementation()
{

}
