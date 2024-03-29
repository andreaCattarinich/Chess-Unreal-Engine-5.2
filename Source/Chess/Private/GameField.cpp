// Fill out your copyright notice in the Description page of Project Settings.



#include "GameField.h"
#include "Kismet/GameplayStatics.h"
#include "Piece.h"
#include "Pawns.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "King.h"
#include "MovesPanel.h"
#include "Queen.h"
#include "..\Public\Chess_GameMode.h"
#include "Components/TextRenderComponent.h"

// Sets default values
AGameField::AGameField()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Size = 8;						// Size of the field (8x8)
	TileSize = 150;					// Tile dimension
	CellPadding = 0;				// Tile padding dimension
	NormalizedCellPadding = 0.0f;
	PieceScalePercentage = 100;
}

void AGameField::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	// Normalized tilepadding
	NormalizedCellPadding = FMath::RoundToDouble(((TileSize + CellPadding) / TileSize) * 100) / 100;
}

// Called when the game starts or when spawned
void AGameField::BeginPlay()
{
	Super::BeginPlay();

	if (ListOfMovesWidgetRef)
	{
		ListOfMovesWidgetRef->AddToViewport(0);
	}



	GenerateField();
	GeneratePieces();
	
	//GeneratePiecesForMinimaxTest();
	//GeneratePiecesForPromotion();
	//GeneratePiecesForPromotion2();
}

void AGameField::SetSelectedTile(const FVector2D Position)
{
	SelectedTile = Position;
}

void AGameField::SetLegalMoves(TArray<FVector2D> NewLegalMoves)
{
	LegalMovesArray = NewLegalMoves;
}

FVector2D AGameField::GetPosition(const FHitResult& Hit)
{
	return Cast<ATile>(Hit.GetActor())->GetGridPosition();
}

TArray<ATile*>& AGameField::GetTileArray()
{
	return TileArray;
}

TArray<ATile*>& AGameField::GetWhiteTileArray()
{
	return WhiteTileArray;
}

TArray<ATile*>& AGameField::GetBlackTileArray()
{
	return BlackTileArray;
}

TArray<FVector2D> AGameField::GetLegalMoves()
{
	return LegalMovesArray;
}

TMap<FVector2D, ATile*> AGameField::GetTileMap()
{
	return TileMap;
}

FVector2D AGameField::GetSelectedTile() const
{
	return SelectedTile;
}

FVector AGameField::GetRelativeLocationByXYPosition(const int32 InX, const int32 InY) const
{
	return TileSize * NormalizedCellPadding * FVector(InX, InY, 0);
}

FVector2D AGameField::GetXYPositionByRelativeLocation(const FVector& Location) const
{
	const double x = Location[0] / (TileSize * NormalizedCellPadding);
	const double y = Location[1] / (TileSize * NormalizedCellPadding);
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("x=%f,y=%f"), x, y));
	return FVector2D(x, y);
}

FVector2D AGameField::GetKingPosition(const int32 Player) const
{
	for (auto& CurrentTile : TileArray)
	{
		if(CurrentTile->GetOwner() == Player)
		{
			if(CurrentTile->GetPiece()->GetPieceType() == EPieceType::KING)
			{
				return CurrentTile->GetGridPosition();
			}
		}
	}
	
	return FVector2D(-1, -1);
}



