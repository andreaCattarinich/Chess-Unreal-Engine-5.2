// Copyright © 2024 Andrea Cattarinich

#pragma once

#include "Piece.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

// TileStatus represent if there's o no a Piece on the Tile
UENUM()
enum class ETileStatus : uint8
{
	EMPTY		UMETA(DisplayName = "Empty"),
	OCCUPIED    UMETA(DisplayName = "Occupied"),
};

// TileGameStatus represents the various statuses a tile can have in the chess game
UENUM()
enum class ETileGameStatus : uint8
{
	FREE		UMETA(DisplayName = "Free"),		// Nothing happened
	SELECTED	UMETA(DisplayName = "Selected"),	// Selected by the Player (or AI) for movement
	LEGAL_MOVE	UMETA(DisplayName = "Legal Move"),	// Tile is legal move for the selected piece
	CAN_CAPTURE UMETA(DisplayName = "Can Capture"),	// Tile contains an enemy piece that can be captured
};

UCLASS()
class CHESS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// ************ CONSTRUCTORS ************
	// Sets default values for this actor's properties
	ATile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	// ************ ATTRIBUTES ************
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* StaticMeshComponent;

	// Keeps track of the Status' Tile
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Property")
		ETileStatus Status;

	// Keeps track of the Game Status' Tile
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Property")
		ETileGameStatus TileGameStatus;

	// Keeps track if the tile is reachable from the Player or not
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game Property")
		bool bIsLegal;

	// (x, y) position of the tile
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Property")
		FVector2D TileGridPosition;

	// Keeps track of the Tile's Player Owner
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Property")
		int32 PlayerOwner;

	// Keeps tack of the Piece currently occupying the tile
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Property")
		APiece* Piece;

public:
	// UI tile Number identifier
	UPROPERTY(VisibleAnywhere)
		class UTextRenderComponent* TileTextNumber;

	// UI tile Letter identifier
	UPROPERTY(VisibleAnywhere)
		class UTextRenderComponent* TileTextLetter;


	
	// ************ SETTERS ************	
	// Set the player owner and the status of a tile
	void SetTileStatus(const int32 TileOwner, const ETileStatus TileStatus, APiece* TilePiece);

	// Set the Tile's Game Status and if is legal or not
	void SetTileGameStatus(ETileGameStatus NewTileGameStatus);

	// Set the (x, y) position
	void SetGridPosition(const double InX, const double InY);
	
	// Set the color of a Tile
	void SetTileMaterial() const;
	

	
	// ************ GETTERS ************
	// Get the tile status
	ETileStatus GetTileStatus() const;

	// Get the tile Game Status
	ETileGameStatus GetTileGameStatus() const;

	// Get true if is reachable
	inline bool IsLegalTile() const;

	// Get the (x, y) position
	FVector2D GetGridPosition() const;

	// Get the tile owner
	int32 GetTileOwner() const;

	// Get the piece on the tile
	APiece* GetPiece() const;


	
	// ************ METHODS ************
	// Returns the material path for the colors used in the game tiles
	FString GetTileMaterialPath() const;

	// Converts the game status to a string representation.
	FString GameStatusToString() const;
};
