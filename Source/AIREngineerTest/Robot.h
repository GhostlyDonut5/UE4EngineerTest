// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Engine/StaticMeshActor.h"
#include "RobotInstruction.h"
#include "Robot.generated.h"

UCLASS()
class AIRENGINEERTEST_API ARobot : public APawn
{
	GENERATED_BODY()

	//Timer Handles
	FTimerHandle Handle;
	FTimerHandle Queue_Handle;

	//Temporary variables.
	FVector prev_loc;
	FVector prev_forward;
	float prev_rot;

	FQuat Look(FVector dest_point, FVector curr_loc);

public:
	// Sets default values for this pawn's properties
	ARobot();

	URobotInstruction* Instruction;
	TQueue<FString> instructions;

	//Queue Functions
	void Start_Queue();
	virtual void Execute_Instruction(TQueue<FString>* queue);

	//Timer Functions
	void Move_Timer();
	void Rotate(FString direction);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Static Mesh Components
	UStaticMeshComponent* Cube;
	UStaticMesh* Cube_Mesh;
	UStaticMesh* Sphere_Mesh;

	//Raycast Modifiable values
	FCollisionQueryParams* TraceParams;

	FString next_instruction;

	//Execution Functions
	void Move();
	void Turn_Left();
	void Turn_Right();
	void Look_At_Target(FVector target);
};