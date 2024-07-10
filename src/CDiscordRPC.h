/*
	Copyright (C) 2024 keyboardcrash

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef CDISCORDRPC_H_INCLUDED
#define CDISCORDRPC_H_INCLUDED

#define APPLICATION_ID "1255488895139709019"

#include <stdio.h> // for printf
#include <memory> // for std::unique_ptr
#include <string>
#include <windows.h> // for ZeroMemory
#include <ctime> // for time
#include "discord_rpc.h"
//#include "wartool.h"

extern void* g_lpGameDLL;

enum GameStateEnum : int
{
    INMENU = 0,
    INGAME = 1,
};

class CDiscordRPC
{
public:
    CDiscordRPC();
    ~CDiscordRPC();

public:
    void Init();
    void SetGameState(GameStateEnum gameState);
    void SetPartySize(int partySize);
    void updateDiscordPresence(DiscordRichPresence& discordPresence);

private:
    DiscordRichPresence m_discordPresence;
    DiscordEventHandlers m_handlers;
};

#endif // CDISCORDRPC_H_INCLUDED