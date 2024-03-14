// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "TTT_GameMode.h"


#include "TTT_GameInstance.h"
#include "TTT_PlayerInterface.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TTT_HumanPlayer.generated.h"

UCLASS()
class TICTACTOE_UE52_API ATTT_HumanPlayer : public APawn, public ITTT_PlayerInterface
{
	GENERATED_BODY()

public:
	// ************ CONSTRUCTORS ************
	// Sets default values for this pawn's properties
	ATTT_HumanPlayer();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// keeps track of turn
	bool IsMyTurn = false;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	// ************ ATTRIBUTES ************	
	// Camera component attacched to player pawn
	UCameraComponent* Camera;

	// Game instance reference
	UTTT_GameInstance* GameInstance;



	// ************ SETTERS ************	



	// ************ GETTERS ************	



	// ************ METHODS ************	
	virtual void OnTurn() override;
	virtual void OnWin() override;
	virtual void OnLose() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// called on left mouse click (binding)
	UFUNCTION()
	void OnClick();

	// Manages Click on Tile
	void HandleTileClick(
		ATTT_GameMode* GameMode,
		ATile* ClickedTile
	);

	// Manages Click on Piece
	void HandlePieceClick(
		ATTT_GameMode* GameMode,
		APiece* ClickedPiece
	);


	void ExecuteTheMoveForHumanPlayer(
		ATTT_GameMode* GameMode,
		ATile* EndTile
		);

};
