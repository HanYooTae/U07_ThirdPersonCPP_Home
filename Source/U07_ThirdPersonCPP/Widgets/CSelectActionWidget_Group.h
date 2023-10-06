#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CSelectActionWidget_Group.generated.h"

UCLASS()
class U07_THIRDPERSONCPP_API UCSelectActionWidget_Group : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable)
		void Pressed(FString InKey);

	UFUNCTION(BlueprintCallable)
		void Hover(FString InKey);

	UFUNCTION(BlueprintCallable)
		void Unhover(FString InKey);

protected:
	UPROPERTY(BlueprintReadOnly)
		TMap<FString, class UCSelectActionWidget_Icon*> IconWidgets;

private:
	UPROPERTY(meta = (BindWidget))
		class UGridPanel* Grid;
};
