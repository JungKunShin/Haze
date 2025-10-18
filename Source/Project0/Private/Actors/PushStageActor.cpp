// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/PushStageActor.h"
#include "Components/BoxComponent.h"
#include "Characters/PlayerCharacters/PlayerCharacter.h"
#include "ActorComponents/QuestSystemComponent.h"
#include "GameInstances/StageGameInstance.h"


APushStageActor::APushStageActor()
{
    Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
    Trigger->SetupAttachment(RootComponent);
    Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Trigger->SetCollisionObjectType(ECC_WorldDynamic);
    Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    Trigger->OnComponentBeginOverlap.AddDynamic(this, &APushStageActor::OnTriggerBegin);
}


void APushStageActor::OnTriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    const double Now = FPlatformTime::Seconds();

    if (Now < NextAllowedTime) return;

    MakeThisCurrentStage();

    NextAllowedTime = Now + CooldownSeconds;
}