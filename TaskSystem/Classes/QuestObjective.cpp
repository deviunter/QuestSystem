
#include "QuestObjective.h"
#include "TaskSystem/Interfaces/QuestInterface.h"

// Sets default values for this component's properties
UQuestObjective::UQuestObjective()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UQuestObjective::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UQuestObjective::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UQuestObjective::OnQuestInvolved(EObjectiveType InvolveType)
{
	if (IsValid(QuestBase))
	{
		switch (InvolveType)
		{
		case EObjectiveType::None:
			break;
		case EObjectiveType::LocEnter:
			if (ObjectiveDetails.ObjectiveType == InvolveType)
			{
				QuestBase->ObjectiveComplete(ObjectiveID, 1);
				IQuestInterface::Execute_DestroyTargetMark(GetOwner());
				return;
			}
			break;
		case EObjectiveType::LocLeave:
			if (ObjectiveDetails.ObjectiveType == InvolveType)
			{
				QuestBase->ObjectiveComplete(ObjectiveID, 1);
				IQuestInterface::Execute_DestroyTargetMark(GetOwner());
				return;
			}
			break;
		case EObjectiveType::Kill:
			if (ObjectiveDetails.ObjectiveType == InvolveType)
			{
				QuestBase->ObjectiveComplete(ObjectiveID, 1);
				IQuestInterface::Execute_DestroyTargetMark(GetOwner());
				return;
			}
			else
			{
				QuestBase->ObjectiveFailed(ObjectiveID, true);
				IQuestInterface::Execute_DestroyTargetMark(GetOwner());
				return;
			}
			break;
		case EObjectiveType::Stun:
			if (ObjectiveDetails.ObjectiveType == InvolveType)
			{
				QuestBase->ObjectiveComplete(ObjectiveID, 1);
				IQuestInterface::Execute_DestroyTargetMark(GetOwner());
				return;
			}
			else
			{
				QuestBase->ObjectiveFailed(ObjectiveID, true);
				IQuestInterface::Execute_DestroyTargetMark(GetOwner());
				return;
			}
			break;
		case EObjectiveType::Interact:
			if (ObjectiveDetails.ObjectiveType == InvolveType)
			{
				QuestBase->ObjectiveComplete(ObjectiveID, 1);
				IQuestInterface::Execute_DestroyTargetMark(GetOwner());
				return;
			}
			else
			{
				QuestBase->ObjectiveFailed(ObjectiveID, true);
				IQuestInterface::Execute_DestroyTargetMark(GetOwner());
				return;
			}
			break;
		case EObjectiveType::Dialogue:
			if (ObjectiveDetails.ObjectiveType == InvolveType)
			{
				QuestBase->ObjectiveComplete(ObjectiveID, 1);
				IQuestInterface::Execute_DestroyTargetMark(GetOwner());
				return;
			}
			else
			{
				QuestBase->ObjectiveFailed(ObjectiveID, true);
				IQuestInterface::Execute_DestroyTargetMark(GetOwner());
				return;
			}
			break;
		case EObjectiveType::LookAt:
			if (ObjectiveDetails.ObjectiveType == InvolveType)
			{
				QuestBase->ObjectiveComplete(ObjectiveID, 1);
				IQuestInterface::Execute_DestroyTargetMark(GetOwner());
				return;
			}
			else
			{
				QuestBase->ObjectiveIgnored(ObjectiveID, true);
				IQuestInterface::Execute_DestroyTargetMark(GetOwner());
				return;
			}
			break;
		default:
			break;
		}
	}
}

void UQuestObjective::ActivateObjective(FObjectiveDetails Objective, UQuestBase* QuestBaseReference)
{
	ObjectiveDetails = Objective;
	QuestBase = QuestBaseReference;
}

void UQuestObjective::DeactivateObjective()
{
}

void UQuestObjective::RefreshObjective(FObjectiveDetails Objective, UQuestBase* QuestBaseReference)
{
	ObjectiveDetails = Objective;
	QuestBase = QuestBaseReference;
}

