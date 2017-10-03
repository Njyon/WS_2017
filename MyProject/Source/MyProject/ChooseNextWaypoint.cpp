// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "AIController.h"
#include <PatrolRoute.h>


 EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
 {
	 //Get patrol points
	 AAIController* aIController = ownerComp.GetAIOwner();
	 APawn* controlledPawn = aIController->GetPawn();
	 UPatrolRoute* patrolRoute = controlledPawn->FindComponentByClass<UPatrolRoute>();

	 if (!ensure(patrolRoute)) 
	 { 
		 FString name = ownerComp.GetOwner()->GetName();
		 UE_LOG(LogTemp, Error, TEXT("[AI]Name = %s has no Patrol Route!"), *name);

		 return EBTNodeResult::Failed;
	 }

	 //Warn about empty Patrol routs
	 TArray<AActor*> patrolPoints = patrolRoute->GetPatrolPoints();
	 if (patrolPoints.Num() == 0)
	 {
		 FString name = ownerComp.GetOwner()->GetName();
		 UE_LOG(LogTemp, Error, TEXT("[AI]Name = %s has no Patrol Route Assigned! (Patroll Route = 0)"), *name);

		 return EBTNodeResult::Failed;
	 }

	 //Set next waypoint
	 UBlackboardComponent* blackboardComp = ownerComp.GetBlackboardComponent();
	 int32 index = blackboardComp->GetValueAsInt(indexKey.SelectedKeyName);

	 blackboardComp->SetValueAsObject(waypointKey.SelectedKeyName, patrolPoints[index]);

	 //Cycle to Next waypoint
	 int nextIndex = (index + 1) % patrolPoints.Num();
	 blackboardComp->SetValueAsInt(indexKey.SelectedKeyName, nextIndex);

	 UE_LOG(LogTemp, Warning, TEXT("Waypoint index: %i"), index);
	 return EBTNodeResult::Succeeded;
 }