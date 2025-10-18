// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseActor.h"
#include "MixGasStove.generated.h"

/**
 * 
 */

class UChildActorComponent;

UENUM(BlueprintType)
enum class EFireState : uint8
{
	Weak,	// ���
	Middle, // �ߺ�
	Strong	// ����
};

UENUM(BlueprintType)
enum class EUseTime : uint8
{
	Half,		// 30��
	OneHour,	// 1�ð�
	OneHalf,	// 1�ð���
	TwoHour,	// 2�ð�
	TwoHalf,	// 2�ð���
	ThreeHour	// 3�ð�
};

USTRUCT(BlueprintType)
struct FMixAnswer : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY()
	EFireState Fire=EFireState::Weak;

	UPROPERTY()
	EUseTime Time=EUseTime::Half;

	UPROPERTY()
	int32 Temperature=90;
};

UCLASS()
class PROJECT0_API AMixGasStove : public ABaseActor
{
	GENERATED_BODY()
	
private:
	const int32 COMPLETE_FIND = 3; // ã�ƾߵǴ� �๰ ����

public:
	AMixGasStove();

	void EndMix();

	UFUNCTION(BlueprintCallable)
	void ResetMix();

protected:
	virtual void BeginPlay()override;
	
	UFUNCTION()
	void OnOverlapGasStove(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapGasStoveEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	void InitCollision();
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	class UBoxComponent* InteractComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StoveMesh;

	UPROPERTY(VisibleAnywhere)
	UChildActorComponent* OatComp;

	UPROPERTY(VisibleAnywhere)
	UChildActorComponent* BrownRiceComp;

	UPROPERTY(VisibleAnywhere)
	UChildActorComponent* CinnamonComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MakingMedicine;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class ACompletedMedicine* CompletedMC;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPlayerMixComponent* MixComp = nullptr;

	static int32 FindMedicine; // ���� ã�� �๰ ����

	UPROPERTY()
	FMixAnswer FirstAnswer;  // ù��° ����

	UPROPERTY()
	FMixAnswer SecondAnswer; // �ι�° ����

	UPROPERTY()
	FMixAnswer ThirdAnswer;  // ����° ����
private:

	bool IsOat = false;
	bool IsBrownRice = false;
	bool IsCinnamon = false;
};
