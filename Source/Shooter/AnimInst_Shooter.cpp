// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInst_Shooter.h"
#include "ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAnimInst_Shooter::UpdateAnimProperties(float Time)
{
	if (ShooterCharacter == NULL)
	{
		ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
	}

	if (ShooterCharacter)
	{
		FVector Velocity(ShooterCharacter->GetVelocity());
		Velocity.Z = 0;

		this->Speed = Velocity.Size();

		this->bAir = ShooterCharacter->GetCharacterMovement()->IsFalling();
	
		this->bAcceleration = (ShooterCharacter->GetCharacterMovement()
				->GetCurrentAcceleration().Size() > 0.f);

		//UE_LOG(LogTemp, Warning, L"speed %f Air %d Acceler %d Time %f", Speed, bAir, bAcceleration, Time);
	}
}
//
//void UAnimInst_Shooter::NativeInitializeAnimation()
//{
//	ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
//}
