// DYL RPG with GAS


#include "UI/Widget/AuraUserWidget.h"

void UAuraUserWidget::SetWidgetCotroller(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}

void UAuraUserWidget::WidgetControllerSet()
{
}
