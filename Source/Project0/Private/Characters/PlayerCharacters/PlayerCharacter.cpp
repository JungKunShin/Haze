// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacters/PlayerCharacter.h"
#include "Characters/NonPlayerCharacters/FriendlyCharacter/SisterCharacter.h"
#include "Characters/NonPlayerCharacters/FriendlyCharacter/MomCharacter.h" 
#include "ActorComponents/PlayerInteractableComponent.h"
#include "ActorComponents/PlayerPickupComponent.h"
#include "ActorComponents/PlayerRotationComponent.h"
#include "ActorComponents/QuestSystemComponent.h"
#include "HUD/WorldHUD.h"

#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Actors/Camera/ActiveCamera.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Quest/ChapterFourQuest.h"

#include "PlayerControllers/WorldPlayerController.h"

#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInput/Public/InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Widget/ConversationWidget.h"


APlayerCharacter::APlayerCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCapsuleComponent()->InitCapsuleSize(CapsuleRadius, CapsuleHalfHeight);	// ĸ�� ������
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));		// �ݸ��� Ÿ��

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 600.f, 0.0f); //ȸ���ӵ�
	GetCharacterMovement()->JumpZVelocity = 800.f;	// ���� ����
	GetCharacterMovement()->MaxWalkSpeed = 500.f;	// �ȱ� �ӵ�
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true; //���� �� �ְ� ����

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));	// ī�޶� �� â��
	CameraBoom->SetupAttachment(RootComponent);										// RootComponent ������ �ֱ�
	CameraBoom->TargetArmLength = 700.0f;											// ī�޶� �� ���� �ʱ� ����
	CameraBoom->bUsePawnControlRotation = false;

	ThirdPersonCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("ThirdPersonCameraBoom"));	// ī�޶� �� â��
	ThirdPersonCameraBoom->SetupAttachment(RootComponent);										// RootComponent ������ �ֱ�
	ThirdPersonCameraBoom->TargetArmLength = 250.0f;											// ī�޶� �� ���� �ʱ� ����
	ThirdPersonCameraBoom->bUsePawnControlRotation = false;

	InteractBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InterectBox"));
	InteractBox->SetupAttachment(RootComponent);

	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));						// ī�޶� â��
	ThirdPersonCamera->SetupAttachment(CameraBoom);																	// ī�޶� �� ������ �ֱ�
	ThirdPersonCamera->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));   // ī�޶� ��ġ ���� �ʱ⼳��
	ThirdPersonCamera->bUsePawnControlRotation = false;

	QuestSystem = CreateDefaultSubobject<UQuestSystemComponent>(TEXT("QuestSystemComponent"));

	SideViewArmLength = CameraBoom->TargetArmLength;
	SideViewTargetOffset = CameraBoom->TargetOffset;
	SideViewSocketOffset = CameraBoom->SocketOffset;

}


void APlayerCharacter::Seq_BeginDialogue_SeqA()
{
	if (QuestSystem) QuestSystem->SequenceStartDialogueByPick(EFinalPick::PickA);
}

void APlayerCharacter::Seq_BeginDialogue_SeqB()
{
	if (QuestSystem) QuestSystem->SequenceStartDialogueByPick(EFinalPick::PickB);
}

void APlayerCharacter::Seq_DialogueNext()
{
	if (QuestSystem) QuestSystem->SequenceNextLine();

}

void APlayerCharacter::Seq_DialogueFinish()
{
	if (QuestSystem) QuestSystem->SequenceFinishDialogue(EQuestState::End);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	SideViewRelativeLocation = CameraBoom->GetRelativeLocation();

	UCameraComponent* MyCam = FindComponentByClass<UCameraComponent>();
	if (!MyCam) return;

	//������ �ִ� AActiveCamera�� ã��
	TArray<AActor*> Found;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActiveCamera::StaticClass(), Found);
	if (Found.IsEmpty()) return;

	ActiveCam = Cast<AActiveCamera>(Found[0]);

	ActiveCam->TargetCamera = MyCam;

	//ã�� ī�޶�� �� ��ȯ
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		constexpr float BlendTime = 1.0f;      
		PC->SetViewTargetWithBlend(ActiveCam, BlendTime,
			EViewTargetBlendFunction::VTBlend_EaseInOut);
	}
}

