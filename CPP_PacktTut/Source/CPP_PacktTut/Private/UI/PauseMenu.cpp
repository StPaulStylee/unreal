// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/PauseMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Player_Character.h"

bool UPauseMenu::Initialize()
{
	// Check if we fail the parent version of super first
	if (!Super::Initialize())
	{
		// If we did then exit the function with a false for failure
		return false;
	}

	// Bind the OnResumeClicked function to the ResumeButton delegate for OnClicked
	if (ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UPauseMenu::OnResumeClicked);
	}

	// Same as above
	if (DesktopButton)
	{
		DesktopButton->OnClicked.AddDynamic(this, &UPauseMenu::OnDesktopClicked);
	}

	// And again for main menu
	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &UPauseMenu::OnMainMenuClicked);
	}
	// CONTINUE AT ~5 min mark. You are getting compile errors.
	return true;
}

void UPauseMenu::OnResumeClicked()
{
	// Check for valid player reference
	if (APlayer_Character* Player = Cast<APlayer_Character>(GetOwningPlayerPawn()))
	{
		// Unpause the game, which hanles removing the menu
		Player->UnPauseGame();
	}
}

void UPauseMenu::OnDesktopClicked()
{
	// Tell our owning player controller to quit the game
	GetOwningPlayer()->ConsoleCommand("quit");
}

void UPauseMenu::OnMainMenuClicked()
{
	// Open the main menu level
	UGameplayStatics::OpenLevel(GetOwningPlayerPawn(), "Map_MainMenu");
}
