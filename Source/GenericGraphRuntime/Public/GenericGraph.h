#pragma once

#include "CoreMinimal.h"
#include "GenericGraphNode.h"
#include "GenericGraphEdge.h"
#include "GameplayTagContainer.h"
#include "GenericGraph.generated.h"

UCLASS(Blueprintable)
class GENERICGRAPHRUNTIME_API UGenericGraph : public UObject
{
	GENERATED_BODY()

public:
	UGenericGraph();
	virtual ~UGenericGraph();

	UPROPERTY(BlueprintReadWrite, Category = "GenericGraph", Meta = (ExposeOnSpawn=true))
	FString Name;

	UPROPERTY(EditDefaultsOnly, Category = "GenericGraph")
	TSubclassOf<UGenericGraphNode> NodeType;

	UPROPERTY(EditDefaultsOnly, Category = "GenericGraph")
	TSubclassOf<UGenericGraphEdge> EdgeType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GenericGraph")
	FGameplayTagContainer GraphTags;

	UPROPERTY(BlueprintReadOnly, Category = "GenericGraph")
	TArray<UGenericGraphNode*> RootNodes;

	UPROPERTY(BlueprintReadOnly, Category = "GenericGraph")
	TArray<UGenericGraphNode*> AllNodes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GenericGraph")
	bool bEdgeEnabled;

	UFUNCTION(BlueprintCallable, Category = "GenericGraph")
	void Print(bool ToConsole = true, bool ToScreen = true);

	UFUNCTION(BlueprintCallable, Category = "GenericGraph")
	int GetLevelNum() const;

	UFUNCTION(BlueprintCallable, Category = "GenericGraph")
	void GetNodesByLevel(int Level, TArray<UGenericGraphNode*>& Nodes);

	void ClearGraph();

	UFUNCTION(BlueprintCallable, Category = "GenericGraph")
	void SetupGraph(TSubclassOf<UGenericGraphNode> NNodeType, TSubclassOf<UGenericGraphEdge> NEdgeType, FGameplayTagContainer NGraphTags, bool EdgeEnabled = true);

	UFUNCTION(BlueprintCallable, Category = "GenericGraph")
	UGenericGraphNode* CreateGraphNode(TSubclassOf<UGenericGraphNode> NodeClass, FName NodeName, int32& NodeUID);
	
	UFUNCTION(BlueprintCallable, Category = "GenericGraph")
	UGenericGraphEdge* CreateGraphEdge(TSubclassOf<UGenericGraphEdge> EdgeClass, UGenericGraphNode* NodeA, UGenericGraphNode* NodeB, FName EdgeName);

	UFUNCTION(BlueprintCallable, Category = "GenericGraph")
	void AddRootNode(UGenericGraphNode* Node, int32& RootNodeUID);




#if WITH_EDITORONLY_DATA
	UPROPERTY()
	class UEdGraph* EdGraph;

	UPROPERTY(EditDefaultsOnly, Category = "GenericGraph_Editor")
	bool bCanRenameNode;
	
	UPROPERTY(EditDefaultsOnly, Category = "GenericGraph_Editor")
	bool bCanBeCyclical;
	
#endif
};
