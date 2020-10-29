#include "GenericGraph.h"
#include "GenericGraphRuntimePCH.h"
#include "Engine/Engine.h"

#define LOCTEXT_NAMESPACE "GenericGraph"

UGenericGraph::UGenericGraph()
{
	NodeType = UGenericGraphNode::StaticClass();
	EdgeType = UGenericGraphEdge::StaticClass();

	bEdgeEnabled = true;

#if WITH_EDITORONLY_DATA
	EdGraph = nullptr;

	bCanRenameNode = true;
#endif
}

UGenericGraph::~UGenericGraph()
{

}

void UGenericGraph::Print(bool ToConsole /*= true*/, bool ToScreen /*= true*/)
{
	int Level = 0;
	TArray<UGenericGraphNode*> CurrLevelNodes = RootNodes;
	TArray<UGenericGraphNode*> NextLevelNodes;

	while (CurrLevelNodes.Num() != 0)
	{
		for (int i = 0; i < CurrLevelNodes.Num(); ++i)
		{
			UGenericGraphNode* Node = CurrLevelNodes[i];
			check(Node != nullptr);

			FString Message = FString::Printf(TEXT("%s, Level %d"), *Node->GetDescription().ToString(), Level);

			if (ToConsole)
			{
				LOG_INFO(TEXT("%s"), *Message);
			}

			if (ToScreen && GEngine != nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, Message);
			}

			for (int j = 0; j < Node->ChildrenNodes.Num(); ++j)
			{
				NextLevelNodes.Add(Node->ChildrenNodes[j]);
			}
		}

		CurrLevelNodes = NextLevelNodes;
		NextLevelNodes.Reset();
		++Level;
	}
}

int UGenericGraph::GetLevelNum() const
{
	int Level = 0;
	TArray<UGenericGraphNode*> CurrLevelNodes = RootNodes;
	TArray<UGenericGraphNode*> NextLevelNodes;

	while (CurrLevelNodes.Num() != 0)
	{
		for (int i = 0; i < CurrLevelNodes.Num(); ++i)
		{
			UGenericGraphNode* Node = CurrLevelNodes[i];
			check(Node != nullptr);

			for (int j = 0; j < Node->ChildrenNodes.Num(); ++j)
			{
				NextLevelNodes.Add(Node->ChildrenNodes[j]);
			}
		}

		CurrLevelNodes = NextLevelNodes;
		NextLevelNodes.Reset();
		++Level;
	}

	return Level;
}

void UGenericGraph::GetNodesByLevel(int Level, TArray<UGenericGraphNode*>& Nodes)
{
	int CurrLEvel = 0;
	TArray<UGenericGraphNode*> NextLevelNodes;

	Nodes = RootNodes;

	while (Nodes.Num() != 0)
	{
		if (CurrLEvel == Level)
			break;

		for (int i = 0; i < Nodes.Num(); ++i)
		{
			UGenericGraphNode* Node = Nodes[i];
			check(Node != nullptr);

			for (int j = 0; j < Node->ChildrenNodes.Num(); ++j)
			{
				NextLevelNodes.Add(Node->ChildrenNodes[j]);
			}
		}

		Nodes = NextLevelNodes;
		NextLevelNodes.Reset();
		++CurrLEvel;
	}
}

void UGenericGraph::ClearGraph()
{
	for (int i = 0; i < AllNodes.Num(); ++i)
	{
		UGenericGraphNode* Node = AllNodes[i];

		Node->ParentNodes.Empty();
		Node->ChildrenNodes.Empty();
		Node->Edges.Empty();
	}

	AllNodes.Empty();
	RootNodes.Empty();
}

void UGenericGraph::SetupGraph(TSubclassOf<UGenericGraphNode> NNodeType, TSubclassOf<UGenericGraphEdge> NEdgeType, FGameplayTagContainer NGraphTags, bool EdgeEnabled)
{
	NodeType = NNodeType;
	EdgeType = NEdgeType;
	GraphTags = NGraphTags;
	bEdgeEnabled = EdgeEnabled;
}

UGenericGraphNode* UGenericGraph::CreateGraphNode(TSubclassOf<UGenericGraphNode> NodeClass, FName NodeName, int32 &NodeUID)
{
	UGenericGraphNode* NewNode = NewObject<UGenericGraphNode>(this,NodeClass,NodeName);
	if (NewNode) {
		//setup node
		NewNode->Graph = this;
		

		//add node to list and store UID
		NodeUID = AllNodes.Add(NewNode);
		return NewNode;
	} 
	else
	{
		NodeUID = -1;
		return nullptr;
	}
}

UGenericGraphEdge* UGenericGraph::CreateGraphEdge(TSubclassOf<UGenericGraphEdge> EdgeClass, UGenericGraphNode* NodeA, UGenericGraphNode* NodeB, FName EdgeName)
{
	UGenericGraphEdge* NewEdge = NewObject<UGenericGraphEdge>(NodeA, EdgeClass, EdgeName);
	if (NewEdge) {
		//Setup Edge
		NewEdge->Graph = this;
		NewEdge->StartNode = NodeA;
		NewEdge->EndNode = NodeB;

		//set child parent relation between nodes
		NodeA->ChildrenNodes.Add(NodeB);
		NodeB->ParentNodes.Add(NodeA);

		//Add Edge to NodeA pointing to NodeB
		NodeA->Edges.Add(NodeB, NewEdge);
		return NewEdge;
	}
	else
	{
		return nullptr;
	}
}

void UGenericGraph::AddRootNode(UGenericGraphNode* Node, int32& RootNodeUID)
{
	if (Node) {
		RootNodeUID = RootNodes.Add(Node);
	}
}

#undef LOCTEXT_NAMESPACE
