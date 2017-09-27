// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"


 EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
 {
	 auto blackboardComp = ownerComp.GetBlackboardComponent();
	 auto index = blackboardComp->GetValueAsInt(indexKey.SelectedKeyName);

	 UE_LOG(LogTemp, Warning, TEXT("Waypoint index: %i"), index);
	 return EBTNodeResult::Succeeded;
 }