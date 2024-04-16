// Copyright © 2024 Andrea Cattarinich

#pragma once

#include "Piece.h"
#include "Chess_GameMode.h"

#include "CoreMinimal.h"
#include "Chess_PlayerInterface.h"
#include "Chess_GameInstance.h"
#include "Tile.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "Chess_MinimaxPlayer.generated.h"

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

	FEvaluation(const int32 NewValue, const FMove& NewMove)
		: Value(NewValue), Move(NewMove)
	{
	}
};

UCLASS()
class CHESS_API AChess_MinimaxPlayer : public APawn, public IChess_PlayerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AChess_MinimaxPlayer();

	UChess_GameInstance* GameInstance;

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
	virtual void OnStalemate() override;


	
	/**** ATTRIBUTES ****/
	int32 STD_DEPTH;

	// Reference to GameMode
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AChess_GameMode* GameMode;
	
	// Number of visited nodes
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		mutable int32 NodesVisited;

	// Number of visited leaves
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		mutable int32 PossibleFutureGames;

	// Number of White break in minimax
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		mutable int32 WhitePruning;

	// Number of Black break in minimax
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		mutable int32 BlackPruning;

	
	
	/**** METHODS ****/
	int32 EvaluateChessGrid(TArray<ATile*>& Board, bool bIsMax) const;

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

	void DecideMove();
	void SelectRandomPiece() const;
	void RandomMove() const;
};
