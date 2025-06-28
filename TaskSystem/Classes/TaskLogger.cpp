// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskLogger.h"
#include "QuestBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Core/Interfaces/InstancesInterface.h"
#include "Player/PlayerInterface.h"
#include "TaskSystem/Interfaces/QuestInterface.h"
// #include "pch.h"

// Sets default values for this component's properties
/* UTaskLogger::UTaskLogger()
{
	PrimaryComponentTick.bCanEverTick = true;
}*/


// Called when the game starts
void UTaskLogger::BeginPlay()
{
	Super::BeginPlay();	
}


// Called every frame
void UTaskLogger::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTaskLogger::CompleteQuest(FName QuestID, UQuestBase* QuestBaseReference)
{
	ReceivedQuests.Remove(QuestID);
	CompletedQuests.Add(QuestID);
	UGameInstance* LocalIns = UGameplayStatics::GetGameInstance(GetWorld());
	if (LocalIns)
	{
		IInstancesInterface::Execute_NewTaskNotification(LocalIns, QuestID, ETaskGiverType::Completed);
	}
	IPlayerInterface::Execute_AddMoney((UObject*)GetOwner(), QuestBaseReference->QuestDetails.MoneyReward);
	IPlayerInterface::Execute_AddAbilityPoints((UObject*)GetOwner(), QuestBaseReference->QuestDetails.AbilityPointsAmmound);
	if (!QuestBaseReference->QuestDetails.NextTask.IsNone())
	{
		AddNewQuest(QuestBaseReference->QuestDetails.NextTask);
		TrackQuest(QuestBaseReference->QuestDetails.NextTask);
	}
	else
	{
		AGameModeBase* CurrentGameMode = UGameplayStatics::GetGameMode(GetWorld());
		if (CurrentGameMode)
		{
			IQuestInterface::Execute_RefreshQuest((UObject*)CurrentGameMode, nullptr);
		}
	}
}

//Add New Quest Name and Spawn QuestBase & add this to "CurrentQuests"
void UTaskLogger::AddNewQuest(FName QuestID)
{
	if (!QuestID.IsNone())
	{
		ReceivedQuests.Add(QuestID);
		UQuestBase* NewQuest = NewObject<UQuestBase>(this, UQuestBase::StaticClass());
		if (NewQuest)
		{
			NewQuest->QuestID = QuestID;
			NewQuest->TaskLogger = this;
			CurrentQuests.Add(NewQuest);
			NewQuest->Initialization();
			UGameInstance* LocalINS = UGameplayStatics::GetGameInstance(GetWorld());
			{
				IInstancesInterface::Execute_NewTaskNotification(LocalINS, QuestID, ETaskGiverType::NewTask);
			}
		}
	}
}

//Проверяет по айдишнику квесты и активирует совпадающий, остальные дективирует
void UTaskLogger::TrackQuest(FName QuestID)
{
	if (ReceivedQuests.Contains(QuestID))
	{
		for (UQuestBase* CurrentElem : CurrentQuests)
		{
			if (CurrentElem)
			{
				if (CurrentElem->QuestID == QuestID)
				{
					CurrentElem->ActivateQuest();
					ActiveQuest = QuestID;
				}
				else
				{
					CurrentElem->DeactivateQuest();
				}
			}
		}

	}
}

//Return True if QuestID non found in Quest Arrays (Received & Completed)
// сделано, но надо убедиться в парвильности работы
bool UTaskLogger::GetActiveQuest(FName QuestID)
{
	if (ReceivedQuests.Contains(QuestID))
	{
		return false;
	}
	if (CompletedQuests.Contains(QuestID))
	{
		return false;
	}
	return true;
}

//Return QuestBase by FName QuestID
//надо протестировать
UQuestBase* UTaskLogger::FindQuestBase(FName QuestID)
{
	for(UQuestBase* CurrentElem : CurrentQuests)
	{
		if (CurrentElem && CurrentElem->QuestID == QuestID)
		{
			return CurrentElem;
		}

	}
	return nullptr;
}

FTaskSystemSave UTaskLogger::SaveQuestSystem()
{
	FTaskSystemSave LocalElem;
	LocalElem.ReceivedQuests = ReceivedQuests;
	LocalElem.CompletedQuests = CompletedQuests;
	LocalElem.ActiveQuest = ActiveQuest;
	for (UQuestBase* Elem : CurrentQuests)
	{
		if (Elem)
		{
			FQuestBaseSave ElemData;
			ElemData = Elem->GetSaveData();
			LocalElem.QuestBaseSaves.Add(ElemData);
		}
	}
	return LocalElem;
}

void UTaskLogger::LoadQuestSystem(FTaskSystemSave LoadedData)
{
	ActiveQuest = LoadedData.ActiveQuest;
	ReceivedQuests = LoadedData.ReceivedQuests;
	CompletedQuests = LoadedData.CompletedQuests;
	CurrentQuests.Empty();
	for (FQuestBaseSave Elem : LoadedData.QuestBaseSaves)
	{
		UQuestBase* NewQuest = NewObject<UQuestBase>(this, UQuestBase::StaticClass());
		if (NewQuest)
		{
			NewQuest->TaskLogger = this;
			NewQuest->SetLoadData(Elem);
		}
	}
}