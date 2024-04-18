// Copyright © 2024 Andrea Cattarinich

#include "Chess_MinimaxPlayer.h"
#include "Chess_GameMode.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AChess_MinimaxPlayer::AChess_MinimaxPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GameInstance = Cast<UChess_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	// Depth of search in the move recursion tree
	STD_DEPTH = 3;
}

// Called when the game starts or when spawned
void AChess_MinimaxPlayer::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
}

// Called every frame
void AChess_MinimaxPlayer::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AChess_MinimaxPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AChess_MinimaxPlayer::OnTurn()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Minimax) Turn"));
	GameInstance->SetTurnMessage(TEXT("AI (Minimax) Turn"));

	FTimerHandle TimerHandle1;

	// Timer to execute FindBestMove after 1 second
	GetWorld()->GetTimerManager().SetTimer(TimerHandle1, [this]()
		{
			NodesVisited = 0;
			PossibleFutureGames = 0;
			BlackPruning = 0;
			WhitePruning = 0;

			FMove BestMove = FindBestMove(GameMode->GField->TileArray);

			GameMode->IsGameOver = false;
		
			if(!(GameMode->GField->IsValidPosition(BestMove.Start)) || !(GameMode->GField->IsValidPosition(BestMove.End)))
			{
				DecideMove();
			}
			else
			{
				GameMode->SetSelectedTile(BestMove.Start);

				FTimerHandle TimerHandle2;
							
				// Make the best move after 1 second
				GetWorld()->GetTimerManager().SetTimer(TimerHandle2, [BestMove, this]()
					{
						GameMode->DoMove(BestMove.End, true);
					}, 1.0f, false);
			}
		}, 0.25f, false);
}

void AChess_MinimaxPlayer::OnWin()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Minimax) Wins!"));
	GameInstance->SetTurnMessage(TEXT("AI Wins!"));
	GameInstance->IncrementScoreAiPlayer();
}

void AChess_MinimaxPlayer::OnLose()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Minimax) Loses!"));
	// GameInstance->SetTurnMessage(TEXT("AI Loses!"));
}

void AChess_MinimaxPlayer::OnStalemate()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Minimax) Stalemate!"));
}

FMove AChess_MinimaxPlayer::FindBestMove(TArray<ATile*> Board)
{
	// MiniMaxChess returns:
	// A pair (score, move)
	//	- score: integer (given by the evaluation)
	//	- move: pair (start, end)

	const FEvaluation Evaluation = MiniMaxChessPruning(
			Board,
			STD_DEPTH,
			-100000,
			100000,
			true
		);
	
	/* Debug
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Green,
		FString::Printf(TEXT("Evaluation score: %d"), Evaluation.Value)
	);
	
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Red,
		FString::Printf(TEXT("Nodes visited: %d"), NodesVisited)
	);
	
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Red,
		FString::Printf(TEXT("Possible futures games: %d"), PossibleFutureGames)
	);
	
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Green,
		FString::Printf(TEXT("Black Pruning: %d"), BlackPruning)
	);

	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Green,
		FString::Printf(TEXT("White Pruning: %d"), WhitePruning)
	);
	*/
	return Evaluation.Move;
}

