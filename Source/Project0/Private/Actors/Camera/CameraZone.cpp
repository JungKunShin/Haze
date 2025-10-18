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


	//��� ���۰� ��
	PathStart = CreateDefaultSubobject<USceneComponent>(TEXT("PathStart"));
	PathStart->SetupAttachment(GetRootComponent());
	PathEnd = CreateDefaultSubobject<USceneComponent>(TEXT("PathEnd"));
	PathEnd->SetupAttachment(GetRootComponent());

	PathStart->SetRelativeLocation(FVector(0, -300, 200));
	PathEnd->SetRelativeLocation(FVector(0, 300, 200));

	// ī�޶� CamRig�� �ٿ� ��Ÿ�ӿ� ī�޶� �����̰�
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

	//��� ��Ŀ ��ǥ
	const FVector L = PathStart ? PathStart->GetComponentLocation() : CamRig->GetComponentLocation();
	const FVector R = PathEnd ? PathEnd->GetComponentLocation() : CamRig->GetComponentLocation();

	//Ʈ���� ���ð������� �÷��̾� ��ġ�� ��������(0~1)�� �ٰŷ� ���
	const FVector Ext = Trigger->GetScaledBoxExtent();
	if (Ext.GetAbsMax() < KINDA_SMALL_NUMBER) return;

	const FTransform T = Trigger->GetComponentTransform();
	const FVector   Local = T.InverseTransformPosition(TargetChar->GetActorLocation());
	float Alpha = (Local.Y + Ext.Y) / FMath::Max(1.f, (Ext.Y * 2.f));
	Alpha = FMath::Clamp(Alpha, 0.f, 1.f);

	//���� ��ġ�� CamRig �̵�
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

	//ù �����Ӻ��� �ùٸ� �ڸ��� �̵�. ���� ���ķ� CamRig �ʱ� ��ġ ����
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

	//ī�޶� ��ȯ
	FViewTargetTransitionParams Params;
	Params.BlendTime = BlendInTime;
	Params.BlendFunction = EViewTargetBlendFunction::VTBlend_Cubic;

	PC->bAutoManageActiveCameraTarget = false;
	if (PC->IsLocalController()) PC->SetViewTarget(this, Params);
	else                         PC->ClientSetViewTarget(this, Params);

	bActive = true;
	SetActorTickEnabled(true);

	//�÷��̾� ī�޶� �ȷο� ��� ����
	if (TargetChar) TargetChar->LockCameraFollow(true);

}


void ACameraZone::ExitZone(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (!bActive || Other != TargetChar || !PC) return;

	//���� ������ �ٸ� ������Ʈ�� ���� ���������� endoverlap �Ǵ� ��Ȳ ����
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
