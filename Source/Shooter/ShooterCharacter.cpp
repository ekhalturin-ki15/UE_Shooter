// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterCharacter.h"
#include <GameFramework\SpringArmComponent.h>
#include <Camera\CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Sound/SoundCue.h>
#include <Kismet/GameplayStatics.h>
#include <Engine/SkeletalMeshSocket.h>
#include <Animation/AnimInstance.h>
#include <Engine/EngineTypes.h>
#include <DrawDebugHelpers.h>
#include <particles/ParticleSystemComponent.h>


// Sets default values
AShooterCharacter::AShooterCharacter() {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject
		<USpringArmComponent>(L"CameraBoom");
		
	//Create CameraBoom, �������������, ���� ��������� ��������
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true; // ���������
	//CameraBoom->SocketOffset = (FVector(0.f, 50.f, 50.f));

	//UCameraComponent* FollowCamera
	FollowCamera = CreateDefaultSubobject
		<UCameraComponent>(L"FollowCamera");

	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // ������ ������ �� ������
	FollowCamera->bUsePawnControlRotation = false; // �� ����� ������� ������ �� ���������

	this->BaseTurnRate = 45.f; // 45 �������� � �������
	this->BaseLookUpRate = 45.f;
	this->LengthTrace = 50'000.f;

	// ��� �� ��������� ������ �  &APawn::AddControllerYawInput
	this->bUseControllerRotationPitch = false; // �������� �� Y (������) (����� ����)
	this->bUseControllerRotationYaw = true; // �������� �� Z (��������) (����� ������)
	this->bUseControllerRotationRoll = false; // �������� �� X (��������)

	//������ ������� � ���� �������� (CharacterMovement)
	//��-�� UseControllerRotationYaw ��� RotationToMovement
	GetCharacterMovement()->bOrientRotationToMovement = false; // �������� � ����������� ��������� 
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 800.0f, 0.0f); //�������� ��������
	GetCharacterMovement()->JumpZVelocity = 1200.0f;
	GetCharacterMovement()->AirControl = 0.6f; //����������� � �������

	UE_LOG(LogTemp, Warning, L"%f %d", GetCharacterMovement()->JumpZVelocity, bUseControllerRotationYaw);
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay() {
	Super::BeginPlay();
}

void AShooterCharacter::MoveForward(float Value)
{
	if (!Controller || !Value) return;
	
	const FRotator Rotation{ Controller->GetControlRotation() };
	//const FRotator YawRotation{ 0, Rotation.Yaw, 0 }; //Only for gamepad

	const FVector Dir {
		FRotationMatrix{Rotation}.GetUnitAxis(EAxis::X)
	};

	AddMovementInput(Dir, Value);
	
}

void AShooterCharacter::MoveRight(float Value)
{
	if (!Controller || !Value) return;

	const FRotator Rotation{ Controller->GetControlRotation() };
	//const FRotator YawRotation{ 0, Rotation.Yaw, 0 }; //Only for gamepad

	const FVector Dir {
		FRotationMatrix{Rotation}.GetUnitAxis(EAxis::Y)
	};

	AddMovementInput(Dir, Value);
}

void AShooterCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * this->BaseTurnRate 
		* GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookUpAtRate(float Rate)
{
	//UE_LOG(LogTemp, Warning, L"%f", Rate);
	AddControllerPitchInput(Rate * this->BaseLookUpRate
		* GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::FireWeapon()
{
	//UE_LOG(LogTemp, Warning, L"shooot");
	if (this->SoundGunFire)	{
		UGameplayStatics::PlaySound2D(this, SoundGunFire);
	}
	const USkeletalMeshSocket* BarrelSocket = GetMesh()->GetSocketByName(L"BarrelSocket");
	if (BarrelSocket){
		const FTransform SocketTransform =
			BarrelSocket->GetSocketTransform(GetMesh());

		if (MuzzleFlash){
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash,
				SocketTransform);
		}

		FVector HitLocation;
		if (DoingFireEffect(SocketTransform.GetLocation(),
			HitLocation))
		{
			if (ImpactParticle)
				UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(), ImpactParticle, HitLocation);

			//������� ��� �� ����
			if (BeamParticle)
			{
				UParticleSystemComponent* Beam =
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
						BeamParticle, SocketTransform); // ������� � ������ ����

				if (Beam)
					Beam->SetVectorParameter(L"Target", HitLocation); //������ ������ �������, ������ ���������
			}
		}
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	//������ �������� ����� ���������� ���������
	if (AnimInstance && HipFireMontage)
	{
		AnimInstance->Montage_Play(HipFireMontage);
		AnimInstance->Montage_JumpToSection(L"StartFire");
	}

}

bool AShooterCharacter::DoingFireEffect(const FVector StartShot,
	FVector& OutBeamLocation)
{
	FVector2D SizeScreen;
	if (!GEngine) return false;
	if (!GEngine->GameViewport) return false;
	GEngine->GameViewport->GetViewportSize(SizeScreen);
	
	//���������, ��� �����������
	FVector2D CrosshairLocation = SizeScreen / 2.f;

	FVector CrosshairWorld;
	FVector CrosshairWorldDir;

	// ��������, ��� ����������� � �������� ����
	bool bOkScreenToWorld =
		UGameplayStatics::DeprojectScreenToWorld(
			UGameplayStatics::GetPlayerController(this, 0),
			CrosshairLocation, CrosshairWorld, CrosshairWorldDir);

	if (!bOkScreenToWorld) return false; //���������� �� �������������
	
	FHitResult ScreenShoot;
	const FVector Start{ CrosshairWorld };
	const FVector End{ CrosshairWorld +
		CrosshairWorldDir * LengthTrace };

	//���� �������� ������� �� ����� �������� ECC_Visibility
	OutBeamLocation = End;
	GetWorld()->LineTraceSingleByChannel(ScreenShoot,
		Start, End, ECollisionChannel::ECC_Visibility);

	//���� �������� ���� ������ ������ ������ ��� ������ ����
	if (ScreenShoot.bBlockingHit)
		OutBeamLocation = ScreenShoot.Location;

	//���� ������������ ���� ����� ����
	FHitResult WeaponTraceHit;
	const FVector _Start{ StartShot };
	const FVector _End{ OutBeamLocation };
	GetWorld()->LineTraceSingleByChannel(
		WeaponTraceHit, _Start, _End,
		ECollisionChannel::ECC_Visibility);

	if (WeaponTraceHit.bBlockingHit)
		OutBeamLocation = WeaponTraceHit.Location;

	return true;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacter::MoveRight);

	PlayerInputComponent->BindAxis("TurnRate", this, &AShooterCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AShooterCharacter::LookUpAtRate);

	PlayerInputComponent->BindAxis("MouseX", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("MouseY", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this,  &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this,  &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("FireButton", IE_Pressed, this, &AShooterCharacter::FireWeapon);
}