FEvaluation AChess_MinimaxPlayer::MiniMaxChess(
	TArray<ATile*>& Board,
	const int32 Depth,
	const bool bIsMax
)
{	
	NodesVisited++;

	if (Depth == 0 || GameMode->IsGameOver)
	{
		return FEvaluation(EvaluateChessGrid(GameMode->GField->TileArray, bIsMax), FMove());
	}
	
	FMove BestMove{};

	if (bIsMax)
	{
		int32 Max_Eval = -100000;

		// Iterate over all the tiles
		for (auto& CurrentTile : GameMode->GField->TileArray)
		{
			// Search for the Pieces of the Maximizer
			if (CurrentTile->GetTileOwner() != 1)
			{
				continue;
			}
		
			// Take all moves
			const FVector2D CurrentPiecePosition = CurrentTile->GetGridPosition();
			GameMode->GField->SetSelectedTile(CurrentPiecePosition);

			const int32 RealCurrentPlayer = GameMode->CurrentPlayer;
			GameMode->CurrentPlayer = 1;
			TArray<FVector2D> CurrentPieceLegalMoves = GameMode->GField->LegalMoves(CurrentPiecePosition);
			GameMode->CurrentPlayer = RealCurrentPlayer;

			// Simulate the Game for all moves
			for (const auto& Move : CurrentPieceLegalMoves)
			{
				const int32 RealCurrentPlayer2 = GameMode->CurrentPlayer;
				GameMode->CurrentPlayer = 1;
				GameMode->GField->SetSelectedTile(CurrentPiecePosition);

				// Do
				GameMode->DoMove(Move);
				
				GameMode->CurrentPlayer = RealCurrentPlayer2;

				const FEvaluation Minimax_Return = MiniMaxChess(GameMode->GField->TileArray, Depth - 1, !bIsMax);

				const int32 Current_Eval = Minimax_Return.Value;

				// Undo
				GameMode->UndoMove(false);

				if (Current_Eval > Max_Eval || Current_Eval == Max_Eval && FMath::Rand() % 2)
				{
					Max_Eval = Current_Eval;
					BestMove = FMove(
						GameMode->Moves.Num() + 1,
						CurrentTile->GetPiece()->GetPieceID(),
						CurrentTile->GetGridPosition(),
						Move,
						-1
					);
				}
			}
		}

		return FEvaluation(Max_Eval, BestMove);
	}
	else
	{
		int32 Min_Eval = 100000;

		for (auto& CurrentTile : GameMode->GField->TileArray)
		{
			if(CurrentTile->GetTileOwner() != 0)
			{
				continue;
			}
		
			const FVector2D CurrentPiecePosition = CurrentTile->GetGridPosition();
			GameMode->GField->SetSelectedTile(CurrentPiecePosition);

			const int32 RealCurrentPlayer = GameMode->CurrentPlayer;
			GameMode->CurrentPlayer = 0;
			TArray<FVector2D> CurrentPieceLegalMoves = GameMode->GField->LegalMoves(CurrentTile->GetGridPosition());
			GameMode->CurrentPlayer = RealCurrentPlayer;

			for (const auto& Move : CurrentPieceLegalMoves)
			{
				const int32 RealCurrentPlayer2 = GameMode->CurrentPlayer;
				GameMode->CurrentPlayer = 0;
				GameMode->GField->SetSelectedTile(CurrentPiecePosition);

				GameMode->DoMove(Move);
				
				GameMode->CurrentPlayer = RealCurrentPlayer2;

				const FEvaluation Minimax_Return = MiniMaxChess(GameMode->GField->TileArray, Depth - 1, !bIsMax);

				const int32 Current_Eval = Minimax_Return.Value;

				GameMode->UndoMove(false);

				if (Current_Eval < Min_Eval || Current_Eval == Min_Eval && FMath::Rand() % 2)
				{
					Min_Eval = Current_Eval;
					BestMove = FMove(
						GameMode->Moves.Num() + 1,
						CurrentTile->GetPiece()->GetPieceID(),
						CurrentTile->GetGridPosition(),
						Move,
						-1
					);
				}
			}
		}

		return FEvaluation(Min_Eval, BestMove);
	}
}

FEvaluation AChess_MinimaxPlayer::MiniMaxChessPruning(TArray<ATile*>& Board, const int32 Depth, int32 Alpha, int32 Beta, const bool bIsMax)
{
	NodesVisited++;

	if (Depth == 0 || GameMode->IsGameOver)
	{
		return FEvaluation(EvaluateChessGrid(GameMode->GField->TileArray, bIsMax), FMove());
	}
	
	FMove BestMove{};

	if (bIsMax)
	{
		int32 Max_Eval = -100000;

		// Iterate over all the tiles
		for (auto& CurrentTile : GameMode->GField->TileArray)
		{
			// Search for pieces of the Maximizer
			if (CurrentTile->GetTileOwner() != 1)
			{
				continue;
			}
			
			// Take all the moves
			const FVector2D CurrentPiecePosition = CurrentTile->GetGridPosition();
			GameMode->GField->SetSelectedTile(CurrentPiecePosition);

			const int32 RealCurrentPlayer = GameMode->CurrentPlayer;
			GameMode->CurrentPlayer = 1;
			TArray<FVector2D> CurrentPieceLegalMoves = GameMode->GField->LegalMoves(CurrentPiecePosition);
			GameMode->CurrentPlayer = RealCurrentPlayer;

			// Simulate the Game for all moves
			for (const auto& Move : CurrentPieceLegalMoves)
			{
				const int32 RealCurrentPlayer2 = GameMode->CurrentPlayer;
				GameMode->CurrentPlayer = 1;
				GameMode->GField->SetSelectedTile(CurrentPiecePosition);

				// Do
				GameMode->DoMove(Move);
				
				GameMode->CurrentPlayer = RealCurrentPlayer2;

				const FEvaluation Minimax_Return = MiniMaxChessPruning(GameMode->GField->TileArray, Depth - 1, Alpha, Beta, !bIsMax);

				const int32 Current_Eval = Minimax_Return.Value;

				// Undo
				GameMode->UndoMove(false);

				if (Current_Eval > Max_Eval || Current_Eval == Max_Eval && FMath::Rand() % 2)
				{
					Max_Eval = Current_Eval;
					BestMove = FMove(
						GameMode->Moves.Num() + 1,
						CurrentTile->GetPiece()->GetPieceID(),
						CurrentTile->GetGridPosition(),
						Move,
						-1
					);
				}

				Alpha = (Alpha > Current_Eval) ? Alpha : Current_Eval;
				if(Beta <= Alpha)
				{
					BlackPruning++;
					//UE_LOG(LogTemp, Warning, TEXT("Break Maximizer"));
					break;
				}
			}
		}

		return FEvaluation(Max_Eval, BestMove);
	}
	else
	{
		int32 Min_Eval = 100000;

		// Iterate over all the tiles
		for (auto& CurrentTile : GameMode->GField->TileArray)
		{
			// Search for pieces of the Minimizer
			if(CurrentTile->GetTileOwner() != 0)
			{
				continue;
			}
		
			// Take all the moves
			const FVector2D CurrentPiecePosition = CurrentTile->GetGridPosition();
			GameMode->GField->SetSelectedTile(CurrentPiecePosition);

			const int32 RealCurrentPlayer = GameMode->CurrentPlayer;
			GameMode->CurrentPlayer = 0;
			TArray<FVector2D> CurrentPieceLegalMoves = GameMode->GField->LegalMoves(CurrentTile->GetGridPosition());
			GameMode->CurrentPlayer = RealCurrentPlayer;

			// Simulate the Game for all moves
			for (const auto& Move : CurrentPieceLegalMoves)
			{
				const int32 RealCurrentPlayer2 = GameMode->CurrentPlayer;
				GameMode->CurrentPlayer = 0;
				GameMode->GField->SetSelectedTile(CurrentPiecePosition);

				// Do
				GameMode->DoMove(Move);
				
				GameMode->CurrentPlayer = RealCurrentPlayer2;

				const FEvaluation Minimax_Return = MiniMaxChessPruning(GameMode->GField->TileArray, Depth - 1, Alpha, Beta, !bIsMax);

				const int32 Current_Eval = Minimax_Return.Value;

				// Undo
				GameMode->UndoMove(false);

				if (Current_Eval < Min_Eval || Current_Eval == Min_Eval && FMath::Rand() % 2)
				{
					Min_Eval = Current_Eval;
					BestMove = FMove(
						GameMode->Moves.Num() + 1,
						CurrentTile->GetPiece()->GetPieceID(),
						CurrentTile->GetGridPosition(),
						Move,
						-1
					);
				}

				Beta = (Beta < Current_Eval) ? Beta : Current_Eval;
				if(Beta <= Alpha)
				{
					WhitePruning++;
					//UE_LOG(LogTemp, Warning, TEXT("Break Minimizer"));
					break;
				}
			}
		}

		return FEvaluation(Min_Eval, BestMove);
	}
}

