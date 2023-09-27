// Copyright Telephone Studios. All Rights Reserved.


#include "Decorators/BTDecorator_HasRecentlyDamaged.h"

void UBTDecorator_HasRecentlyDamaged::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	Super::OnNodeActivation(SearchData);

	// TODO bind delegate
}

void UBTDecorator_HasRecentlyDamaged::OnNodeProcessed(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type& NodeResult)
{
	Super::OnNodeProcessed(SearchData, NodeResult);

}
