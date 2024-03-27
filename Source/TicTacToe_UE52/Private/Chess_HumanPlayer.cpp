// Fill out your copyright notice in the Description page of Project Settings.

//#include "..\Public\Chess_HumanPlayer.h"

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

	// create a camera component
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	//set the camera as RootComponent
	SetRootComponent(Camera);

	// get the game instance reference
	GameInstance = Cast<UChess_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	// default init values
	PlayerNumber = -1;
}

// Called when the game starts or when spawned
void AChess_HumanPlayer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AChess_HumanPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AChess_HumanPlayer::OnTurn()
{
	IsMyTurn = true;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Your Turn"));
	GameInstance->SetTurnMessage(TEXT("Human Turn!"));
}

void AChess_HumanPlayer::OnWin()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("You Win!"));
	GameInstance->SetTurnMessage(TEXT("Human Wins!"));
	GameInstance->IncrementScoreHumanPlayer();
}

void AChess_HumanPlayer::OnLose()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You Lose!"));
	GameInstance->SetTurnMessage(TEXT("Human Loses!"));
}

// Called to bind functionality to input
void AChess_HumanPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AChess_HumanPlayer::OnClick()
{
	//Structure containing information about one hit of a trace, such as point of impact and surface normal at that point
	FHitResult Hit = FHitResult(ForceInit);

	// GetHitResultUnderCursor function sends a ray from the mouse position and gives the corresponding hit results
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit);
	
	if (!Hit.bBlockingHit || !IsMyTurn)
	{
		return;
	}

	ATTT_GameMode* GameMode = Cast<ATTT_GameMode>(GetWorld()->GetAuthGameMode());

	// SE CLICCO SU UNA TILE
	if (ATile* ClickedTile = Cast<ATile>(Hit.GetActor()))
	{
		HandleTileClick(GameMode, ClickedTile);
	}
	// SE CLICCO SU UN PEZZO
	else if (APiece* ClickedPiece = Cast<APiece>(Hit.GetActor()))
	{
		HandlePieceClick(GameMode, ClickedPiece);
	}
}

void AChess_HumanPlayer::HandleTileClick(
	ATTT_GameMode* GameMode,
	ATile* ClickedTile
)
{
	// SE CLICCO SU UNA TILE CHE MI APPARTIENE
	// => Setto la tile come attiva (selezionata)
	if (ClickedTile->GetOwner() == PlayerNumber)
	{
		FVector2D Position = ClickedTile->GetGridPosition();
		GameMode->SetSelectedTile(Position);
	}

	// SE LA TILE CHE HO CLICCATO E' UNA MOSSA LEGALE
	// => Eseguo la mossa
	if (ClickedTile->IsLegalTile())
	{
		/******* ESEGUI LA MOSSA (solo spostamento) *******/
		ExecuteTheMoveForHumanPlayer(GameMode, ClickedTile);
	}
}

void AChess_HumanPlayer::HandlePieceClick(
	ATTT_GameMode* GameMode,
	APiece* ClickedPiece
)
{
	// SE IL PEZZO DELLO HUMAN PLAYER
	if (ClickedPiece->GetOwner() == PlayerNumber)
	{
		// SE IL PEZZO APPENA PREMUTO E' QUELLO ATTIVO
		// => Allora disattivo la Tile attiva (selezionata)
		if (ClickedPiece->GetGridPosition() == GameMode->GField->GetSelectedTile())
		{
			GameMode->GField->ResetGameStatusField();
		}
		else // Altrimenti setto la tile come attiva (selezionata)
		{
			GameMode->SetSelectedTile(ClickedPiece->GetGridPosition());
		}
	}
	else
	{	// SE IL PEZZO E' DELL'AVVERSARIO ED E' UNA MOSSA LEGALE
		// => Allora mangia la pedina avversaria
		ATile* ClickedTile = *GameMode->GField->TileMap.Find(ClickedPiece->GetGridPosition());
		if (ClickedTile->IsLegalTile())
		{
			/******* ESEGUI LA MOSSA (mangia una pedina) *******/
			ExecuteTheMoveForHumanPlayer(GameMode, ClickedTile);
		}
	}
}

void AChess_HumanPlayer::ExecuteTheMoveForHumanPlayer(
	ATTT_GameMode* GameMode,
	ATile* EndTile
)
{
	// TODO: rinominare questa funzione
	//GameMode->Gesture(PlayerNumber, EndTile, true);
	GameMode->DoMove(EndTile->GetGridPosition(), true);
	IsMyTurn = false;
}