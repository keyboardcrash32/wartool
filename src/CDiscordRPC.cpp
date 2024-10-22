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

#include "CDiscordRPC.h"

CDiscordRPC::CDiscordRPC()
{

}

CDiscordRPC::~CDiscordRPC()
{

}

void handleDiscordReady(const DiscordUser* connectedUser)
{

}

void handleDiscordDisconnected(BOOL errcode, LPCSTR message)
{

}

void handleDiscordError(BOOL errcode, LPCSTR message)
{

}

void handleDiscordJoin(LPCSTR secret)
{

}

void handleDiscordSpectate(LPCSTR secret)
{

}

void handleDiscordJoinRequest(const DiscordUser* request)
{
    Discord_Respond(request->userId, DISCORD_REPLY_IGNORE);
}

void CDiscordRPC::updateDiscordPresence(DiscordRichPresence& discordPresence)
{
    Discord_UpdatePresence(&discordPresence);
    ZeroMemory(&discordPresence, sizeof(discordPresence));

#ifdef DISCORD_DISABLE_IO_THREAD
    Discord_UpdateConnection();
#endif
    Discord_RunCallbacks();

    printf("[Discord RPC] Updated the Discord presence\n");
}

void CDiscordRPC::Init()
{
    ZeroMemory(&m_discordPresence, sizeof(m_discordPresence));
    ZeroMemory(&m_handlers, sizeof(m_handlers));

    m_handlers.ready = handleDiscordReady;
    m_handlers.disconnected = handleDiscordDisconnected;
    m_handlers.errored = handleDiscordError;
    m_handlers.joinGame = handleDiscordJoin;
    m_handlers.spectateGame = handleDiscordSpectate;
    m_handlers.joinRequest = handleDiscordJoinRequest;

    Discord_Initialize(APPLICATION_ID, &m_handlers, 1, NULL);
    printf("[Discord RPC] Discord RPC initialized!\n");

    updateDiscordPresence(m_discordPresence);
}

void CDiscordRPC::SetGameState(GameStateEnum gameState)
{
    std::string discordDetails;
    switch (gameState)
    {
    case INMENU:
        discordDetails = "In Menu";
        break;

    case INGAME: {
        discordDetails = "In Game";

        m_discordPresence.startTimestamp = time(0);
        break;
    }

    default:
        discordDetails = "Unknown state";
        break;
    }
    
    m_discordPresence.largeImageKey = "main";
    m_discordPresence.state = discordDetails.c_str();
    printf("[Discord RPC] Changed the game state to %s\n", discordDetails.c_str());
    
    updateDiscordPresence(m_discordPresence);
}