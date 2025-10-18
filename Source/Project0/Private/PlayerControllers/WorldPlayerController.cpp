// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllers/WorldPlayerController.h"
#include "Characters/PlayerCharacters/PlayerCharacter.h"
#include "InputActionValue.h"
#include "HUD/WorldHUD.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameMode.h"
#include "Manager/SoundManager.h"
#include "ActorComponents/QuestSystemComponent.h"
#include "Misc/ConfigCacheIni.h"

static float LoadSavedGamma()
{
	float Gamma = 2.2f; // 기본값
	GConfig->GetFloat(TEXT("/Script/Engine.GameUserSettings"),
		TEXT("BrightnessGamma"), Gamma, GGameUserSettingsIni);
	return Gamma;
}


void AWorldPlayerController::TogglePauseMenu(const FInputActionValue& Value)
{
	if (AWorldHUD* HUD = Cast<AWorldHUD>(GetHUD()))
	{
		const bool bOpen = !HUD->IsPauseMenuOpen();
		HUD->SetPauseMenuVisible(bOpen);
	}
}

void AWorldPlayerController::Debug_GotoFinalState()
{
	if (APlayerCharacter* My = Cast<APlayerCharacter>(GetPawn()))
	{
		if (My->QuestSystem)
		{
			My->QuestSystem->EnterMazeAfter();

			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan,
					TEXT("MazeAfter enabled"));
		}
	}
}

void AWorldPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = false;
	FInputModeGameOnly Mode;
	SetInputMode(Mode);

	const float Gamma = LoadSavedGamma();
	ConsoleCommand(FString::Printf(TEXT("r.TonemapperGamma %.2f"), Gamma), false);
}

void AWorldPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//로컬 플레이어 인풋시스템
	Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	//기본 IMC 추가
	Subsystem->AddMappingContext(MoveIMC, 0);
	Subsystem->AddMappingContext(LookIMC, 0);
	Subsystem->AddMappingContext(NoneGameIMC, 0);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWorldPlayerController::Move);
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Completed, this, &AWorldPlayerController::StopMove);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWorldPlayerController::Look);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &AWorldPlayerController::Jump);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &AWorldPlayerController::JumpEnd);
		EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AWorldPlayerController::Crouch);
		EnhancedInput->BindAction(InteractAction, ETriggerEvent::Started, this, &AWorldPlayerController::Interact);
		EnhancedInput->BindAction(ThrowAction, ETriggerEvent::Started, this, &AWorldPlayerController::Throw);
		EnhancedInput->BindAction(TestAction, ETriggerEvent::Triggered, this, &AWorldPlayerController::QuestTest);
		EnhancedInput->BindAction(EnterAction, ETriggerEvent::Triggered, this, &AWorldPlayerController::ConversationSkip);
		EnhancedInput->BindAction(LeftAction, ETriggerEvent::Triggered, this, &AWorldPlayerController::LeftRot);
		EnhancedInput->BindAction(RightAction, ETriggerEvent::Triggered, this, &AWorldPlayerController::RightRot);
		EnhancedInput->BindAction(PauseAction, ETriggerEvent::Started, this, &AWorldPlayerController::TogglePauseMenu);
		EnhancedInput->BindAction(TalkAction, ETriggerEvent::Started, this, &AWorldPlayerController::Talk);
		

		if (DebugGotoFinalAction)
		{
			EnhancedInput->BindAction(
				DebugGotoFinalAction, ETriggerEvent::Started, this, &AWorldPlayerController::Debug_GotoFinalState);
		}
	}

}


void AWorldPlayerController::Move(const FInputActionValue& Value)
{
	APlayerCharacter* MyCharacter = Cast<APlayerCharacter>(GetPawn());
	if (MyCharacter)
	{
		MyCharacter->Move(Value.Get<FVector2D>());
	}
}

void AWorldPlayerController::StopMove(const FInputActionValue& Value)
{
	APlayerCharacter* MyCharacter = Cast<APlayerCharacter>(GetPawn());
	if (MyCharacter)
	{
		MyCharacter->Move(FVector2D::ZeroVector);
	}
}


void AWorldPlayerController::Crouch(const FInputActionValue& Value)
{
	if (APlayerCharacter* MyChar = Cast<APlayerCharacter>(GetPawn()))
	{
		MyChar->ToggleCrouch();
	}
}


