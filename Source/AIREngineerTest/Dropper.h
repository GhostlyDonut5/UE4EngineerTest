// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Robot.h"
#include "Dropper.generated.h"

/**
 * 
 */
UCLASS()
class AIRENGINEERTEST_API ADropper : public ARobot
{
	GENERATED_BODY()

		UStaticMesh* Sphere_Mesh;
		ADropper();

		//Initializing Objects At Start
		void Pool_Objects();

		//Dropping Sphere Functionality
		void Drop_Sphere();

		//Getting Next Sphere in Pool
		AStaticMeshActor* Get_Sphere();

public:

	AActor* curr_compared_object_actor;
	
	//Storing Spheres in Linked List.
	TLinkedList<AStaticMeshActor*> Trail_List;
	
	//Temporaray values
	AStaticMeshActor* temp_mesh;
	FVector equi_distance_vector;
	FActorSpawnParameters SpawnInfo;
	FTimerHandle Dropper_Handle;
	bool all_actors_exposed;
	int init_amount;
	int i;

	TLinkedList<AStaticMeshActor*>* current;
	TLinkedList<AStaticMeshActor*>* next;


protected:

	virtual void BeginPlay() override;

	virtual void Execute_Instruction(TQueue<FString>* queue) override;
};
