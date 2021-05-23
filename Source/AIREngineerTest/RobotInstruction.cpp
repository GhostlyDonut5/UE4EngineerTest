// Copyright Epic Games, Inc. All Rights Reserved.

#include "RobotInstruction.h"

URobotInstruction::URobotInstruction()
{
	
}

bool URobotInstruction::ExecuteInstruction()
{
	//Get from a text file to execute specific instructions, populate a set of instructions here.

	FString test_list[10];

	test_list[0] = "move";
	test_list[1] = "left";
	test_list[2] = "right";
	test_list[3] = "left";
	test_list[4] = "move";

	//Once list is populated call each function corresponding with command to add to the robot queue.

	//Problem: How to let designers edit the values of the speed and distance traveled.

	return true;
}



void URobotInstruction::Move(float distance, float speed)
{
	//Add to queue for Robot to ingest
	Robot->travel_distance = distance;
	Robot->speed = speed;

	Robot->Move_Timer();
}

FRotator URobotInstruction::Rotate_Left(FRotator rotation, float rot_speed)
{
	FQuat QuatRotation = FRotator(0.0f, rotation.Pitch - 90.0f, 0.0f).Quaternion();

	return FMath::Lerp(rotation, QuatRotation.Rotator(), rot_speed);
}

FRotator URobotInstruction::Rotate_Right(FRotator rotation, float rot_speed)
{
	FQuat QuatRotation = FRotator(0.0f, rotation.Pitch - 90.0f, 0.0f).Quaternion();

	return FMath::Lerp(rotation, QuatRotation.Rotator(), rot_speed);
}


FQuat URobotInstruction::Look(FVector dest_point, FVector curr_loc)
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

FRotator URobotInstruction::Look_At_Target(FRotator rotation, FVector position, FVector target, float rot_speed)
{
	//More rotation math.
	FRotator curr_rot = rotation.Quaternion().Rotator();
	curr_rot.Quaternion() = Look(curr_rot.Vector(), target);
	FVector look_direction = target - position;

	return FMath::Lerp(rotation.Quaternion().Rotator(), Look(curr_rot.Quaternion().Vector(), look_direction).Rotator(), rot_speed);
}