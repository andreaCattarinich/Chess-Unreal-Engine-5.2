// Copyright © 2024 Andrea Cattarinich

#pragma once

#include "Tile.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "GameField.generated.h"

// macro declaration for a dynamic multicast delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReset);

UCLASS()
class CHESS_API AGameField : public AActor
{
	GENERATED_BODY()
	
public:	
	// ************ CONSTRUCTORS ************
	// Sets default values for this actor's properties
	AGameField();

	// Called when an instance of this class is placed (in editor) or spawned
	virtual void OnConstruction(const FTransform& Transform) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

	// ************ ATTRIBUTES ************
	static constexpr int32 NOT_ASSIGNED = -1;
	
	// Keeps track of Tiles
	UPROPERTY(Transient)
		TArray<ATile*> TileArray;

	// Given a position returns a Tile
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TMap<FVector2D, ATile*> TileMap;

	// Given an ID returns a Piece
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TMap<int32, APiece*> PieceMap;

	// Tile (or Piece) selected in the game
	UPROPERTY(EditDefaultsOnly)
		FVector2D SelectedTile;

	// Keeps track of LegalMoves
	UPROPERTY(Transient)
		TArray<FVector2D> LegalMovesArray;

	// Size of field
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 Size;

	// Tile size
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float TileSize;

	// Tile padding dimension
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float CellPadding;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float NormalizedCellPadding;

	// Indicates the size of the piece relative to the tile
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float PieceScalePercentage;
	
	// BlueprintAssignable Usable with Multicast Delegates only. Exposes the property for assigning in Blueprints.
	// declare a variable of type FOnReset (delegate)
	UPROPERTY(BlueprintAssignable)
		FOnReset OnResetEvent;

	// TSubclassOf template class that provides UClass type safety
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ATile> TileClassDark;

	// TSubclassOf template class that provides UClass type safety
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ATile> TileClassLight;

	// It's used for spawning the widget Panel for moves
	UPROPERTY(EditAnyWhere)
		UUserWidget* ListOfMovesWidgetRef;


	
	// ************ SETTERS ************
	// Set the Tile that is Selected from the Player (or AI)
	void SetSelectedTile(FVector2D Position);

	// Set LegalMovesArray
	void SetLegalMoves(const TArray<FVector2D>& NewLegalMoves);


	
	// ************ GETTERS ************
	// Return a (x,y) position given a hit (click) on a field tile
	FVector2D GetPosition(const FHitResult& Hit); // TODO: controllare se la uso

	// Return the array of Tile Pointers
	TArray<ATile*>& GetTileArray();

	// Return Tile Map [use with TileMap.Find(FVector2D ...)]
	TMap<FVector2D, ATile*> GetTileMap();
		
	// Return Position of Selected (Active) Tile
	FVector2D GetSelectedTile() const;
	
	// Return the array of Legal Moves' Position
	TArray<FVector2D> GetLegalMoves();
	
	// Return a relative position given (x,y) position
	FVector GetRelativeLocationByXYPosition(const int32 InX, const int32 InY) const;

	// Return (x,y) position given a relative position
	FVector2D GetXYPositionByRelativeLocation(const FVector& Location) const;

	// Return white or black King Position
	FVector2D GetKingPosition(int32 Player) const;



	// ************ METHODS ************
	// Generate an empty game field
	void GenerateField();

	// Generate Letters and Numbers on the first row and first column of the GameField
	void GenerateLettersAndNumbers(int32 X, int32 Y);

	// Generic function to Generate Pieces in the field
	void GeneratePieces();
	
	// TODO: cancellare
	void GeneratePiecesForMinimaxTest();
	void GeneratePiecesForPromotion();

	// Specific function to Generate a particular Piece
	template<typename T>
	void GeneratePiece(FVector2D Position, int32 Player);
	 
	// Checking if is a valid field position
	inline bool IsValidPosition(const FVector2D Position) const;

	// Sets select Tile in the game
	void SelectTile(const FVector2D Position);

	// Reset tile colors
	void ResetGameStatusField();
	
	// Returns the legal moves for the piece at the given position
	TArray<FVector2D> LegalMoves(FVector2D Position) const;

	// Returns the possible moves without considering illegal moves
	TArray<FVector2D> PossibleMoves(FVector2D Position) const;	

	// First you have to set LegalMovesArray, then use this function to show legal moves in the game board
	void ShowLegalMovesInTheField();
	
	// Remove all Pieces from the field
	UFUNCTION(BlueprintCallable) 
		void ResetField();
};
