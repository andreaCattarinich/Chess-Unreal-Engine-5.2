// Fill out your copyright notice in the Description page of Project Settings.


#include "..\Public\Chess_MinimaxPlayer.h"
#include "..\Public\Chess_GameMode.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AChess_MinimaxPlayer::AChess_MinimaxPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GameInstance = Cast<UChess_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	// Profondità di ricerca dell'albero di ricorsione delle mosse
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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Minimax) Turn"));
	GameInstance->SetTurnMessage(TEXT("AI (Minimax) Turn"));

	FTimerHandle TimerHandle1;

	// Timer per eseguire FindBestMove dopo 1 secondo
	GetWorld()->GetTimerManager().SetTimer(TimerHandle1, [this]()
		{
			NodesVisited = 0;
			PossibleFutureGames = 0;
			BlackPruning = 0;
			WhitePruning = 0;

			FMove BestMove = FindBestMove(GameMode->GField->TileArray);

			GameMode->SetSelectedTile(BestMove.Start);

			FTimerHandle TimerHandle2;

			// Effettua la mossa migliore dopo 1 secondo
			GetWorld()->GetTimerManager().SetTimer(TimerHandle2, [BestMove, this]()
				{
					GameMode->DoMove(BestMove.End, true);
				}, 1.0f, false);
			
		}, 1.0f, false);
}

void AChess_MinimaxPlayer::OnWin()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Minimax) Wins!"));
	GameInstance->SetTurnMessage(TEXT("AI Wins!"));
	GameInstance->IncrementScoreAiPlayer();
}

void AChess_MinimaxPlayer::OnLose()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Minimax) Loses!"));
	// GameInstance->SetTurnMessage(TEXT("AI Loses!"));
}

FMove AChess_MinimaxPlayer::FindBestMove(TArray<ATile*> Board)
{
	//return TPair<FVector2D, FVector2D>();
	
	// MiniMaxChess ritorna :
	//- Una coppia (score, mossa)
	//	* score: intero (� dato dall'evaluation)
	//	* mossa: coppia (start, end)

	const FEvaluation Evaluation = MiniMaxChessPruning(Board, STD_DEPTH, -10000, 10000, true);

	// Stampa score dell'evaluation
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Blue,
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
	return Evaluation.Move;
}

