// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
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

public:
	// Sets default values for this pawn's properties
	ARobot();

	//URobotInstruction* Instruction;
	TQueue<FString> instructions;

	//Modifiable values
	float travel_distance;
	float speed;
	float rot_speed;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Queue Functions
	void Start_Queue(FString commands[]);
	void Execute_Queue();
	void Add_To_Queue();

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



	//Execution Functions
	UFUNCTION()
	void Move();

	UFUNCTION()
	void Turn_Left();

	UFUNCTION()
	void Turn_Right();
};