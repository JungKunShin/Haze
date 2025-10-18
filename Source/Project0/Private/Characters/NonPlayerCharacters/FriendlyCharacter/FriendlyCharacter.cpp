// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NonPlayerCharacters/FriendlyCharacter/FriendlyCharacter.h"
#include "Characters/PlayerCharacters/PlayerCharacter.h"
#include "ActorComponents/QuestSystemComponent.h"

#include "Components/SphereComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"

AFriendlyCharacter::AFriendlyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AFriendlyCharacter::ForceStandUpAndFollow(APlayerCharacter* Player)
{
	if (!Player) return;

	if (bIsFollowing && FollowTarget.Get() == Player)
		return;

	ExitSitPose();

	if (SittingUpMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		bIsStandingUp = true;

		UAnimInstance* Anim = GetMesh()->GetAnimInstance();
		if (!Anim->Montage_IsPlaying(SittingUpMontage))
		{
			if (AAIController* AI = Cast<AAIController>(GetController()))
			{
				AI->StopMovement();
			}
			Anim->Montage_Play(SittingUpMontage);
		}

		FollowTarget = Player;

		const float Total = SittingUpMontage->GetPlayLength();
		const float Delay = FMath::Max(0.0f, Total - 0.05f);

		GetWorldTimerManager().ClearTimer(StartFollowDelayHandle);
		GetWorldTimerManager().SetTimer(
			StartFollowDelayHandle,
			this, &AFriendlyCharacter::StartFollowAfterDelay,
			Delay, false
		);
	}
	else
	{
		StartFollow(Player);
	}
}

void AFriendlyCharacter::BeginPlay()
{
	Super::BeginPlay();

	//앉아있는채로 시작
	SetSitPose();

	//거리 재기 로직을 주기적으로 돌림
	GetWorldTimerManager().SetTimer(FollowLogicTimer, this, &AFriendlyCharacter::UpdateFollowByDistance, RepathInterval, true, 0.0f);
	
}


bool AFriendlyCharacter::IsFollowUnlocked(APlayerCharacter*) const
{
	return bRequireQuestUnlock;
}

bool AFriendlyCharacter::TryStartFollow(APlayerCharacter* Player)
{
	if (!Player) return false;

	//이미 같은 대상을 따라가는 중이면 무시
	if (bIsFollowing && FollowTarget.Get() == Player)
		return true;

	StartFollow(Player);
	return true;
}

void AFriendlyCharacter::StartFollow(APlayerCharacter* Player)
{
	if (!Player) return;

	FollowTarget = Player;
	bIsFollowing = true;

	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		if (Move->MovementMode == MOVE_None)
			Move->SetMovementMode(MOVE_Walking);
	}


	//즉시 한 번 이동 이후 타이머로 유지
	TickFollow();
	GetWorldTimerManager().SetTimer(RepathTimer, this, &AFriendlyCharacter::TickFollow, RepathInterval, true, RepathInterval);
}

void AFriendlyCharacter::StopFollow()
{
	GetWorldTimerManager().ClearTimer(RepathTimer);
	bIsFollowing = false;
	FollowTarget = nullptr;

	//AI 컨트롤러 기반인데 BT는 없음(NavMesh 필요)
	if (AAIController* AI = Cast<AAIController>(GetController()))
	{
		AI->StopMovement();
	}
}

void AFriendlyCharacter::UpdateFollowByDistance()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!Player) return;

	const float Dist = FVector::Dist(Player->GetActorLocation(), GetActorLocation());

	if (bFollowDebug)
	{
		const FVector C = GetActorLocation() + FVector(0, 0, 10);
		DrawDebugSphere(GetWorld(), C, TriggerRadius, 24, FColor::Green, false, RepathInterval, 0, 1.5f);  // 200
		DrawDebugSphere(GetWorld(), C, KeepFollowRadius, 32, FColor::Yellow, false, RepathInterval, 0, 1.5f);  // 2000
	}

	if (!bIsFollowing)
	{
		//시작 조건:트리거 반경 이내 + 특정 퀘스트 완료
		if (Dist <= TriggerRadius && IsFollowUnlocked(Player))
		{
			//이미 일어나는 중이면 재진입 금지
			if (bIsStandingUp || GetWorldTimerManager().IsTimerActive(StartFollowDelayHandle))
			{
				return;
			}


			ExitSitPose();

			UAnimInstance* Anim = GetMesh()->GetAnimInstance();

			//몽타주 재생, 몽타주 재생이 끝나야 따라가도록
			if (SittingUpMontage && Anim)
			{
				if (!Anim->Montage_IsPlaying(SittingUpMontage))
				{
					if (AAIController* AI = Cast<AAIController>(GetController()))
					{
						AI->StopMovement();
						Anim->Montage_Play(SittingUpMontage);
					}

					const float Total = SittingUpMontage->GetPlayLength();
					const float Delay = FMath::Max(0.0f, Total - 0.05f);

					GetWorldTimerManager().ClearTimer(StartFollowDelayHandle);
					GetWorldTimerManager().SetTimer(
						StartFollowDelayHandle,
						this, &AFriendlyCharacter::StartFollowAfterDelay,
						Delay, false
					);
				}
			}
			else
			{
				StartFollow(Player);
			}
		}
	}
	else
	{
		//유지 조건해제 조건
		if (Dist > KeepFollowRadius)
		{
			StopFollow();
		}
		//유지 중 이동 갱신
		else
		{
			TickFollow();
		}
	}
}

void AFriendlyCharacter::StartFollowAfterDelay()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!Player) return;

	//거리가 멀어졌다면 해당 체크도 필요함
	const float Dist = FVector::Dist(Player->GetActorLocation(), GetActorLocation());
	if (!IsFollowUnlocked(Player)) return;
	if (Dist > KeepFollowRadius)   return;

	StartFollow(Player);
}


void AFriendlyCharacter::TickFollow()
{
	APlayerCharacter* Target = FollowTarget.Get();
	if (!bIsFollowing || !Target)
	{
		StopFollow();
		return;
	}

	const float Dist = FVector::Dist(Target->GetActorLocation(), GetActorLocation());


	//충분히 가까우면 MoveTo 미갱신
	if (Dist <= AcceptRadius) return;

	if (AAIController* AI = Cast<AAIController>(GetController()))
	{
		AI->MoveToActor(Target, AcceptRadius);
	}
}

void AFriendlyCharacter::SetSitPose()
{

	//이동 막기(안전상)
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->DisableMovement(); 
	}

	if (SitLoop)
	{
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		GetMesh()->PlayAnimation(SitLoop, true);
	}
}

void AFriendlyCharacter::ExitSitPose()
{
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		if (Move->MovementMode == MOVE_None)
			Move->SetMovementMode(MOVE_Walking);
	}

	//기존 IdleWalkRun 모드로 돌아감
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
}
