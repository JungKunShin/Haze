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

	//�� ���� �̸� 
	UPROPERTY(EditAnywhere, Category = "Pickup")
	FName AttachSocketName = TEXT("hand_rSocket");

	//���� �� �� 
	UPROPERTY(EditAnywhere, Category = "Pickup")
	float ThrowImpulse = 1000.f;

	//���� �� ����
	UPROPERTY(EditAnywhere, Category = "Pickup")
	float ThrowDegree = 20.f;

	//���� �� N�� �� Pawn�� �����ϴ� ����(�ݸ��� ���� ����)
	UPROPERTY(EditAnywhere, Category = "Pickup")
	float ThrowPawnIgnoreDelay = 1.0f ;

	

	//������ ����
	UPROPERTY(EditAnywhere, Category = "Throw")
	float StrainRadius = 120.f;

	//������ ����(�Է�)
	UPROPERTY(EditAnywhere, Category = "Throw")
	FVector2D ImpactToMagnitude = FVector2D(5000.f, 30000.f);

	//��� strain ����
	UPROPERTY(EditAnywhere, Category = "Throw")
	FVector2D MagnitudeRange = FVector2D(25000.f, 100000.f);  

	//�ش� �±װ� �־�� ���߸�
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



	//�Ҿƹ��� ���� �ٸ� Ÿ�� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throw", meta = (AllowPrivateAccess = "true"))
	bool bAssistGrandFatherOnly = true;

	//��ǥ�� �ڵ� ���� �Ÿ� �Ѱ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throw", meta = (AllowPrivateAccess = "true"))
	float AssistMaxRange = 1600.f;

	//��ǥ�� �ڵ� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throw", meta = (AllowPrivateAccess = "true", ClampMin = "1.0", UIMin = "1.0", ClampMax = "45.0", UIMax = "20.0"))
	float AssistHalfAngleDeg = 12.f;

	//��ֹ� �������� �ڵ����� ��Ȱ��ȭ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throw", meta = (AllowPrivateAccess = "true"))
	bool bRequireLineOfSight = true;

	//����� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throw", meta = (AllowPrivateAccess = "true"))
	bool bDrawAssistDebug = false;

	//���� ��� �ִ��� Ȯ��
	class APlayerCharacter* Holder = nullptr;

	//���� ������ �� ����
	bool bSavedSimulatePhysics = false;



	//Pickup�� �ı��Ǹ� �ٽ� �Է� ������
	UFUNCTION()
	void OnDestroyedPickup(AActor* DestroyedActor);

	void SetPawnIgnore(bool bIgnore);
	FTimerHandle PawnIgnoreTimer;

	UFUNCTION()
	void RestorePawnIgnore();

	//�Ҿƹ��� ���� ����
	UPROPERTY(Transient)
	TWeakObjectPtr<class AGrandFatherCharacter> CachedGrandFather;

	
};
