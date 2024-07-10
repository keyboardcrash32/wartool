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
		std::string map = "" /*(char*)MakePtr(g_lpGameDLL, MAPNAME_OFFSET)*/;

		size_t begin = map.size();
		for (; begin > 0 && map[begin - 1] != '\\'; begin--);
		map = map.substr(begin, map.size() - begin - 4);

		discordDetails = "In Game (" + map + ")";

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

void CDiscordRPC::SetPartySize(int partySize)
{
	m_discordPresence.partySize = partySize;
	updateDiscordPresence(m_discordPresence);
}