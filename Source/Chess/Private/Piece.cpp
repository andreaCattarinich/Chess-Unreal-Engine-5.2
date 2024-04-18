// Copyright © 2024 Andrea Cattarinich

#include "Piece.h"
#include "GameField.h"
#include "Chess_GameMode.h"
#include "UObject/ConstructorHelpers.h"

int32 APiece::NewPieceID = 0;

// Sets default values
APiece::APiece()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// template function that creates a components
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// every actor has a RootComponent that defines the transform in the World
	SetRootComponent(Scene);
	StaticMeshComponent->SetupAttachment(Scene);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane"));
	
	if (StaticMeshAsset.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(StaticMeshAsset.Object);
	}
	
	PieceGridPosition = FVector2D(-1, -1);
	PlayerOwner = -1;
	PieceID = -100;
	PieceValue = 0;
	GameMode = nullptr;
	bIsPromoted = false;
}

// Called every frame
void APiece::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APiece::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// Called when the game starts or when spawned
void APiece::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
}

void APiece::SetPieceID()
{
	// Increment the ID and assign it to the current piece
	PieceID = NewPieceID++;
}

void APiece::SetPlayerOwner(const int32 PlayerNumber)
{
	PlayerOwner = PlayerNumber;
}

void APiece::SetGridPosition(const double InX, const double InY)
{
	PieceGridPosition.Set(InX, InY);
}

void APiece::SetTexture() const
{
	// W: white
	// B: black	
	const FString StringOwner = (PlayerOwner) ? FString("B") : FString("W");

	const FString StringType = PieceTypeToString(PieceType);
	const FString MaterialPath =
		FString("/Game/Materials/Textures/M_") +
		StringOwner +
		StringType;

	UMaterialInterface* Material = LoadObject<UMaterialInterface>(
		nullptr, *MaterialPath);

	StaticMeshComponent->SetMaterial(0, Material);
}

void APiece::SetIsPromoted(const bool bNewIsPromoted)
{
	bIsPromoted = bNewIsPromoted;
}

int32 APiece::GetPieceID() const
{
	return PieceID;
}

int32 APiece::GetPieceOwner() const
{
	return PlayerOwner;
}

FVector2D APiece::GetGridPosition() const
{
	return PieceGridPosition;
}

int32 APiece::GetPieceValue() const
{
	return PieceValue;
}

EPieceType APiece::GetPieceType() const
{
	return PieceType;
}

bool APiece::IsPromoted() const
{
	return bIsPromoted;
}

int32 APiece::GetNumberMovesSincePromotion() const
{
	return MovesSincePromotion;
}

TArray<FVector2D> APiece::PieceLegalMoves()
{
	return TArray<FVector2D>();
}

TArray<FVector2D> APiece::SearchLegalMoves(
	const TArray<FVector2D>& Directions,
	const bool bMoveUntilPiece) const
{
	TArray<FVector2D> LegalMoves;

	for (const FVector2D& Direction : Directions)
	{
		FVector2D Position = GetGridPosition();
		Position += Direction;
		do
		{
			if (!(GameMode->GField->IsValidPosition(Position)))
			{
				break;
			}

			const ATile* CurrentTile = *GameMode->GField->TileMap.Find(Position);
			
			if (CurrentTile->GetTileStatus() == ETileStatus::OCCUPIED)
			{
				if (CurrentTile->GetTileOwner() == ((GameMode->CurrentPlayer) ^ 1))
				{
					LegalMoves.Add(Position);
				}
				break;
			}

			LegalMoves.Add(Position);
			Position += Direction;
		} while (bMoveUntilPiece);
	}

	return LegalMoves;
}

FString APiece::PieceTypeToString(const EPieceType Type)
{
	switch (Type)
	{
	case EPieceType::PAWN:	return FString("Pawn");
	case EPieceType::ROOK:	return FString("Rook");
	case EPieceType::KNIGHT:return FString("Knight");
	case EPieceType::BISHOP:return FString("Bishop");
	case EPieceType::QUEEN:	return FString("Queen");
	case EPieceType::KING:	return FString("King");
	default:				return FString("Unknown");
	}
}

void APiece::SelfDestroy()
{
	Destroy();
}

void APiece::IncreaseNumberMovesSincePromotion()
{
	MovesSincePromotion++;
}

void APiece::DecreaseNumberMovesSincePromotion()
{
	MovesSincePromotion--;
}
