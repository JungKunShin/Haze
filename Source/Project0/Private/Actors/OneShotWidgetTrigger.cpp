#include "Actors/OneShotWidgetTrigger.h"
#include "HUD/WorldHUD.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

AOneShotWidgetTrigger::AOneShotWidgetTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(Box);


	Box->SetBoxExtent(FVector(120.f));               
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Box->SetCollisionObjectType(ECC_WorldDynamic);
	Box->SetCollisionResponseToAllChannels(ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Box->SetGenerateOverlapEvents(true);

	Box->OnComponentBeginOverlap.AddDynamic(this, &AOneShotWidgetTrigger::OnBeginOverlap);
}

void AOneShotWidgetTrigger::OnBeginOverlap(UPrimitiveComponent* Comp, AActor* Other, UPrimitiveComponent* OtherComp,
	int32 BodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	
	if (bPlayerOnly)
	{
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
		if (Other != PlayerPawn) return;
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC) return;

	// WorldHUD�� ���� ���� 5�� ǥ�á� ��û
	if (AWorldHUD* WorldHUD = Cast<AWorldHUD>(PC->GetHUD()))
	{
		WorldHUD->ShowConversationKeyOnce();   // �⺻ 5��(���ϸ� ShowConversationKeyOnce(5.f))
	}
}