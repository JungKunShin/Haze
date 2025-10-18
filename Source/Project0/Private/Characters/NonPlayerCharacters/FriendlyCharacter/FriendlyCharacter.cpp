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

	//�ɾ��ִ�ä�� ����
	SetSitPose();

	//�Ÿ� ��� ������ �ֱ������� ����
	GetWorldTimerManager().SetTimer(FollowLogicTimer, this, &AFriendlyCharacter::UpdateFollowByDistance, RepathInterval, true, 0.0f);
	
}


bool AFriendlyCharacter::IsFollowUnlocked(APlayerCharacter*) const
{
	return bRequireQuestUnlock;
}

bool AFriendlyCharacter::TryStartFollow(APlayerCharacter* Player)
{
	if (!Player) return false;

	//�̹� ���� ����� ���󰡴� ���̸� ����
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


	//��� �� �� �̵� ���� Ÿ�̸ӷ� ����
	TickFollow();
	GetWorldTimerManager().SetTimer(RepathTimer, this, &AFriendlyCharacter::TickFollow, RepathInterval, true, RepathInterval);
}

void AFriendlyCharacter::StopFollow()
{
	GetWorldTimerManager().ClearTimer(RepathTimer);
	bIsFollowing = false;
	FollowTarget = nullptr;

	//AI ��Ʈ�ѷ� ����ε� BT�� ����(NavMesh �ʿ�)
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
		//���� ����:Ʈ���� �ݰ� �̳� + Ư�� ����Ʈ �Ϸ�
		if (Dist <= TriggerRadius && IsFollowUnlocked(Player))
		{
			//�̹� �Ͼ�� ���̸� ������ ����
			if (bIsStandingUp || GetWorldTimerManager().IsTimerActive(StartFollowDelayHandle))
			{
				return;
			}


			ExitSitPose();

			UAnimInstance* Anim = GetMesh()->GetAnimInstance();

			//��Ÿ�� ���, ��Ÿ�� ����� ������ ���󰡵���
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
		//���� �������� ����
		if (Dist > KeepFollowRadius)
		{
			StopFollow();
		}
		//���� �� �̵� ����
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

	//�Ÿ��� �־����ٸ� �ش� üũ�� �ʿ���
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


	//����� ������ MoveTo �̰���
	if (Dist <= AcceptRadius) return;

	if (AAIController* AI = Cast<AAIController>(GetController()))
	{
		AI->MoveToActor(Target, AcceptRadius);
	}
}

void AFriendlyCharacter::SetSitPose()
{

	//�̵� ����(������)
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

	//���� IdleWalkRun ���� ���ư�
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
}