int32 AChess_MinimaxPlayer::EvaluateChessGrid(TArray<ATile*>& Board, bool bIsMax) const
{
	// +1 -> white wins
	// -1 -> black wins
	
	int BlackScore = 0;
	int WhiteScore = 0;
	for(const auto& CurrentTile : GameMode->GField->TileArray)
	{
		if(CurrentTile->GetTileOwner() == 1)
		{
			BlackScore += CurrentTile->GetPiece()->GetPieceValue(); 
		}
		else if(CurrentTile->GetTileOwner() == 0)
		{
			WhiteScore += CurrentTile->GetPiece()->GetPieceValue();
		}
	}
	
	PossibleFutureGames++;
	
	if(STD_DEPTH % 2 == 1)
	{
		bIsMax = !bIsMax;
	}
	if(bIsMax)
	{
		if(GameMode->IsWinnerMove(1, false))
		{
			GameMode->IsGameOver = false;
			return 100000;
		}
	}
	else
	{
		if(GameMode->IsWinnerMove(0, false))
		{
			GameMode->IsGameOver = false;
			return -100000;	
		}
	}
	if (bIsMax)
	{
		return BlackScore - WhiteScore;
	}
	else
	{
		return WhiteScore - BlackScore;
	}
}

void AChess_MinimaxPlayer::DecideMove()
{
	SelectRandomPiece();

	if (GameMode->GField->GetLegalMoves().Num() == 0)
	{	
		DecideMove();
	}
	else
	{
		RandomMove();
	}
}

void AChess_MinimaxPlayer::SelectRandomPiece() const
{
	TArray<ATile*> AIPieces;

	for (ATile* CurrentTile : GameMode->GField->TileArray)
	{
		if (CurrentTile->GetTileOwner() == PlayerNumber)
		{
			AIPieces.Add(CurrentTile);
		}
	}

	if (AIPieces.Num() == 0)
	{
		return;
	}

	const int32 RandIdx = FMath::Rand() % AIPieces.Num();
	const FVector2D Position = FVector2D(
		(AIPieces[RandIdx])->GetGridPosition()[0],
		(AIPieces[RandIdx])->GetGridPosition()[1]
	);
	GameMode->SetSelectedTile(Position);
}

void AChess_MinimaxPlayer::RandomMove() const
{	
	TArray<FVector2D> AILegalMoves = GameMode->GField->GetLegalMoves();

	const int32 RandIdx = FMath::Rand() % AILegalMoves.Num();
	const FVector2D Position = AILegalMoves[RandIdx];

	const ATile* RandomTile = *GameMode->GField->TileMap.Find(Position);
	if (RandomTile->IsLegalTile())
	{
		GameMode->DoMove(Position, true);
	}
}