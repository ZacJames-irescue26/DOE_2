// Fill out your copyright notice in the Description page of Project Settings.


#include "DOEPlayerController.h"
#include "SelectionActor.h"
#include "Net/UnrealNetwork.h"
#include "DOE_2/TestUnits/BasicCharacter.h"
#include "DOE_2/AIControllers/UnitController.h"
#include "DOE_2/Character/Overseer.h"
#include "DOE_2/PlayerState/DOEPlayerState.h"
#include "GameFramework/PlayerState.h"

//ADOEPlayerController::ADOEPlayerController()
//{
//
//}

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
void ADOEPlayerController::SelectUnit()
{
	SelectedUnit = Hit.GetActor();
	if (SelectedUnit != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected unit: %s"), *SelectedUnit->GetName());
		ServerSelectUnit(SelectedUnit);

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
	
}

void ADOEPlayerController::BeginPlay()
{
}

