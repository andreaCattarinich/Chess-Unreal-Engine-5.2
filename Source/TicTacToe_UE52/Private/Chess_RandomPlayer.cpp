// Fill out your copyright notice in the Description page of Project Settings.

#include "..\Public\Chess_RandomPlayer.h"

// Sets default values
AChess_RandomPlayer::AChess_RandomPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GameInstance = Cast<UChess_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

// Called when the game starts or when spawned
void AChess_RandomPlayer::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<ATTT_GameMode>(GetWorld()->GetAuthGameMode());
}

// Called every frame
void AChess_RandomPlayer::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AChess_RandomPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AChess_RandomPlayer::OnTurn()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Random) Turn"));
	GameInstance->SetTurnMessage(TEXT("AI (Random) Turn"));
	
	// AI Random Decide the Move
	DecideMove(); 
}

void AChess_RandomPlayer::OnWin()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Random) Wins!"));
	GameInstance->SetTurnMessage(TEXT("AI Wins!"));
	GameInstance->IncrementScoreAiPlayer();
}

void AChess_RandomPlayer::OnLose()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Random) Loses!"));
	// GameInstance->SetTurnMessage(TEXT("AI Loses!"));
}

void AChess_RandomPlayer::DecideMove()
{
	FTimerHandle TimerHandle1;

	// Timer per eseguire SelectRandomPiece dopo 1 secondo
	GetWorld()->GetTimerManager().SetTimer(TimerHandle1, [this]()
		{
			SelectRandomPiece();

			if (GameMode->GField->GetLegalMoves().Num() == 0)
			{	// Se non esistono mosse valide per la pedina scelta casualmente,
				// allora ri-esegui il la decisione della mossa
				DecideMove();
			}
			else
			{
				FTimerHandle TimerHandle2;

				// Altrimenti effettua la Mossa Casuale dopo 2 secondi
				GetWorld()->GetTimerManager().SetTimer(TimerHandle2, [this]()
					{
						RandomMove();
					}, 1.0f, false);
			}
		}, 0.5f, false);
}

void AChess_RandomPlayer::SelectRandomPiece() const
{
	TArray<ATile*> AIPieces;

	for (ATile* CurrentTile : GameMode->GField->TileArray)
	{
		if (CurrentTile->GetOwner() == PlayerNumber)
		{
			AIPieces.Add(CurrentTile);
		}
	}

	if (AIPieces.Num() == 0)
	{
		return;
	}

	const int32 RandIdx = FMath::Rand() % AIPieces.Num();
	const FVector2D Position = FVector2D(
		(AIPieces[RandIdx])->GetGridPosition()[0],
		(AIPieces[RandIdx])->GetGridPosition()[1]
	);
	GameMode->SetSelectedTile(Position);
}

void AChess_RandomPlayer::RandomMove() const
{	
	// MOSSA CASUALE
	TArray<FVector2D> AILegalMoves = GameMode->GField->GetLegalMoves();

	const int32 RandIdx = FMath::Rand() % AILegalMoves.Num();
	const FVector2D Position = AILegalMoves[RandIdx];

	const ATile* RandomTile = *GameMode->GField->TileMap.Find(Position);
	if (RandomTile->IsLegalTile())
	{
		GameMode->DoMove(Position, true);
	}
}
