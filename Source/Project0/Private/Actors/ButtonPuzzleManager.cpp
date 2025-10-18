#include "Actors/ButtonPuzzleManager.h"
#include "Actors/EmergencyButton.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Characters/NonPlayerCharacters/GrandFatherCharacter.h"

AButtonPuzzleManager::AButtonPuzzleManager()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AButtonPuzzleManager::BeginPlay()
{
    Super::BeginPlay();

    TArray<AActor*> Found;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEmergencyButton::StaticClass(), Found);
    for (AActor* A : Found)
    {
        if (auto* B = Cast<AEmergencyButton>(A))
        {
            const FName Key = (B->Tags.Num() > 0) ? B->Tags[0] : NAME_None;
            Buttons.FindOrAdd(Key) = B;
            B->OnButtonToggled.AddDynamic(this, &AButtonPuzzleManager::OnAnyButtonToggled);
        }
    }
}

void AButtonPuzzleManager::OnAnyButtonToggled(AEmergencyButton*)
{
    CheckGoal();
}

void AButtonPuzzleManager::CheckGoal()
{
    if (bHasActiveSpawn) return;

    auto IsOn = [&](FName N)
        {
            if (AEmergencyButton** P = Buttons.Find(N)) return (*P && (*P)->bIsOn);
            return false;
        };

    if (IsOn("One") && IsOn("Four") && IsOn("Six"))
    {
        SpawnPushTests();
        bHasActiveSpawn = true;
    }
}

void AButtonPuzzleManager::SpawnPushTests()
{
    int32 Total = 0;
    if (PushTestClass)  Total += SpawnEachCount;
    if (PushTestClassB) Total += SpawnEachCount;
    if (Total <= 0) return;

    TArray<FVector> Positions;
    GenerateSpawnPositions(Total, Positions);

    const FRotator Base = GetActorRotation();
    int32 Index = 0;

    if (PushTestClass)
    {
        for (int32 i = 0; i < SpawnEachCount; ++i)
        {
            SpawnAt(PushTestClass, Positions[Index++], Base + ClassARotation);
        }
    }
    if (PushTestClassB)
    {
        for (int32 i = 0; i < SpawnEachCount; ++i)
        {
            SpawnAt(PushTestClassB, Positions[Index++], Base + ClassBRotation);
        }
    }
}

AActor* AButtonPuzzleManager::SpawnAt(TSubclassOf<AActor> ClassToSpawn,
    const FVector& Loc, const FRotator& Rot)
{
    if (!ClassToSpawn) return nullptr;

    FActorSpawnParameters P;
    P.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    P.Owner = this;

    AActor* NewA = GetWorld()->SpawnActor<AActor>(ClassToSpawn, Loc, Rot, P);
    if (NewA)
    {
        SpawnedActors.Add(NewA);
        NewA->Tags.AddUnique(SpawnedTag);

        if (Grandfather)
        {
            // 할아버지 측: 이동 스윕에서 NewA 무시
            Grandfather->MoveIgnoreActorAdd(NewA);

            // 주사위 측: 이동 시 할아버지 무시 + 내비 영향 제거
            TArray<UPrimitiveComponent*> PrimComps;
            NewA->GetComponents<UPrimitiveComponent>(PrimComps);
            for (UPrimitiveComponent* PC : PrimComps)
            {
                PC->IgnoreActorWhenMoving(Grandfather, true);
                PC->SetCanEverAffectNavigation(false); // navmesh 장애물로 간주 안 함
            }
        }
    }
    return NewA;
}
void AButtonPuzzleManager::GenerateSpawnPositions(int32 TotalCount, TArray<FVector>& OutPositions) const
{
    OutPositions.Reset();
    TArray<FVector> Taken;

    for (int32 i = 0; i < TotalCount; ++i)
    {
        FVector L;
        int32 Tries = 0;
        while (Tries++ < MaxPlacementTriesPerActor && !TryPickLocation(Taken, L)) {}
        if (Tries >= MaxPlacementTriesPerActor) L = GetActorLocation();
        Taken.Add(L);
        OutPositions.Add(L);
    }
}

bool AButtonPuzzleManager::TryPickLocation(const TArray<FVector>& Taken, FVector& OutLoc) const
{
    const FVector Origin = GetActorLocation();
    const float Angle = FMath::FRandRange(0.f, 360.f);
    const float Radius = FMath::FRandRange(0.f, SpawnRadius);
    FVector Offset = FVector(FMath::Cos(FMath::DegreesToRadians(Angle)) * Radius,
        FMath::Sin(FMath::DegreesToRadians(Angle)) * Radius,
        0.f);
    FVector Candidate = Origin + Offset + FVector(0.f, 0.f, SpawnHeight);

    for (const FVector& T : Taken)
    {
        if (FVector::Dist2D(T, Candidate) < MinSeparation) return false;
    }
    OutLoc = Candidate;
    return true;
}

void AButtonPuzzleManager::ClearSpawned()
{
    for (AActor* A : SpawnedActors)
    {
        if (IsValid(A)) A->Destroy();
    }
    SpawnedActors.Empty();

    auto NukeByClass = [&](TSubclassOf<AActor> C)
        {
            if (!*C) return;
            TArray<AActor*> Found;
            UGameplayStatics::GetAllActorsOfClass(GetWorld(), *C, Found);
            for (AActor* A : Found)
            {
                if (IsValid(A) && A->GetOwner() == this) A->Destroy();
            }
        };
    NukeByClass(PushTestClass);
    NukeByClass(PushTestClassB);

    TArray<AActor*> Tagged;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), SpawnedTag, Tagged);
    for (AActor* A : Tagged)
    {
        if (IsValid(A)) A->Destroy();
    }

    bHasActiveSpawn = false;
}

void AButtonPuzzleManager::OnReset_Implementation()
{
    UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("BPM::OnReset  Spawned=%d"), SpawnedActors.Num()), true, true, FLinearColor::Red, 1.5f);
    ClearSpawned();
}

void AButtonPuzzleManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
