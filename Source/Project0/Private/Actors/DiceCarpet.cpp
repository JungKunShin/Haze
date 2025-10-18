// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/DiceCarpet.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Actors/DiceActor.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogDicePuzzle, Log, All);

ADiceCarpet::ADiceCarpet()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ADiceCarpet::AutoFindComponents()
{
    if (!Trigger)
    {
        //태그 우선, 없으면 첫 Box
        TInlineComponentArray<UBoxComponent*> Boxes(this);
        GetComponents(Boxes);
        for (UBoxComponent* B : Boxes)
        {
            if (B->ComponentTags.Contains(TriggerTag)) { Trigger = B; break; }
        }
        if (!Trigger && Boxes.Num() > 0) Trigger = Boxes[0];
    }

    if (!Front)
    {
        //태그 우선, 없으면 첫 Arrow
        TInlineComponentArray<UArrowComponent*> Arrows(this);
        GetComponents(Arrows);
        for (UArrowComponent* A : Arrows)
        {
            if (A->ComponentTags.Contains(FrontTag)) { Front = A; break; }
        }
        if (!Front && Arrows.Num() > 0) Front = Arrows[0];
    }
}

void ADiceCarpet::BeginPlay()
{
    Super::BeginPlay();
    AutoFindComponents();

    if (Trigger)
    {
        Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        Trigger->SetGenerateOverlapEvents(true);
        Trigger->OnComponentBeginOverlap.AddDynamic(this, &ADiceCarpet::OnBeginOverlap);
        Trigger->OnComponentEndOverlap.AddDynamic(this, &ADiceCarpet::OnEndOverlap);
    }

    GetWorldTimerManager().SetTimer(PollHandle, this, &ADiceCarpet::Poll, 0.1f, true);
}

void ADiceCarpet::OnBeginOverlap(UPrimitiveComponent*, AActor* Other, UPrimitiveComponent*, int32, bool, const FHitResult&)
{
    CurrentDice = Cast<ADiceActor>(Other);
}

void ADiceCarpet::OnEndOverlap(UPrimitiveComponent*, AActor* Other, UPrimitiveComponent*, int32)
{
    if (Other == CurrentDice)
    {
        CurrentDice = nullptr;
        if (bSolved) { bSolved = false; OnSolvedChanged.Broadcast(false); }
    }
}

void ADiceCarpet::Poll()
{
    bool NewSolved = false;

    if (CurrentDice && Trigger && Trigger->IsOverlappingActor(CurrentDice))
    {
        if (CurrentDice->IsSleeping() && Front)
        {
            const int32 Face = CurrentDice->GetFaceTowardDir(Front->GetForwardVector());

      
            NewSolved = (Face == RequiredValue);
        }
    }

    if (NewSolved != bSolved)
    {
        bSolved = NewSolved;
     OnSolvedChanged.Broadcast(bSolved);
    }
}