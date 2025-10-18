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

	// 가스레인지 위에 약물들 숨기기
	OatComp->SetHiddenInGame(true);
	BrownRiceComp->SetHiddenInGame(true);
	CinnamonComp->SetHiddenInGame(true);
	MakingMedicine->SetHiddenInGame(true);
	
	//1~3번째 정답
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
	CompletedMC->ResetMedicine(); // 완성된 약은 reset과 hidden 반대로 사용. 왜냐하면, 완성된 약은 처음부터 보이면 안되니까
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
	if (Cast<APlayerCharacter>(OtherActor)&& Cast<APlayerCharacter>(OtherActor)->HeldPickup) // 오버랩된 게 플레이어이고, 플레이어가 무언가를 들고 있을때
	{
		if (UQuestSystemComponent::CurQuestState != EQuestState::MomMix) return;			 // 조합퍼즐을 푸는 상태가 아니면 return
		APlayerCharacter* CurCharacter = Cast<APlayerCharacter>(OtherActor);						
		AWorldPlayerController* WPC = Cast< AWorldPlayerController>(CurCharacter->GetController());	
		AWorldHUD* WH = Cast< AWorldHUD>(WPC->GetHUD());
		if (CurCharacter && WPC && WH)
		{
			WH->SetPutDownVisible(true); // 내려놓기 위젯 띄우기
		}
	}

	if (!Cast<ABaseMedicineElements>(OtherActor))return;

	if (Cast<AMedicineOat>(OtherActor)) // 귀리
	{
		IsOat = true;
		++FindMedicine;
		OatComp->SetHiddenInGame(false);				  // 가스레인지 위에 보이게 하기
		Cast<AMedicineOat>(OtherActor)->HiddenMedicine(); // 오버랩된 인스턴스는 안보이게 하기
	}
	else if (Cast<AMedicineBrowRice>(OtherActor)) // 현미
	{
		IsBrownRice = true;
		++FindMedicine;
		BrownRiceComp->SetHiddenInGame(false);					// 가스레인지 위에 보이게 하기
		Cast<AMedicineBrowRice>(OtherActor)->HiddenMedicine();  // 오버랩된 인스턴스는 안보이게 하기
	}
	else if (Cast<AMedicineCinnamon>(OtherActor)) // 시나몬
	{
		IsCinnamon = true;
		++FindMedicine;
		CinnamonComp->SetHiddenInGame(false);					// 가스레인지 위에 보이게 하기
		Cast<AMedicineCinnamon>(OtherActor)->HiddenMedicine();  // 오버랩된 인스턴스는 안보이게 하기
	}

	if (FindMedicine == COMPLETE_FIND) // 전부 모았을때
	{
		//상호작용 콜리젼박스 비활성화 및 가스레인지 위 약품들 안보이게 하기. 가열 중인 그릇 띄우기
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