void AGameField::GenerateField()
{
	for (int32 x = 0; x < Size; x++)
	{
		for (int32 y = 0; y < Size; y++)
		{
			FVector Location = GetRelativeLocationByXYPosition(x, y);
			ATile* CurrentTile = GetWorld()->SpawnActor<ATile>(
				((x + y) % 2 == 0)
				? TileClassDark
				: TileClassLight,
				Location,
				FRotator::ZeroRotator
			);

			const float TileScale = TileSize / 100;
			CurrentTile->SetActorScale3D(FVector(TileScale, TileScale, 0.2));
			CurrentTile->SetGridPosition(x, y);

			/************ CODICE PER GESTIRE LA STAMPA DEI NUMERI/LETTERE A BORDO PARTITA ************/
			// TODO: eventualmente mettere queste linee di codice dentro a un metodo
			CurrentTile->TileTextNumber->SetRelativeLocation(FVector(Location.X + (TileSize / 6), Location.Y - (TileSize * 5/12), 5.0f));
			CurrentTile->TileTextNumber->SetRelativeRotation(FRotator(90,0,180));
			CurrentTile->TileTextNumber->SetXScale(TileSize/80);
			CurrentTile->TileTextNumber->SetYScale(TileSize/80);

			if((x+y) % 2 == 0)
			{
				CurrentTile->TileTextNumber->SetTextRenderColor(FColor(255,255,255,255));	
				CurrentTile->TileTextLetter->SetTextRenderColor(FColor(255,255,255,255));	
			}
			else
			{
				CurrentTile->TileTextNumber->SetTextRenderColor(FColor(0,0,0,255));
				CurrentTile->TileTextLetter->SetTextRenderColor(FColor(0,0,0,255));
			}

			CurrentTile->TileTextNumber->SetText(FText::FromString(TEXT("")));
			CurrentTile->TileTextLetter->SetText(FText::FromString(TEXT("")));

			
				
			CurrentTile->TileTextLetter->SetRelativeLocation(FVector(Location.X - (TileSize/2), Location.Y + (TileSize * 35/120), 5.0f));
			CurrentTile->TileTextLetter->SetRelativeRotation(FRotator(90,0,180));
			CurrentTile->TileTextLetter->SetXScale(TileSize/80);
			CurrentTile->TileTextLetter->SetYScale(TileSize/80);


			if(y == 0)
			{
				CurrentTile->TileTextNumber->SetText(FText::FromString(FString::Printf(TEXT("%c"), '1' + x)));
			}
			if(x == 0)
			{
				CurrentTile->TileTextLetter->SetText(FText::FromString(FString::Printf(TEXT("%c"), 'a' + y)));
			}
			
			/************ FINE - CODICE PER GESTIRE LA STAMPA DEI NUMERI/LETTERE A BORDO PARTITA ************/ 

			TileArray.Add(CurrentTile);
			TileMap.Add(FVector2D(x, y), CurrentTile);
		}
	}
}

void AGameField::GeneratePieces()
{
	GeneratePiece<ARook>	(FVector2D(0, 0), 0);
	GeneratePiece<AKnight>	(FVector2D(0, 1), 0);
	GeneratePiece<ABishop>	(FVector2D(0, 2), 0);
	GeneratePiece<AQueen>	(FVector2D(0, 3), 0);
	GeneratePiece<AKing>	(FVector2D(0, 4), 0);
	GeneratePiece<ABishop>	(FVector2D(0, 5), 0);
	GeneratePiece<AKnight>	(FVector2D(0, 6), 0);
	GeneratePiece<ARook>	(FVector2D(0, 7), 0);

	for (int32 i = 0; i < 8; i++)
	{
		GeneratePiece<APawns>(FVector2D(1, i), 0);
	}

	GeneratePiece<ARook>	(FVector2D(Size - 1, 0), 1);
	GeneratePiece<AKnight>	(FVector2D(Size - 1, 1), 1);
	GeneratePiece<ABishop>	(FVector2D(Size - 1, 2), 1);
	GeneratePiece<AQueen>	(FVector2D(Size - 1, 3), 1);
	GeneratePiece<AKing>	(FVector2D(Size - 1, 4), 1);
	GeneratePiece<ABishop>	(FVector2D(Size - 1, 5), 1);
	GeneratePiece<AKnight>	(FVector2D(Size - 1, 6), 1);
	GeneratePiece<ARook>	(FVector2D(Size - 1, 7), 1);

	for (int32 i = 0; i < 8; i++)
	{
		GeneratePiece<APawns>(FVector2D(Size - 2, i), 1);
	}
}

