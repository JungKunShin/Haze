// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/NonPlayerCharacters/EnemyCharacter.h"
#include "GrandFatherCharacter.generated.h"


class UQuestSystemComponent;
/**
 * 
 */
UENUM(BlueprintType)
enum class EPosition : uint8
{
	Kitchen,
	DaughtersRoom,
	MomsRoom,
	RoofTop,
	BarnFirst,
	BarnSecond,
};

UCLASS()
class PROJECT0_API AGrandFatherCharacter : public AEnemyCharacter
{
	GENERATED_BODY()
	
public:

	AGrandFatherCharacter();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<EPosition,AActor*> PatrolPos;

protected:
	virtual void BeginPlay() override;

	void BeginGFSoundInit();

	UFUNCTION()
	void OnCapsuleHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void ApplyHitSlow();
	void RestoreHitSlow();

	virtual void ResetEnemyLocation()override;

public:
	void ChasingSoundStart();
	void ChasingSoundEnd();

protected:

	//Hit 이후 이동속도 비율
	UPROPERTY(EditAnywhere)
	float HitSlowRate = 0.45f;

	//Hit 지속시간
	UPROPERTY(EditAnywhere)
	float HitSlowDuration = 5.00f;

	//Hit이 작동하는 구간 설정
	UPROPERTY(EditAnywhere)
	float HitImpulseLimit = 200.f;

private:
	FTimerHandle HitSlowTimer;
	float CachedMaxSpeed = 0.f;
	bool bHitSlowed = false;

	FTimerHandle BeginSoundTH;

public:

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* BeginSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* ChasingSound;

	UPROPERTY()
	UAudioComponent* TempChasing = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundAttenuation* AttenuationSetting;
};
