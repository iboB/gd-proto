//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// Game states manager
#pragma once

#include "ExplicitSingleton.h"

class GameState;

class GameStateManager : public ExplicitSingleton<GameStateManager>
{
    DECLARE_EXPLICIT_SINGLETON(GameStateManager);
public:

    void setState(GameState* newState);

    bool processEvent(const SDL_Event& event); // returns true if any of the states returned true in handleEvent
    void updateStates(int dt);
    void drawFrame();

protected:
    GameState* m_currentState;
};
