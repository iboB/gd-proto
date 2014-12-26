//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// Game main menu
#include "GDProto.pch.h"
#include "MainMenuState.h"

#include "GUILayer.h"

MainMenuState::MainMenuState()
    : m_guiLayer(nullptr)
{
}

void MainMenuState::initialize()
{
    m_guiLayer = new GUILayer("gui layer");
    m_guiLayer->initialize();
    m_guiLayer->loadRootRml("gui/main.xml");
}

void MainMenuState::deinitialize()
{
    m_guiLayer->deinitialize();
    delete m_guiLayer;
}

bool MainMenuState::handleEvent(const SDL_Event& event)
{
    return m_guiLayer->processSDLEvent(event);
}

void MainMenuState::update(uint dt)
{
    m_guiLayer->update();
}

void MainMenuState::draw()
{
    m_guiLayer->draw();
}
