// Fill out your copyright notice in the Description page of Project Settings.
#include "DrawDebugHelpers.h"
#include "Robot.h"


// --------------------Sets default values--------------------
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


//--------------------Called when the game starts or when spawned--------------------
void ARobot::BeginPlay()
{
	Super::BeginPlay();
	//Get reference to the current instance of the Robot instructions
	AAIREngineerTestGameModeBase* GameMode = Cast<AAIREngineerTestGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	Robot_Instruction = GameMode->RobotInstruction;

	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, 300.0f));

	instructions = new TQueue<FString>();

	Start_Queue();
}


//--------------------Queue Functions--------------------
void ARobot::Start_Queue()
{
	Execute_Instruction(instructions);

	GetWorld()->GetTimerManager().SetTimer(Queue_Handle, [this]()
	{
		if (!GetWorldTimerManager().IsTimerActive(Handle))
		{
			Execute_Instruction(instructions);
		}

	}, 1.0f, true);
}


void ARobot::Execute_Instruction(TQueue<FString>* queue)
{
	if (!queue->IsEmpty())
	{
		//Get, then check the next command in the queue. Then execute.
		if (queue->Dequeue(next_instruction))
		{
			if (next_instruction == "move")
			{
				Move_Timer();
			}
			
			if (next_instruction == "left" || next_instruction == "right")
			{
				Rotate(next_instruction);
			}
		}
	}

	else
	{
		GetWorldTimerManager().ClearTimer(Queue_Handle);
	}
}

//--------------------Functions that start the timers--------------------
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


//--------------------Actual Logic--------------------
void ARobot::Move()
{
	//Keep track of how long we've been moving
	timer++;
	if (timer >= 200.0f)
	{
		timer = 0;
		GetWorldTimerManager().ClearTimer(Handle);
	}

	//Raycast to check if about to collide with something.
	FHitResult* HitResult = new FHitResult();
	FVector Ray_Start = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector Ray_End = ((ForwardVector * 5000.0f) + Ray_Start);

	TraceParams = new FCollisionQueryParams();
	TraceParams->AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(*HitResult, Ray_Start, Ray_End, ECC_Visibility, *TraceParams))
	{
		//Checks to see if close to a wall, and stops.
		if (HitResult->Distance < (GetActorScale().X * 50) + 100)
		{
			timer = 0;
			GetWorldTimerManager().ClearTimer(Handle);
		}
		else
		{
			//Move actor if nothing in the way.
			SetActorLocation(FMath::Lerp(GetActorLocation(), GetActorLocation() + (GetActorForwardVector() * Robot_Instruction->travel_distance), Robot_Instruction->speed * 0.001f));
		}
	}

	//Check distance between destination and current location.
	if (FVector::Dist(GetActorLocation(), prev_loc + (prev_forward * Robot_Instruction->travel_distance)) <= 1.0f)
	{
		timer = 0;
		GetWorldTimerManager().ClearTimer(Handle);
	}
}


//--------------------Rotation Functions, need to be fixed--------------------
void ARobot::Turn_Left()
{
	//Need to find some way to lerp this for smoother turning. Currently Gimbal lock is happening.
		//SetActorRotation(FMath::Lerp(GetActorRotation(), QuatRotation.Rotator(), Robot_Instruction->rotation_speed * 0.01f));
		//FMath::Lerp(GetActorRotation(), QuatRotation.Rotator(), Robot_Instruction->rotation_speed * 0.01f);
	FQuat QuatRotation;

	QuatRotation = FRotator(0.0f, GetActorRotation().Pitch - 90.0f, 0.0f).Quaternion();

	AddActorLocalRotation(QuatRotation);

	if((GetActorRotation().Yaw < 0.0f && GetActorRotation().Yaw <= prev_rot - 90.0f) ||
		(GetActorRotation().Yaw > 0.0f && prev_rot - 90.0f <= GetActorRotation().Yaw))
	{
		GetWorldTimerManager().ClearTimer(Handle);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(Handle);
	}
}

void ARobot::Turn_Right()
{
	//Need to find some way to lerp this for smoother turning. Currently Gimbal lock is happening.
		//SetActorRotation(FMath::Lerp(GetActorRotation(), QuatRotation.Rotator(), Robot_Instruction->rotation_speed * 0.01f));
		//FMath::Lerp(GetActorRotation(), QuatRotation.Rotator(), Robot_Instruction->rotation_speed * 0.01f);
	FQuat QuatRotation = FRotator(0.0f, GetActorRotation().Pitch + 90.0f, 0.0f).Quaternion();

	AddActorLocalRotation(QuatRotation);

	if ((GetActorRotation().Yaw > 0.0f && GetActorRotation().Yaw >= prev_rot + 90.0f) ||
		(GetActorRotation().Yaw < 0.0f && prev_rot + 90.0f >= GetActorRotation().Yaw))
	{
		GetWorldTimerManager().ClearTimer(Handle);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(Handle);
	}
}