// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SecretEntranceActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "TimerManager.h"

ASecretEntranceActor::ASecretEntranceActor()
{
    PrimaryActorTick.bCanEverTick = false;

    CabinetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cabinet"));
    SetRootComponent(CabinetMesh);

    //경첩 기준 정의용 Arrow
    HingeArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("HingeArrow"));
    HingeArrow->SetupAttachment(CabinetMesh);
}

void ASecretEntranceActor::BeginPlay()
{
    Super::BeginPlay();

    if (!CabinetMesh)
        CabinetMesh = FindComponentByClass<UStaticMeshComponent>();

    if (!HingeArrow)
    {
        //이름-태그로 자동 찾기 
        TInlineComponentArray<UArrowComponent*> Arrows(this);
        GetComponents(Arrows);
        for (UArrowComponent* A : Arrows)
        {
            if (A->GetName().Contains(TEXT("Hinge")) || A->ComponentTags.Contains("Hinge"))
            {
                HingeArrow = A; break;
            }
        }
        if (!HingeArrow && Arrows.Num() > 0) HingeArrow = Arrows[0];
    }

    SetMirrorActorAndRebind(MirrorActor);
}

void ASecretEntranceActor::OnMirrorBroken(const FChaosBreakEvent& )
{
    if (bTriggered) return;
    bTriggered = true;

    GetWorldTimerManager().SetTimerForNextTick(this, &ASecretEntranceActor::StartSwingOpen);
}




void ASecretEntranceActor::SetMirrorActorAndRebind(AActor* NewMirror)
{
    MirrorActor = NewMirror;
    GetWorldTimerManager().ClearTimer(SwingTimer);
    bTriggered = false;

 
    if (BoundGC)
    {
        BoundGC->OnChaosBreakEvent.RemoveDynamic(this, &ASecretEntranceActor::OnMirrorBroken);
        BoundGC = nullptr;
    }

  
    UGeometryCollectionComponent* GCC =
        (MirrorActor ? MirrorActor->FindComponentByClass<UGeometryCollectionComponent>() : nullptr);

    if (!GCC)
    {
        TArray<UChildActorComponent*> CACs;
        if (MirrorActor) MirrorActor->GetComponents(CACs);
        for (auto* CAC : CACs)
        {
            if (AActor* Child = CAC->GetChildActor())
            {
                GCC = Child->FindComponentByClass<UGeometryCollectionComponent>();
                if (GCC) break;
            }
        }
        if (!GCC && MirrorActor)
        {
            TArray<AActor*> Attached;
            MirrorActor->GetAttachedActors(Attached);
            for (AActor* A : Attached)
            {
                GCC = A->FindComponentByClass<UGeometryCollectionComponent>();
                if (GCC) break;
            }
        }
    }

    if (GCC)
    {
        GCC->SetNotifyBreaks(true);
        GCC->OnChaosBreakEvent.AddUniqueDynamic(this, &ASecretEntranceActor::OnMirrorBroken);
        BoundGC = GCC;
    }
}

void ASecretEntranceActor::StartSwingOpen()
{
    if (!CabinetMesh || !HingeArrow) return;

    //회전 준비(물리-중력 배제)
    CabinetMesh->SetMobility(EComponentMobility::Movable);
    CabinetMesh->SetSimulatePhysics(false);
    CabinetMesh->SetEnableGravity(false);
    CabinetMesh->SetLinearDamping(LinDamping);
    CabinetMesh->SetAngularDamping(AngDamping);

    //피벗-축 : HingeArrow 위치, Up을 사용
    HingePivotWS = HingeArrow->GetComponentLocation();
    HingeAxisWS = HingeArrow->GetUpVector().GetSafeNormal();

    StartRotQ = CabinetMesh->GetComponentQuat();
    StartOffset = CabinetMesh->GetComponentLocation() - HingePivotWS;

    SwingElapsed = 0.f;
    const float TickRate = 1.f / 60.f;
    GetWorldTimerManager().SetTimer(SwingTimer, this, &ASecretEntranceActor::TickSwing, TickRate, true);
}

void ASecretEntranceActor::TickSwing()
{
    if (!CabinetMesh)
    {
        GetWorldTimerManager().ClearTimer(SwingTimer);
        return;
    }

    const float Dt = GetWorld()->GetDeltaSeconds();
    SwingElapsed += Dt;

    const float Alpha = FMath::Clamp(SwingElapsed / FMath::Max(0.01f, SwingDuration), 0.f, 1.f);
    const float Eased = 1.f - FMath::Pow(1.f - Alpha, 3.f);     

    const float Sign = bOpenOutwards ? 1.f : -1.f;
    const float AngleNowRad = FMath::DegreesToRadians(Sign * FMath::Lerp(0.f, SwingAngleDeg, Eased));
    const FQuat  DeltaQ(HingeAxisWS, AngleNowRad);
    const FQuat  NewRot = DeltaQ * StartRotQ;

    FHitResult Hit;
    SetAroundPivot(CabinetMesh, HingePivotWS, NewRot, true, &Hit);

    const bool bDone = (Alpha >= 1.f) || (bStopOnHit && Hit.bBlockingHit);
    if (bDone)
    {
        GetWorldTimerManager().ClearTimer(SwingTimer);
        // 열린 상태 유지
        CabinetMesh->SetEnableGravity(false);
        CabinetMesh->WakeAllRigidBodies();
    }
}

void ASecretEntranceActor::SetAroundPivot(UPrimitiveComponent* Comp, const FVector& PivotWS, const FQuat& NewRot, bool bSweep, FHitResult* OutHit)
{
    const FVector NewLoc = PivotWS + NewRot.RotateVector(StartOffset);
    Comp->SetWorldLocationAndRotation(NewLoc, NewRot, bSweep, OutHit);
}

void ASecretEntranceActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}