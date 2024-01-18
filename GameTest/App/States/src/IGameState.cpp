#include "stdafx.h"
#include "States/include/IGameState.h"
#include "States/include/LoadingState.h"
#include "Systems/include/InputHandler.h"
#include "Managers/include/SoundManager.h"
#include "Managers/include/GameManager.h"
using std::make_unique;

void IGameState::HandleButtonClick(EntityId buttonEntityId)
{
    InputHandler& inputHandler = InputHandler::GetInstance();

    if (!inputHandler.IsButtonClicked(buttonEntityId))
        return;

    SoundManager::GetInstance().PlaySoundFromFile(Helper::PATH_TO_BUTTON_CLICK);
    GameManager::GetInstance().ChangeState(make_unique<LoadingState>(GetStateEnum()));
}
