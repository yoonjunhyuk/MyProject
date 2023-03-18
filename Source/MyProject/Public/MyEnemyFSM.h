// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyEnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Move,
	Attack,
	Damage,
	Die,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UMyEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	EEnemyState mState = EEnemyState::Idle;

	UPROPERTY(EditDefaultsOnly, Category = FSM)
	float IdleDelayTime = 1;

	UPROPERTY(VisibleAnywhere, Category = FSM)
	class AMyCharacter* Target;

	UPROPERTY()
	class AMyEnemy* me;

	UPROPERTY(EditAnywhere, Category = FSM)
	float AttackRange = 100.0f;

	UPROPERTY(EditAnywhere, Category = FSM)
	float AttackDelayTime = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	float hp = 2.0f;

	UPROPERTY(EditAnywhere, Category = FSM)
	float DamageDelayTime = 2.0f;

	UPROPERTY(EditAnywhere, Category = FSM)
	float DieSpeed = 70.0f;

	void IdleState();
	void MoveState();
	void AttackState();
	void DamageState();
	void DieState();
	void OnDamageProcess();

	float CurrentTime = 0;
};
