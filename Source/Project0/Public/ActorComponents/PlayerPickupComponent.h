// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/PlayerInteractableComponent.h"
#include "Chaos/ChaosEngineInterface.h"
#include "PlayerPickupComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT0_API UPlayerPickupComponent : public UPlayerInteractableComponent
{
	GENERATED_BODY()
	

public:

	UPlayerPickupComponent();

	//손 소켓 이름 
	UPROPERTY(EditAnywhere, Category = "Pickup")
	FName AttachSocketName = TEXT("hand_rSocket");

	//던질 때 힘 
	UPROPERTY(EditAnywhere, Category = "Pickup")
	float ThrowImpulse = 1000.f;

	//던질 때 각도
	UPROPERTY(EditAnywhere, Category = "Pickup")
	float ThrowDegree = 20.f;

	//던진 후 N초 후 Pawn을 무시하는 구조(콜리전 꼬임 방지)
	UPROPERTY(EditAnywhere, Category = "Pickup")
	float ThrowPawnIgnoreDelay = 1.0f ;

	

	//깨지는 범위
	UPROPERTY(EditAnywhere, Category = "Throw")
	float StrainRadius = 120.f;

	//깨지는 구간(입력)
	UPROPERTY(EditAnywhere, Category = "Throw")
	FVector2D ImpactToMagnitude = FVector2D(5000.f, 30000.f);

	//출력 strain 구간
	UPROPERTY(EditAnywhere, Category = "Throw")
	FVector2D MagnitudeRange = FVector2D(25000.f, 100000.f);  

	//해당 태그가 있어야 깨뜨림
	UPROPERTY(EditAnywhere, Category = "Throw")
	FName BreakableTag = "Breakable";   

	UPROPERTY(BlueprintReadOnly, Category = "Throw")
	TWeakObjectPtr<class APlayerCharacter> LastThrower;

	UFUNCTION(BlueprintPure, Category = "Throw")
	APlayerCharacter* GetLastThrower() const { return LastThrower.Get(); }


	UPROPERTY(EditAnywhere, Category = "Throw")
	bool bOneHitOneBreak = true;
	
	virtual void BeginPlay() override;

	virtual void BeginInteraction_Implementation(APlayerCharacter* Player) override;

	
	UFUNCTION(BlueprintCallable)
	void AttachToHand();

	void Throw();
	
	void PutDown();

	UFUNCTION(BlueprintCallable)
	void ResetPickUpComp();

private:

	UFUNCTION()
	void OnThrownHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);



	//할아버지 제외 다른 타깃 제외
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throw", meta = (AllowPrivateAccess = "true"))
	bool bAssistGrandFatherOnly = true;

	//목표물 자동 조준 거리 한계
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throw", meta = (AllowPrivateAccess = "true"))
	float AssistMaxRange = 1600.f;

	//목표물 자동 조준 각도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throw", meta = (AllowPrivateAccess = "true", ClampMin = "1.0", UIMin = "1.0", ClampMax = "45.0", UIMax = "20.0"))
	float AssistHalfAngleDeg = 12.f;

	//장애물 가려지면 자동조준 비활성화
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throw", meta = (AllowPrivateAccess = "true"))
	bool bRequireLineOfSight = true;

	//디버그 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throw", meta = (AllowPrivateAccess = "true"))
	bool bDrawAssistDebug = false;

	//누가 들고 있는지 확인
	class APlayerCharacter* Holder = nullptr;

	//본래 피직스 값 저장
	bool bSavedSimulatePhysics = false;



	//Pickup이 파괴되면 다시 입력 돌려옴
	UFUNCTION()
	void OnDestroyedPickup(AActor* DestroyedActor);

	void SetPawnIgnore(bool bIgnore);
	FTimerHandle PawnIgnoreTimer;

	UFUNCTION()
	void RestorePawnIgnore();

	//할아버지 참조 유지
	UPROPERTY(Transient)
	TWeakObjectPtr<class AGrandFatherCharacter> CachedGrandFather;

	
};
