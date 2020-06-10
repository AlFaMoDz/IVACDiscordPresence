#include "Discord.h"
#include <ctime>
#include <string>

const std::time_t t = std::time(0);

void Discord::Init() {
	DiscordEventHandlers Handle;
	memset(&Handle, 0, sizeof(Handle));
	Discord_Initialize(APP_ID, &Handle, 1, NULL);
}

void Discord::Update(char* posic, char* fir, char* firNombre) {
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    discordPresence.state = posic;
    discordPresence.details = "Conectado como";
    discordPresence.startTimestamp = t;
    discordPresence.largeImageKey = "ivac11";
    discordPresence.largeImageText = "IVAO ES";
    discordPresence.smallImageKey = fir;
    discordPresence.smallImageText = firNombre;
    Discord_UpdatePresence(&discordPresence);
}
