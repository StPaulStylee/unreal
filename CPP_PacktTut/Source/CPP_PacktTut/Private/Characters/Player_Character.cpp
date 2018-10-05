// Fill out your copyright notice in the Description page of Project Settings.

#include "Player_Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

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
}

void APlayer_Character::BeginPlay()
{
	// We call Super::BeginPlay to call it's parents BeginPlay.. Since it is a virtual method
	// we can trust that it's parent has it and is doing the same thing. If all SubClasses are calling
	// super on their virtual methods, we can trust that they get called all the way up 
	// the chain and this is good practice
	Super::BeginPlay();

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
}

void APlayer_Character::OnDeath_Implementation()
{
}



