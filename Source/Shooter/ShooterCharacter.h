// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "CoreMinimal.h"
#include "ShooterCharacter.generated.h"

UCLASS()
class SHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void TurnAtRate(float Rate); // Поворот головой влево\вправо [-1 1]

	void LookUpAtRate(float Rate); // Поворот головой вверх\вниз [-1 1]

	//Когда нажимаем левую клавишу мыши
	void FireWeapon();

	bool DoingFireEffect(const FVector StartShot, FVector& OutBeamLocation);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:
	/** Штатив камеры, на расстоянии от игрока r*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Камера, следующая за игроком  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float BaseLookUpRate;

	//Дальность луча куда летит пуля
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = true))
		float LengthTrace;

	//Звуки выстрела
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = true))
		class USoundCue* SoundGunFire;

	// Вспышка выстрела из BarrelSocket
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = true))
		class UParticleSystem* MuzzleFlash;

	//Добавления эффекта отдачи
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = true))
		class UAnimMontage* HipFireMontage;

	// Эффект попадания пули
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = true))
		UParticleSystem* ImpactParticle;

	// Дым от выстрела
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = true))
		UParticleSystem* BeamParticle;

public:

	FORCEINLINE USpringArmComponent* GetCameraBoom() const
		{ return CameraBoom;  }

	FORCEINLINE UCameraComponent* GetFollowCamera() const
	{
		return FollowCamera;
	}
};
