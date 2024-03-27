// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Chess_GameInstance.h"
#include "Chess_PlayerInterface.h"
#include "Chess_GameMode.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

//TODO: keep this include 
#include "Kismet/GameplayStatics.h" 
#include "Chess_RandomPlayer.generated.h"

UCLASS()
class TICTACTOE_UE52_API AChess_RandomPlayer : public APawn, public IChess_PlayerInterface
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AChess_GameMode* GameMode;



	// ************ SETTERS ************	



	// ************ GETTERS ************	



	// ************ METHODS ************
	virtual void OnTurn() override;
	virtual void OnWin() override;
	virtual void OnLose() override;

	void DecideMove();
	void SelectRandomPiece() const;
	void RandomMove() const;
};