void APlayerCharacter::Tick(float DeltaSeconds)
{

	Super::Tick(DeltaSeconds);

	// 2025-08-06 �̿��� : ���̵�� ī�޶� �ȷ��� �Լ��� ���� + 3��Ī ������ ��� ���� X
	if (bIsSideView) {
		SideViewCameraFollowing(DeltaSeconds);
	}

	// ĳ���Ͱ� ��ȣ�ۿ� ������ �����̸� (bCanInteract = true)
	if (bCanInteract && InteractBox) {
		// ƽ���� ���ͷ��� ������ ����(���ͷ��ͺ� ������Ʈ�� �ִ� ����) ã��
		FindInteractable();
	}

	
}

void APlayerCharacter::FindInteractable()
{
	if (InteractBox) { // null Check
		CurrentInteractables.Empty(); // ã�� �� ����

		TArray<AActor*> OverlappedActors;
		InteractBox->GetOverlappingActors(OverlappedActors); // InterectBox ���� ���� ���� ������

		// ���� �� ���͵� �߿��� UPlayerInteractableComponent�� ��ӹ��� ������Ʈ�� �ִ��� Ȯ��
		for (const AActor* OverlappedActor : OverlappedActors) {
			UPlayerInteractableComponent* InteractableComponent = OverlappedActor->GetComponentByClass<UPlayerInteractableComponent>();

			if(InteractableComponent){ // �ִٸ�
				FHitResult HitResult;
				// ���� ���������� Ȯ��
				GetWorld()->LineTraceSingleByChannel(
					HitResult,
					GetCapsuleComponent()->GetComponentLocation(),
					OverlappedActor->GetActorLocation(), 
					InteractLineChannel, 
					FCollisionQueryParams(FName(), false, this)
				);

				// �Ȱ������� ���
				if (HitResult.GetActor() == OverlappedActor) {
					// CurrentInteractables�� �߰�
					CurrentInteractables.Add(InteractableComponent);
				}
			}
		}

		if (CurrentInteractables.IsEmpty()) { // ã���� ������Ʈ�� ���ٸ�
			if (CurrentSelectedInteractable) { // ���� ��Ŀ���� ������ ��Ŀ�� ����
				CurrentSelectedInteractable->EndFocus();
				CurrentSelectedInteractable = nullptr;
			}
		}
		else { // ã���� ������Ʈ�� �ִٸ� ���� ����� ������Ʈ ��Ŀ��
			if (CurrentSelectedInteractable) {
				CurrentSelectedInteractable->EndFocus();
				CurrentSelectedInteractable = nullptr;
			}
			float MinDistance = InteractBox->GetScaledBoxExtent().GetMax() * 10.0f;

			for (UPlayerInteractableComponent* SelectedInteractable : CurrentInteractables) {
				float Dist = SelectedInteractable->GetOwner()->GetDistanceTo(this);
				if (Dist < MinDistance 
					&& 
					(SelectedInteractable->ConnectedQuest == EQuestState::None
						|| 
						SelectedInteractable->ConnectedQuest == QuestSystem->CurQuestState)) {
					MinDistance = Dist;
					CurrentSelectedInteractable = SelectedInteractable;
				}
			}

			if (CurrentSelectedInteractable) {
				CurrentSelectedInteractable->BeginFocus();
			}
		}

	
	}


}

void APlayerCharacter::SideViewCameraFollowing(float DeltaSeconds)
{
	//�Է� ���� �� ��� �ð� ���
	float TimeSinceLastLook = GetWorld()->GetTimeSeconds() - LastLookTime;

	//Look ���� ������ �ð� ������ ĳ���� ������ ī�޶� �ε巴�� �ٽ� ���ƿ�
	if (TimeSinceLastLook >= ReturnDelay)
	{
		const float Speed = 15.0f;

		FRotator Relative = CameraBoom->GetRelativeRotation();
		Relative.Yaw = FMath::FInterpTo(Relative.Yaw, 0.f, DeltaSeconds, Speed);
		Relative.Pitch = FMath::FInterpTo(Relative.Pitch, 0.f, DeltaSeconds, Speed);

		CameraBoom->SetRelativeRotation(Relative);
	}
}

