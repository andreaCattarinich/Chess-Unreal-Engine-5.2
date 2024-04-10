// Copyright © 2024 Andrea Cattarinich

#pragma once

#include "Chess_GameInstance.h"
#include "Chess_PlayerInterface.h"
#include "Chess_GameMode.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h" 
#include "Chess_RandomPlayer.generated.h"

UCLASS()
class CHESS_API AChess_RandomPlayer : public APawn, public IChess_PlayerInterface
{
	GENERATED_BODY()

public:
	// ************ CONSTRUCTORS ************
	// Sets default values for this pawn's properties
	AChess_RandomPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



	// ************ ATTRIBUTES ************	
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

	// Generic function
	void DecideMove();
	void SelectRandomPiece() const;
	void RandomMove() const;
};
