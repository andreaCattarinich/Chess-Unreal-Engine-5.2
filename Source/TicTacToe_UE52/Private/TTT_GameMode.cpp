// Fill out your copyright notice in the Description page of Project Settings.

#include "TTT_GameMode.h"

#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"

#include "GameField.h"
#include "TTT_PlayerController.h"
#include "TTT_HumanPlayer.h"
#include "TTT_RandomPlayer.h"
#include "TTT_MinimaxPlayer.h"

#include "MovesPanel.h"

#include "EngineUtils.h"

ATTT_GameMode::ATTT_GameMode()
{
	PlayerControllerClass = ATTT_PlayerController::StaticClass();
	DefaultPawnClass = ATTT_HumanPlayer::StaticClass();

	FieldSize = 8;
	IsGameOver = false;
	CurrentPlayer = -1;
	MoveCounter = -1;
	GField = nullptr;
	MovesPanel = nullptr;
}

void ATTT_GameMode::BeginPlay()
{
	Super::BeginPlay();

	IsGameOver = false;

	MoveCounter = 0;

	ATTT_HumanPlayer* HumanPlayer = Cast<ATTT_HumanPlayer>(*TActorIterator<ATTT_HumanPlayer>(GetWorld()));

	if (GameFieldClass != nullptr)
	{
		GField = GetWorld()->SpawnActor<AGameField>(GameFieldClass);
		GField->Size = FieldSize;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Game Field is null"));
	}


	const float CameraPosX = ((GField->TileSize * (FieldSize + ((FieldSize - 1) * GField->NormalizedCellPadding) - (FieldSize - 1))) / 2) - (GField->TileSize / 2);
	const FVector CameraPos(CameraPosX, CameraPosX, 1500.0f);
	HumanPlayer->SetActorLocationAndRotation(CameraPos, FRotationMatrix::MakeFromX(FVector(0, 0, -1)).Rotator());

	// Human player = 0
	Players.Add(HumanPlayer);
	PlayerNames.Add(0, "Player");

	// Random Player
	//auto* AI = GetWorld()->SpawnActor<ATTT_RandomPlayer>(FVector(), FRotator());
	//PlayerNames.Add(1, "AI");

	// MiniMax Player
	auto* AI = GetWorld()->SpawnActor<ATTT_MinimaxPlayer>(FVector(), FRotator());

	// AI player = 1
	Players.Add(AI);
	PlayerNames.Add(1, "AI");


	MovesPanel = CreateWidget<UMovesPanel>(GetGameInstance(), WidgetClass);

	if(MovesPanel)
	{
		MovesPanel->AddToViewport(0);		
	}
	ChoosePlayerAndStartGame();
}

void ATTT_GameMode::SetSelectedTile(const FVector2D Position) const
{
	// Reset dei colori del campo
	GField->ResetGameStatusField();

	// Mostra Tile selezionata (colore blu)
	GField->SelectTile(Position);

	// Restituisci le mosse del pezzo la cui posizione � passata per parametro
	GField->SetLegalMoves(GField->LegalMoves(Position));
	
	// Mostra le Mosse Legali (colore giallo)
	GField->ShowLegalMovesInTheField();
}

int32 ATTT_GameMode::GetNextPlayer(int32 Player) const
{
	Player++;
	if (!Players.IsValidIndex(Player))
	{
		Player = 0;
	}
	return Player;
}

AGameField* ATTT_GameMode::GetGameField() const
{
	return GField;
}

void ATTT_GameMode::ChoosePlayerAndStartGame()
{
	// The game starts with Human Player
	CurrentPlayer = 0;
	
	for (int32 i = 0; i < Players.Num(); i++)
	{
		Players[i]->PlayerNumber = i;
	}
	MoveCounter += 1;
	Players[CurrentPlayer]->OnTurn();
}

void ATTT_GameMode::TurnNextPlayer()
{
	MoveCounter += 1;
	CurrentPlayer = GetNextPlayer(CurrentPlayer);
	Players[CurrentPlayer]->OnTurn();
}

