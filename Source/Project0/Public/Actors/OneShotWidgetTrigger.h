// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseActor.h"
#include "OneShotWidgetTrigger.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT0_API AOneShotWidgetTrigger : public ABaseActor
{
	GENERATED_BODY()

	public:
	AOneShotWidgetTrigger();

protected:
	// Ʈ���� �ڽ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trigger")
	class UBoxComponent* Box;

	// Pawn�� ��ġ�� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger")
	bool bPlayerOnly = true;

	// Overlap �� HUD ȣ��
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};