void AGameField::GeneratePiecesForMinimaxTest()
{
	// WHITE
	GeneratePiece<ARook>(FVector2D(2, 1), 0);
	GeneratePiece<AKnight>(FVector2D(1, 0), 0);
	GeneratePiece<ABishop>(FVector2D(1, 2), 0);
	GeneratePiece<APawns>(FVector2D(5, 5), 0);
	GeneratePiece<AKing>(FVector2D(1, 1), 0);
	
	// BLACK
	GeneratePiece<ABishop>(FVector2D(0, 1), 1);
	GeneratePiece<APawns>(FVector2D(1, 6), 1);
	GeneratePiece<AKing>(FVector2D(0, 7), 1);

}

void AGameField::GeneratePiecesForPromotion()
{
	GeneratePiece<ARook>(FVector2D(0, 0), 0);
	GeneratePiece<AKnight>(FVector2D(0, 1), 0);
	GeneratePiece<ABishop>(FVector2D(0, 2), 0);
	GeneratePiece<AQueen>(FVector2D(0, 3), 0);
	GeneratePiece<AKing>(FVector2D(0, 4), 0);
	GeneratePiece<ABishop>(FVector2D(0, 5), 0);
	GeneratePiece<AKnight>(FVector2D(0, 6), 0);
	GeneratePiece<ARook>(FVector2D(0, 7), 0);

	for (int32 i = 0; i < 8; i++)
	{
		GeneratePiece<APawns>(FVector2D(1, i), 0);
	}

	//GeneratePiece<ARook>(FVector2D(Size - 1, 0), 1);
	GeneratePiece<AKnight>(FVector2D(Size - 1, 1), 1);
	GeneratePiece<ABishop>(FVector2D(Size - 1, 2), 1);
	GeneratePiece<AQueen>(FVector2D(Size - 1, 3), 1);
	GeneratePiece<AKing>(FVector2D(Size - 1, 4), 1);

}

void AGameField::GeneratePiecesForPromotion2()
{
	GeneratePiece<AKing>(FVector2D(0, 0), 0);
	GeneratePiece<APawns>(FVector2D(1, 0), 0);
	GeneratePiece<APawns>(FVector2D(1, 1), 0);
	GeneratePiece<APawns>(FVector2D(0, 1), 0);
	GeneratePiece<ABishop>(FVector2D(2, 2), 0);
	GeneratePiece<ABishop>(FVector2D(2, 1), 0);

	//GeneratePiece<ARook>(FVector2D(Size - 1, 0), 1);
	//GeneratePiece<AKnight>(FVector2D(Size - 1, 1), 1);
	GeneratePiece<ABishop>(FVector2D(Size - 1, 2), 1);
	//GeneratePiece<AQueen>(FVector2D(Size - 1, 3), 1);
	GeneratePiece<AKing>(FVector2D(Size - 1, 4), 1);
	GeneratePiece<APawns>(FVector2D(2, 7), 1);


}

template<typename T>
void AGameField::GeneratePiece(FVector2D Position, int32 PlayerOwner)
{
	const FVector Location = GetRelativeLocationByXYPosition(Position.X, Position.Y);
	
	T* SpawnedPiece = GetWorld()->SpawnActor<T>(
		T::StaticClass(),
		FVector(Location.X, Location.Y, Location.Z + 5),
		FRotator(0.0f, 90.0f, 0.0f)
	);


	const float PieceScale = (TileSize / 100) * (PieceScalePercentage / 100);

	if(SpawnedPiece)
	{
		SpawnedPiece->SetPieceID();
		SpawnedPiece->SetGridPosition(Position.X, Position.Y);
		SpawnedPiece->SetActorScale3D(FVector(PieceScale, PieceScale, 0.5));
		SpawnedPiece->SetPlayerOwner(PlayerOwner);
		SpawnedPiece->SetTexture();
	
		PieceMap.Add(SpawnedPiece->GetPieceID(), SpawnedPiece);
		(*TileMap.Find(Position))->SetTileStatus(PlayerOwner, ETileStatus::OCCUPIED, SpawnedPiece);	
	}
}

