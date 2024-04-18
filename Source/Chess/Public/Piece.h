// Copyright © 2024 Andrea Cattarinich

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Piece.generated.h"

class AChess_GameMode;
class AGameField;

UENUM()
enum class EPieceType : uint8
{
	PAWN	UMETA(DisplayName = "Pawn"),
	ROOK	UMETA(DisplayName = "Rook"),
	KNIGHT	UMETA(DisplayName = "Knight"),
	BISHOP	UMETA(DisplayName = "Bishop"),
	QUEEN	UMETA(DisplayName = "Queen"),
	KING	UMETA(DisplayName = "King"),
};

UCLASS()
class CHESS_API APiece : public APawn
{
	GENERATED_BODY()

public:
	// ************ CONSTRUCTORS ************
	// Sets default values for this pawn's properties
	APiece();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



	// ************ ATTRIBUTES ************
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* StaticMeshComponent;
		
	// To track the last used ID
	static int32 NewPieceID;

	// ID of the Piece
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 PieceID;

	// Player Owner (white or black)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 PlayerOwner;

	// Piece's position
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FVector2D PieceGridPosition;

	// Type indicates (PAWN, ROOK, KNIGHT, BISHOP, QUEEN or KING)	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		EPieceType PieceType;

	// Value is used for Evaluation (10, 30, 30, 50, 90, 900)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float PieceValue;

	// Reference to GameMode
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AChess_GameMode* GameMode;

	// Indicates if the Piece was promoted 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsPromoted;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 MovesSincePromotion = 0;


	
public:
	// ************ SETTERS ************
	// Set Piece ID
	void SetPieceID();
	
	// Set Player Owner
	void SetPlayerOwner(int32 PlayerNumber);

	// Set the (x, y) position
	void SetGridPosition(const double InX, const double InY);

	// Method that can be access by all Pieces (children) to set the texture
	void SetTexture() const;

	// Set if the Piece is promoted or not
	void SetIsPromoted(const bool bIsPromoted);


	
	// ************ GETTERS ************
	// Get Piece ID
	int32 GetPieceID() const;

	// Get the Piece Owner
	int32 GetPieceOwner() const;
	
	// Get the (x, y) position
	FVector2D GetGridPosition() const;

	// Get the Value of the Piece
	int32 GetPieceValue() const;

	// Get the Type of the Piece
	EPieceType GetPieceType() const;

	// Get if the Piece was promoted or not
	bool IsPromoted() const;

	// If the Piece was promoted returns the number of moves made in the game, else returns zero
	int32 GetNumberMovesSincePromotion() const;


	
	// ************ METHODS ************
	// Method to check if the piece can move in a specific direction
	virtual TArray<FVector2D> PieceLegalMoves();

	// General function used to search legal moves from directions
	TArray<FVector2D> SearchLegalMoves(
		const TArray<FVector2D>& Directions,
		const bool bMoveUntilPiece = true
	) const;

	// Converts the Type to a string
	static FString PieceTypeToString(const EPieceType Type);

	// Function for ResetEvent (destroy the piece)
	UFUNCTION()
		void SelfDestroy();

	// Add one to Number of moves since promotion variable
	void IncreaseNumberMovesSincePromotion();
	
	// Subtract one to Number of moves since promotion variable
	void DecreaseNumberMovesSincePromotion();
};
