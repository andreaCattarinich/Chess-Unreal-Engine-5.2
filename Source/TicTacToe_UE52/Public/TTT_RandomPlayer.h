// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TTT_GameInstance.h"
#include "TTT_PlayerInterface.h"
#include "TTT_GameMode.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h" // TODO: NON TOGLIERE QUESTO INCLUDE
#include "TTT_RandomPlayer.generated.h"

UCLASS()
class TICTACTOE_UE52_API ATTT_RandomPlayer : public APawn, public ITTT_PlayerInterface
{
	GENERATED_BODY()

public:
	// ************ CONSTRUCTORS ************
	// Sets default values for this pawn's properties
	ATTT_RandomPlayer();

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
	UTTT_GameInstance* GameInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ATTT_GameMode* GameMode;



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
