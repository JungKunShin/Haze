// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/TreasureChest.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Actors/DiceCarpet.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/PrimitiveComponent.h"
#include "Components/ArrowComponent.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogDicePuzzle, Log, All);


// Sets default values
ATreasureChest::ATreasureChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
}

void ATreasureChest::OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
    if (NotifyName == FName("EjectMoney"))
    {
        EjectMoney();
    }
}

void ATreasureChest::EjectMoney()
{
    const FVector Origin = (EjectDir ? EjectDir->GetComponentLocation() : Mesh->GetComponentLocation());
    const FVector Fwd = (EjectDir ? EjectDir->GetForwardVector() : GetActorForwardVector());

    for (AActor* A : MoneyActors)
    {
        if (!IsValid(A)) continue;

        //루트나 첫 프리미티브 컴포넌트 찾아서 물리 적용
        UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(A->GetRootComponent());
        if (!Prim)
        {
            TArray<UPrimitiveComponent*> Comps; A->GetComponents<UPrimitiveComponent>(Comps);
            if (Comps.Num() > 0) Prim = Comps[0];
        }
        if (!Prim) continue;

        A->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        A->SetActorHiddenInGame(false);
        Prim->SetSimulatePhysics(true);
        Prim->WakeAllRigidBodies();

        const FVector Dir = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(Fwd, EjectConeDeg);
        Prim->AddImpulseAtLocation(Dir * EjectImpulse, Origin);
    }
}

// Called when the game starts or when spawned
void ATreasureChest::BeginPlay()
{
	Super::BeginPlay();

	if (CarpetA) CarpetA->OnSolvedChanged.AddDynamic(this, &ATreasureChest::OnCarpetChanged);
	if (CarpetB) CarpetB->OnSolvedChanged.AddDynamic(this, &ATreasureChest::OnCarpetChanged);

    if (UAnimInstance* Anim = Mesh->GetAnimInstance())
    {
        Anim->OnPlayMontageNotifyBegin.AddDynamic(this, &ATreasureChest::OnMontageNotifyBegin);
    }
}

void ATreasureChest::OnCarpetChanged(bool None)
{

    TryOpen();
}

void ATreasureChest::TryOpen()
{

    if (bOpened || !CarpetA || !CarpetB) return;

   
    if (CarpetA->IsSolved() && CarpetB->IsSolved())
    {

        if (UAnimInstance* Anim = Mesh->GetAnimInstance())
        {

            if (OpenMontage) Anim->Montage_Play(OpenMontage, 1.f);

        }

        bOpened = true;
    }
}

// Called every frame
void ATreasureChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

