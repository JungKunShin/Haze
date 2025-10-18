// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/NonPlayerCharacters/NonPlayerCharacter.h"
#include "../../ActorComponents/QuestSystemComponent.h"
#include "EnemyCharacter.generated.h"

class APlayerCharacter;
class AEnemyAIController;
/**
 * 
 */
UCLASS()
class PROJECT0_API AEnemyCharacter : public ANonPlayerCharacter
{
	GENERATED_BODY()

private:
	const float TARGET_LENGTH = 50.f;
	const float ZOOM_SPEED = 150.0f; // 초당 증가량
	
public:
	AEnemyCharacter();

	virtual void HitPlayer(APlayerCharacter* _Player);

	virtual void KillConversation(APlayerCharacter* _Player);

	virtual void KillPlayer();

private:
	void CameraZoomIn();
	
	void DelayZoomOut();

protected:
	virtual void ResetEnemyLocation();

public:
	void ResetEnemy();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage)
	class UAnimMontage* AttackMontage; //스킬 몽타주

protected:
	EQuestState DeathType;

private:
	FTimerHandle ZoomTH;

	UPROPERTY()
	APlayerCharacter* TargetCharacter;

	float GeometricalSum = 0;

	AEnemyAIController* CurAI;

	float OriginalZoomLength;
};
