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

	GetCapsuleComponent()->InitCapsuleSize(CapsuleRadius, CapsuleHalfHeight);	// 캡슐 사이즈
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));		// 콜리젼 타입

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 600.f, 0.0f); //회전속도
	GetCharacterMovement()->JumpZVelocity = 800.f;	// 점프 정도
	GetCharacterMovement()->MaxWalkSpeed = 500.f;	// 걷기 속도
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true; //앉을 수 있게 설정

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));	// 카메라 붐 창조
	CameraBoom->SetupAttachment(RootComponent);										// RootComponent 밑으로 넣기
	CameraBoom->TargetArmLength = 700.0f;											// 카메라 붐 길이 초기 설정
	CameraBoom->bUsePawnControlRotation = false;

	ThirdPersonCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("ThirdPersonCameraBoom"));	// 카메라 붐 창조
	ThirdPersonCameraBoom->SetupAttachment(RootComponent);										// RootComponent 밑으로 넣기
	ThirdPersonCameraBoom->TargetArmLength = 250.0f;											// 카메라 붐 길이 초기 설정
	ThirdPersonCameraBoom->bUsePawnControlRotation = false;

	InteractBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InterectBox"));
	InteractBox->SetupAttachment(RootComponent);

	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));						// 카메라 창조
	ThirdPersonCamera->SetupAttachment(CameraBoom);																	// 카메라 붐 밑으로 넣기
	ThirdPersonCamera->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));   // 카메라 위치 각도 초기설정
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

	//레벨에 있는 AActiveCamera를 찾음
	TArray<AActor*> Found;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActiveCamera::StaticClass(), Found);
	if (Found.IsEmpty()) return;

	ActiveCam = Cast<AActiveCamera>(Found[0]);

	ActiveCam->TargetCamera = MyCam;

	//찾은 카메라로 뷰 전환
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

	// 2025-08-06 이우주 : 사이드뷰 카메라 팔로잉 함수로 묶음 + 3인칭 시점일 경우 실행 X
	if (bIsSideView) {
		SideViewCameraFollowing(DeltaSeconds);
	}

	// 캐릭터가 상호작용 가능한 상태이면 (bCanInteract = true)
	if (bCanInteract && InteractBox) {
		// 틱마다 인터랙팅 가능한 액터(인터랙터블 컴포넌트가 있는 액터) 찾기
		FindInteractable();
	}

	
}

void APlayerCharacter::FindInteractable()
{
	if (InteractBox) { // null Check
		CurrentInteractables.Empty(); // 찾기 전 비우기

		TArray<AActor*> OverlappedActors;
		InteractBox->GetOverlappingActors(OverlappedActors); // InterectBox 범위 안의 액터 들고오기

		// 범위 안 액터들 중에서 UPlayerInteractableComponent를 상속받은 컴포넌트가 있는지 확인
		for (const AActor* OverlappedActor : OverlappedActors) {
			UPlayerInteractableComponent* InteractableComponent = OverlappedActor->GetComponentByClass<UPlayerInteractableComponent>();

			if(InteractableComponent){ // 있다면
				FHitResult HitResult;
				// 벽에 가려지는지 확인
				GetWorld()->LineTraceSingleByChannel(
					HitResult,
					GetCapsuleComponent()->GetComponentLocation(),
					OverlappedActor->GetActorLocation(), 
					InteractLineChannel, 
					FCollisionQueryParams(FName(), false, this)
				);

				// 안가려지는 경우
				if (HitResult.GetActor() == OverlappedActor) {
					// CurrentInteractables에 추가
					CurrentInteractables.Add(InteractableComponent);
				}
			}
		}

		if (CurrentInteractables.IsEmpty()) { // 찾아진 컴포넌트가 없다면
			if (CurrentSelectedInteractable) { // 현재 포커스된 액터의 포커싱 종료
				CurrentSelectedInteractable->EndFocus();
				CurrentSelectedInteractable = nullptr;
			}
		}
		else { // 찾아진 컴포넌트가 있다면 제일 가까운 컴포넌트 포커싱
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
	//입력 끊긴 뒤 경과 시간 계산
	float TimeSinceLastLook = GetWorld()->GetTimeSeconds() - LastLookTime;

	//Look 이후 딜레이 시간 지나면 캐릭터 쪽으로 카메라 부드럽게 다시 돌아옴
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
	//카메라의 상대 위치값
	float Sens = 60.f;
	float DeltaYaw = -Value.X * Sens * GetWorld()->GetDeltaSeconds();
	float DeltaPitch = -Value.Y * Sens * GetWorld()->GetDeltaSeconds();

	//몸통 회전과 겹치지 않게 카메라 위치를 상대 위치로 설정 및 변경
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
		// 2025-08-07 상호작용중 인풋 반응 다르게 적용
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
	//쪼그린 상태에서 점프 차단
	if (GetCharacterMovement()->IsCrouching()) return;
	Super::Jump();
}

void APlayerCharacter::Look(FVector2D Value)
{
	LastLookTime = GetWorld()->GetTimeSeconds();

	// 2025-08-06 이우주 : 사이드뷰 카메라 입력 함수로 묶기 + 3인칭일 경우 입력 반응 다르게
	if (bIsSideView) { // 사이드뷰일 경우
		SideViewLook(Value);
	}
	else { // 사이드뷰가 아닌 경우 3인칭 시점
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

	// 이미 대화창이 열려 있으면 새 대화 시작 금지 (중복 방지)
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
		if (AWorldHUD* HUD = PC ? Cast<AWorldHUD>(PC->GetHUD()) : nullptr)
			if (HUD->ConversationWidget && HUD->ConversationWidget->IsInViewport())
				return;

	//플레이어 앞방향으로, Pawn만 히트
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
	
	//들고 있는 상태면 내려놓기로 
	if (HeldPickup)
	{
		bCanInteract = false;

		if (AWorldPlayerController* PC = Cast<AWorldPlayerController>(GetController()))
		{
			PC->RemoveGameIMC(); //내려놓는 동안 입력 제거
		}

		//Pickup 몽타주의 PutDown 재생
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
			// 컨트롤러가 인풋을 소모하는 시점이 늦기 때문에 이전 시점 인풋을 사용하는 경우 방지
			PC->RotationInput = FRotator::ZeroRotator;

			CameraBoom->TargetArmLength = ThirdPersonCameraBoom->TargetArmLength;
			CameraBoom->SetRelativeLocation(ThirdPersonCameraBoom->GetRelativeLocation());
			CameraBoom->TargetOffset = ThirdPersonCameraBoom->TargetOffset;
			CameraBoom->SocketOffset = ThirdPersonCameraBoom->SocketOffset;

			// 플레이어 캠으로 전환
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
			// 컨트롤러가 인풋을 소모하는 시점이 늦기 때문에 이전 시점 인풋을 사용하는 경우 방지
			PC->RotationInput = FRotator::ZeroRotator;

			CameraBoom->TargetArmLength = SideViewArmLength;
			CameraBoom->SetRelativeLocation(SideViewRelativeLocation);
			CameraBoom->TargetOffset = SideViewTargetOffset;
			CameraBoom->SocketOffset = SideViewSocketOffset;

			// 액티브 캠으로 전환
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


//AnimBP에 구현돼 있음!
void APlayerCharacter::Anim_PickupAttach()
{
	if (PendingPickup)
	{
		PendingPickup->AttachToHand();
	}
}

//AnimBP에 구현돼 있음!
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

//AnimBP에 구현돼 있음!
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