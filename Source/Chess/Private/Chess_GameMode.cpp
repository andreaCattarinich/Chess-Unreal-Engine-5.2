// Fill out your copyright notice in the Description page of Project Settings.

#include "..\Public\Chess_GameMode.h"

#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"

#include "GameField.h"
#include "..\Public\Chess_PlayerController.h"
#include "..\Public\Chess_HumanPlayer.h"
#include "..\Public\Chess_RandomPlayer.h"
#include "..\Public\Chess_MinimaxPlayer.h"

#include "MovesPanel.h"
#include "Promotion.h"

#include "EngineUtils.h"


AChess_GameMode::AChess_GameMode()
{
	PlayerControllerClass = AChess_PlayerController::StaticClass();
	DefaultPawnClass = AChess_HumanPlayer::StaticClass();

	FieldSize = 8;
	IsGameOver = false;
	CurrentPlayer = -1;
	MoveCounter = -1;
	GField = nullptr;
	MovesPanel = nullptr;
	Promotion = nullptr;
	CurrentPlayerForPromotion = -1;
	bIsChoosing = false;
	
}

void AChess_GameMode::BeginPlay()
{
	Super::BeginPlay();

	IsGameOver = false;

	MoveCounter = 0;

	AChess_HumanPlayer* HumanPlayer = Cast<AChess_HumanPlayer>(*TActorIterator<AChess_HumanPlayer>(GetWorld()));

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

	
	UChess_GameInstance* GameInstance = Cast<UChess_GameInstance>(GetGameInstance());
	
	if(GameInstance->Difficulty == 0)
	{
		// Random Player
		auto* AI = GetWorld()->SpawnActor<AChess_RandomPlayer>(FVector(), FRotator());
		Players.Add(AI);
		PlayerNames.Add(1, "AI");
	}
	else
	{
		// MiniMax Player
		auto* AI = GetWorld()->SpawnActor<AChess_MinimaxPlayer>(FVector(), FRotator());
		Players.Add(AI);
		PlayerNames.Add(1, "AI");
	}
	

	MovesPanel = CreateWidget<UMovesPanel>(GetGameInstance(), WidgetClass);

	if(MovesPanel)
	{
		MovesPanel->AddToViewport(0);		
	}

	/****/
	PrepareReset();

	/*****/
	ChoosePlayerAndStartGame();
}

void AChess_GameMode::SetSelectedTile(const FVector2D Position) const
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

int32 AChess_GameMode::GetNextPlayer(int32 Player) const
{
	Player++;
	if (!Players.IsValidIndex(Player))
	{
		Player = 0;
	}
	return Player;
}

AGameField* AChess_GameMode::GetGameField() const
{
	return GField;
}

void AChess_GameMode::ChoosePlayerAndStartGame()
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

void AChess_GameMode::TurnNextPlayer()
{
	MoveCounter += 1;
	CurrentPlayer = GetNextPlayer(CurrentPlayer);
	Players[CurrentPlayer]->OnTurn();
}

void AChess_GameMode::DoMove(const FVector2D EndPosition, const bool bIsGameMove)
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
	}

	// Se il pezzo mosso è una pedina
	if (IsPawn(EndPosition))
	{
		// Gestisco la promozione
		HandlePawnPromotionIfExists(EndPosition, bIsGameMove);
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
		
		if (!IsWinnerMove(CurrentPlayer, bIsGameMove))
		{
			TurnNextPlayer();
		}
	}
}

void AChess_GameMode::UndoMove(const bool bIsGameMove)
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

void AChess_GameMode::SetTileMapStatus(const FVector2D Start, const FVector2D End) const
{
	ATile*	StartTile	= *GField->TileMap.Find(Start);
	const int32	StartOwner	= StartTile->GetOwner();
	APiece*	Piece		= StartTile->GetPiece();
	Piece->SetGridPosition(End.X, End.Y);
	
	StartTile->SetTileStatus(-1, ETileStatus::EMPTY, nullptr);
	
	(*GField->TileMap.Find(End))->SetTileStatus(StartOwner, ETileStatus::OCCUPIED, Piece);
}

int32 AChess_GameMode::CaptureThePieceIfExist(FVector2D End, const bool bIsGamemove)
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

int32 AChess_GameMode::CaptureThePiece(FVector2D End, const bool bIsGameMove)
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

void AChess_GameMode::SetPieceLocation(const FVector2D End) const
{
	FVector EndLocation = GField->GetRelativeLocationByXYPosition(End.X, End.Y);
	EndLocation.Z += 5;

	(*GField->TileMap.Find(End))->GetPiece()->SetActorLocation(EndLocation);
}

bool AChess_GameMode::IsPawn(const FVector2D Position) const
{
	return (*GField->TileMap.Find(Position))->GetPiece()->GetPieceType() == EPieceType::PAWN;
}

void AChess_GameMode::HandlePawnPromotionIfExists(const FVector2D Position, const bool bIsGameMove)
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