void APlayerCharacter::SideViewLook(const FVector2D& Value)
{
	//ī�޶��� ��� ��ġ��
	float Sens = 60.f;
	float DeltaYaw = -Value.X * Sens * GetWorld()->GetDeltaSeconds();
	float DeltaPitch = -Value.Y * Sens * GetWorld()->GetDeltaSeconds();

	//���� ȸ���� ��ġ�� �ʰ� ī�޶� ��ġ�� ��� ��ġ�� ���� �� ����
	FRotator Relative = CameraBoom->GetRelativeRotation();
	Relative.Yaw = FMath::Clamp(Relative.Yaw + DeltaYaw, -LookLeftRightLimit, LookLeftRightLimit);
	Relative.Pitch = FMath::Clamp(Relative.Pitch + DeltaPitch, -LookUpDownLimit, LookUpDownLimit);
	CameraBoom->SetRelativeRotation(Relative);
}

void APlayerCharacter::ThirdPersonLook(const FVector2D& Value)
{
	AddControllerYawInput(Value.X);
	AddControllerPitchInput(-Value.Y);
}

void APlayerCharacter::SetThrowWindowActive(bool bActive)
{
	bThrowWindowActive = bActive;
}

void APlayerCharacter::TestQuest()
{
	QuestSystem->QuestProgress(QuestSystem->CurQuestState);
}


void APlayerCharacter::Move(FVector2D Value)
{
	FVector2D MovementVector = Value;

	if (Controller != nullptr)
	{
		// 2025-08-07 ��ȣ�ۿ��� ��ǲ ���� �ٸ��� ����
		if (!CurrentInteractingInteractable) {
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			AddMovementInput(ForwardDirection, MovementVector.X);
			AddMovementInput(RightDirection, MovementVector.Y);
		}
		else {
			CurrentInteractingInteractable->MoveInput(Value);
		}
		
	}
}

void APlayerCharacter::Jump()
{
	//�ɱ׸� ���¿��� ���� ����
	if (GetCharacterMovement()->IsCrouching()) return;
	Super::Jump();
}

void APlayerCharacter::Look(FVector2D Value)
{
	LastLookTime = GetWorld()->GetTimeSeconds();

	// 2025-08-06 �̿��� : ���̵�� ī�޶� �Է� �Լ��� ���� + 3��Ī�� ��� �Է� ���� �ٸ���
	if (bIsSideView) { // ���̵���� ���
		SideViewLook(Value);
	}
	else { // ���̵�䰡 �ƴ� ��� 3��Ī ����
		ThirdPersonLook(Value);
	}
}

void APlayerCharacter::ToggleCrouch()
{
	if (GetCharacterMovement()->IsCrouching())
		UnCrouch();
	else
		Crouch();
}

void APlayerCharacter::TryStartConversation()
{
	if (!QuestSystem) return;

	// �̹� ��ȭâ�� ���� ������ �� ��ȭ ���� ���� (�ߺ� ����)
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
		if (AWorldHUD* HUD = PC ? Cast<AWorldHUD>(PC->GetHUD()) : nullptr)
			if (HUD->ConversationWidget && HUD->ConversationWidget->IsInViewport())
				return;

	//�÷��̾� �չ�������, Pawn�� ��Ʈ
	const FVector Start = GetActorLocation() + FVector(0.f, 0.f, 60.f);
	const FVector End = Start + GetActorForwardVector() * TalkRange;

	FHitResult Hit;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(TalkSweep), false, this);
	FCollisionObjectQueryParams ObjParams;
	ObjParams.AddObjectTypesToQuery(ECC_Pawn);

	const bool bHit = GetWorld()->SweepSingleByObjectType(
		Hit, Start, End, FQuat::Identity,
		ObjParams, FCollisionShape::MakeSphere(TalkProbeRadius),
		Params);

	if (!bHit)
	{
		return;
	}

	AActor* A = Hit.GetActor();
	if (!A && Hit.GetComponent()) A = Hit.GetComponent()->GetOwner();



	const EQuestState S = UQuestSystemComponent::CurQuestState;

	if (ASisterCharacter* SISTER = A ? Cast<ASisterCharacter>(A) : nullptr)
	{
		if (S == EQuestState::None)
			QuestSystem->QuestProgress(UQuestSystemComponent::CurQuestState);

		if (S == EQuestState::MomTalking) return;

	

		if (S == EQuestState::MomBarn || S == EQuestState::MomMix || S == EQuestState::MazeBefore)
			QuestSystem->SetTalkerSister();

		QuestSystem->Conversation(UQuestSystemComponent::CurQuestState);
		return;
	}

	if (AMomCharacter* MOM = A ? Cast<AMomCharacter>(A) : nullptr)
	{
		if (S == EQuestState::MomMix || S == EQuestState::MazeBefore)
			QuestSystem->SetTalkerMom();

		if (S == EQuestState::MomTalking || S == EQuestState::MomBarn || S == EQuestState::MomMix || S == EQuestState::MazeBefore)
		{
			QuestSystem->Conversation(UQuestSystemComponent::CurQuestState);
		}
		return;
	}
}