FEvaluation AChess_MinimaxPlayer::MiniMaxChess(
	TArray<ATile*>& Board,
	const int32 Depth,
	const bool bIsMax
)
{	
	NodesVisited++;

	if (Depth == 0)
	{
		return FEvaluation(EvaluateChessGrid(GameMode->GField->TileArray, bIsMax), FMove());
	}
	
	FMove BestMove{};

	if (bIsMax)
	{
		int32 Max_Eval = -10000;

		// ITERO SU TUTTE LE TILE
		for (auto& CurrentTile : GameMode->GField->TileArray)
		{
			// CERCO SOLO I PEZZI DEL MAXIMIZER
			if (CurrentTile->GetOwner() != 1)
			{
				continue;
			}
		
			// PRENDO TUTTE LE MOSSE
			const FVector2D CurrentPiecePosition = CurrentTile->GetGridPosition();
			GameMode->GField->SetSelectedTile(CurrentPiecePosition);

			const int32 RealCurrentPlayer = GameMode->CurrentPlayer;
			GameMode->CurrentPlayer = 1;
			TArray<FVector2D> CurrentPieceLegalMoves = GameMode->GField->LegalMoves(CurrentPiecePosition);
			GameMode->CurrentPlayer = RealCurrentPlayer;

			// SIMULO LA PARTITA PER TUTTE LE MOSSE
			for (const auto& Move : CurrentPieceLegalMoves)
			{
				const int32 RealCurrentPlayer2 = GameMode->CurrentPlayer;
				GameMode->CurrentPlayer = 1;
				GameMode->GField->SetSelectedTile(CurrentPiecePosition);

				// DO
				GameMode->DoMove(Move);
				
				GameMode->CurrentPlayer = RealCurrentPlayer2;

				const FEvaluation Minimax_Return = MiniMaxChess(GameMode->GField->TileArray, Depth - 1, !bIsMax);

				const int32 Current_Eval = Minimax_Return.Value;

				// UNDO
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
		int32 Min_Eval = 10000;

		for (auto& CurrentTile : GameMode->GField->TileArray)
		{
			if(CurrentTile->GetOwner() != 0)
			{
				continue;
			}
		
			// ORA CERCO TUTTE LE MOSSE
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

FEvaluation AChess_MinimaxPlayer::MiniMaxChessPruning(TArray<ATile*>& Board, int32 Depth, int32 Alpha, int32 Beta,
	bool bIsMax)
{
	NodesVisited++;

	if (Depth == 0)
	{
		return FEvaluation(EvaluateChessGrid(GameMode->GField->TileArray, bIsMax), FMove());
	}
	
	FMove BestMove{};

	if (bIsMax)
	{
		int32 Max_Eval = -10000;

		// ITERO SU TUTTE LE TILE
		for (auto& CurrentTile : GameMode->GField->TileArray)
		{
			// CERCO SOLO I PEZZI DEL MAXIMIZER
			if (CurrentTile->GetOwner() != 1)
			{
				continue;
			}
			
			// PRENDO TUTTE LE MOSSE
			const FVector2D CurrentPiecePosition = CurrentTile->GetGridPosition();
			GameMode->GField->SetSelectedTile(CurrentPiecePosition);

			const int32 RealCurrentPlayer = GameMode->CurrentPlayer;
			GameMode->CurrentPlayer = 1;
			TArray<FVector2D> CurrentPieceLegalMoves = GameMode->GField->LegalMoves(CurrentPiecePosition);
			GameMode->CurrentPlayer = RealCurrentPlayer;

			// SIMULO LA PARTITA PER TUTTE LE MOSSE
			for (const auto& Move : CurrentPieceLegalMoves)
			{
				const int32 RealCurrentPlayer2 = GameMode->CurrentPlayer;
				GameMode->CurrentPlayer = 1;
				GameMode->GField->SetSelectedTile(CurrentPiecePosition);

				// DO
				GameMode->DoMove(Move);
				
				GameMode->CurrentPlayer = RealCurrentPlayer2;

				const FEvaluation Minimax_Return = MiniMaxChessPruning(GameMode->GField->TileArray, Depth - 1, Alpha, Beta, !bIsMax);

				const int32 Current_Eval = Minimax_Return.Value;

				// UNDO
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
					UE_LOG(LogTemp, Warning, TEXT("Break Maximizer"));
					break;
				}
			}
		}

		return FEvaluation(Max_Eval, BestMove);
	}
	else
	{
		int32 Min_Eval = 10000;

		for (auto& CurrentTile : GameMode->GField->TileArray)
		{
			if(CurrentTile->GetOwner() != 0)
			{
				continue;
			}
		
			// ORA CERCO TUTTE LE MOSSE
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

				const FEvaluation Minimax_Return = MiniMaxChessPruning(GameMode->GField->TileArray, Depth - 1, Alpha, Beta, !bIsMax);

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

				Beta = (Beta < Current_Eval) ? Beta : Current_Eval;
				if(Beta <= Alpha)
				{
					WhitePruning++;
					UE_LOG(LogTemp, Warning, TEXT("Break Minimizer"));
					break;
				}
			}
		}

		return FEvaluation(Min_Eval, BestMove);
	}
}

int32 AChess_MinimaxPlayer::EvaluateChessGrid(TArray<ATile*>& Board, bool bIsMax) const
{
	// TODO: FARE L'EURISTICA
	/*
	//
	//	1: VINCE IL NERO
	//	-1: VINCE IL BIANCO
	*/
	int BlackScore = 0;
	int WhiteScore = 0;
	for(const auto& CurrentTile : GameMode->GField->TileArray)
	{
		if(CurrentTile->GetOwner() == 1)
		{
			BlackScore += CurrentTile->GetPiece()->GetPieceValue(); 
		}
		else if(CurrentTile->GetOwner() == 0)
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
		if(GameMode->IsWinMove(1))
		{
			return 10000;
		}
	}
	else
	{
		if(GameMode->IsWinMove(0))
		{
			return -10000;	
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