void ATTT_GameMode::DoMove(const FVector2D EndPosition, const bool bIsGameMove)
{
	const FVector2D StartPosition = GField->GetSelectedTile();
	const int32 IDPieceToMove = (*GField->TileMap.Find(StartPosition))->GetPiece()->GetPieceID();
	const int32 IDPieceEaten = CaptureThePieceIfExist(EndPosition, bIsGameMove);

	SetTileMapStatus(StartPosition, EndPosition);

	if (bIsGameMove)
	{
		/*** CHANGE LOCATION ***/
		SetPieceLocation(EndPosition);
		GField->ResetGameStatusField();

		/*
		TODO: SCEGLIERE SE STAMPARLO O NO
		(CurrentPlayer) 
			? GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("AI made the move"))
			: GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Player made the move"));
		*/
	}

	// Se il pezzo mosso è una pedina
	if (IsPawn(EndPosition))
	{
		// Gestisco la promozione
		HandlePawnPromotionIfExists(EndPosition, bIsGameMove);
		
		/* TODO: aggiungere un timer di mezzo secondo
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, EndPosition, bIsGameMove]()
			{
				HandlePawnPromotionIfExists(EndPosition, bIsGameMove);
			}, 0.5f, false);
		*/
	}

	// Se il pezzo che ho mosso era un pezzo già promosso
	APiece* Piece = (*GField->TileMap.Find(EndPosition))->GetPiece();
	if (Piece->IsPromoted())
	{
		// Aggiungo 1 alle mosse eseguite dopo la promozione
		Piece->MovesSincePromotion++;
	}
	const FMove CurrentMove = FMove(Moves.Num() + 1, IDPieceToMove, StartPosition, EndPosition, IDPieceEaten); 
	Moves.Add(CurrentMove);

	if(bIsGameMove)
	{
		MovesPanel->AddMoveToPanel(CurrentMove);
		
		if (!IsWinMove(CurrentPlayer))
		{
			TurnNextPlayer();
		}
	}
}

void ATTT_GameMode::UndoMove(const bool bIsGameMove)
{
	const FMove LastMove = Moves.Pop();

	HandleUndoMoveForPawnsAndPromotion(LastMove.End, bIsGameMove);	

	SetTileMapStatus(LastMove.End, LastMove.Start);

	if(bIsGameMove)
	{
		SetPieceLocation(LastMove.Start);
		MovesPanel->PopFromPanel();
	}

	if(LastMove.IDPieceEaten != -1)
	{
		APiece* PieceToRespawn;
		if((*GField->TileMap.Find(LastMove.Start))->GetOwner() == 0)
		{
			PieceToRespawn = BPiecesKilled.Pop(); 
		}
		else
		{
			PieceToRespawn = WPiecesKilled.Pop();
		}

		(*GField->TileMap.Find(LastMove.End))->SetTileStatus(
			PieceToRespawn->GetOwner(),
			ETileStatus::OCCUPIED,
			PieceToRespawn
			);

		PieceToRespawn->SetActorHiddenInGame(false);
		PieceToRespawn->SetActorEnableCollision(true);
	}
}

void ATTT_GameMode::SetTileMapStatus(const FVector2D Start, const FVector2D End) const
{
	ATile*	StartTile	= *GField->TileMap.Find(Start);
	const int32	StartOwner	= StartTile->GetOwner();
	APiece*	Piece		= StartTile->GetPiece();
	Piece->SetGridPosition(End.X, End.Y);
	
	StartTile->SetTileStatus(-1, ETileStatus::EMPTY, nullptr);
	//StartTile->SetTileStatus(-1, ETileStatus::EMPTY, nullptr);
	(*GField->TileMap.Find(End))->SetTileStatus(StartOwner, ETileStatus::OCCUPIED, Piece);
}

int32 ATTT_GameMode::CaptureThePieceIfExist(FVector2D End, const bool bIsGamemove)
{
	ATile* EndTile = *GField->TileMap.Find(End);

	// Se la pedina � dell'avversario,
	// => La mangio e restituisco il suo ID
	if (EndTile->GetOwner() == (CurrentPlayer ^ 1))
	{
		return CaptureThePiece(End, bIsGamemove);
	}

	return -1;
}

int32 ATTT_GameMode::CaptureThePiece(FVector2D End, const bool bIsGameMove)
{
	APiece* PieceKilled = (*GField->TileMap.Find(End))->GetPiece();
	
	(CurrentPlayer == 0)
		? BPiecesKilled.Add(PieceKilled)
		: WPiecesKilled.Add(PieceKilled);

	// TODO: cambiare la posizione e metterli a lato della scacchiera
	//PieceKilled->SetActorLocation(FVector(350, -350, 0));
	PieceKilled->SetActorHiddenInGame(true);
	PieceKilled->SetActorEnableCollision(false);

	
	// TODO: serve?
	(*GField->TileMap.Find(End))->SetTileStatus(-1, ETileStatus::EMPTY, nullptr);
	
	return PieceKilled->GetPieceID();

	/*
	* Non uso pi� questo distruttore perch� i pezzi devono rimanere
	* nel background del gioco
	*
	* PieceToRemove->Destroy();
	*/

	/* TODO: aggiungi la seguente implementazione
	*
	*	LISTA DEI PEZZI IN VITA
	* Quando entro in questa funzione, rimuovo il pezzo dalla lista
	*/
}

void ATTT_GameMode::SetPieceLocation(const FVector2D End) const
{
	FVector EndLocation = GField->GetRelativeLocationByXYPosition(End.X, End.Y);
	EndLocation.Z += 5;

	(*GField->TileMap.Find(End))->GetPiece()->SetActorLocation(EndLocation);
}

