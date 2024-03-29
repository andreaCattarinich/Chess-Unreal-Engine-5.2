// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Chess_PlayerInterface.h"
#include "GameField.h"

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Chess_GameMode.generated.h"

class AActor;

struct FPosition;

USTRUCT(BlueprintType)
struct FMove
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moves")
		int32 NumberMove;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moves")
		int32 IDPiece;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moves")
		FVector2D Start;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moves")
		FVector2D End;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moves")
		int32 IDPieceEaten;

	/*
	Costruttore personalizzato
	FMoves(int32 _IDPiece, FVector2D _Start, FVector2D _End, int32 _IDPieceEaten)
	{
		IDPiece			= _IDPiece;
		Start			= _Start;
		End				= _End;
		IDPieceEaten	= _IDPieceEaten;
	}
	*/
	FMove()
		: NumberMove(-1), IDPiece(-1), Start(FVector2D(-1,-1)), End(FVector2D(-1,-1)), IDPieceEaten(-1)
	{
	}

	FMove(int32 _NumberMove, int32 _IDPiece, FVector2D _Start, FVector2D _End, int32 _IDPieceEaten)
		: NumberMove(_NumberMove), IDPiece(_IDPiece), Start(_Start), End(_End), IDPieceEaten(_IDPieceEaten)
	{
	}
};

UCLASS()
class CHESS_API AChess_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// ************ CONSTRUCTORS ************
	AChess_GameMode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



	// ************ ATTRIBUTES ************	
	// Tracks if the game is over
	bool IsGameOver;

	// Array of player interfaces
	TArray<IChess_PlayerInterface*> Players;
	TMap<int32, FString> PlayerNames;

	int32 CurrentPlayer;

	// Tracks the number of moves in order to signal a drawn game
	int32 MoveCounter;
	
	// Stack di mosse
	TArray<FMove> Moves;

	// Array di Pedine Mangiate
	// TODO: CONTROLLA QUESTI DUE ATTRIBUTI
	TArray<APiece*> WPiecesKilled;
	TArray<APiece*> BPiecesKilled;
	

	// TSubclassOf is a template class that provides UClass type safety.
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AGameField> GameFieldClass;
	
	// Field size
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 FieldSize;

	// Reference to a GameField object
	UPROPERTY(VisibleAnywhere)
		AGameField* GField;


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UMovesPanel> WidgetClass;

	UPROPERTY(VisibleAnywhere)
	UMovesPanel* MovesPanel;


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> PromotionClass;

	UPROPERTY(VisibleAnywhere)
		UUserWidget* Promotion;

	UPROPERTY(VisibleAnywhere)
		int32 bIsChoosing; // 0 sceglie, 1 queen, 2 ...


	UPROPERTY(VisibleAnywhere)
		FVector2D CurrentPositionForPromotion; // 0 sceglie, 1 queen, 2 ...

	UPROPERTY(VisibleAnywhere)
		int32 CurrentPlayerForPromotion; // 0 sceglie, 1 queen, 2 ...
	
	// ************ SETTERS ************
	// Esegue tutte le operazioni sulla GameField per selezionare la Tile
	// cliccata dallo Human Player (oppure la Tile cliccata dal
	// Random/Minimax Player)
	void SetSelectedTile(const FVector2D Position) const;



	// ************ GETTERS ************	
	// Get the next player index
	int32 GetNextPlayer(int32 Player) const;

	AGameField* GetGameField() const;

	// ************ METHODS ************	
	// Called at the start of the game
	void ChoosePlayerAndStartGame();

	// Called at the end of the game turn
	void TurnNextPlayer();
	
	// Do the move in the game
	// First you have to SelectTile(), then you can do the move
	void DoMove(const FVector2D EndPosition, const bool bIsGameMove = false);
	
	void UndoMove(const bool bIsGameMove = false);
	
	void SetTileMapStatus(const FVector2D Start, const FVector2D End) const;
	
	// Return Piece eaten's ID
	int32 CaptureThePieceIfExist(FVector2D End, const bool bIsGameMove);
	int32 CaptureThePiece(FVector2D End, const bool bIsGameMove);

	// Optional (if is a GameMove)
	void SetPieceLocation(const FVector2D End) const;

	bool IsPawn(const FVector2D Position) const;
	void HandlePawnPromotionIfExists(const FVector2D Position, const bool bIsGameMove);

	void HandlePawnPromotion(const int32 Player, const FVector2D Position, const bool bIsGameMove);
	
	void HandleUndoMoveForPawnsAndPromotion(FVector2D End, const bool bIsGameMove);

	bool IsIllegalMove();
	bool IsWinMove(const int32 Player);

	// Revert status of game (2 moves backward)
	UFUNCTION(BlueprintCallable)
	void UndoGesture(bool bIsGameMove = false);
	
	void SetPromotionChoice(EPieceType PromotionType);

	void PrepareReset();

	bool IsWinnerMove(const int32 Player, const bool bIsGameMove);
	bool HasEnemyLegalMoves(const int32 Player);
	bool IsCheckMate(const int32 Player, const bool bIsGameMove);

};
