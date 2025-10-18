// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/FootStepAnimNotify.h"
#include "Characters/BaseCharacter.h"
#include "ActorComponents/QuestSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerControllers/BasePlayerController.h"
#include "Manager/SoundManager.h"

void UFootStepAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp || !HallSound )return;

	ABaseCharacter* Character = Cast< ABaseCharacter>(MeshComp->GetOwner());

	if (!Character)return;

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	ABasePlayerController* BPC = Cast< ABasePlayerController>(PC);

	if (!BPC) return;

	if (IsLeft)
	{
		UGameplayStatics::PlaySoundAtLocation(Character, HallSound, MeshComp->GetSocketLocation("LeftToeBase"), BPC->GetSoundManager()->EffectVolume, 1.0f, 0.0f, AttenuationSetting);
	}
	else
	{
		UGameplayStatics::PlaySoundAtLocation(Character, HallSound, MeshComp->GetSocketLocation("RightToeBase"), BPC->GetSoundManager()->EffectVolume, 1.0f, 0.0f, AttenuationSetting);
	}
}
