// Copyright © 2024 Andrea Cattarinich

#pragma once

#include "Piece.h"

#include "CoreMinimal.h"
#include "King.generated.h"

UCLASS()
class CHESS_API AKing : public APiece
{
	GENERATED_BODY()
	
public:
    // ************ CONSTRUCTORS ************
    AKing();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;



    // ************ ATTRIBUTES ************	



    // ************ SETTERS ************	


    
    // ************ GETTERS ************	



    // ************ METHODS ************	
    virtual TArray<FVector2D> PieceLegalMoves() override;
};
