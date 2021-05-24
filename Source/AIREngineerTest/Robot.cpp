// Fill out your copyright notice in the Description page of Project Settings.
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "AIREngineerTestGameModeBase.h"
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

	Start_Queue();
}


//--------------------Queue Functions--------------------
void ARobot::Start_Queue()
{
	//Get reference to the current instance of the Robot instructions
	AAIREngineerTestGameModeBase* GameMode = Cast<AAIREngineerTestGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	Instruction = GameMode->RobotInstruction;

	Execute_Instruction(&Instruction->instructions);

	GetWorld()->GetTimerManager().SetTimer(Queue_Handle, [this]()
	{
		if (!GetWorldTimerManager().IsTimerActive(Handle))
		{
			Execute_Instruction(&Instruction->instructions);

		}

	}, 1.0f, true);
}


void ARobot::Execute_Instruction(TQueue<FString>* queue)
{
	//Temporary variable to store current command.
	FString temp;

	if (!queue->IsEmpty())
	{
		//Get, then check the next command in the queue. Then execute.
		if (queue->Dequeue(temp))
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


//Actual Logic
void ARobot::Move()
{
	//Raycast to check if about to collide with something.
	FHitResult* HitResult = new FHitResult();
	FVector Ray_Start = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector Ray_End = ((ForwardVector * 5000.0f) + Ray_Start);

	//Set Trace Params, can modify this in child classes.
	TraceParams = new FCollisionQueryParams();
	TraceParams->AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(*HitResult, Ray_Start, Ray_End, ECC_Visibility, *TraceParams))
	{
		DrawDebugLine(GetWorld(), Ray_Start, Ray_End, FColor(255, 0, 0), true);

		if (HitResult->Distance < (GetActorScale().X * 50) + 100)
		{
			GetWorldTimerManager().ClearTimer(Handle);
		}
		else
		{
			//Move actor if nothing in the way.
			SetActorLocation(FMath::Lerp(GetActorLocation(), GetActorLocation() + (GetActorForwardVector() * Instruction->travel_distance), Instruction->speed * 0.001f));
		}
	}

	//Check distance between destination and current location.
	if (FVector::Dist(GetActorLocation(), prev_loc + (prev_forward * Instruction->travel_distance)) <= 0.1f)
	{
		GetWorldTimerManager().ClearTimer(Handle);
	}
}

void ARobot::Turn_Left()
{

	//Need to find some way to lerp this for smoother turning. Currently Gimbal lock is happening.
		//SetActorRotation(FMath::Lerp(GetActorRotation(), QuatRotation.Rotator(), Instruction->rotation_speed * 0.01f));
		//FMath::Lerp(GetActorRotation(), QuatRotation.Rotator(), Instruction->rotation_speed * 0.01f);
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
		//SetActorRotation(FMath::Lerp(GetActorRotation(), QuatRotation.Rotator(), Instruction->rotation_speed * 0.01f));
		//FMath::Lerp(GetActorRotation(), QuatRotation.Rotator(), Instruction->rotation_speed * 0.01f);
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

//Look Math for Looking at Sphere
FQuat ARobot::Look(FVector dest_point, FVector curr_loc)
{
	FVector v_to_normalize = dest_point - curr_loc;
	FVector cross_prod;
	float dot;

	if (v_to_normalize.Normalize())
	{
		cross_prod.X = ((curr_loc.ForwardVector.Y * v_to_normalize.Z) + (curr_loc.ForwardVector.Z * v_to_normalize.Y));
		cross_prod.Y = -((curr_loc.ForwardVector.X * v_to_normalize.Z) + (curr_loc.ForwardVector.Z * v_to_normalize.X));
		cross_prod.Z = ((curr_loc.ForwardVector.X * v_to_normalize.Y) + (curr_loc.ForwardVector.Y * v_to_normalize.X));

		dot = (curr_loc.ForwardVector.X * v_to_normalize.X) +
			(curr_loc.ForwardVector.Y * v_to_normalize.Y) +
			(curr_loc.ForwardVector.Z * v_to_normalize.Z);
	}

	FQuat q;
	q.X = cross_prod.X;
	q.Y = cross_prod.Y;
	q.Z = cross_prod.Z;
	q.W = dot + 1;
	q.Normalize();
	return q;
}


//Function to look at grabber's next target
void ARobot::Look_At_Target(FVector target)
{
	//More rotation math.
	FRotator curr_rot = GetActorRotation().Quaternion().Rotator();
	curr_rot.Quaternion() = Look(curr_rot.Vector(), target);
	FVector look_direction = target - GetActorLocation();

	SetActorRotation(FMath::Lerp(GetActorRotation().Quaternion().Rotator(), Look(curr_rot.Quaternion().Vector(), look_direction).Rotator(), Instruction->rotation_speed * 0.01f));
}