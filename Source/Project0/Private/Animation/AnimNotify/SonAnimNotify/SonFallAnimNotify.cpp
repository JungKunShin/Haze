// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/SonAnimNotify/SonFallAnimNotify.h"
#include "Characters/PlayerCharacters/PlayerCharacter.h"

void USonFallAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp)return;

	APlayerCharacter* PlayerCh = Cast< APlayerCharacter>(MeshComp->GetOwner());
	if (!PlayerCh)return;

	PlayerCh->CharacterPlayMontage(PlayerCh->DeathMontage, "Crawl");
}