bool ATTT_GameMode::IsPawn(const FVector2D Position) const
{
	return (*GField->TileMap.Find(Position))->GetPiece()->GetPieceType() == EPieceType::PAWN;
}

void ATTT_GameMode::HandlePawnPromotionIfExists(const FVector2D Position, const bool bIsGameMove)
{
	// Handle White Pawns
	if (Position.X == GField->Size - 1)
	{
		HandlePawnPromotion(0, Position, bIsGameMove);
	}
	else if(Position.X == 0)
	{
		HandlePawnPromotion(1, Position, bIsGameMove);
	}
}

void ATTT_GameMode::HandlePawnPromotion(const int32 Player, const FVector2D Position, const bool bIsGameMove)
{
	if (bIsGameMove)
	{
		(Player == 0)
			? GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Promotion for HumanPlayer"))
			: GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Promotion for AI"));
	}		

	APiece* PawnPromoted = (*GField->TileMap.Find(Position))->GetPiece();

	(Player == 0)
		? WPiecesKilled.Add(PawnPromoted)
		: BPiecesKilled.Add(PawnPromoted);

	PawnPromoted->SetActorHiddenInGame(true);
	PawnPromoted->SetActorEnableCollision(false);

	// TODO: aggiungere la possibilità di scelta tra Knight, Bishop, Rook, Queen
	// - Ricordare gli include

	// GENERA LA REGINA
	GField->GeneratePiece<AQueen>(Position, Player);
	(*GField->TileMap.Find(Position))->GetPiece()->SetIsPromoted(true);
}

void ATTT_GameMode::HandleUndoMoveForPawnsAndPromotion(FVector2D End, const bool bIsGameMove)
{
	APiece* Piece = (*GField->TileMap.Find(End))->GetPiece();
	if (Piece->IsPromoted())
	{
		Piece->MovesSincePromotion--;
		if (Piece->MovesSincePromotion == 0)
		{
			Piece->SetActorHiddenInGame(true);
			Piece->SetActorEnableCollision(false);

			APiece* PieceToRespawn = nullptr;
			(Piece->GetOwner() == 0)
				? PieceToRespawn = WPiecesKilled.Pop()
				: PieceToRespawn = BPiecesKilled.Pop();

			(*GField->TileMap.Find(End))->SetTileStatus(
				PieceToRespawn->GetOwner(),
				ETileStatus::OCCUPIED,
				PieceToRespawn
			);

			PieceToRespawn->SetActorHiddenInGame(false);
			PieceToRespawn->SetActorEnableCollision(true);
		}
	}

}

bool ATTT_GameMode::IsIllegalMove()
{
	// Given a state of Match

	// Check for all Pieces if there's at least one illegal move
	for(const auto& CurrentTile : GField->TileArray)
	{
		if(CurrentTile->GetOwner() == (CurrentPlayer ^ 1))
		{
			CurrentPlayer = GetNextPlayer(CurrentPlayer);
			TArray<FVector2D> EnemyLegalMoves = CurrentTile->GetPiece()->PieceLegalMoves(); 
			CurrentPlayer = GetNextPlayer(CurrentPlayer);

			FVector2D KingPosition = GField->GetKingPosition(CurrentPlayer);
			if (EnemyLegalMoves.Contains(KingPosition))
			{
				return true;
			}
		}
	}
	
	return false;
}

bool ATTT_GameMode::IsWinMove(const int32 Player)
{
	for(const auto& CurrentTile : GField->TileArray)
	{
		if(CurrentTile->GetOwner() == (Player ^ 1))
		{
			GField->SetSelectedTile(CurrentTile->GetGridPosition());

			TArray<FVector2D> CurrentLegalMoves;
			
			CurrentPlayer = GetNextPlayer(CurrentPlayer);
			CurrentLegalMoves = GField->LegalMoves(CurrentTile->GetGridPosition());
			CurrentPlayer = GetNextPlayer(CurrentPlayer);
			
			if(CurrentLegalMoves.Num() > 0)
			{
				return false;
			}
		}
	}
	
	IsGameOver = true;
	Players[CurrentPlayer]->OnWin();
	for (int32 i = 0; i < Players.Num(); i++)
	{
		if (i != CurrentPlayer)
		{
			Players[i]->OnLose();
		}
	}

	return true;
}

void ATTT_GameMode::UndoGesture(bool bIsGameMove)
{

	if (Moves.Num() > 0 && CurrentPlayer == 0)
	{
		GField->ResetGameStatusField();
		UndoMove(bIsGameMove);

		if (Moves.Num() > 0)
		{
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, bIsGameMove]()
				{
					UndoMove(bIsGameMove);
				}, 0.5f, false);
		}
	}
}