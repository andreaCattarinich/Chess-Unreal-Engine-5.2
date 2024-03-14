// Fill out your copyright notice in the Description page of Project Settings.

/*** Oggetto persistente per tutta la durata del gioco ***/
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TTT_GameInstance.generated.h"

UCLASS()
class TICTACTOE_UE52_API UTTT_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// ************ ATTRIBUTES ************
	// Score value for human player
	UPROPERTY(EditAnywhere)
		int32 ScoreHumanPlayer = 0;

	// Score value for AI player
	UPROPERTY(EditAnywhere)
		int32 ScoreAiPlayer = 0;

	// Message to show every turn
	UPROPERTY(EditAnywhere)
		FString CurrentTurnMessage = "Current Player";



	// ************ SETTERS ************ 
	// Set the turn message
	UFUNCTION(BlueprintCallable)
		void SetTurnMessage(FString Message);



	// ************ GETTERS ************
	// Get the score for human player
	UFUNCTION(BlueprintCallable)
		int32 GetScoreHumanPlayer();

	// Get the score for AI player
	UFUNCTION(BlueprintCallable)
		int32 GetScoreAiPlayer();

	// Get the current turn message
	UFUNCTION(BlueprintCallable)
		FString GetTurnMessage();



	// ************ METHODS ************
	// Increment the score for human player
	void IncrementScoreHumanPlayer();

	// Increment the score for AI player
	void IncrementScoreAiPlayer();
};
