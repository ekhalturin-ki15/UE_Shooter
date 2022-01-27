// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "GameFramework\SpringArmComponent.h"
#include "Camera\CameraComponent.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject
		<USpringArmComponent>(L"CameraBoom");
		
	//Create CameraBoom, притяшивается, если произойдёт коллизия
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 1000.f;
	CameraBoom->bUsePawnControlRotation = true; // Вращается

	//UCameraComponent* FollowCamera
	FollowCamera = CreateDefaultSubobject
		<UCameraComponent>(L"FollowCamera");

	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Крепим камеру на штатив
	FollowCamera->bUsePawnControlRotation = false; // На самом штативе камера не вращается

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

		
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

