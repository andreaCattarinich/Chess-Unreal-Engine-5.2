// Fill out your copyright notice in the Description page of Project Settings.

/*  PlayerController è un Asset che serve a trasformare
	input esterni ad azioni del gioco.
*/
#pragma once

#include "TTT_HumanPlayer.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TTT_PlayerController.generated.h"

UCLASS()
class TICTACTOE_UE52_API ATTT_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// ************ CONSTRUCTORS ************
	ATTT_PlayerController();

protected:
	virtual void BeginPlay() override;

public:
	// ************ ATTRIBUTES ************
	UPROPERTY(EditAnywhere, Category = Input)
		UInputMappingContext* TTTContext;

	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* ClickAction;



	// ************ SETTERS ************	



	// ************ GETTERS ************


	
	// ************ METHODS ************
	void ClickOnGrid();
protected:
	virtual void SetupInputComponent() override;
};
