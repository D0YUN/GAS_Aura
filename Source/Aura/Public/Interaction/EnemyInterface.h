#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

class AURA_API IEnemyInterface
{
	GENERATED_BODY()

public:
	// 0을 붙이는 이유 : 순수 가상 함수임을 나타냄
	virtual void HighlightActor() = 0;
	virtual void UnHighlightActor() = 0;
};
