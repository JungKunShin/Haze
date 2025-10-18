// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Camera/CameraZone.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Characters/PlayerCharacters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

// Sets default values
ACameraZone::ACameraZone()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(GetRootComponent());
	Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Trigger->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	Trigger->SetGenerateOverlapEvents(true);
	Trigger->SetBoxExtent(FVector(50, 50, 50));
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ACameraZone::EnterZone);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &ACameraZone::ExitZone);


	//경로 시작과 끝
	PathStart = CreateDefaultSubobject<USceneComponent>(TEXT("PathStart"));
	PathStart->SetupAttachment(GetRootComponent());
	PathEnd = CreateDefaultSubobject<USceneComponent>(TEXT("PathEnd"));
	PathEnd->SetupAttachment(GetRootComponent());

	PathStart->SetRelativeLocation(FVector(0, -300, 200));
	PathEnd->SetRelativeLocation(FVector(0, 300, 200));

	// 카메라를 CamRig에 붙여 런타임에 카메라만 움직이게
	CamRig = CreateDefaultSubobject<USceneComponent>(TEXT("CamRig"));
	CamRig->SetupAttachment(GetRootComponent());
	CamRig->SetMobility(EComponentMobility::Movable);

	GetCameraComponent()->SetupAttachment(CamRig);
	GetCameraComponent()->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void ACameraZone::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void ACameraZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (!bActive || !TargetChar || !Trigger || !CamRig) return;

	//경로 앵커 좌표
	const FVector L = PathStart ? PathStart->GetComponentLocation() : CamRig->GetComponentLocation();
	const FVector R = PathEnd ? PathEnd->GetComponentLocation() : CamRig->GetComponentLocation();

	//트리거 로컬공간에서 플레이어 위치를 선형보간(0~1)의 근거로 사용
	const FVector Ext = Trigger->GetScaledBoxExtent();
	if (Ext.GetAbsMax() < KINDA_SMALL_NUMBER) return;

	const FTransform T = Trigger->GetComponentTransform();
	const FVector   Local = T.InverseTransformPosition(TargetChar->GetActorLocation());
	float Alpha = (Local.Y + Ext.Y) / FMath::Max(1.f, (Ext.Y * 2.f));
	Alpha = FMath::Clamp(Alpha, 0.f, 1.f);

	//알파 위치로 CamRig 이동
	const FVector TargetLoc = FMath::Lerp(L, R, Alpha);
	CamRig->SetWorldLocation(
		FMath::VInterpTo(CamRig->GetComponentLocation(), TargetLoc, DeltaTime, InterpSpeed));


	CamRig->SetWorldLocation(FMath::VInterpTo(CamRig->GetComponentLocation(), TargetLoc, DeltaTime, InterpSpeed));

}

void ACameraZone::EnterZone(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (bActive) return;

	TargetChar = Cast<APlayerCharacter>(Other);
	if (!TargetChar) return;

	PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC) return;

	//첫 프레임부터 올바른 자리로 이동. 현재 알파로 CamRig 초기 위치 세팅
	const FVector L = PathStart ? PathStart->GetComponentLocation() : GetActorLocation();
	const FVector R = PathEnd ? PathEnd->GetComponentLocation() : GetActorLocation();

	const FVector Ext = Trigger->GetScaledBoxExtent();
	float Alpha = 0.5f;
	if (Ext.GetAbsMax() >= KINDA_SMALL_NUMBER)
	{
		const FTransform T = Trigger->GetComponentTransform();
		const FVector   Local = T.InverseTransformPosition(TargetChar->GetActorLocation());
		Alpha = (Local.Y + Ext.Y) / FMath::Max(1.f, (Ext.Y * 2.f));
		Alpha = FMath::Clamp(Alpha, 0.f, 1.f);
	}
	if (CamRig) CamRig->SetWorldLocation(FMath::Lerp(L, R, Alpha));

	//카메라 전환
	FViewTargetTransitionParams Params;
	Params.BlendTime = BlendInTime;
	Params.BlendFunction = EViewTargetBlendFunction::VTBlend_Cubic;

	PC->bAutoManageActiveCameraTarget = false;
	if (PC->IsLocalController()) PC->SetViewTarget(this, Params);
	else                         PC->ClientSetViewTarget(this, Params);

	bActive = true;
	SetActorTickEnabled(true);

	//플레이어 카메라 팔로우 잠시 끊기
	if (TargetChar) TargetChar->LockCameraFollow(true);

}


void ACameraZone::ExitZone(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (!bActive || Other != TargetChar || !PC) return;

	//같은 액터의 다른 컴포넌트가 먼저 빠져나가며 endoverlap 되는 상황 방지
	if (Trigger && Trigger->IsOverlappingActor(TargetChar)) return;

	FViewTargetTransitionParams Params;
	Params.BlendTime = BlendOutTime;
	Params.BlendFunction = EViewTargetBlendFunction::VTBlend_Cubic;

	if (PC->IsLocalController()) PC->SetViewTarget(TargetChar, Params);
	else                         PC->ClientSetViewTarget(TargetChar, Params);

	PC->bAutoManageActiveCameraTarget = true;

	if (TargetChar) TargetChar->LockCameraFollow(false); 

	bActive = false;
	TargetChar = nullptr;
	PC = nullptr;
	SetActorTickEnabled(false);

}
