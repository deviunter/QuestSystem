//govno
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UObject/Object.h"
#include "TaskSystem/Structures/TaskSystemSave.h"
#include "TaskLogger.generated.h"

class UQuestBase;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AFTERDARKNESS_API UTaskLogger : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Exec, Category = "Default")
	void CompleteQuest(FName QuestID, UQuestBase* QuestBaseReference);

	UFUNCTION(BlueprintCallable, Exec, Category = "Default")
	void AddNewQuest(FName QuestID);

	UFUNCTION(BlueprintCallable, Exec, Category = "Default")
	void TrackQuest(FName QuestID);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Default")
	bool GetActiveQuest(FName QuestID);

	UFUNCTION(BlueprintCallable, Category = "Default")
	UQuestBase* FindQuestBase(FName QuestID);

	UFUNCTION(BlueprintCallable, Category = "Default")
	FTaskSystemSave SaveQuestSystem();

	UFUNCTION(BlueprintCallable, Category = "Default")
	void LoadQuestSystem(FTaskSystemSave LoadedData);
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	TArray<FName> ReceivedQuests;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	TArray<FName> CompletedQuests;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	TArray<UQuestBase*> CurrentQuests;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	FName ActiveQuest;

	UPROPERTY(BlueprintReadOnly, Category = "Default")
	FTimerHandle UpdateDelay;
};