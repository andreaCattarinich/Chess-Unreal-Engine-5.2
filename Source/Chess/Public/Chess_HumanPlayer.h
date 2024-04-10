// Copyright © 2024 Andrea Cattarinich

#pragma once

#include "Chess_GameMode.h"
#include "Chess_GameInstance.h"
#include "Chess_PlayerInterface.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Chess_HumanPlayer.generated.h"

UCLASS()
class CHESS_API AChess_HumanPlayer : public APawn, public IChess_PlayerInterface
{
	GENERATED_BODY()

public:
	// ************ CONSTRUCTORS ************
	// Sets default values for this pawn's properties
	AChess_HumanPlayer();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// keeps track of turn
	bool IsMyTurn = false;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	// ************ ATTRIBUTES ************	
	// Camera component attached to player pawn
	UCameraComponent* Camera;

	// Game instance reference
	UChess_GameInstance* GameInstance;

	// Game Mode reference
	AChess_GameMode* GameMode;


	// ************ SETTERS ************	



	// ************ GETTERS ************	



	// ************ METHODS ************	
	virtual void OnTurn() override;
	virtual void OnWin() override;
	virtual void OnLose() override;
	virtual void OnStalemate() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// called on left mouse click (binding)
	UFUNCTION()
		void OnClick();

	// Manages Click on Tile
	void HandleTileClick(const ATile* ClickedTile);

	// Manages Click on Piece
	void HandlePieceClick(const APiece* ClickedPiece);

	// Execute the move
	void ExecuteTheMoveForHumanPlayer(const ATile* EndTile);
};
