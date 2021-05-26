// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "AIREngineerTestGameModeBase.h"
#include "Engine/StaticMeshActor.h"
#include "RobotInstruction.h"
#include "Robot.generated.h"

UCLASS()
class AIRENGINEERTEST_API ARobot : public APawn
{
	GENERATED_BODY()

	//timer to track how long we've been moving.
	float timer;

	//Timer Handles
	FTimerHandle Queue_Handle;

	//Temporary variables.
	FVector prev_loc;
	FVector prev_forward;
	float prev_rot;


public:
	// Sets default values for this pawn's properties
	ARobot();

	//Used to store an instance of the queue as well as the Robot_Instruction UObject
	URobotInstruction* Robot_Instruction;
	TQueue<FString>* instructions;

	//Queue Functions
	virtual void Start_Queue();
	virtual void Execute_Instruction(TQueue<FString>* queue);

	//Timer Functions
	void Move_Timer();
	void Rotate(FString direction);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Static Mesh Components for Actor
	UStaticMeshComponent* Cube;
	UStaticMesh* Cube_Mesh;
	UStaticMesh* Sphere_Mesh;

	//Raycast Modifiable values
	FCollisionQueryParams* TraceParams;

	//Stores the next instruction in the Queue
	FString next_instruction;

	//The Handle for the timers used for movement.
	FTimerHandle Handle;

	//Actual Logic Functions
	void Move();
	void Turn_Left();
	void Turn_Right();
};