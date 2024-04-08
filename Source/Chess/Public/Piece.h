// Fill out your copyright notice in the Description page of Project Settings.

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 PieceID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 PlayerOwner;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FVector2D PieceGridPosition;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EPieceType PieceType;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float PieceValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AChess_GameMode* GameMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsPromoted;

public: // TODO: METTERE PRIVATI
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 MovesSincePromotion = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 NumberMovePromotion;


public:
	// ************ SETTERS ************
	// Set Piece ID
	void SetPieceID();
	
	// Set Player Owner
	void SetPlayerOwner(int32 PlayerNumber);

	// set the (x, y) position
	void SetGridPosition(const double InX, const double InY);

	// Method that can be access by all Pieces (children)
	// in order to set the texture
	void SetTexture();

	void SetIsPromoted(const bool bIsPromoted);

	// ************ GETTERS ************
	// Get Piece ID
	int32 GetPieceID() const;

	// Get the Piece Owner
	int32 GetPieceOwner();
	
	// Get the (x, y) position
	FVector2D GetGridPosition() const;

	int32 GetPieceValue() const;
	
	EPieceType GetPieceType() const;

	bool IsPromoted() const;

	
	// ************ METHODS ************
	// Method to check if the piece can move in a specific direction
	virtual TArray<FVector2D> PieceLegalMoves();
	
	TArray<FVector2D> SearchLegalMoves(
		const TArray<FVector2D>& Directions,
		const bool bMoveUntilPiece = true
	) const;

	FString PieceTypeToString(const EPieceType Type);

	UFUNCTION()
	void SelfDestroy();
};