void APlayerCharacter::Interact()
{
	
	//��� �ִ� ���¸� ��������� 
	if (HeldPickup)
	{
		bCanInteract = false;

		if (AWorldPlayerController* PC = Cast<AWorldPlayerController>(GetController()))
		{
			PC->RemoveGameIMC(); //�������� ���� �Է� ����
		}

		//Pickup ��Ÿ���� PutDown ���
		if (PutDownMontage)
		{
			PlayAnimMontage(PutDownMontage, 1.f, FName(TEXT("PutDown")));
		}
		return;
	}

	if (CurrentSelectedInteractable && !CurrentSelectedInteractable->IsInteracting() && bCanInteract) {
		CurrentSelectedInteractable->BeginInteraction(this);
	}
	else if (CurrentSelectedInteractable && CurrentSelectedInteractable->IsInteracting()) {
		CurrentInteractingInteractable->InteractInput();
	}
}

void APlayerCharacter::SwitchToThirdPersonView()
{
	if (bIsSideView) {
		SideViewControlRotation = GetControlRotation();

		CameraBoom->bInheritYaw = true;
		bIsSideView = false;

		APlayerController* PC = Cast<APlayerController>(GetController());

		if (PC) {
			// ��Ʈ�ѷ��� ��ǲ�� �Ҹ��ϴ� ������ �ʱ� ������ ���� ���� ��ǲ�� ����ϴ� ��� ����
			PC->RotationInput = FRotator::ZeroRotator;

			CameraBoom->TargetArmLength = ThirdPersonCameraBoom->TargetArmLength;
			CameraBoom->SetRelativeLocation(ThirdPersonCameraBoom->GetRelativeLocation());
			CameraBoom->TargetOffset = ThirdPersonCameraBoom->TargetOffset;
			CameraBoom->SocketOffset = ThirdPersonCameraBoom->SocketOffset;

			// �÷��̾� ķ���� ��ȯ
			PC->SetViewTargetWithBlend(this, 0.0f,
				EViewTargetBlendFunction::VTBlend_EaseInOut);
		}
	}
}

void APlayerCharacter::SwitchToSideView()
{
	if (!bIsSideView) {
		GetController()->SetControlRotation(SideViewControlRotation);

		CameraBoom->bInheritYaw = false;
		bIsSideView = true;

		APlayerController* PC = Cast<APlayerController>(GetController());
		
		if (PC) {
			// ��Ʈ�ѷ��� ��ǲ�� �Ҹ��ϴ� ������ �ʱ� ������ ���� ���� ��ǲ�� ����ϴ� ��� ����
			PC->RotationInput = FRotator::ZeroRotator;

			CameraBoom->TargetArmLength = SideViewArmLength;
			CameraBoom->SetRelativeLocation(SideViewRelativeLocation);
			CameraBoom->TargetOffset = SideViewTargetOffset;
			CameraBoom->SocketOffset = SideViewSocketOffset;

			// ��Ƽ�� ķ���� ��ȯ
			PC->SetViewTargetWithBlend(ActiveCam, 0.0f,
				EViewTargetBlendFunction::VTBlend_EaseInOut);
		}
	}
}

void APlayerCharacter::ToggleView()
{
	if (bIsSideView) {
		SwitchToThirdPersonView();
	}
	else {
		SwitchToSideView();
	}
}

void APlayerCharacter::SetCanInteract(bool Value)
{
	bCanInteract = Value;
}

void APlayerCharacter::StartInteractTo(UPlayerInteractableComponent* TargetInteractable)
{
	bCanInteract = false;
	CurrentInteractingInteractable = TargetInteractable;
}

