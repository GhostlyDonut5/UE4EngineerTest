// Fill out your copyright notice in the Description page of Project Settings.


#include "Dropper.h"


ADropper::ADropper()
{
	Sphere_Mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'")).Object;
}

void ADropper::BeginPlay()
{
	Super::BeginPlay();

	instructions = &Robot_Instruction->dropper_instructions;

	Pool_Objects();
}

void ADropper::Execute_Instruction(TQueue<FString>* queue)
{
	Super::Execute_Instruction(queue);

	if (next_instruction == "drop")
	{
		Drop_Sphere();
	}
}

//--------------------Object Pooling for Spheres, storing in Linked List--------------------
void ADropper::Pool_Objects()
{
	init_amount = 10;
	int it = 0;
	
	//With Linked List
	temp_mesh = GetWorld()->SpawnActor<AStaticMeshActor>(FVector(0.0f,0.0f,0.0f), GetActorRotation(), SpawnInfo);
	temp_mesh->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
	temp_mesh->GetStaticMeshComponent()->SetStaticMesh(Sphere_Mesh);
	temp_mesh->Tags.Add("Grabber_Sphere");
	temp_mesh->SetActorHiddenInGame(true);

	Trail_List = TLinkedList<AStaticMeshActor*>(temp_mesh);
	TLinkedList<AStaticMeshActor*>* prev_node = &Trail_List;


	while (it < init_amount)
	{
		temp_mesh = GetWorld()->SpawnActor<AStaticMeshActor>(FVector(0.0f, 0.0f, 0.0f), GetActorRotation(), SpawnInfo);
		temp_mesh->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
		temp_mesh->GetStaticMeshComponent()->SetStaticMesh(Sphere_Mesh);
		temp_mesh->Tags.Add("Grabber_Sphere");
		temp_mesh->SetActorHiddenInGame(true);

		TLinkedList<AStaticMeshActor*>* const node_next = new TLinkedList<AStaticMeshActor*>(temp_mesh);
		node_next->LinkAfter(prev_node);
		prev_node = node_next;

		it++;
	}
}

//--------------------Getting Next Sphere in Linked List--------------------
AStaticMeshActor* ADropper::Get_Sphere()
{
	current = &Trail_List;
	next = current->GetNextLink();

	if (all_actors_exposed == false)
	{
		for (AStaticMeshActor* mesh : Trail_List)
		{
			if (next == nullptr)
			{
				all_actors_exposed = true;
				break;
			}

			if (current->GetNextLink() != nullptr)
			{
				current = next;
				next = current->GetNextLink();
			}

			if (mesh->IsHidden() && !all_actors_exposed)
			{
				return mesh;
			}
		}
	}

	return NULL;
}

//--------------------Dropping the Sphere--------------------
void ADropper::Drop_Sphere()
{
	//Linked List
	equi_distance_vector = -GetActorForwardVector() * (GetActorScale().X * 50) - 100;
	
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	temp_mesh = Get_Sphere();

	if (temp_mesh != NULL)
	{
		temp_mesh->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
		temp_mesh->SetActorLocation(GetActorLocation() + equi_distance_vector);
		temp_mesh->SetActorRotation(GetActorRotation());
		temp_mesh->SetActorLocation(FVector(temp_mesh->GetActorLocation().X, temp_mesh->GetActorLocation().Y, 300.0f));
		temp_mesh->SetHidden(false);
	}
}