void AWorldPlayerController::Look(const FInputActionValue& Value)
{
	APlayerCharacter* MyCharacter = Cast<APlayerCharacter>(GetPawn());
	if (MyCharacter)
	{
		MyCharacter->Look(Value.Get<FVector2D>()* MouseSpeed);
	}
}


void AWorldPlayerController::Jump(const FInputActionValue& Value)
{
	APlayerCharacter* MyCharacter = Cast<APlayerCharacter>(GetPawn());
	if (MyCharacter)
	{
		MyCharacter->Jump();
	}
}

void AWorldPlayerController::JumpEnd(const FInputActionValue& Value)
{
	APlayerCharacter* MyCharacter = Cast<APlayerCharacter>(GetPawn());
	if (MyCharacter)
	{
		MyCharacter->StopJumping();
	}
}

void AWorldPlayerController::Interact(const FInputActionValue& Value)
{
	APlayerCharacter* MyCharacter = Cast<APlayerCharacter>(GetPawn());
	if (MyCharacter)
	{
		MyCharacter->Interact();
	}
}

void AWorldPlayerController::Throw(const FInputActionValue& Value)
{
	if (auto* MyChar = Cast<APlayerCharacter>(GetPawn())) MyChar->Throw();
}

void AWorldPlayerController::QuestTest(const FInputActionValue& Value)
{
	//APlayerCharacter* MyCharacter = Cast<APlayerCharacter>(GetPawn());
	//if (MyCharacter)
	//{
	//	MyCharacter->TestQuest();//지워야됨
	//}
	AWorldHUD* MyHud = Cast< AWorldHUD>(GetHUD());
	MyHud->SetVolumeTest();
}

void AWorldPlayerController::ConversationSkip(const FInputActionValue& Value)
{
	if (APlayerCharacter* P = Cast<APlayerCharacter>(GetPawn()))
	{
		if (P->QuestSystem && P->QuestSystem->IsSequenceInputBlocked())
		{
			return; 
		}
	}

	AWorldHUD* HUD = Cast<AWorldHUD>(GetHUD());
	if (HUD)
	{
		HUD->SetSkipText();
	}
}

void AWorldPlayerController::LeftRot(const FInputActionValue& Value)
{
	APlayerCharacter* MyCharacter = Cast<APlayerCharacter>(GetPawn());
	if (MyCharacter)
	{
		MyCharacter->LeftRotPicture();
	}
}

void AWorldPlayerController::RightRot(const FInputActionValue& Value)
{
	APlayerCharacter* MyCharacter = Cast<APlayerCharacter>(GetPawn());
	if (MyCharacter)
	{
		MyCharacter->RightRotPicture();
	}
}

void AWorldPlayerController::Talk(const FInputActionValue& Value)
{
	if (auto* My = Cast<APlayerCharacter>(GetPawn()))
	{
		My->TryStartConversation(); 
	}
}

void AWorldPlayerController::RemoveGameIMC()
{
	Subsystem->RemoveMappingContext(MoveIMC);
	Subsystem->RemoveMappingContext(LookIMC);
}

void AWorldPlayerController::AddGameIMC()
{
	Subsystem->AddMappingContext(MoveIMC, 0);
	Subsystem->AddMappingContext(LookIMC, 0);
}

void AWorldPlayerController::ChangeViewCamera(ABaseCharacter* Who)
{
	SetViewTargetWithBlend(Who, 1.0f);
}

void AWorldPlayerController::InitAllIMC()
{
	Subsystem->AddMappingContext(MoveIMC, 0);
	Subsystem->AddMappingContext(LookIMC, 0);
	Subsystem->AddMappingContext(NoneGameIMC, 0);
}

void AWorldPlayerController::DeleteAllIMC()
{
	while (Subsystem->HasMappingContext(MoveIMC)) {
		Subsystem->RemoveMappingContext(MoveIMC);
	}
	while (Subsystem->HasMappingContext(LookIMC)) {
		Subsystem->RemoveMappingContext(LookIMC);
	}
	while (Subsystem->HasMappingContext(NoneGameIMC)) {
		Subsystem->RemoveMappingContext(NoneGameIMC);
	}
}

void AWorldPlayerController::InitSound()
{
	Super::InitSound();

	if (SoundManager)
	{
		if (UQuestSystemComponent::CurQuestState == EQuestState::None)
		{
			SoundManager->PlayBGM(SoundManager->BGM_Tutorial); // 원하는 BGM 지정
		}
		else
		{
			SoundManager->PlayBGM(SoundManager->BGM_World); // 원하는 BGM 지정
		}
	}
}
