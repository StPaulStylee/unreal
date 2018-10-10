// Fill out your copyright notice in the Description page of Project Settings.

#include "Player_Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Animation/AnimInstance.h"

APlayer_Character::APlayer_Character()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	if (SpringArm)
	{
		// Must allows SetupAttachment as all components are required to have a parent
		SpringArm->SetupAttachment(GetCapsuleComponent());
		SpringArm->RelativeLocation = FVector(-20.0f, 0.0f, 40.0f);
		// How long the camera's spring arm is
		SpringArm->TargetArmLength = 0.0f;
		SpringArm->bDoCollisionTest = false;
		SpringArm->bUsePawnControlRotation = true;
		// Setting what we can and cannot inherit from the pawn rotation
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
	}

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	if (Camera)
	{
		Camera->FieldOfView = 90.0f;
		Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	}

	FP_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_MESH"));
	if (FP_Mesh)
	{
		FP_Mesh->SetupAttachment(Camera);
	}

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	if (GunMesh)
	{
		GunMesh->SetupAttachment(Camera);
	}

	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;

	bCanShoot = true;
	bUnlimitedAmmo = false;
	MaxAmmo = 10;
	CurrentAmmo = MaxAmmo;

	BaseTurnRate = 100.0f;
	BaseLookUpRate = 100.0f;
	CameraPitchMin = -89.0f;
	CameraPitchMax = 80.0f;

	SprintSpeed = 1500.0f;

	DamageAmount = 10.0f;

	FireAnimation = nullptr;

	TrailEffect = nullptr;
	HitEffect = nullptr;
}

void APlayer_Character::BeginPlay()
{
	// We call Super::BeginPlay to call it's parents BeginPlay.. Since it is a virtual method
	// we can trust that it's parent has it and is doing the same thing. If all SubClasses are calling
	// super on their virtual methods, we can trust that they get called all the way up 
	// the chain and this is good practice
	Super::BeginPlay();

	// This will attache the gun to the hand's mesh
	// The "GripPoint" has already been established on the skeletal mesh and hence we can reference it here
	GunMesh->AttachToComponent(FP_Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GripPoint"));

	// For when the max ammo value was changed
	if (CurrentAmmo != MaxAmmo)
	{
		CurrentAmmo = MaxAmmo;
	}
}

void APlayer_Character::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Assertion check - This is a macro that will cause the program to crash
	// if it is false. Think of it has a super aggressive if statment
	check(PlayerInputComponent);

	// Shoot input
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayer_Character::OnFire);

	// Movment input 
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayer_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayer_Character::MoveRight);
	
	// This points to 'ACharacter' because 'jump' is a built in method of the Character class
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Here you will notice we are just calling the Blueprint version of these methods... That is fine and is best practice
	// These will know to include the C++ implementation as well
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayer_Character::OnSprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayer_Character::OnSprintEnd);

	// Camera input
	PlayerInputComponent->BindAxis("Turn", this, &APlayer_Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayer_Character::LookUpRate);
}

void APlayer_Character::OnDeath_Implementation()
{
}

float APlayer_Character::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	// Don't continue if health is already below or at zero
	if (CurrentHealth <= 0.0f) {
		return 0.0f;
	}
	// Does the math for damaging the player
	const float SubtractedHealth = CurrentHealth - DamageAmount;
	//Check if the subtracted health is equal or lower than zero
	if (FMath::IsNearlyZero(SubtractedHealth) || SubtractedHealth < 0.0f)
	{
		// If it is then just lock it to zero and then call our OnDeath function
		CurrentHealth = 0.0f;
		OnDeath();
	}
	else
	{
		// Otherwise set the current health to the subtracted amount
		CurrentHealth = SubtractedHealth;
	}
	return CurrentHealth;
}

