// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Base_Character.h"
#include "Player_Character.generated.h"

/**
 * 
 */
UCLASS()
class CPP_PACKTTUT_API APlayer_Character : public ABase_Character
{
	GENERATED_BODY()

	// the meta tag allow for extra features in view property... Since this is a private property, view propety allows us to edit in Blueprint
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent * Camera;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent * FP_Mesh;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* GunMesh;

public:
	// You must always put the constructor in the public namespace so you can access it
	// Constructor
	APlayer_Character();

	UFUNCTION(BlueprintPure, Category = "Character|Gun|Ammo")
		int32 GetCurrentAmmo() const { return CurrentAmmo; }

	UFUNCTION(BlueprintPure, Category = "Character|Gun|Ammo")
		int32 GetMaxAmmo() const { return MaxAmmo; }

	UFUNCTION(BlueprintCallable, Category = "Character|Gun|Ammo")
		void SetCurrentAmmo(int NewAmmo) { CurrentAmmo = NewAmmo; }

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Camera")
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Camera")
		float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Movement")
		float SprintSpeed;

protected:
	
	// We inherit the BeginPlay and OnDeath_Implementation from our Base class
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void OnDeath_Implementation() override;

	/** Check and see if we have any remaining ammo */
	bool HasAmmo() const { return (CurrentAmmo > 0); }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character|Input|Movement")
		void OnSprintStart();
	void OnSprintStart_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character|Input|Movement")
		void OnSprintEnd();
	void OnSprintEnd_Implementation();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character|Gun")
		class UParticleSystem* TrailEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character|Gun")
		class UParticleSystem* HitEffect;

	/** Flag for when the play can shoot */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Gun")
		bool bCanShoot;

	UFUNCTION(BlueprintCallable, Category = "Character|Input|Movement")
		void MoveForward(float Scalar);

	UFUNCTION(BlueprintCallable, Category = "Character|Input|Movement")
		void MoveRight(float Scalar);

	UFUNCTION(BlueprintCallable, Category = "Character|Input|Camera")
		void LookUpRate(float Rate);

	UFUNCTION(BlueprintCallable, Category = "Character|Input|Camera")
		void TurnAtRate(float Rate);
	
	/** Flag for when the play can shoot unlimited amount of bullets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Gun|Ammo")
		bool bUnlimitedAmmo;

	int CurrentAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character|Gun|Ammo")
		int MaxAmmo;

	/** Minimum view Pitch, in degree. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Camera")
		float CameraPitchMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Camera")
		float CameraPitchMax;

private:
	
	float PreviousWalkSpeed;

	void SpawnShootingParticles(FVector ParticleLocation);
	
};