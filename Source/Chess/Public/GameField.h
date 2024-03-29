// Fill out your copyright notice in the Description page of Project Settings.

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
	static const int32 NOT_ASSIGNED = -1;
	
	// Keeps track of Tiles
	UPROPERTY(Transient)
		TArray<ATile*> TileArray;
	
	UPROPERTY(Transient)
		TArray<ATile*> WhiteTileArray;
	
	UPROPERTY(Transient)
		TArray<ATile*> BlackTileArray;

	// Given a position returns a Tile
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TMap<FVector2D, ATile*> TileMap;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TMap<int32, APiece*> PieceMap;
	
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

	




	// ************ SETTERS ************
	// Set the Tile that is Selected from the Player
	void SetSelectedTile(FVector2D Position);

	void SetLegalMoves(TArray<FVector2D> NewLegalMoves);

	// ************ GETTERS ************
	// Return a (x,y) position given a hit (click) on a field tile
	FVector2D GetPosition(const FHitResult& Hit); // TODO: controllare se la uso

	// Return the array of Tile Pointers
	TArray<ATile*>& GetTileArray();

	// Return the array of Black Tile Pointers
	TArray<ATile*>& GetWhiteTileArray();

	// Return the array of Black Tile Pointers
	TArray<ATile*>& GetBlackTileArray();

	// Return Tile Map [use with TileMap.Find(FVector2D ...)]
	TMap<FVector2D, ATile*> GetTileMap();
		
	// Return Position of Active (Selected) Tile
	FVector2D GetSelectedTile() const;
	
	// Return the array of Legal Moves' Position
	TArray<FVector2D> GetLegalMoves();


	
	// Return a relative position given (x,y) position
	FVector GetRelativeLocationByXYPosition(const int32 InX, const int32 InY) const;

	// Return (x,y) position given a relative position
	FVector2D GetXYPositionByRelativeLocation(const FVector& Location) const;

	FVector2D GetKingPosition(int32 Player) const;



	// ************ METHODS ************
	// Generate an empty game field
	void GenerateField();

	// Generic function to Generate Pieces in the field
	void GeneratePieces();
	void GeneratePiecesForMinimaxTest();
	void GeneratePiecesForPromotion();
	void GeneratePiecesForPromotion2();

	// Specific function to Generate a particular Piece
	template<typename T>
	void GeneratePiece(FVector2D Position, int32 Player);
	 
	// Checking if is a valid field position
	inline bool IsValidPosition(const FVector2D Position) const; // TODO: da implementare

	void SelectTile(const FVector2D Position);

	/*
		TODO: ho cambiato il nome da ResetColorField()
		a ResetGameStatusColorField()

		Il nome è corretto quando si tratta di resettare i colori delle Tile.
		Ma non è coerente quando la GAMEMODE e il MINIMAX esegue in background
		le operazioni di DO e UNDO.

		Tutta via, il cambio di colore non viene visualizzato runtime.
		Non so il motivo. Probabilmente, sono calcoli sufficientemente veloci
	 */
	void ResetGameStatusField();


	// Restituisci le mosse del pezzo la cui posizione
	// è passata per parametro
	TArray<FVector2D> LegalMoves(FVector2D Position) const;
	TArray<FVector2D> PossibleMoves(FVector2D Position) const;	

	// First you have to set LegalMovesArray, then use this function to show
	// legal moves in the game board
	void ShowLegalMovesInTheField();

	
	// TODO: to implement
	// Remove all Pieces from the field
	UFUNCTION(BlueprintCallable) 
	void ResetField();


	/******** LIST OF MOVES **********/
public:
	UPROPERTY(EditAnyWhere)
		UUserWidget* ListOfMovesWidgetRef;
	/******** END LIST OF MOVES **********/
};
