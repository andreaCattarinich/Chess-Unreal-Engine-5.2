// Copyright © 2024 Andrea Cattarinich

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
	
	FMove()
		:	NumberMove(-1),
			IDPiece(-1),
			Start(FVector2D(-1,-1)),
			End(FVector2D(-1,-1)),
			IDPieceEaten(-1) {}

	FMove(int32 NewNumberMove, int32 NewIDPiece, FVector2D NewStart, FVector2D NewEnd, int32 NewIDPieceEaten)
		:	NumberMove(NewNumberMove),
			IDPiece(NewIDPiece),
			Start(NewStart),
			End(NewEnd),
			IDPieceEaten(NewIDPieceEaten) {}
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
	
	// Stack of moves
	TArray<FMove> Moves;

	// Array of Captured Pieces
	TArray<APiece*> WhiteCapturedPieces;
	TArray<APiece*> BlackCapturedPieces;

	// TSubclassOf is a template class that provides UClass type safety.
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AGameField> GameFieldClass;
	
	// Field size
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 FieldSize;

	// Reference to a GameField object
	UPROPERTY(VisibleAnywhere)
		AGameField* GField;

	// Reference to a Blueprint of Scroll box and Title for the replay
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UMovesPanel> PanelWidgetClass;

	// Reference to the panel
	UPROPERTY(VisibleAnywhere)
		UMovesPanel* MovesPanel;

	// Widget for Promotion
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUserWidget> PromotionClass;

	// Reference to the promotion element
	UPROPERTY(VisibleAnywhere)
		UUserWidget* Promotion;

	// Temporary Variables for Promotion
	UPROPERTY(VisibleAnywhere)
		FVector2D CurrentPositionForPromotion;

	UPROPERTY(VisibleAnywhere)
		int32 CurrentPlayerForPromotion;
	
	bool CanClickUndoButton = true;
	
	// ************ SETTERS ************
	// Executes all operations on the GameField to select the Tile
	// clicked by the Human Player (or the Tile clicked by the
	// Random/Minimax Player)
	void SetSelectedTile(const FVector2D Position) const;



	// ************ GETTERS ************	
	// Get the next player index
	int32 GetNextPlayer(int32 Player) const;

	

	// ************ METHODS ************	
	// Called at the start of the game
	void ChoosePlayerAndStartGame();

	// Called at the end of the game turn
	void TurnNextPlayer();
	
	// Do the move in the game. First you have to SelectTile(), then you can do the move
	// Note: when bIsGameMove is false, it indicates that the moves is performed in the background of the game
	void DoMove(const FVector2D EndPosition, const bool bIsGameMove = false);

	// Undo the move in the game
	// Note: when bIsGameMove is false, it indicates that the moves is performed in the background of the game
	void UndoMove(const bool bIsGameMove = false);

	// Changes the values of the TileMap
	void SetTileMapStatus(const FVector2D Start, const FVector2D End) const;
	
	// Return the ID of the eaten Piece
	int32 CaptureThePieceIfExist(FVector2D End, const bool bIsGameMove);
	int32 CaptureThePiece(FVector2D End, const bool bIsGameMove);

	// Optional (if bIsGameMove is true)
	void SetPieceLocation(const FVector2D End) const;

	// Check if the piece passed by argument is a Pawn
	bool IsPawn(const FVector2D Position) const;

	// Functions that handle the promotion of a Pawn
	void HandlePawnPromotionIfExists(const FVector2D Position, const bool bIsGameMove);
	void HandlePawnPromotion(const int32 Player, const FVector2D Position, const bool bIsGameMove);
	void HandleUndoMoveForPawnsAndPromotion(FVector2D End, const bool bIsGameMove);
	void SetPromotionChoice(EPieceType PromotionType);

	// Given a state of Match, check for all Pieces if there's at least one illegal move
	bool IsIllegalMove();

	// Check if the enemy has legal moves and if is in checkmate
	bool IsWinnerMove(const int32 Player, const bool bIsGameMove);
	bool HasEnemyLegalMoves(const int32 Player);
	bool IsCheckMate(const int32 Player, const bool bIsGameMove);
	
	// Revert status of game (2 moves backward)
	UFUNCTION(BlueprintCallable)
		void UndoGesture(bool bIsGameMove = false);

	// For each piece, the OnResetEvent must be declared calling SelfDestroy
	void PrepareReset();
};
