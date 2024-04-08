// Copyright © 2024 Andrea Cattarinich

#include "Tile.h"
#include "Components/TextRenderComponent.h"

// Sets default values for this actor's properties
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// template function that creates a components
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	TileTextNumber = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TileTextNumber"));
	TileTextNumber->SetupAttachment(RootComponent);
	
	TileTextLetter = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TileTextLetter"));
	TileTextLetter->SetupAttachment(RootComponent);

	// every actor has a RootComponent that defines the transform in the World
	SetRootComponent(Scene);
	StaticMeshComponent->SetupAttachment(Scene);

	Status = ETileStatus::EMPTY;
	TileGameStatus = ETileGameStatus::FREE;
	bIsLegal = false;
	TileGridPosition = FVector2D(-1, -1);
	PlayerOwner = -1;
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

void ATile::SetTileStatus(const int32 TileOwner, const ETileStatus TileStatus, APiece* TilePiece)
{
	PlayerOwner = TileOwner;
	Status = TileStatus;
	Piece = TilePiece;
}

void ATile::SetTileGameStatus(const ETileGameStatus NewTileGameStatus)
{
	TileGameStatus = NewTileGameStatus;

	(NewTileGameStatus == ETileGameStatus::LEGAL_MOVE || NewTileGameStatus == ETileGameStatus::CAN_CAPTURE)
		? bIsLegal = true
		: bIsLegal = false;
	
	SetTileMaterial();
}

void ATile::SetGridPosition(const double InX, const double InY)
{
	TileGridPosition.Set(InX, InY);
}

void ATile::SetTileMaterial() const
{
	const FString MaterialPath = GetTileMaterialPath();

	UMaterialInterface* Material = LoadObject<UMaterialInterface>(
		nullptr,
		*MaterialPath
	);

	StaticMeshComponent->SetMaterial(0, Material);	
}

ETileStatus ATile::GetTileStatus() const
{
	return Status;
}

ETileGameStatus ATile::GetTileGameStatus() const
{
	return TileGameStatus;
}

bool ATile::IsLegalTile() const
{
	return bIsLegal;
}

FVector2D ATile::GetGridPosition() const
{
	return TileGridPosition;
}

int32 ATile::GetTileOwner() const
{
	return PlayerOwner;
}

APiece* ATile::GetPiece() const
{
	return Piece;
}

FString ATile::GetTileMaterialPath() const
{
	const FString MaterialPath = "/Game/Materials/MI_";

	if (TileGameStatus == ETileGameStatus::FREE)
	{
		const FVector2D Position = GetGridPosition();
		const int32 X = static_cast<int>(Position.X);
		const int32 Y = static_cast<int>(Position.Y);

		const FString Color = ((X + Y) % 2 == 0)
			                      ? "DARK"
			                      : "LIGHT";

		return MaterialPath + Color;
	}
	
	return MaterialPath + GameStatusToString();
}

FString ATile::GameStatusToString() const
{
	switch (TileGameStatus)
	{
	case ETileGameStatus::FREE:			return FString("FREE");
	case ETileGameStatus::SELECTED:		return FString("SELECTED");
	case ETileGameStatus::LEGAL_MOVE:	return FString("LEGAL_MOVE");
	case ETileGameStatus::CAN_CAPTURE:	return FString("CAN_CAPTURE");
	default: return FString("Unknown");
	}
}
