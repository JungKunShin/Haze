// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Mix/MixGasStove.h"
#include "Components/BoxComponent.h"
#include "Actors/Mix/BaseMedicineElements.h"
#include "Actors/Mix/MedicineOat.h"
#include "Actors/Mix/MedicineBrowRice.h"
#include "Actors/Mix/MedicineCinnamon.h"
#include "Actors/Mix/CompletedMedicine.h"
#include "ActorComponents/PlayerMixComponent.h"
#include "Characters/PlayerCharacters/PlayerCharacter.h"
#include "PlayerControllers/WorldPlayerController.h"
#include "HUD/WorldHUD.h"

int32 AMixGasStove::FindMedicine = 0;

AMixGasStove::AMixGasStove()
{
	MixComp = CreateDefaultSubobject<UPlayerMixComponent>(TEXT("MixComp"));

	StoveMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StoveMesh"));
	RootComponent = StoveMesh;

	InteractComp = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractComp"));
	InteractComp->SetupAttachment(StoveMesh);

	OatComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("OatComp"));
	OatComp->SetupAttachment(StoveMesh);

	BrownRiceComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("BrownRiceComp"));
	BrownRiceComp->SetupAttachment(StoveMesh);

	CinnamonComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("CinnamonComp"));
	CinnamonComp->SetupAttachment(StoveMesh);

	MakingMedicine = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MakingMedicine"));
	MakingMedicine->SetupAttachment(StoveMesh);

	// ���������� ���� �๰�� �����
	OatComp->SetHiddenInGame(true);
	BrownRiceComp->SetHiddenInGame(true);
	CinnamonComp->SetHiddenInGame(true);
	MakingMedicine->SetHiddenInGame(true);
	
	//1~3��° ����
	FirstAnswer.Fire = EFireState::Weak;
	FirstAnswer.Time = EUseTime::OneHour;
	FirstAnswer.Temperature = 130;
	SecondAnswer.Fire = EFireState::Strong;
	SecondAnswer.Time = EUseTime::TwoHour;
	SecondAnswer.Temperature = 180;
	ThirdAnswer.Fire = EFireState::Middle;
	ThirdAnswer.Time = EUseTime::Half;
	ThirdAnswer.Temperature = 150;
}

void AMixGasStove::EndMix()
{
	CompletedMC->ResetMedicine(); // �ϼ��� ���� reset�� hidden �ݴ�� ���. �ֳ��ϸ�, �ϼ��� ���� ó������ ���̸� �ȵǴϱ�
	MakingMedicine->SetHiddenInGame(true);
}

void AMixGasStove::ResetMix()
{
	FindMedicine = 0;
	IsOat = false;
	IsBrownRice = false;
	IsCinnamon = false;

	OatComp->SetHiddenInGame(true);
	BrownRiceComp->SetHiddenInGame(true);
	CinnamonComp->SetHiddenInGame(true);
	MakingMedicine->SetHiddenInGame(true);
	InteractComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AMixGasStove::BeginPlay()
{
	Super::BeginPlay();

	InteractComp->OnComponentBeginOverlap.AddDynamic(this, &AMixGasStove::OnOverlapGasStove);
	InteractComp->OnComponentEndOverlap.AddDynamic(this, &AMixGasStove::OnOverlapGasStoveEnd);

	InitCollision();
}

void AMixGasStove::OnOverlapGasStove(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APlayerCharacter>(OtherActor)&& Cast<APlayerCharacter>(OtherActor)->HeldPickup) // �������� �� �÷��̾��̰�, �÷��̾ ���𰡸� ��� ������
	{
		if (UQuestSystemComponent::CurQuestState != EQuestState::MomMix) return;			 // ���������� Ǫ�� ���°� �ƴϸ� return
		APlayerCharacter* CurCharacter = Cast<APlayerCharacter>(OtherActor);						
		AWorldPlayerController* WPC = Cast< AWorldPlayerController>(CurCharacter->GetController());	
		AWorldHUD* WH = Cast< AWorldHUD>(WPC->GetHUD());
		if (CurCharacter && WPC && WH)
		{
			WH->SetPutDownVisible(true); // �������� ���� ����
		}
	}

	if (!Cast<ABaseMedicineElements>(OtherActor))return;

	if (Cast<AMedicineOat>(OtherActor)) // �͸�
	{
		IsOat = true;
		++FindMedicine;
		OatComp->SetHiddenInGame(false);				  // ���������� ���� ���̰� �ϱ�
		Cast<AMedicineOat>(OtherActor)->HiddenMedicine(); // �������� �ν��Ͻ��� �Ⱥ��̰� �ϱ�
	}
	else if (Cast<AMedicineBrowRice>(OtherActor)) // ����
	{
		IsBrownRice = true;
		++FindMedicine;
		BrownRiceComp->SetHiddenInGame(false);					// ���������� ���� ���̰� �ϱ�
		Cast<AMedicineBrowRice>(OtherActor)->HiddenMedicine();  // �������� �ν��Ͻ��� �Ⱥ��̰� �ϱ�
	}
	else if (Cast<AMedicineCinnamon>(OtherActor)) // �ó���
	{
		IsCinnamon = true;
		++FindMedicine;
		CinnamonComp->SetHiddenInGame(false);					// ���������� ���� ���̰� �ϱ�
		Cast<AMedicineCinnamon>(OtherActor)->HiddenMedicine();  // �������� �ν��Ͻ��� �Ⱥ��̰� �ϱ�
	}

	if (FindMedicine == COMPLETE_FIND) // ���� �������
	{
		//��ȣ�ۿ� �ݸ����ڽ� ��Ȱ��ȭ �� ���������� �� ��ǰ�� �Ⱥ��̰� �ϱ�. ���� ���� �׸� ����
		InteractComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		OatComp->SetHiddenInGame(true);
		BrownRiceComp->SetHiddenInGame(true);
		CinnamonComp->SetHiddenInGame(true);
		MakingMedicine->SetHiddenInGame(false);
	}
}

void AMixGasStove::OnOverlapGasStoveEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<APlayerCharacter>(OtherActor) && Cast<APlayerCharacter>(OtherActor)->HeldPickup)
	{
		APlayerCharacter* CurCharacter = Cast<APlayerCharacter>(OtherActor);
		AWorldPlayerController* WPC = Cast< AWorldPlayerController>(CurCharacter->GetController());
		AWorldHUD* WH = Cast< AWorldHUD>(WPC->GetHUD());
		if (CurCharacter && WPC && WH)
		{
			if (WH->PutDownWidget)
			{
				WH->SetPutDownVisible(false);
			}
		}
	}
}

void AMixGasStove::InitCollision()
{
	AActor* OatCompChild = OatComp->GetChildActor();
	AActor* BrownRiceCompChild = BrownRiceComp->GetChildActor();
	AActor* CinnamonCompChild = CinnamonComp->GetChildActor();

	UStaticMeshComponent* OatMeshComp = OatCompChild->FindComponentByClass<UStaticMeshComponent>();
	UStaticMeshComponent* BrownRiceMeshComp = BrownRiceCompChild->FindComponentByClass<UStaticMeshComponent>();
	UStaticMeshComponent* CinnamonMeshComp = CinnamonCompChild->FindComponentByClass<UStaticMeshComponent>();

	if (OatMeshComp)
	{
		OatMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (BrownRiceMeshComp)
	{
		BrownRiceMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (CinnamonMeshComp)
	{
		CinnamonMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	MakingMedicine->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
