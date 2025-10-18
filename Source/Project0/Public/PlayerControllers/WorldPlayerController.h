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
	UInputMappingContext* MoveIMC; // �����¿� ������ IMC

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* LookIMC; // ���콺 �þ� IMC

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* NoneGameIMC; // ���ӿ��� IMC

	float MouseSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;		//�����¿� IA

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;		//���콺 ���� IA

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;		//���� IA

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	
	class UInputAction* CrouchAction; //�ɱ׸��� IA
	
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction; //��ȣ�ۿ� IA

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* ThrowAction; //������ IA

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* TestAction; //�����

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* EnterAction; //��ȭâ �ѱ��

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* LeftAction; //���� ������

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* RightAction; //������ ������

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	 class UInputAction* PauseAction; // ���߱�

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* TalkAction = nullptr; //���ɱ�


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
	void QuestTest(const FInputActionValue& Value);//�����
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
