// Copyright © 2024 Andrea Cattarinich

/*** Persistent object for the duration of the game ***/
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Chess_GameInstance.generated.h"

UCLASS()
class CHESS_API UChess_GameInstance : public UGameInstance
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

	// Difficulty: random, minimax
	UPROPERTY(BlueprintReadWrite, Category = "Difficulty")
	int32 Difficulty;


	
	// ************ SETTERS ************ 
	// Set the turn message
	UFUNCTION(BlueprintCallable)
		void SetTurnMessage(FString Message);

	// Set the difficulty
	UFUNCTION(BlueprintCallable)
		void SetOpponent(const int32 SelectedOpp);


	
	// ************ GETTERS ************
	// Get the score for human player
	UFUNCTION(BlueprintCallable)
		int32 GetScoreHumanPlayer() const;

	// Get the score for AI player
	UFUNCTION(BlueprintCallable)
		int32 GetScoreAiPlayer() const;

	// Get the current turn message
	UFUNCTION(BlueprintCallable)
		FString GetTurnMessage();



	// ************ METHODS ************
	// Increment the score for human player
	void IncrementScoreHumanPlayer();

	// Increment the score for AI player
	void IncrementScoreAiPlayer();
};
