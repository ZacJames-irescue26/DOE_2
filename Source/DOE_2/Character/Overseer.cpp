// Fill out your copyright notice in the Description page of Project Settings.


#include "Overseer.h"
#include "DOE_2/PlayerController/DOEPlayerController.h"
#include "DOE_2/PlayerController/SelectionActor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include <Kismet/GameplayStatics.h>
#include "DOE_2/HexGrid/HexGridManager.h"
#include "DOE_2/HexGrid/HexTile.h"
#include "DrawDebugHelpers.h"
// Sets default values
AOverseer::AOverseer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->MaxWalkSpeed = 2000;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.0f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}



void AOverseer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
}


// Called when the game starts or when spawned
void AOverseer::BeginPlay()
{
	Super::BeginPlay();
	SpawnSelectionArea();
	HexManager = Cast<AHexGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AHexGridManager::StaticClass()));

}

// Called every frame
void AOverseer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PlayerController = PlayerController == nullptr ? Cast<ADOEPlayerController>(Controller) : PlayerController;
	if (SelectionArea)
	{
		FVector MousePos = PlayerController->GetMousePos();

		AHexTile* Tile = Cast<AHexTile>(PlayerController->Hit.GetActor());
		if (Tile)
		{
			FVector2D Round = HexManager->Axial_Round(Tile);
			FVector2D Axial_Neighbour = HexManager->Axial_Neighbor(Tile,0);
			//UE_LOG(LogTemp, Warning, TEXT("%f, %f"), Axial_Neighbour.X, Axial_Neighbour.Y);
		}
		FVector2D mouse = HexManager->SpaceToHexTile(FVector2D(PlayerController->GetMousePos().X, PlayerController->GetMousePos().Y));
		//UE_LOG(LogTemp, Warning, TEXT("SpaceToHexTile %f, %f"), mouse.X, mouse.Y);
		FVector2D Hex_To_pixel = HexManager->Hex_To_Pixel(mouse.X, mouse.Y);
		//UE_LOG(LogTemp, Warning, TEXT("Hex_ToPixel %f, %f"), Hex_To_pixel.X, Hex_To_pixel.Y);

		SelectionArea->SetActorLocation(FVector(Hex_To_pixel.X, Hex_To_pixel.Y, MousePos.Z));
		//FVector2D Round = HexManager->Axial_Round(FVector2D(PlayerController->GetMousePos().X, PlayerController->GetMousePos().Y));
		
		if(Tile)
		{
			//HexManager->A_star_Search(HexManager->Hash_Table_Lookup(0, 0), HexManager->Hash_Table_Lookup(2, 3), Path);
			FVector2D MouseQR = HexManager->SpaceToHexTile(FVector2D(MousePos.X, MousePos.Y));
			{
				FVector2D intMouseQR;
				intMouseQR.X = (int)(MouseQR.X);
				intMouseQR.Y = (int)(MouseQR.Y);

				HexManager->A_star_Search(HexManager->Hash_Table_Lookup(intMouseQR.X, intMouseQR.Y), HexManager->Hash_Table_Lookup(2, 3), Path);

				for (int i = 0; i < Path.Num() - 1; i++)
				{
					if(Path[i] == nullptr) break;
					FVector Start = Path[i]->GetActorLocation();
					Start.Z += 50;
					FVector End = Path[i + 1]->GetActorLocation();
					End.Z += 50;
					DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0, 0, 10.0f);
				}
				int Distance = HexManager->Axial_Distance(Tile, HexManager->Hash_Table_Lookup(2, 3));
				if (!bIsSpawned)
				{
					AHexTile* TestTile = HexManager->Hash_Table_Lookup(2, 3);
					for (int i = (- 2); i <= 2; i++)
					{
						for (int j = FMath::Max(-2, -i-2); j <= FMath::Min(2, -i+2); j++)
						{
							FVector2D Hex_To_pixel1 = HexManager->Hex_To_Pixel(TestTile->q+i, TestTile->r+j);
							ASelectionActor* Area = GetWorld()->SpawnActor<ASelectionActor>(SelectionAreaClass, FVector(Hex_To_pixel1.X, Hex_To_pixel1.Y, 10.0f), FRotator::ZeroRotator);
						}
					}
					bIsSpawned = true;
				}
				//UE_LOG(LogTemp, Warning, TEXT("Distance From 2,3: %i"), Distance);

			}
		}
	}
}

void AOverseer::SpawnSelectionArea()
{
	if (IsLocallyControlled())
	{
		if (SelectionAreaClass)
		{
			SelectionArea = GetWorld()->SpawnActor<ASelectionActor>(SelectionAreaClass, FVector(0.0f, 0.0f, 0.0f), FRotator::ZeroRotator);
		}

	}
}

void AOverseer::SpawnArea()
{

}