void APlayerCharacter::EndInteract()
{

	bCanInteract = true;
	CurrentInteractingInteractable = nullptr;

}

void APlayerCharacter::LockCameraFollow(bool bLock)
{
	if (bLock)
	{
		CameraBoom->bUsePawnControlRotation = false;  
		CameraBoom->bInheritYaw = false;          
		CameraBoom->bInheritPitch = false;
		CameraBoom->bInheritRoll = false;
	}
	else
	{
		CameraBoom->bUsePawnControlRotation = true; 
		CameraBoom->bInheritYaw = true;
		CameraBoom->bInheritPitch = true;
		CameraBoom->bInheritRoll = true;
	}
}

void APlayerCharacter::BeforeDeath(EQuestState _Death)
{
	QuestSystem->HitTempQuestState = QuestSystem->CurQuestState;
	QuestSystem->CurQuestState = _Death;
	QuestSystem->Conversation(QuestSystem->CurQuestState);
	/*QuestSystem->GrandFatherDeath();*/
}


//AnimBP�� ������ ����!
void APlayerCharacter::Anim_PickupAttach()
{
	if (PendingPickup)
	{
		PendingPickup->AttachToHand();
	}
}

//AnimBP�� ������ ����!
void APlayerCharacter::Anim_PickupEnd()
{
	if (AWorldPlayerController* PC = Cast<AWorldPlayerController>(GetController()))
	{
		PC->AddGameIMC();  
	}

	if (CurrentInteractingInteractable && CurrentInteractingInteractable->IsInteracting())
	{
		CurrentInteractingInteractable->EndInteraction();
	}
	else if (CurrentSelectedInteractable && CurrentSelectedInteractable->IsInteracting())
	{
		CurrentSelectedInteractable->EndInteraction();
	}

	EndInteract();

	PendingPickup = nullptr;
}

//AnimBP�� ������ ����!
void APlayerCharacter::Anim_ThrowRelease()
{
	if (HeldPickup)
	{
		HeldPickup->Throw();
	}


	EndInteract();
}

void APlayerCharacter::Anim_PutDownRelease()
{
	if (HeldPickup)
	{
		HeldPickup->PutDown();
	}
}

void APlayerCharacter::Anim_Rotation()
{
	if (AWorldPlayerController* PC = Cast<AWorldPlayerController>(GetController()))
	{
		PC->AddGameIMC();
	}

	EndInteract();
}

void APlayerCharacter::LeftRotPicture()
{
	if (Cast<UPlayerRotationComponent>(CurrentInteractingInteractable))
	{
		Cast<UPlayerRotationComponent>(CurrentInteractingInteractable)->LeftRotationPicture();
	}
}

void APlayerCharacter::RightRotPicture()
{
	if (Cast<UPlayerRotationComponent>(CurrentInteractingInteractable))
	{
		Cast<UPlayerRotationComponent>(CurrentInteractingInteractable)->RightRotationPicture();
	}
}

void APlayerCharacter::Keeping(UPlayerInteractableComponent* Current)
{
	CurrentInteractingInteractable = Current;
}

void APlayerCharacter::OverlapSizeUp(bool _IsBig)
{
	if (_IsBig)
	{
		InteractBox->SetBoxExtent(OriginalSize);
	}
	else
	{
		OriginalSize = InteractBox->GetUnscaledBoxExtent();
		InteractBox->SetBoxExtent(OriginalSize * 1.5f);
	}
}

void APlayerCharacter::PlayDeadScene(AActor* Blendtarget, UAnimMontage* MontageToPlay, const FRotator& Rotation)
{
	AWorldPlayerController* WorldPC = GetController<AWorldPlayerController>();

	WorldPC->DeleteAllIMC();
	WorldPC->SetViewTargetWithBlend(Blendtarget, 1.0f, EViewTargetBlendFunction::VTBlend_EaseIn, 2.0f);
	SetActorRotation(Rotation);
	PlayAnimMontage(MontageToPlay);
}

void APlayerCharacter::AddMappingContext()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PC->GetLocalPlayer()))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

		if (Subsystem && IMC_Look)
		{
			Subsystem->AddMappingContext(IMC_Look, 1);
		}
	}
}

void APlayerCharacter::Throw()             
{
	
	if (HeldPickup && bCanInteract)
	{
		PlayAnimMontage(ThrowMontage);
	}
}