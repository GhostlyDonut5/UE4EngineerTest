// Fill out your copyright notice in the Description page of Project Settings.
#include "DrawDebugHelpers.h"
#include "Robot.h"


// Sets default values
ARobot::ARobot()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	Cube_Mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;
	Sphere_Mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'")).Object;
	Cube->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Cube->SetStaticMesh(Cube_Mesh);
	Cube->SetHiddenInGame(false);
}


// Called when the game starts or when spawned
void ARobot::BeginPlay()
{
	Super::BeginPlay();

	FString test_list[10];

	test_list[0] = "move";
	test_list[1] = "left";
	test_list[2] = "right";
	test_list[3] = "left";
	test_list[4] = "move";

	Start_Queue(test_list);
}



// Called every frame
void ARobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//Queue Functions

void ARobot::Start_Queue(FString commands[])
{
	for (int i = 0; i <= commands->Len(); ++i)
	{
		instructions.Enqueue(commands[i]);
	}

	Execute_Queue();

	GetWorld()->GetTimerManager().SetTimer(Queue_Handle, [this]()
	{
		if (!GetWorldTimerManager().IsTimerActive(Handle))
		{
			Execute_Queue();
		}

	}, 1.0f, true);
}


void ARobot::Execute_Queue()
{
	//Temporary variable to store current command.
	FString temp;


	if (!instructions.IsEmpty())
	{
		//Get, then check the next command in the queue. Then execute.
		if (instructions.Dequeue(temp))
		{
			UE_LOG(LogTemp, Warning, TEXT("Currently dequeuing: %s"), *temp);

			if (temp == "move")
			{
				Move_Timer();
			}
			
			if (temp == "left" || temp == "right")
			{
				Rotate(temp);
			}
			
		}
	}

	else
	{
		GetWorldTimerManager().ClearTimer(Queue_Handle);
	}
}

//Functions that start the timers.
 void ARobot::Move_Timer()
{
	prev_loc = GetActorLocation();
	prev_forward = GetActorForwardVector();

	GetWorldTimerManager().SetTimer(Handle, this, &ARobot::Move, FApp::GetDeltaTime(), true, 0.0f);
}

void ARobot::Rotate(FString direction)
{
	prev_rot = GetActorRotation().Yaw;

	if (direction == "left")
	{
		GetWorldTimerManager().SetTimer(Handle, this, &ARobot::Turn_Left, FApp::GetDeltaTime(), true, 0.0f);
	}
	if (direction == "right")
	{
		GetWorldTimerManager().SetTimer(Handle, this, &ARobot::Turn_Right, FApp::GetDeltaTime(), true, 0.0f);
	}
}


//Actual functions
void ARobot::Move()
{
	//For blueprint set variable
	//float dist = Instruction->set_distance;

	//Raycast to check if about to collide with something.
	FHitResult* HitResult = new FHitResult();
	FVector Start = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector End = ((ForwardVector * 5000.0f) + Start);

	//Set Trace Params, can modify this in child classes.
	TraceParams = new FCollisionQueryParams();
	TraceParams->AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(*HitResult, Start, End, ECC_Visibility, *TraceParams))
	{
		DrawDebugLine(GetWorld(), Start, End, FColor(255, 0, 0), true);

		if (HitResult->Distance < (GetActorScale().X * 50) + 100)
		{
			GetWorldTimerManager().ClearTimer(Handle);
		}
	}

	SetActorLocation(FMath::Lerp(GetActorLocation(), GetActorLocation() + (GetActorForwardVector() * travel_distance), speed));

	if (FVector::Dist(GetActorLocation(), prev_loc + (prev_forward * travel_distance)) <= 0.1f)
	{
		GetWorldTimerManager().ClearTimer(Handle);
	}
}

void ARobot::Turn_Left()
{
	FQuat QuatRotation = FRotator(0.0f, GetActorRotation().Pitch - 90.0f, 0.0f).Quaternion();

	SetActorRotation(FMath::Lerp(GetActorRotation(), QuatRotation.Rotator(), 0.05f));

	if(GetActorRotation().Yaw <= prev_rot - 89.0f)
	{
		GetWorldTimerManager().ClearTimer(Handle);
	}	
}

void ARobot::Turn_Right()
{
	FQuat QuatRotation = FRotator(0.0f, GetActorRotation().Pitch + 90.0f, 0.0f).Quaternion();

	SetActorRotation(FMath::Lerp(GetActorRotation(), QuatRotation.Rotator(), 0.05f));

	if (GetActorRotation().Yaw >= prev_rot + 90.0f)
	{
		GetWorldTimerManager().ClearTimer(Handle);
	}
}