// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MoveBeforeMaze.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/PlayerCharacters/PlayerCharacter.h"
#include "Characters/NonPlayerCharacters/FriendlyCharacter/FriendlyCharacter.h"

AMoveBeforeMaze::AMoveBeforeMaze()
{
    PrimaryActorTick.bCanEverTick = false;

    Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    RootComponent = Box;

    
    Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Box->SetCollisionObjectType(ECC_WorldDynamic);
    Box->SetCollisionResponseToAllChannels(ECR_Ignore);
    Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    Box->SetGenerateOverlapEvents(true);
    Box->SetBoxExtent(FVector(120.f, 220.f, 180.f), false);

    
    DebugLocation = FVector(7303.755371f, 2439.782715f, 200.0f);
}

void AMoveBeforeMaze::Tick(float DeltaTime)
{
}

void AMoveBeforeMaze::BeginPlay()
{
    Super::BeginPlay();

    Box->OnComponentBeginOverlap.AddDynamic(this, &AMoveBeforeMaze::OnBeginOverlap);

    //플레이어 캐시
    if (ACharacter* PC = UGameplayStatics::GetPlayerCharacter(this, 0))
    {
        CachedPlayer = Cast<APlayerCharacter>(PC);
    }

   
}

void AMoveBeforeMaze::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (bFiredOnce || !OtherActor) return;

    if (APlayerCharacter* P = Cast<APlayerCharacter>(OtherActor))
    {
        bTouchedPlayer = true;
        CachedPlayer = P;
    }
    else if (AFriendlyCharacter* F = Cast<AFriendlyCharacter>(OtherActor))
    {
        // 참조가 비어있다면 자동 대입
        if (!MomRef && F->ActorHasTag(TEXT("Mom")))    MomRef = F;
        if (!SisterRef && F->ActorHasTag(TEXT("Sister"))) SisterRef = F;

        if (F == MomRef || F->ActorHasTag(TEXT("Mom")))
            bTouchedMom = true;
        else if (F == SisterRef || F->ActorHasTag(TEXT("Sister")))
            bTouchedSister = true;
    }

    TryExecute();
}

void AMoveBeforeMaze::TryExecute()
{
    if (bFiredOnce) return;

    if (!(bTouchedPlayer && bTouchedMom && bTouchedSister))
        return;

    APlayerCharacter* Player = CachedPlayer.Get();
    if (!Player || !MomRef || !SisterRef)
        return;

    const bool bMomUnlocked = MomRef->IsFollowUnlocked(Player);
    const bool bSisterUnlocked = SisterRef->IsFollowUnlocked(Player);
    if (!(bMomUnlocked && bSisterUnlocked))
        return;
    
    if (Player && Player->QuestSystem)
    {
        Player->QuestSystem->EnterMazeAfter();  
    }

    
  
    bFiredOnce = true;

   
    Player->ToggleView(); 

    Player->SetActorLocation(DebugLocation, false, nullptr, ETeleportType::TeleportPhysics);

    if (MomRef)
        MomRef->SetActorLocation(DebugLocation + MomOffset, false, nullptr, ETeleportType::TeleportPhysics);

    if (SisterRef)
        SisterRef->SetActorLocation(DebugLocation + SisterOffset, false, nullptr, ETeleportType::TeleportPhysics);

   
}

