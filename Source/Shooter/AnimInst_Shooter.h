// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInst_Shooter.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UAnimInst_Shooter : public UAnimInstance
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
		void UpdateAnimProperties(float Time);

	/*virtual void NativeInitializeAnimation() override;*/
	
private:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Movement", meta = (AllowPrivateAccess = true))
		class AShooterCharacter* ShooterCharacter;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Movement", meta = (AllowPrivateAccess = true))
		float Speed;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Movement", meta = (AllowPrivateAccess = true))
		bool bAir;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Movement", meta = (AllowPrivateAccess = true))
		bool bAcceleration;

};
