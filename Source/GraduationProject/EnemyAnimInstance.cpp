// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
UEnemyAnimInstance::UEnemyAnimInstance()
{
	MovingThreshould = 3.0f;
	
}

void UEnemyAnimInstance::NativeInitializeAnimation()
{

	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UEnemyAnimInstance::NativeUpdateAnimation(float dt)
{

	Super::NativeUpdateAnimation(dt);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;
		bIsRun = GroundSpeed < 500.0f;
		
	}
}
