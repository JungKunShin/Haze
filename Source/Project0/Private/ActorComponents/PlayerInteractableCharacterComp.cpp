// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/PlayerInteractableCharacterComp.h"

#include "GameFramework/Character.h"

void UPlayerInteractableCharacterComp::BeginFocus_Implementation()
{
	Super::BeginFocus_Implementation();

	ACharacter* OwnerCharacter = GetOwner<ACharacter>();
	if (OwnerCharacter) {
		// 설정된 오버레이 머티리얼을 보여주기 위해서 MaxDrawDistance를 적정량(일단 5000)으로 늘리기
		OwnerCharacter->GetMesh()->SetOverlayMaterialMaxDrawDistance(5000.0f);
	}
}

void UPlayerInteractableCharacterComp::EndFocus_Implementation()
{
	Super::EndFocus_Implementation();

	ACharacter* OwnerCharacter = GetOwner<ACharacter>();
	if (OwnerCharacter) {
		// 설정된 오버레이 머티리얼을 끄기 위해서 MaxDrawDistance를 적정량(일단 0.1)으로 줄이기
		OwnerCharacter->GetMesh()->SetOverlayMaterialMaxDrawDistance(0.1f);
	}
}

void UPlayerInteractableCharacterComp::BeginInteraction_Implementation(APlayerCharacter* Player)
{

}

void UPlayerInteractableCharacterComp::EndInteraction_Implementation()
{

}
