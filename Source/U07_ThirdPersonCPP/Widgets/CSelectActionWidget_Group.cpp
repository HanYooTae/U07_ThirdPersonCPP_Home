#include "CSelectActionWidget_Group.h"
#include "Global.h"
#include "Components/GridPanel.h"
#include "Components/Border.h"
#include "CSelectActionWidget_Icon.h"

void UCSelectActionWidget_Group::NativeConstruct()
{
	Super::NativeConstruct();

	// Add Item Widget to Map(with string key)
	CheckNull(Grid);
	TArray<UWidget*> children = Grid->GetAllChildren();

	// child->GetName() ==> Icon1, 2, 3.... Map의 Key로 사용
	for (const auto& child : children)
	{
		UCSelectActionWidget_Icon* iconWidget = Cast<UCSelectActionWidget_Icon>(child);

		if (!!iconWidget)
			IconWidgets.Add(child->GetName(), iconWidget);
	}
}

void UCSelectActionWidget_Group::Pressed(FString InKey)
{
	CLog::Print(InKey + " is Pressed");
}

void UCSelectActionWidget_Group::Hover(FString InKey)
{
	// BorderColor => Red
	UBorder* border = Cast<UBorder>(IconWidgets[InKey]->GetWidgetFromName("MyBorder"));		// == IconWidgets.Find(InKey);
	CheckNull(border);
	border->SetBrushColor(FLinearColor::Red);
}

void UCSelectActionWidget_Group::Unhover(FString InKey)
{
	// BorderColor => White
	UBorder* border = Cast<UBorder>(IconWidgets[InKey]->GetWidgetFromName("MyBorder"));
	CheckNull(border);
	border->SetBrushColor(FLinearColor::White);
}