//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// Game states manager
#include "GDProto.pch.h"
#include "GameStateManager.h"

#include "GameState.h"

GameStateManager::GameStateManager()
    : m_currentState(nullptr)
{

}

GameStateManager::~GameStateManager()
{
    m_currentState->deinitialize();
    delete m_currentState;
}

void GameStateManager::setState(GameState* newState)
{
    if (m_currentState)
    {
        m_currentState->deinitialize();
        delete m_currentState;
    }
    
    m_currentState = newState;
    m_currentState->initialize();
}

bool GameStateManager::processEvent(const SDL_Event& event)
{
    return m_currentState->handleEvent(event);
}

void GameStateManager::updateStates(int dt)
{
    m_currentState->update(dt);
}

void GameStateManager::drawFrame()
{
    m_currentState->draw();
}