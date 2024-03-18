// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Piece.h" // TODO: da togliere ma controllare inclusione di TTT_GameMode
#include "TTT_GameMode.h"

#include "CoreMinimal.h"
#include "TTT_PlayerInterface.h"
#include "TTT_GameInstance.h"
#include "Tile.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "TTT_MinimaxPlayer.generated.h"

USTRUCT(BlueprintType)
struct FEvaluation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Evaluation")
		int32 Value;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moves")
		FMove Move;
	
	FEvaluation()
		: Value(-1), Move()
	{
	}

	FEvaluation(int32 _Value, const FMove& _Move)
		: Value(_Value), Move(_Move)
	{
	}
};

UCLASS()
class TICTACTOE_UE52_API ATTT_MinimaxPlayer : public APawn, public ITTT_PlayerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATTT_MinimaxPlayer();

	UTTT_GameInstance* GameInstance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnTurn() override;
	virtual void OnWin() override;
	virtual void OnLose() override;

	/**** ATTRIBUTES ****/
	int32 STD_DEPTH;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ATTT_GameMode* GameMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	mutable int32 NodesVisited;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	mutable int32 PossibleFutureGames;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	mutable int32 WhitePruning;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	mutable int32 BlackPruning;

	
	/**** METHODS ****/
	int32 EvaluateChessGrid(
		TArray<ATile*>& Board,
		bool bIsMax
	) const;

	FEvaluation MiniMaxChess(
		TArray<ATile*>& Board,
		int32 Depth,
		bool bIsMax
	);

	FEvaluation MiniMaxChessPruning(
		TArray<ATile*>& Board,
		int32 Depth,
		int32 Alpha,
		int32 Beta,
		bool bIsMax
	);

	FMove FindBestMove(TArray<ATile*> Board);

};
