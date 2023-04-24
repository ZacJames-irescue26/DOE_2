// Fill out your copyright notice in the Description page of Project Settings.


#include "DOEPlayerController.h"
#include "SelectionActor.h"
#include "Net/UnrealNetwork.h"
#include "DOE_2/TestUnits/BasicCharacter.h"
#include "DOE_2/AIControllers/UnitController.h"
#include "DOE_2/Character/Overseer.h"
#include "DOE_2/PlayerState/DOEPlayerState.h"
#include "DOE_2/HUD/UnitOverlay.h"
#include "DOE_2/HUD/MoveIcon.h"
#include "DOE_2/HUD/AttackIcon.h"
#include "GameFramework/PlayerState.h"

ADOEPlayerController::ADOEPlayerController()
{
	ConstructorHelpers::FClassFinder<UUserWidget> UnitOverlayClass(TEXT("/Game/Blueprints/HUD/BP_UnitOverlay"));
	if(UnitOverlayClass.Class != nullptr)
	{
		UnitOverlayTemplate = UnitOverlayClass.Class;
	}

	ConstructorHelpers::FClassFinder<UUserWidget> MoveIconClass(TEXT("/Game/Blueprints/HUD/BP_MoveIcon"));
	if (MoveIconClass.Class != nullptr)
	{
		MoveIconTemplate = MoveIconClass.Class;
	}

	ConstructorHelpers::FClassFinder<UUserWidget> AttackIconClass(TEXT("/Game/Blueprints/HUD/BP_AttackIcon"));
	if (AttackIconClass.Class != nullptr)
	{
		AttackIconTemplate = AttackIconClass.Class;
	}
}

void ADOEPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADOEPlayerController, SelectedUnit);
}

void ADOEPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

}

AOverseer* ADOEPlayerController::GetOverseerer()
{
	if (overseerer == nullptr)
	{
		overseerer = Cast<AOverseer>(GetPawn());
	}
	return overseerer;
}

void ADOEPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &ADOEPlayerController::LeftMouseButtonDown);
	InputComponent->BindAction("LeftMouseButton", IE_Released, this, &ADOEPlayerController::LeftMouseButtonReleased);

	InputComponent->BindAction("RightClick", IE_Pressed, this, &ADOEPlayerController::RightClick);
	//InputComponent->BindAction("RightCLick", IE_Released, this, &ADOEPlayerController::);

}

void ADOEPlayerController::LeftMouseButtonDown()
{
	leftMouseButton = true;
	GetHitResultUnderCursor(ECC_Visibility, true, Hit);
	SelectUnit();
	
}

void ADOEPlayerController::LeftMouseButtonReleased()
{
	leftMouseButton = false;
}

void ADOEPlayerController::RightClick()
{
	MoveUnit(Hit.Location);

}


void ADOEPlayerController::MoveUnit_Implementation(FVector location)
{
	if (SelectedUnit != nullptr)
	{
		if (SelectedUnit->IsA(ABasicCharacter::StaticClass()))
		{
			ABasicCharacter* character = Cast<ABasicCharacter>(SelectedUnit);
			if (character == nullptr) return;
			
			AUnitController* Controller = Cast<AUnitController>(character->GetController());
			if (Controller)
			{
				playerState = GetPlayerState<ADOEPlayerState>();
				
				if (playerState == nullptr) return;
				if (character->GetPlayerOwner() == playerState->GetPlayerOwner())
				{
					Controller->MoveUnit(location);

				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Controller is nullptr"));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("selected unit is nullptr"));
	}
}

void ADOEPlayerController::CreateUnitUI(int UnitId)
{
	if(IsLocalController() && UnitOverlay)
	{
		UnitOverlay->RemoveAllChildren();
		switch (UnitId)
		{
		case 0:
			if (MoveIcon && AttackIcon)
			{
				UnitOverlay->AddIcon(MoveIcon);
				UnitOverlay->AddIcon(AttackIcon);
			}
			break;
		case 1:
			break;
		default:
			break;
		}
	}
}

void ADOEPlayerController::SelectUnit()
{
	SelectedUnit = Hit.GetActor();
	if (SelectedUnit != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected unit: %s"), *SelectedUnit->GetName());

		ServerSelectUnit(SelectedUnit);
		UnitUI();

	}

}

void ADOEPlayerController::UnitUI()
{
	if (SelectedUnit->IsA(ABasicCharacter::StaticClass()))
	{
		ABasicCharacter* BasicCharacter = Cast<ABasicCharacter>(SelectedUnit);
		if (BasicCharacter)
		{
			//playerState = playerState == nullptr ? GetPlayerState<ADOEPlayerState>() : playerState;

			//if (playerState == nullptr) return;
			//if (BasicCharacter->GetPlayerOwner() == playerState->GetPlayerOwner())
			{
				CreateUnitUI(BasicCharacter->GetUnitId());
			}
		}
	}
}

void ADOEPlayerController::ServerSelectUnit_Implementation(AActor* SelectingCharacter)
{
		SelectedUnit = SelectingCharacter;

}

void ADOEPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	GetHitResultUnderCursor(ECC_Visibility, true, Hit);
	MousePos = Hit.Location;
	//UE_LOG(LogTemp, Warning, TEXT("X: %f Y: %f Z: %f"), MousePos.X, MousePos.Y, MousePos.Z);
	if (playerState->GetMovingGrid())
	{
		if (bIsSpawnedMoving == false)
		{
			
		}
	}
}

void ADOEPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalController())
	{
		if (UnitOverlayTemplate != nullptr)
		{
			UnitOverlay = CreateWidget<UUnitOverlay>(this, UnitOverlayTemplate);
			UnitOverlay->AddToViewport();
			UnitOverlay->SetVisibility(ESlateVisibility::Visible);
		}
		if (MoveIconTemplate != nullptr)
		{
			MoveIcon = CreateWidget<UMoveIcon>(this, MoveIconTemplate);
		}
		if (AttackIconTemplate != nullptr)
		{
			AttackIcon = CreateWidget<UAttackIcon>(this, AttackIconTemplate);
		}
	}
}

