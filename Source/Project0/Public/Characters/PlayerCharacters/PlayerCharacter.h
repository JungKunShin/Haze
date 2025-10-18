// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "InputMappingContext.h"
#include "../../ActorComponents/QuestSystemComponent.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */

class UBoxComponent;
class UPlayerInteractableComponent;
class UQuestSystemComponent;
class UPlayerPickupComponent;
class AActiveCamera;

UCLASS()
class PROJECT0_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:

	APlayerCharacter();

	FORCEINLINE UBoxComponent* GetInteractBox() { return InteractBox; };

	virtual void Move(FVector2D Value);

	virtual void Jump() override;

	void Look(FVector2D Value);

	void ToggleCrouch();

	
	UFUNCTION() 
	void TryStartConversation();


	UFUNCTION(BlueprintCallable)
	void Throw();

	void Interact();

	void TestQuest();

	UFUNCTION(BlueprintCallable)
	void SwitchToThirdPersonView();

	UFUNCTION(BlueprintCallable)
	void SwitchToSideView();
	UFUNCTION(BlueprintCallable)
	void ToggleView();

	UFUNCTION(BlueprintCallable)
	void SetCanInteract(bool Value);

	UFUNCTION(BlueprintCallable)
	void StartInteractTo(UPlayerInteractableComponent* TargetInteractable);
	UFUNCTION(BlueprintCallable)
	void EndInteract();

	//카메라 매니저 위치 연동
	void LockCameraFollow(bool bLock);

	void BeforeDeath(EQuestState _Death);

	//픽업 노티파이 관련
	UFUNCTION(BlueprintCallable)
	void Anim_PickupAttach();

	//끝날 때도 notify 부여해 이동 잠금 해제
	UFUNCTION(BlueprintCallable)
	void Anim_PickupEnd();

	//던지기 노티파이 관련
	UFUNCTION(BlueprintCallable)
	void Anim_ThrowRelease();

	//놓기 노티파이 관련
	UFUNCTION(BlueprintCallable)
	void Anim_PutDownRelease();

	//돌리기 노티파이 관련
	UFUNCTION(BlueprintCallable)
	void Anim_Rotation();

	void LeftRotPicture();
	void RightRotPicture();

	void Keeping(UPlayerInteractableComponent* Current);

	void OverlapSizeUp(bool _IsBig);

	UFUNCTION(BlueprintCallable)
	virtual void PlayDeadScene(AActor* Blendtarget, UAnimMontage* MontageToPlay, const FRotator& Rotation);

	//손에 붙지 않은 대기중 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
	class UPlayerPickupComponent* PendingPickup = nullptr;

	//기본 카메라 스프링암
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom; 

	// 3인칭 카메라 스프링 암
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* ThirdPersonCameraBoom;

	//기본 카메라
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* ThirdPersonCamera; 

	UFUNCTION(BlueprintCallable)
	void AddMappingContext();
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> IMC_Look;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	class UPlayerPickupComponent* HeldPickup = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	class UAnimMontage* PickupMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	class UAnimMontage* PutDownMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Talk")
	float TalkRange = 180.f;            //대화 시작 거리

	UPROPERTY(EditAnywhere, Category = "Talk")
	float TalkProbeRadius = 60.f;       //탐지 반경

	UFUNCTION(BlueprintCallable) 
	void Seq_BeginDialogue_SeqA();

	UFUNCTION(BlueprintCallable) 
	void Seq_BeginDialogue_SeqB();

	UFUNCTION(BlueprintCallable) 
	void Seq_DialogueNext();
	UFUNCTION(BlueprintCallable) 
	void Seq_DialogueFinish();


protected:
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void FindInteractable();

	virtual void SideViewCameraFollowing(float DeltaSeconds);

	virtual void SideViewLook(const FVector2D& Value);
	virtual void ThirdPersonLook(const FVector2D& Value);

	


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	class UAnimMontage* ThrowMontage = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Throw", meta = (AllowPrivateAccess = "true"))
	bool bThrowWindowActive = false;

	UFUNCTION(BlueprintCallable, Category = "Throw")
	void SetThrowWindowActive(bool bActive);

	UFUNCTION(BlueprintPure, Category = "Throw")
	bool IsThrowWindowActive() const { return bThrowWindowActive; }


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanInteract = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsSideView = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)

	TEnumAsByte<ECollisionChannel> InteractLineChannel;

	AActiveCamera* ActiveCam;

	float SideViewArmLength;
	FVector SideViewRelativeLocation;
	FVector SideViewTargetOffset;
	FVector SideViewSocketOffset;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage)
	class UAnimMontage* DeathMontage; //죽는 몽타주

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage)
	class UAnimMontage* InteractMontage; //상호작용 몽타주
	

private:
	FRotator SideViewControlRotation;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))

	UBoxComponent* InteractBox;

	UPROPERTY(VisibleAnywhere)
	UPlayerInteractableComponent* CurrentSelectedInteractable;
	UPROPERTY(VisibleAnywhere)
	UPlayerInteractableComponent* CurrentInteractingInteractable;

	UPROPERTY(VisibleAnywhere)
	TArray<UPlayerInteractableComponent*> CurrentInteractables;

	FVector OriginalSize;
public:

	UPROPERTY(VisibleAnywhere)
	UQuestSystemComponent* QuestSystem; // 퀘스트 시스템



	// 좌우 30도 제한
	UPROPERTY(EditAnywhere, Category = "Camera") 
	float LookLeftRightLimit = 20.0f;  

	// 위아래 5도 제한
	UPROPERTY(EditAnywhere, Category = "Camera") 
	float LookUpDownLimit = 7.0f;  

	//카메라 복귀 딜레이
	UPROPERTY(EditAnywhere, Category = "Camera")
	float ReturnDelay = 0.2f;

	//마지막으로 Look 입력 들어온 시간
	float LastLookTime = 0.0f;

	//누적값 저장
	float AccLeftRight = 0.0f;
	float AccUpDown = 0.0f;

	//캐릭터 캡슐 크기
	float CapsuleRadius = 42.0f; 
	float CapsuleHalfHeight = 96.0f;

	bool IsDeskIn = false;
};
