// Fill out your copyright notice in the Description page of Project Settings.


#include "TTT_PlayerController.h"
#include "Components/InputComponent.h"

ATTT_PlayerController::ATTT_PlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
}

void ATTT_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(TTTContext, 0);
	}
}

void ATTT_PlayerController::ClickOnGrid()
{
	const auto HumanPlayer = Cast<ATTT_HumanPlayer>(GetPawn());
	if (IsValid(HumanPlayer))
	{
		HumanPlayer->OnClick();
	}
}

void ATTT_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Triggered, this, &ATTT_PlayerController::ClickOnGrid);
	}
}