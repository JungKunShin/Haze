// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerControllers/BasePlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "WorldPlayerController.generated.h"

class UInputAction;
class ABaseCharacter;

/**
 * 
 */

UCLASS()
class PROJECT0_API AWorldPlayerController : public ABasePlayerController
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* MoveIMC; // 상하좌우 움직임 IMC

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* LookIMC; // 마우스 시야 IMC

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* NoneGameIMC; // 게임외적 IMC

	float MouseSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;		//상하좌우 IA

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;		//마우스 시점 IA

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;		//점프 IA

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	
	class UInputAction* CrouchAction; //쪼그리기 IA
	
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction; //상호작용 IA

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* ThrowAction; //던지기 IA

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* TestAction; //지우기

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* EnterAction; //대화창 넘기기

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* LeftAction; //왼쪽 돌리기

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* RightAction; //오른쪽 돌리기

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	 class UInputAction* PauseAction; // 멈추기

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* TalkAction = nullptr; //말걸기


	UFUNCTION() 
	void TogglePauseMenu(const struct FInputActionValue& Value);

	UFUNCTION()
	void Debug_GotoFinalState();


	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* DebugGotoFinalAction = nullptr;

protected:

	virtual void BeginPlay() override;
	void SetupInputComponent() override;
	
	void Move(const FInputActionValue& Value);
	void StopMove(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void Crouch(const FInputActionValue& Value);
	void JumpEnd(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
	void Throw(const FInputActionValue& Value);
	void QuestTest(const FInputActionValue& Value);//지우기
	void ConversationSkip(const FInputActionValue& Value);
	void LeftRot(const FInputActionValue& Value);
	void RightRot(const FInputActionValue& Value);
	void Talk(const struct FInputActionValue& Value);

public:
	void RemoveGameIMC();
	void AddGameIMC();
	void ChangeViewCamera(ABaseCharacter* Who);

	UFUNCTION(BlueprintCallable)
	void InitAllIMC();
	UFUNCTION(BlueprintCallable)
	void DeleteAllIMC();

protected:
	virtual void InitSound()override;
};
