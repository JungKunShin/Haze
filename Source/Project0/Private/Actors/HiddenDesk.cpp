// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/HiddenDesk.h"
#include "Components/BoxComponent.h"
#include "Characters/PlayerCharacters/PlayerCharacter.h"

AHiddenDesk::AHiddenDesk()
{
	DeskMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DeskMesh"));
	RootComponent = DeskMesh;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(RootComponent);
	InnerBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("InnerBoxComp"));
	InnerBoxComp->SetupAttachment(BoxComp);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	InnerBoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InnerBoxComp->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void AHiddenDesk::BeginPlay()
{
	Super::BeginPlay();

	InnerBoxComp->OnComponentBeginOverlap.AddDynamic(this, &AHiddenDesk::OnOverlapDeskStart);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &AHiddenDesk::OnOverlapDeskFinish);
}

void AHiddenDesk::OnOverlapDeskStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* PlayerCh = Cast< APlayerCharacter>(OtherActor);

	if (!PlayerCh)return;

	PlayerCh->IsDeskIn = true;
}

void AHiddenDesk::OnOverlapDeskFinish(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter* PlayerCh = Cast< APlayerCharacter>(OtherActor);

	if (!PlayerCh)return;

	PlayerCh->IsDeskIn = false;
}
