// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BaseActor.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerControllers/BasePlayerController.h"
#include "Manager/SoundManager.h"

// Sets default values
ABaseActor::ABaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseActor::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(DelaySoundTH, this, &ABaseActor::BeginSoundInit, 0.5f,false);
	
}

void ABaseActor::BeginSoundInit()
{
	if (BeginSound)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		ABasePlayerController* BPC = Cast< ABasePlayerController>(PC);
		UAudioComponent* BeginComp = NewObject<UAudioComponent>(this);

		BeginComp->SetSound(BeginSound);

		if (AttenuationSetting)BeginComp->AttenuationSettings = AttenuationSetting;

		BeginComp->RegisterComponent();
		BeginComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		BeginComp->SetWorldLocation(GetActorLocation());

		BeginComp->SetVolumeMultiplier(BPC->GetSoundManager()->MasterVolume * BPC->GetSoundManager()->EffectVolume);
		BeginComp->Play();
		BPC->GetSoundManager()->ActiveEffectComponents.Add(BeginComp);

		BeginComp->OnAudioFinishedNative.AddLambda(
			[this, BeginComp, BPC](UAudioComponent* FinishedComp)
			{
				BPC->GetSoundManager()->ActiveEffectComponents.Remove(FinishedComp);
				FinishedComp->DestroyComponent();
			}
		);
	}


}

void ABaseActor::InteractSoundInit()
{
	if (InteractSound)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		ABasePlayerController* BPC = Cast< ABasePlayerController>(PC);
		UAudioComponent* InteractComp = NewObject<UAudioComponent>(this);

		InteractComp->SetSound(InteractSound);

		if (AttenuationSetting)InteractComp->AttenuationSettings = AttenuationSetting;

		InteractComp->RegisterComponent();
		InteractComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		InteractComp->SetWorldLocation(GetActorLocation());

		InteractComp->SetVolumeMultiplier(BPC->GetSoundManager()->MasterVolume * BPC->GetSoundManager()->EffectVolume);
		InteractComp->Play();
		BPC->GetSoundManager()->ActiveEffectComponents.Add(InteractComp);

		InteractComp->OnAudioFinishedNative.AddLambda(
			[this, InteractComp, BPC](UAudioComponent* FinishedComp)
			{
				BPC->GetSoundManager()->ActiveEffectComponents.Remove(FinishedComp);
				FinishedComp->DestroyComponent();
			}
		);
	}
}

// Called every frame
void ABaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

