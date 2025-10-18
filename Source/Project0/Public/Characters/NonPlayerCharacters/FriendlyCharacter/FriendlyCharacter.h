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

	//������� ���� ����
	UPROPERTY(EditAnywhere, Category = "Follow")
	float TriggerRadius = 200.f;

	//����� �� �ִ� ����
	UPROPERTY(EditAnywhere, Category = "Follow")
	float KeepFollowRadius = 1200.f;


	//�÷��̾�� ���� �Ÿ�
	UPROPERTY(EditAnywhere, Category = "Follow")
	float AcceptRadius = 150.f;

	//��� ���û �ֱ�
	UPROPERTY(EditAnywhere, Category = "Follow")
	float RepathInterval = 0.3f;

	//����Ʈ �Ϸ� �� true�� ����
	UPROPERTY(EditAnywhere, Category = "Follow")
	bool bRequireQuestUnlock = false;

	//�����
	UPROPERTY(EditAnywhere, Category = "Follow")
	bool bFollowDebug = true;



	//���ʿ� �ɾ��ִ� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pose")
	class UAnimSequence* SitLoop =nullptr;

	//�ɾ��ִ�->�Ͼ�� ��Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pose")
	class UAnimMontage* SittingUpMontage = nullptr;

	//�Ÿ� ��� �ȷο� ����
	void UpdateFollowByDistance();
	
	//�ȷο� ����-����-����
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

	//��Ÿ�� ���������� ��ٸ��� Ÿ�̸�(������)
	FTimerHandle StartFollowDelayHandle;

	//�Ͼ�� �� ������ ����
	UPROPERTY(VisibleAnywhere, Category = "Follow")
	bool bIsStandingUp = false;

	UFUNCTION()
	void StartFollowAfterDelay();



	
};
