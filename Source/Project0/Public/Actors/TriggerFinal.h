// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggerFinal.generated.h"

class UBoxComponent;
class ULevelSequence;
class ULevelSequencePlayer;
class ALevelSequenceActor;

UENUM(BlueprintType)
enum class EFinalPick : uint8
{
	None,
	PickA,
	PickB
};

//오버랩 → 퀘스트 선택지
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFinalPromptEvent, class ATriggerFinal*, Trigger);
//시퀀스 재생 시작(선택 직후) 알림
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFinalPlayedEvent, class ATriggerFinal*, Trigger, EFinalPick, Pick);
//시퀀스 재생 완료 알림
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFinalFinishedEvent, class ATriggerFinal*, Trigger, EFinalPick, Pick);

UCLASS()
class PROJECT0_API ATriggerFinal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATriggerFinal();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) 
	UBoxComponent* Box;

	//MazeAfter에서만 반응하도록 외부에서 Enable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Final")
	bool bEnabled = false;

	//UI에 노출될 선택 태그
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Final")
	FText ChoiceALabel = FText::FromString(TEXT("PickA"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Final")
	FText ChoiceBLabel = FText::FromString(TEXT("PickB"));

	//각 선택지에 연결될 시퀀스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Final")
	TSoftObjectPtr<ULevelSequence> SequenceA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Final")
	TSoftObjectPtr<ULevelSequence> SequenceB;

	UPROPERTY(BlueprintAssignable) 
	FFinalPromptEvent   OnPromptRequested;

	UPROPERTY(BlueprintAssignable) 
	FFinalPlayedEvent   OnSequencePlayed;

	UPROPERTY(BlueprintAssignable) 
	FFinalFinishedEvent OnSequenceFinished;

	UFUNCTION(BlueprintCallable) 
	void SetEnabled(bool bInEnabled);

	//퀘스트에서 선택 확정시 호출
	UFUNCTION(BlueprintCallable) 
	void PlayChosen(EFinalPick Pick);

protected:

	UFUNCTION() void OnBeginOverlap(UPrimitiveComponent* Overlapped, AActor* Other,
	UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& Sweep);

private:

	void PlayInternal(TSoftObjectPtr<ULevelSequence> Asset, EFinalPick Pick);

	

	UPROPERTY() 
	ULevelSequencePlayer* Player = nullptr;

	UPROPERTY() 
	ALevelSequenceActor* SpawnedActor = nullptr;

	EFinalPick LastPick = EFinalPick::None;
};