void AChess_GameMode::HandlePawnPromotion(const int32 Player, const FVector2D Position, const bool bIsGameMove)
{
	/* FOR DEBUG
	if (bIsGameMove)
	{
		(Player == 0)
			? GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Promotion for HumanPlayer"))
			: GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Promotion for AI"));
	}		
	*/
	APiece* PawnPromoted = (*GField->TileMap.Find(Position))->GetPiece();

	(Player == 0)
		? WPiecesKilled.Add(PawnPromoted)
		: BPiecesKilled.Add(PawnPromoted);

	PawnPromoted->SetActorHiddenInGame(true);
	PawnPromoted->SetActorEnableCollision(false);
	
	// GENERA LA REGINA
	if (bIsGameMove)
	{
		// TODO: Queste righe di codice  creano il widget per la scelta della pedina da promuovere
		// Dopodiché la funzione che genera effettivamente la pedina promossa è delegata al click di uno dei 4 bottoni
		// Attraverso la funzione SetPromotionChoice viene effettivamente spawnato il pezzo.
		
		if (Player == 0)
		{
			Promotion = CreateWidget<UUserWidget>(GetGameInstance(), PromotionClass);

			if (Promotion)
			{
				Promotion->AddToViewport(1);
				UGameplayStatics::SetGamePaused(GetWorld(), true);  // Metti in pausa il gioco
			}

			CurrentPositionForPromotion = Position;
			CurrentPlayerForPromotion = Player;
		}
		else
		{
			GField->GeneratePiece<AQueen>(Position, Player);
			
			APiece* Piece = (*GField->TileMap.Find(Position))->GetPiece();
			Piece->SetIsPromoted(true);
			GField->OnResetEvent.AddDynamic(Piece, &APiece::SelfDestroy);			

		}
		
	}
	else
	{
		GField->GeneratePiece<AQueen>(Position, Player);
		(*GField->TileMap.Find(Position))->GetPiece()->SetIsPromoted(true);
	}
}

void AChess_GameMode::SetPromotionChoice(EPieceType PromotionType)
{
	if (Promotion)
	{
		//Promotion->RemoveFromViewport();
		Promotion->RemoveFromParent();
		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}
	
	switch (PromotionType)
	{
	case EPieceType::QUEEN:
		GField->GeneratePiece<AQueen>(CurrentPositionForPromotion, CurrentPlayerForPromotion);
		break;
	case EPieceType::BISHOP:
		GField->GeneratePiece<ABishop>(CurrentPositionForPromotion, CurrentPlayerForPromotion);
		break;
	case EPieceType::KNIGHT:
		GField->GeneratePiece<AKnight>(CurrentPositionForPromotion, CurrentPlayerForPromotion);
		break;
	case EPieceType::ROOK:
		GField->GeneratePiece<ARook>(CurrentPositionForPromotion, CurrentPlayerForPromotion);
		break;
	default: ;
	}
	
	APiece* Piece = (*GField->TileMap.Find(CurrentPositionForPromotion))->GetPiece();
	GField->OnResetEvent.AddDynamic(Piece, &APiece::SelfDestroy);			

	Piece->SetIsPromoted(true);
	
	//Piece = (*GField->TileMap.Find(CurrentPositionForPromotion))->GetPiece();
	if (Piece->IsPromoted())
	{
		// Aggiungo 1 alle mosse eseguite dopo la promozione
		Piece->MovesSincePromotion++;
	}

}

void AChess_GameMode::HandleUndoMoveForPawnsAndPromotion(const FVector2D End, const bool bIsGameMove)
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

bool AChess_GameMode::IsIllegalMove()
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

bool AChess_GameMode::IsWinnerMove(const int32 Player, const bool bIsGameMove)
{
	if(HasEnemyLegalMoves(Player))
	{
		return false;
	}

	return IsCheckMate(Player, bIsGameMove);
}

bool AChess_GameMode::HasEnemyLegalMoves(const int32 Player)
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
				return true;
			}
		}
	}
	
	return false;
}

bool AChess_GameMode::IsCheckMate(const int32 Player, const bool bIsGameMove)
{
	for(const auto& CurrentTile : GField->TileArray)
	{
		if(CurrentTile->GetOwner() == Player)
		{
			GField->SetSelectedTile(CurrentTile->GetGridPosition());

			TArray<FVector2D> CurrentLegalMoves = GField->LegalMoves(CurrentTile->GetGridPosition());
			if(CurrentLegalMoves.Contains(GField->GetKingPosition(GetNextPlayer(Player))))
			{
				if(bIsGameMove)
				{
					IsGameOver = true;
					Players[CurrentPlayer]->OnWin();
					for (int32 i = 0; i < Players.Num(); i++)
					{
						if (i != CurrentPlayer)
						{
							Players[i]->OnLose();
						}
					}
				}
				return true; 
			}
		}
	}

	if(bIsGameMove)
	{
		IsGameOver = true;
		for (int32 i = 0; i < Players.Num(); i++)
		{
			Players[i]->OnStalemate();
		}	
	}
	return true;
}

bool AChess_GameMode::IsWinMove(const int32 Player)
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

void AChess_GameMode::UndoGesture(bool bIsGameMove)
{

	//if (Moves.Num() > 0 && CurrentPlayer == 0 || Moves.Num() > 0 && IsGameOver)
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

void AChess_GameMode::PrepareReset()
{
	for(auto& CurrentTile : GField->TileArray)
	{
		if(CurrentTile->GetOwner() != -1)
		{
			APiece* CurrentPiece = CurrentTile->GetPiece();
			GField->OnResetEvent.AddDynamic(CurrentPiece, &APiece::SelfDestroy);			
		}
	}
	
}
	