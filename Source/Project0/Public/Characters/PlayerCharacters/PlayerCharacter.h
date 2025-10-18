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

	//ī�޶� �Ŵ��� ��ġ ����
	void LockCameraFollow(bool bLock);

	void BeforeDeath(EQuestState _Death);

	//�Ⱦ� ��Ƽ���� ����
	UFUNCTION(BlueprintCallable)
	void Anim_PickupAttach();

	//���� ���� notify �ο��� �̵� ��� ����
	UFUNCTION(BlueprintCallable)
	void Anim_PickupEnd();

	//������ ��Ƽ���� ����
	UFUNCTION(BlueprintCallable)
	void Anim_ThrowRelease();

	//���� ��Ƽ���� ����
	UFUNCTION(BlueprintCallable)
	void Anim_PutDownRelease();

	//������ ��Ƽ���� ����
	UFUNCTION(BlueprintCallable)
	void Anim_Rotation();

	void LeftRotPicture();
	void RightRotPicture();

	void Keeping(UPlayerInteractableComponent* Current);

	void OverlapSizeUp(bool _IsBig);

	UFUNCTION(BlueprintCallable)
	virtual void PlayDeadScene(AActor* Blendtarget, UAnimMontage* MontageToPlay, const FRotator& Rotation);

	//�տ� ���� ���� ����� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
	class UPlayerPickupComponent* PendingPickup = nullptr;

	//�⺻ ī�޶� ��������
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom; 

	// 3��Ī ī�޶� ������ ��
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* ThirdPersonCameraBoom;

	//�⺻ ī�޶�
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
	float TalkRange = 180.f;            //��ȭ ���� �Ÿ�

	UPROPERTY(EditAnywhere, Category = "Talk")
	float TalkProbeRadius = 60.f;       //Ž�� �ݰ�

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
	class UAnimMontage* DeathMontage; //�״� ��Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage)
	class UAnimMontage* InteractMontage; //��ȣ�ۿ� ��Ÿ��
	

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
	UQuestSystemComponent* QuestSystem; // ����Ʈ �ý���



	// �¿� 30�� ����
	UPROPERTY(EditAnywhere, Category = "Camera") 
	float LookLeftRightLimit = 20.0f;  

	// ���Ʒ� 5�� ����
	UPROPERTY(EditAnywhere, Category = "Camera") 
	float LookUpDownLimit = 7.0f;  

	//ī�޶� ���� ������
	UPROPERTY(EditAnywhere, Category = "Camera")
	float ReturnDelay = 0.2f;

	//���������� Look �Է� ���� �ð�
	float LastLookTime = 0.0f;

	//������ ����
	float AccLeftRight = 0.0f;
	float AccUpDown = 0.0f;

	//ĳ���� ĸ�� ũ��
	float CapsuleRadius = 42.0f; 
	float CapsuleHalfHeight = 96.0f;

	bool IsDeskIn = false;
};
