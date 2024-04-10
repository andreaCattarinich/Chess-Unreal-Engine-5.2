// Copyright © 2024 Andrea Cattarinich

#pragma once

#include "Piece.h"

#include "CoreMinimal.h"
#include "Rook.generated.h"

UCLASS()
class CHESS_API ARook : public APiece
{
	GENERATED_BODY()

public:
    // ************ CONSTRUCTORS ************
    ARook();

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
