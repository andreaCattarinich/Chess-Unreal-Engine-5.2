// Copyright © 2024 Andrea Cattarinich

#include "Chess_HumanPlayer.h"
#include "GameField.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
AChess_HumanPlayer::AChess_HumanPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Create a camera component
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	// Set the camera as RootComponent
	SetRootComponent(Camera);

	// Get the game instance reference
	GameInstance = Cast<UChess_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	// Default init values
	PlayerNumber = -1;
}

// Called when the game starts or when spawned
void AChess_HumanPlayer::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
}

// Called every frame
void AChess_HumanPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AChess_HumanPlayer::OnTurn()
{
	IsMyTurn = true;

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Your Turn"));
	GameInstance->SetTurnMessage(TEXT("Human Turn!"));
}

void AChess_HumanPlayer::OnWin()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("You Win!"));
	GameInstance->SetTurnMessage(TEXT("Human Wins!"));
	GameInstance->IncrementScoreHumanPlayer();
}

void AChess_HumanPlayer::OnLose()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You Lose!"));
	GameInstance->SetTurnMessage(TEXT("Human Loses!"));
}

void AChess_HumanPlayer::OnStalemate()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Stalemate!"));
	GameInstance->SetTurnMessage(TEXT("Stalemate!"));
}

// Called to bind functionality to input
void AChess_HumanPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AChess_HumanPlayer::OnClick()
{
	// Structure containing information about one hit of a trace, such as point of impact and surface normal at that point
	FHitResult Hit = FHitResult(ForceInit);

	// GetHitResultUnderCursor function sends a ray from the mouse position and gives the corresponding hit results
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit);
	
	if (!Hit.bBlockingHit || !IsMyTurn)
	{
		return;
	}
	
	// If human clicks on a Tile
	if (const ATile* ClickedTile = Cast<ATile>(Hit.GetActor()))
	{
		HandleTileClick(ClickedTile);
	}
	
	// If human clicks on a Piece
	else if (const APiece* ClickedPiece = Cast<APiece>(Hit.GetActor()))
	{
		HandlePieceClick(ClickedPiece);
	}
}

void AChess_HumanPlayer::HandleTileClick(const ATile* ClickedTile)
{
	// If human clicks on a Tile that belongs to him
	// => Set Tile selected (active)
	if (ClickedTile->GetTileOwner() == PlayerNumber)
	{
		const FVector2D Position = ClickedTile->GetGridPosition();
		GameMode->SetSelectedTile(Position);
	}

	// If the Tile clicked is legal
	// => Execute the move
	if (ClickedTile->IsLegalTile())
	{
		ExecuteTheMoveForHumanPlayer(ClickedTile);
	}
}

void AChess_HumanPlayer::HandlePieceClick(const APiece* ClickedPiece)
{
	// If the Piece clicked belong to the player
	if (ClickedPiece->GetPieceOwner() == PlayerNumber)
	{
		// If the Piece clicked is active
		// => De-activate active Tile
		if (ClickedPiece->GetGridPosition() == GameMode->GField->GetSelectedTile())
		{
			GameMode->GField->ResetGameStatusField();
		}
		else // Set Tile selected (active)
		{
			GameMode->SetSelectedTile(ClickedPiece->GetGridPosition());
		}
	}
	else
	{	// If the enemy piece is legal move
		// => Eat the piece
		const ATile* ClickedTile = *GameMode->GField->TileMap.Find(ClickedPiece->GetGridPosition());
		if (ClickedTile->IsLegalTile())
		{
			ExecuteTheMoveForHumanPlayer(ClickedTile);
		}
	}
}

void AChess_HumanPlayer::ExecuteTheMoveForHumanPlayer(const ATile* EndTile)
{
	IsMyTurn = false;
	GameMode->DoMove(EndTile->GetGridPosition(), true);
}