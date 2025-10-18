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
	Weak,	// 약불
	Middle, // 중불
	Strong	// 강불
};

UENUM(BlueprintType)
enum class EUseTime : uint8
{
	Half,		// 30분
	OneHour,	// 1시간
	OneHalf,	// 1시간반
	TwoHour,	// 2시간
	TwoHalf,	// 2시간반
	ThreeHour	// 3시간
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
	const int32 COMPLETE_FIND = 3; // 찾아야되는 약물 갯수

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

	static int32 FindMedicine; // 현재 찾은 약물 갯수

	UPROPERTY()
	FMixAnswer FirstAnswer;  // 첫번째 정답

	UPROPERTY()
	FMixAnswer SecondAnswer; // 두번째 정답

	UPROPERTY()
	FMixAnswer ThirdAnswer;  // 세번째 정답
private:

	bool IsOat = false;
	bool IsBrownRice = false;
	bool IsCinnamon = false;
};
