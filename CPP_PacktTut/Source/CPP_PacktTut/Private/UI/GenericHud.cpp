// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/GenericHud.h"
#include "Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/Userwidget.h"

AGenericHud::AGenericHud()
{
	GameplayHUDClass = NULL;
	DeadMenuClass = NULL;
	PauseMenuClass = NULL;
}

bool AGenericHud::ShowSpecificMenu(TSubclassOf<class UUserWidget> ClassToShow, bool GameOnly, bool ShowMouse)
{
	// Remove adll widgets first
	HideAllMenus();
	if (ClassToShow)
	{
		//Create widget then add it to the screen
		UUserWidget* CreatedWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), ClassToShow);
		CreatedWidget->AddToPlayerScreen();

		if (GameOnly)
		{
			// Set our input mode to game only which allows us full control of the player
			GetOwningPlayerController()->SetInputMode(FInputModeGameOnly());
		}
		else
		{
			// Set our input mode to UI only, just in case we really don't want the player to be able to move
			FInputModeUIOnly InputMode;

			// This allows us to take the mouse out of the view port
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

			// Confirm our input mode
			GetOwningPlayerController()->SetInputMode(InputMode);
		}

		// Update mouse cursor
		GetOwningPlayerController()->bShowMouseCursor = ShowMouse;

		// Exit the method
		return true;
	}
	// Meaning the inputed class to show was invalid
	return false;
}

void AGenericHud::HideAllMenus()
{
	// Call an already established method to remove all widgets from the player's screen
	UWidgetLayoutLibrary::RemoveAllWidgets(this);
}
