#include "stdafx.h"
#include "Systems/include/System.h"

void System::HandleEvent(const Event& event, float deltaTime)
{
	// Default implementation does nothing so that only systems that actually need to handle events will override
}
