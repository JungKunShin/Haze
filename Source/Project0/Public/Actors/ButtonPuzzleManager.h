#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IStageDependent.h"
#include "ButtonPuzzleManager.generated.h"

class AEmergencyButton;

UCLASS()
class PROJECT0_API AButtonPuzzleManager : public AActor, public IIStageDependent
{
    GENERATED_BODY()

    public:
    AButtonPuzzleManager();

    UPROPERTY(EditAnywhere, Category = "Spawn")
    TSubclassOf<AActor> PushTestClass;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    TSubclassOf<AActor> PushTestClassB;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    float SpawnHeight = 10.f;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    float SpawnRadius = 600.f;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    float MinSeparation = 120.f;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    int32 SpawnEachCount = 3;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    int32 MaxPlacementTriesPerActor = 25;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    FRotator ClassARotation = FRotator::ZeroRotator;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    FRotator ClassBRotation = FRotator(0.f, -90.f, 0.f);

    UPROPERTY(EditAnywhere, Category = "Spawn")
    FName SpawnedTag = "BTN_Spawned";

protected:
    virtual void BeginPlay() override;

    UPROPERTY()
    TMap<FName, AEmergencyButton*> Buttons;

    UPROPERTY(Transient)
    TArray<TObjectPtr<AActor>> SpawnedActors;

    bool bHasActiveSpawn = false;

    UFUNCTION()
    void OnAnyButtonToggled(AEmergencyButton* Button);

    void CheckGoal();
    void SpawnPushTests();
    void GenerateSpawnPositions(int32 TotalCount, TArray<FVector>& OutPositions) const;
    bool TryPickLocation(const TArray<FVector>& Taken, FVector& OutLoc) const;
    AActor* SpawnAt(TSubclassOf<AActor> ClassToSpawn, const FVector& Location, const FRotator& Rotation);

    UPROPERTY(EditAnywhere, Category = "NPC")
    TObjectPtr<class AGrandFatherCharacter> Grandfather;

    UFUNCTION(BlueprintCallable, Category = "Spawn")
    void ClearSpawned();

public:
    virtual void Tick(float DeltaTime) override;
    virtual void OnReset_Implementation() override;
};