void APlayer_Character::OnFire_Implementation()
{
	// First, check if we're allowed to shoot
	if (bCanShoot)
	{
		// Then check if we don't have ammo and we don't ahve unlimed ammo on
		if (!HasAmmo() && !bUnlimitedAmmo)
		{
			return;
		}
		// If we're not allowed to shoot then don't continue
		else 
		{
			return;
		}
	}
	// Declare our particle's ending location to be set in the ine trace
	FVector ParticleLocation = FVector::ZeroVector;

	// Prepare our invisible ray's values
	FHitResult Hit;
	const FVector StartTrace = Camera->GetComponentLocation();
	const FVector EndTrace = StartTrace + (Camera->GetForwardVector() * 10000.0f);

	// Collision objects to ignore
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	QueryParams.AddIgnoredActor(this);

	// Fire an invisible ray
	GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECollisionChannel::ECC_Visibility, QueryParams);

	// If we hit something then damage it and set our particle location
	if (Hit.bBlockingHit && Hit.GetActor())
	{
		// Set the location for the particles to end at
		ParticleLocation = Hit.ImpactPoint;

		// Create the object for a general object type
		TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());

		// Set out general damage type to a specific type: Point Damage
		FPointDamageEvent PointDamageEvent(DamageAmount, Hit, Hit.ImpactNormal, ValidDamageTypeClass);

		// Damage Actor
		Hit.GetActor()->TakeDamage(DamageAmount, PointDamageEvent, GetController(), GetOwner());
	}
	else
	{
		// Set the location for the particles to end at
		ParticleLocation = EndTrace;
	}
	SpawnShootingParticles(ParticleLocation);

	// If we don't have unlimited ammo on, then use up on bullet
	if (!bUnlimitedAmmo)
	{
		// Subtract current ammo by one and then set it to that value
		CurrentAmmo--;
	}
}

void APlayer_Character::OnSprintStart_Implementation()
{
	bIsSprinting = true;
	bCanShoot = false;
	// Save what our walk speed was prior to sprint
	PreviousWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void APlayer_Character::OnSprintEnd_Implementation()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = PreviousWalkSpeed;
	bCanShoot = true;
}

void APlayer_Character::MoveForward(float Scalar)
{
	if (Scalar != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Scalar);
	}
}

void APlayer_Character::MoveRight(float Scalar)
{
	if (Scalar != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Scalar);
	}
}

void APlayer_Character::LookUpRate(float Rate)
{
	if (SpringArm)
	{
		// Get current rotation
		FRotator CameraRelativeRot = SpringArm->RelativeRotation;
		// Get intended pitch
		const float CameraNewPitch = FMath::ClampAngle(CameraRelativeRot.Pitch + Rate * BaseLookUpRate * GetWorld()->DeltaTimeSeconds, CameraPitchMin, CameraPitchMax);

		// Combine the values to become the new intended pitch
		CameraRelativeRot.Pitch = CameraNewPitch;

		// Confirm/Update the spring arm rotation
		SpringArm->SetRelativeRotation(CameraRelativeRot);
	}
}

void APlayer_Character::TurnAtRate(float Rate)
{
	// Adds to the Yaw rotation of the controller at the specified rate(speed)
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->DeltaTimeSeconds);
}

void APlayer_Character::SpawnShootingParticles(FVector ParticleLocation)
{
	if (TrailEffect)
	{
		// Spawn the particle
		UParticleSystemComponent* SpawnedParticle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TrailEffect, GunMesh->GetSocketLocation(FName("Muzzle")));
		
		// Scale the paticle up so its easily visible
		SpawnedParticle->SetWorldScale3D(FVector(5.0f));

		// Set the end of the particle beam
		SpawnedParticle->SetVectorParameter(FName("ShockBeamEnd"), ParticleLocation);

		// Check if our hit effect point is valid
		if (HitEffect)
		{
			// Spawn the particle
			UParticleSystemComponent* SpawnedParticle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, ParticleLocation, FRotator::ZeroRotator, true);

			// Scale the particle up so its easily visible
			SpawnedParticle->SetWorldScale3D(FVector(0.25f));
		}
	}
}



