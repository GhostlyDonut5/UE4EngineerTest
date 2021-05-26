// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/Engine.h"
#include "Camera/PlayerCameraManager.h"
#include "Grabber.h"

AGrabber::AGrabber()
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	bUseControllerRotationYaw = false;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachTo(RootComponent);
	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 40.0f));
}

void AGrabber::BeginPlay()
{
	Super::BeginPlay();
	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 40.0f));

	instructions = &Robot_Instruction->grabber_instructions;
}

void AGrabber::Execute_Instruction(TQueue<FString>* queue)
{
	Super::Execute_Instruction(queue);

	if (next_instruction == "grab")
	{
		Find_Spheres();

		if (Dropper_Spheres.Num() > 0)
		{
			Find_Closest_Sphere();
			GetWorldTimerManager().SetTimer(Grabber_Handle, this, &AGrabber::Grab_Sphere, FApp::GetDeltaTime(), true, 0.0f);
		}
	}
}
//--------------------Populate list of spheres--------------------
void AGrabber::Find_Spheres()
{
	Dropper_Spheres.Empty();
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Grabber_Sphere", Dropper_Spheres);
}

//--------------------Find closest spheres from list of spheres--------------------
void AGrabber::Find_Closest_Sphere()
{
	//In Progress
	AActor* closest_sphere = Dropper_Spheres[0];
	AActor* temp = closest_sphere;

	for (int i = 0; i < Dropper_Spheres.Num(); ++i)
	{
		if (!Dropper_Spheres[i]->IsHidden())
		{
			if (FVector::Dist(GetActorLocation(), Dropper_Spheres[i]->GetActorLocation()) <
				FVector::Dist(GetActorLocation(), temp->GetActorLocation()))
			{
				closest_sphere = Dropper_Spheres[i];
			}

			temp = Dropper_Spheres[i];
		}
	}

	Closest_Sphere = closest_sphere;
}

//--------------------Grabbing Sphere Timer--------------------
void AGrabber::Grab_Sphere()
{
	if (Closest_Sphere->IsHidden())
	{
		GetWorldTimerManager().ClearTimer(Grabber_Handle);
	}

	if (FVector::Dist(GetActorLocation(), Closest_Sphere->GetActorLocation()) < (GetActorScale().X * 50) + 200)
	{
		spheres_documented++;
		Take_Screenshot();
		Get_All_Names();

		GetWorld()->DestroyActor(Closest_Sphere);
		GetWorldTimerManager().ClearTimer(Grabber_Handle);
	}

	Look_At_Target(Closest_Sphere->GetActorLocation());
	Move();
}

//--------------------Take a screenshot of sphere--------------------

void AGrabber::Take_Screenshot()
{
	FString fileName(FPaths::ProjectContentDir() + "Sphere_" + FString::FromInt(spheres_documented) + ".png");
	FScreenshotRequest::RequestScreenshot(fileName, false, false);
}

//--------------------Get all names of actors in viewport--------------------

/*
Used the code from this link: 
https://stackoverflow.com/questions/32082239/ue4-is-there-a-method-of-getting-all-actors-in-camera-view
*/
void AGrabber::Get_All_Names()
{
	UWorld* World = GetWorld();

	APlayerCameraManager* Manager = World->GetFirstPlayerController()->PlayerCameraManager;
	ULocalPlayer* LocalPlayer = World->GetFirstLocalPlayerFromController();

	FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(
		LocalPlayer->ViewportClient->Viewport,
		World->Scene,
		LocalPlayer->ViewportClient->EngineShowFlags)
		.SetRealtimeUpdate(true));

	FVector ViewLocation;
	FRotator ViewRotation;
	FSceneView* SceneView = LocalPlayer->CalcSceneView(&ViewFamily, /*out*/ ViewLocation, /*out*/ ViewRotation, LocalPlayer->ViewportClient->Viewport);
	Viewport_Actors.Empty();

	for (TObjectIterator<UPrimitiveComponent> ScenePrimitsItr; ScenePrimitsItr; ++ScenePrimitsItr)
	{
		// Using the Object iterator so we have to skip objects not in the 
		// relevant world
		if (ScenePrimitsItr->GetWorld() == World)
		{
			FPrimitiveSceneProxy* Proxy = ScenePrimitsItr->SceneProxy;
			if (Proxy)
			{
				bool bIsShown =
					SceneView->ViewFrustum.IntersectSphere(Proxy->GetBounds().Origin, Proxy->GetBounds().SphereRadius) ||
					SceneView->ViewFrustum.IntersectBox(Proxy->GetBounds().Origin, Proxy->GetBounds().BoxExtent);

				if (bIsShown)
				{
					Viewport_Actors.Add(Proxy->GetOwnerName().ToString());
				}
			}
		}
	}

	FString Dir(FPaths::ProjectContentDir() + "Viewport_Actors_" + FString::FromInt(spheres_documented) + ".txt");
	FFileHelper::SaveStringArrayToFile(Viewport_Actors, *Dir);
}

//--------------------Look Math for Looking at Sphere--------------------
FQuat AGrabber::Look(FVector dest_point, FVector curr_loc)
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


//--------------------Function to look at grabber's next target--------------------
void AGrabber::Look_At_Target(FVector target)
{
	//More rotation math.
	FRotator curr_rot = GetActorRotation().Quaternion().Rotator();
	curr_rot.Quaternion() = Look(target, curr_rot.Vector());
	FVector look_direction = target - GetActorLocation();

	SetActorRotation(FMath::Lerp(GetActorRotation().Quaternion().Rotator(), Look(look_direction ,curr_rot.Quaternion().Vector()).Rotator(), Robot_Instruction->rotation_speed * 0.01f));
}