// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/DiceActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"


// Sets default values
ADiceActor::ADiceActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    

}


int32 ADiceActor::GetFaceTowardDir(const FVector& InDir, float MinDot) const
{

    BuildFaceMap();
    if (FaceArrows.Num() == 0) return -1;

    const FVector Dir = InDir.GetSafeNormal();
    float BestDot = -1.f;
    int32 BestVal = -1;

    for (const auto& KV : FaceArrows)
    {
        const int32 Face = KV.Key;
        const UArrowComponent* A = KV.Value.Get();
        if (!A) continue;

        const float Dot = FVector::DotProduct(A->GetForwardVector().GetSafeNormal(), Dir);
        if (Dot > BestDot)
        {
            BestDot = Dot;
            BestVal = Face;
        }
    }
    return (BestDot >= MinDot) ? BestVal : -1;
}

bool ADiceActor::IsSleeping(float LinThresh, float AngThresh) const
{
    if (!Mesh) return true;
    const float Lin = Mesh->GetComponentVelocity().Size();
    const float Ang = Mesh->GetPhysicsAngularVelocityInDegrees().Size();
    return (Lin < LinThresh) && (Ang < AngThresh) && !Mesh->IsAnyRigidBodyAwake();
}

// Called when the game starts or when spawned
void ADiceActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADiceActor::BuildFaceMap() const
{
    if (bFaceMapBuilt) return;
    bFaceMapBuilt = true;
    FaceArrows.Empty();

    TInlineComponentArray<UArrowComponent*> Arrows(this);
    GetComponents(Arrows);
    for (UArrowComponent* A : Arrows)
    {
        if (!A) continue;
        for (const FName& Tag : A->ComponentTags)
        {
            const FString T = Tag.ToString();               
            const FString Prefix = FaceTagPrefix.ToString();
            if (T.StartsWith(Prefix))
            {
                const int32 Num = FCString::Atoi(*T.RightChop(Prefix.Len()));
                if (Num >= 1 && Num <= 6)
                {
                    FaceArrows.FindOrAdd(Num) = A;
                }
            }
        }
    }
}



// Called every frame
void ADiceActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

