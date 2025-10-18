// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DiceCarpet.generated.h"

class UBoxComponent;
class UArrowComponent;
class ADiceActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSolvedChanged, bool, bSolved);


UCLASS()
class PROJECT0_API ADiceCarpet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADiceCarpet();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Carpet")
	UBoxComponent* Trigger = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Carpet")
	UArrowComponent* Front = nullptr; //이 Arrow의 Forward가 정면

	//자동 탐색 시 사용할 태그-이름 키워드
	UPROPERTY(EditAnywhere, Category = "Carpet")
	FName TriggerTag = TEXT("Trigger");

	UPROPERTY(EditAnywhere, Category = "Carpet")
	FName FrontTag = TEXT("Front");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Carpet")
	int32 RequiredValue = 1;

	UPROPERTY(BlueprintAssignable) FOnSolvedChanged OnSolvedChanged;

	UFUNCTION(BlueprintPure) bool IsSolved() const { return bSolved; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION() 
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION() 
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void Poll();

private:

	void AutoFindComponents();

	UPROPERTY() ADiceActor* CurrentDice = nullptr;
	bool bSolved = false;
	FTimerHandle PollHandle;

};
