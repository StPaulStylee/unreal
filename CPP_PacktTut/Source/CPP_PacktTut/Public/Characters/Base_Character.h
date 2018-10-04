// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Base_Character.generated.h"

UCLASS()
class CPP_PACKTTUT_API ABase_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABase_Character();

	// These functions are all getters/setters because our variables below are protected and will not be 
	// visible outside of the ABase_Character class
	UFUNCTION(BlueprintCallable, Category = "Character|Health")
		void SetCurrentHealth(float NewHealth) { CurrentHealth = NewHealth; }

	//The BlueprintPure allows for complex values and getting accurate values for floats
	UFUNCTION(BlueprintPure, Category = "Character|Health")
		float GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintPure, Category = "Character|Health")
		float GetMaxHealth() const { return MaxHealth; }

protected:
	// Called when the game starts or when spawned
	// Virtual allows function to be inherited by its subclasses and have its own implementation
	// Override keyword blocks it from being inheritied by subclasses
	// However, the virtual keyword overrides the override, and for some reason this is best practice...
	virtual void BeginPlay( ) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character|Death")
		void OnDeath( );
	// Since we are declaring this a BlueprintNativeEvent, we must implement this function again using 
	// the virtual keyword - this is our C++ implementation
	// The PURE_VIRTUAL allows us to declare this function but not give it a definition. The comma afterwards 
	// tells the compiler that we must define this function on all subclasses
	// The Blueprintcallable allows us to actually call it in blueprint
	// The category is used to help organize the functions in the blueprint library
	virtual void OnDeath_Implementation() PURE_VIRTUAL(ABase_Character::OnDeath_Implementation, );

	// This is not needed in blueprint, so we don't use a UPROPERTY
	float CurrentHealth;

	// MaxHealth will be editable in blueprints in the DefaultsOnly panel
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character|Health")
		float MaxHealth;

	// See the variable on instances and in DefaultsOnly
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
		bool bIsSprinting;

	
	
};
