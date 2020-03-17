#pragma once
#include <discord_register.h> //sdk shit
#include <discord_rpc.h> // sdk shit
#include <Windows.h>
#include <string>

class Discord {
public:
	void Init();
	void Update(char* posic, char* fir, char* firNombre);
};