inline bool AGameField::IsValidPosition(const FVector2D Position) const
{
	return 0 <= Position[0] && Position[0] < Size && 0 <= Position[1] && Position[1] < Size;
}

void AGameField::SelectTile(const FVector2D Position)
{
	if (IsValidPosition(Position))
	{
		(*TileMap.Find(Position))->SetTileGameStatus(ETileGameStatus::SELECTED);
		SetSelectedTile(Position);
	}
}

void AGameField::ResetGameStatusField()
{
	for (ATile* CurrentTile : TileArray)
	{
		if (CurrentTile->GetTileGameStatus() != ETileGameStatus::FREE)
		{
			CurrentTile->SetTileGameStatus(ETileGameStatus::FREE);
		}

		// TODO: togliere
		//CurrentTile->SetTileMaterial();
	}

	SetSelectedTile(FVector2D(-1, -1));
}

TArray<FVector2D> AGameField::LegalMoves(const FVector2D Position) const
{

	// If the tile passed as an argument does not belong (non appartiene) to any player
	if(!IsValidPosition(Position) || (*TileMap.Find(Position))->GetOwner() == NOT_ASSIGNED)
	{
		return TArray<FVector2D>();
	}

	// Here we have all possible moves
	TArray<FVector2D> PossibleMovesOfPiece = PossibleMoves(Position);

	// We must delete illegal moves
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());

	for (int32 index = PossibleMovesOfPiece.Num() - 1; index >= 0; --index)
	{
		// PossibleMovesOfPiece[index] contienere la posizione della mossa possibile i-esima
		
		GameMode->DoMove(PossibleMovesOfPiece[index], false);
		
		if (GameMode->IsIllegalMove())
		{	
			PossibleMovesOfPiece.RemoveAt(index); // REMOVE IF IS ILLEGAL
		}

		/*** UNDO THE MOVE ***/
		GameMode->UndoMove(false);
	}
	
	return PossibleMovesOfPiece;
}

TArray<FVector2D> AGameField::PossibleMoves(FVector2D Position) const
{
	return (*TileMap.Find(Position))->GetPiece()->PieceLegalMoves();
}

void AGameField::ShowLegalMovesInTheField()
{
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());

	for (const FVector2D& Position : LegalMovesArray)
	{
		ATile* CurrentTile = *TileMap.Find(Position);

		if (CurrentTile->GetOwner() == (GameMode->CurrentPlayer ^ 1))
		{
			CurrentTile->SetTileGameStatus(ETileGameStatus::CAN_CAPTURE);
		}
		else
		{
			CurrentTile->SetTileGameStatus(ETileGameStatus::LEGAL_MOVE);
		}

		CurrentTile->SetTileMaterial();
	}
}



// TODO: implementare il RESET
void AGameField::ResetField()
{
	
	for (ATile* CurrTile : TileArray)
	{
		CurrTile->SetTileStatus(NOT_ASSIGNED, ETileStatus::EMPTY, nullptr);
	}

	
	// send broadcast event to registered objects 
	OnResetEvent.Broadcast();
	
	// TODO: devo resettare tutti gli attributi
	//	Es: TileMap, PieceMap, SelectedTile etc... 
	//	E devo ri-spawnare tutti i pezzi.
	
	GeneratePieces();
	
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	GameMode->PrepareReset();
	GameMode->Moves.Empty();
	GameMode->WPiecesKilled.Empty();
	GameMode->BPiecesKilled.Empty();
	
	while(GameMode->MovesPanel->GetVerticalBox()->GetChildrenCount() > 0)
	{
		GameMode->MovesPanel->PopFromPanel();
	}
	
	GameMode->IsGameOver = false;
	GameMode->MoveCounter = 0;
	GameMode->ChoosePlayerAndStartGame();
	
}