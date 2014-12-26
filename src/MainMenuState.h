//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// Game main menu
#pragma once

#include "GameState.h"

class GUILayer;

class MainMenuState : public GameState
{
public:
    MainMenuState();

    void initialize() override;
    void deinitialize() override;

    bool handleEvent(const SDL_Event& event) override;
    void update(uint dt) override;
    void draw() override;

private:
    GUILayer* m_guiLayer;
};
