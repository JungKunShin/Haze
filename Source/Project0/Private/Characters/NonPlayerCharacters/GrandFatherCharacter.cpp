// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NonPlayerCharacters/GrandFatherCharacter.h"
#include "ActorComponents/QuestSystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ActorComponents/PlayerPickupComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerControllers/BasePlayerController.h"
#include "Manager/SoundManager.h"

AGrandFatherCharacter::AGrandFatherCharacter()
{
	DeathType = EQuestState::GrandFatherDeath;
}

void AGrandFatherCharacter::BeginPlay()
{
	Super::BeginPlay();

    if (UCapsuleComponent* Cap = GetCapsuleComponent())
    {
        Cap->OnComponentHit.AddDynamic(this, &AGrandFatherCharacter::OnCapsuleHit);
        Cap->SetNotifyRigidBodyCollision(true);
    }

    GetWorld()->GetTimerManager().SetTimer(BeginSoundTH, this, &AGrandFatherCharacter::BeginGFSoundInit, 0.5f, false);
}

void AGrandFatherCharacter::BeginGFSoundInit()
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

void AGrandFatherCharacter::OnCapsuleHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!OtherActor || !OtherComp) return;

    //충분히 세게 맞았는지 확인
   // if (NormalImpulse.Size() < HitImpulseLimit) return;

    
    if (!OtherActor->FindComponentByClass<UPlayerPickupComponent>()) return;

   

    ApplyHitSlow();
}

void AGrandFatherCharacter::ApplyHitSlow()
{
    UCharacterMovementComponent* Move = GetCharacterMovement();
    if (!Move) return;

    if (!bHitSlowed)
    {
        CachedMaxSpeed = Move->MaxWalkSpeed; //할아버지 속도 저장
        bHitSlowed = true;
    }
    else
    {
        //이미 슬로우 중이면 타이머만 연장
        GetWorldTimerManager().ClearTimer(HitSlowTimer);
    }

    Move->MaxWalkSpeed = CachedMaxSpeed * HitSlowRate;

    GetWorldTimerManager().SetTimer(
        HitSlowTimer, this, &AGrandFatherCharacter::RestoreHitSlow,
        HitSlowDuration, false
    );
}

void AGrandFatherCharacter::RestoreHitSlow()
{
    if (UCharacterMovementComponent* Move = GetCharacterMovement())
    {
        Move->MaxWalkSpeed = (CachedMaxSpeed > 0.f) ? CachedMaxSpeed : Move->MaxWalkSpeed;
    }
    bHitSlowed = false;
}

void AGrandFatherCharacter::ResetEnemyLocation()
{
    AActor* PositionValue1 = PatrolPos.FindChecked(EPosition::MomsRoom);
    AActor* PositionValue2 = PatrolPos.FindChecked(EPosition::Kitchen);
    AActor* PositionValue3 = PatrolPos.FindChecked(EPosition::RoofTop);

    switch (UQuestSystemComponent::CurQuestState)
    {
    case EQuestState::None:
    case EQuestState::FirstConversation:
    case EQuestState::DaughterBook:
        this->SetActorLocation(PositionValue1->GetActorLocation());
        break;
    case EQuestState::MomTalking:
    case EQuestState::MomBarn:
        this->SetActorLocation(PositionValue2->GetActorLocation());
        break;
    case EQuestState::MomMix:
        this->SetActorLocation(PositionValue3->GetActorLocation());
        break;
    case EQuestState::MazeBefore:
    case EQuestState::MazeMiddle:
    case EQuestState::MazeAfter:
    case EQuestState::SequenceOne:
    case EQuestState::SequenceTwo:
    case EQuestState::End:
    case EQuestState::GrandFatherDeath:
    case EQuestState::ZombieDeath:
    default:
        break;
    }
}

void AGrandFatherCharacter::ChasingSoundStart()
{
    if (ChasingSound&&!TempChasing)
    {
        APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        ABasePlayerController* BPC = Cast< ABasePlayerController>(PC);
        UAudioComponent* ChasingComp = NewObject<UAudioComponent>(this);

        TempChasing = ChasingComp;

        TempChasing->SetSound(ChasingSound);

        if (AttenuationSetting)TempChasing->AttenuationSettings = AttenuationSetting;

        TempChasing->RegisterComponent();
        TempChasing->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        TempChasing->SetWorldLocation(GetActorLocation());

        TempChasing->SetVolumeMultiplier(BPC->GetSoundManager()->MasterVolume * BPC->GetSoundManager()->EffectVolume);
        TempChasing->Play();
        BPC->GetSoundManager()->ActiveEffectComponents.Add(TempChasing);

    }
}

void AGrandFatherCharacter::ChasingSoundEnd()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    ABasePlayerController* BPC = Cast< ABasePlayerController>(PC);

    if (TempChasing)
    {
        BPC->GetSoundManager()->ActiveEffectComponents.Remove(TempChasing);
        TempChasing->DestroyComponent();
        TempChasing = nullptr;
    }
}

