// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/NonPlayerCharacters/NonPlayerCharacter.h"
#include "FriendlyCharacter.generated.h"

/**
 * 
 */
class APlayerCharacter;

UENUM(BlueprintType)
enum class EFriendlyState : uint8
{
	SittingLocked,
	Following
};

UCLASS()
class PROJECT0_API AFriendlyCharacter : public ANonPlayerCharacter
{
	GENERATED_BODY()
	
public:
	AFriendlyCharacter();

	UFUNCTION(BlueprintCallable, Category = "Follow")
	void ForceStandUpAndFollow(class APlayerCharacter* Player);

	UFUNCTION(BlueprintCallable, Category = "Follow")
	void SetRequireQuestUnlock(bool bNew) { bRequireQuestUnlock = bNew; }

	bool IsFollowUnlocked(APlayerCharacter*) const;

protected:

	virtual void BeginPlay() override;

	//따라오기 시작 범위
	UPROPERTY(EditAnywhere, Category = "Follow")
	float TriggerRadius = 200.f;

	//따라올 수 있는 범위
	UPROPERTY(EditAnywhere, Category = "Follow")
	float KeepFollowRadius = 1200.f;


	//플레이어에게 붙을 거리
	UPROPERTY(EditAnywhere, Category = "Follow")
	float AcceptRadius = 150.f;

	//경로 재요청 주기
	UPROPERTY(EditAnywhere, Category = "Follow")
	float RepathInterval = 0.3f;

	//퀘스트 완료 때 true로 변경
	UPROPERTY(EditAnywhere, Category = "Follow")
	bool bRequireQuestUnlock = false;

	//디버깅
	UPROPERTY(EditAnywhere, Category = "Follow")
	bool bFollowDebug = true;



	//최초에 앉아있는 상태
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pose")
	class UAnimSequence* SitLoop =nullptr;

	//앉아있다->일어나는 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pose")
	class UAnimMontage* SittingUpMontage = nullptr;

	//거리 기반 팔로우 로직
	void UpdateFollowByDistance();
	
	//팔로우 시작-유지-해제
	bool TryStartFollow(APlayerCharacter* Player);
	void StartFollow(APlayerCharacter* Player);
	void StopFollow();
	void TickFollow();

	void SetSitPose();
	void ExitSitPose();

	UPROPERTY(BlueprintReadOnly, Category = "Follow")
	TWeakObjectPtr<APlayerCharacter> FollowTarget;

	UPROPERTY(BlueprintReadOnly, Category = "Follow")
	bool bIsFollowing = false;

	FTimerHandle FollowLogicTimer;
	FTimerHandle RepathTimer;

	//몽타주 끝날때까지 기다리는 타이머(안전용)
	FTimerHandle StartFollowDelayHandle;

	//일어나는 중 재진입 방지
	UPROPERTY(VisibleAnywhere, Category = "Follow")
	bool bIsStandingUp = false;

	UFUNCTION()
	void StartFollowAfterDelay();



	
};
