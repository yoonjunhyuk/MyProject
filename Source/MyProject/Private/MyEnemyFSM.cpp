#include "MyEnemyFSM.h"
#include "MyCharacter.h"
#include "MyEnemy.h"
#include <Kismet/GameplayStatics.h>
#include <Components/CapsuleComponent.h>

// Sets default values for this component's properties
UMyEnemyFSM::UMyEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMyEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	auto Actor = UGameplayStatics::GetActorOfClass(GetWorld(), AMyCharacter::StaticClass());
	Target = Cast<AMyCharacter>(Actor);
	me = Cast<AMyEnemy>(GetOwner());
}


// Called every frame
void UMyEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mState)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	}
}

void UMyEnemyFSM::IdleState()
{
	CurrentTime += GetWorld()->DeltaTimeSeconds;
	if (CurrentTime > IdleDelayTime)
	{
		mState = EEnemyState::Move;
		CurrentTime = 0;
	}
}

void UMyEnemyFSM::MoveState()
{
	FVector Destination = Target->GetActorLocation();
	FVector EnemyDirection = Destination - me->GetActorLocation();
	me->AddMovementInput(EnemyDirection.GetSafeNormal());

	if (EnemyDirection.Size() < AttackRange)
	{
		mState = EEnemyState::Attack;
	}

	float Distance = FVector::Distance(Target->GetActorLocation(), me->GetActorLocation());
	if (Distance > AttackRange)
	{
		mState = EEnemyState::Move;
	}
}

void UMyEnemyFSM::AttackState()
{
	CurrentTime += GetWorld()->DeltaTimeSeconds;
	if (CurrentTime > AttackDelayTime)
	{
		CurrentTime = 0;
	}
}

void UMyEnemyFSM::DamageState()
{
	CurrentTime += GetWorld()->DeltaTimeSeconds;
	if (CurrentTime > DamageDelayTime)
	{
		mState = EEnemyState::Idle;
		CurrentTime = 0;

		float distance = FVector::Distance(Target->GetActorLocation(), me->GetActorLocation());
		if (distance > AttackRange)
		{
			mState = EEnemyState::Move;
		}
	}
}

void UMyEnemyFSM::DieState()
{
	FVector P0 = me->GetActorLocation();
	FVector vt = FVector::DownVector * DieSpeed * GetWorld()->DeltaTimeSeconds;
	FVector P = P0 + vt;
	me->SetActorLocation(P);

	if (P.Z < -200.0f)
	{
		me->Destroy();
	}
}

void UMyEnemyFSM::OnDamageProcess()
{
	hp--;
	if (hp > 0)
	{
		mState = EEnemyState::Damage;
	}
	else
	{
		mState = EEnemyState::Die;
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}