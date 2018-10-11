// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GenericHud.generated.h"

/**
 * 
 */
UCLASS()
class CPP_PACKTTUT_API AGenericHud : public AHUD
{
	GENERATED_BODY()

public:

	AGenericHud();

	class TSubclassOf<UUserWidget> GetGameplayHUDClass() const { return GameplayHUDClass;  }
	class TSubclassOf<UUserWidget> GetDeadMenuClass() const { return DeadMenuClass;  }
	class TSubclassOf<UUserWidget> GetPauseMenuClass() const { return PauseMenuClass;  }

	bool ShowSpecificMenu(class TSubclassOf<class UUserWidget> ClassToShow, bool GameOnly, bool ShowMouse);

	/** Hides all other menu's on screen */
	UFUNCTION(BlueprintCallable, Category = "GenericHUD")
		void HideAllMenus();


protected:

	/** The player's hud during gameplay, showing health, ammo, and the crosshair */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GenericHUD")
		class TSubclassOf<UUserWidget> GameplayHUDClass;

	/** The mentu to use when the player is dead	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GenericHUD")
		class TSubclassOf<UUserWidget> DeadMenuClass;

	/** The menu to use when the player has paused the game */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GenericHUD")
		class TSubclassOf<UUserWidget> PauseMenuClass;

	
	
	
	
};
