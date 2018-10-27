// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"

/**
 *
 */
UCLASS()
class CPP_PACKTTUT_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()

protected:

	/** Occurs on compile from the blueprint editor */
	virtual bool Initialize() override;

private:

	/** Widget subclass of type Button that we are looking for named ResumeButton */
	UPROPERTY(meta = (BindWidget))
		class UButton* ResumeButton;

	/** This will occur when the ResumeButton is clicked */
	UFUNCTION()
		void OnResumeClicked();

	/** Widget subclass of type Button that we are looking for named DesktopButton */
	UPROPERTY(meta = (BindWidget))
		class UButton* DesktopButton;

	/** This will occur when the DesktopButton is clicked */
	UFUNCTION()
		void OnDesktopClicked();

	/** Widget subclass of type Button that we are looking for named MainMenuButton */
	UPROPERTY(meta = (BindWidget))
		class UButton* MainMenuButton;

	/** This will occur when the MainMenuButton is clicked */
	UFUNCTION()
		void OnMainMenuClicked();

};
