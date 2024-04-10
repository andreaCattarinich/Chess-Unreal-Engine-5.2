// Copyright © 2024 Andrea Cattarinich

// PlayerController is an Asset that is used to transform external inputs to game actions
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Chess_PlayerController.generated.h"

UCLASS()
class CHESS_API AChess_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// ************ CONSTRUCTORS ************
	AChess_PlayerController();

protected:
	virtual void BeginPlay() override;

public:
	// ************ ATTRIBUTES ************
	UPROPERTY(EditAnywhere, Category = Input)
		UInputMappingContext* ChessContext;

	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* ClickAction;



	// ************ SETTERS ************	



	// ************ GETTERS ************


	
	// ************ METHODS ************
	void ClickOnGrid();
	
protected:
	virtual void SetupInputComponent() override;
};
