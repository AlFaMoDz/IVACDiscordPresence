#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <tlhelp32.h>
#include <codecvt>
#include <thread>
#include <locale.h>
#include <chrono>
#include "Aeropuertos.h"
#include "Discord.h"

std::vector<std::string> pidTitle;
Discord* g_Discord;
std::string títuloVentana;
std::string fir;
std::string firNombre;

void agregar(char* s, char c) {
	int len = strlen(s);
	s[len] = c;
	s[len + 1] = '\0';
}

bool esIVAC(const PROCESSENTRY32W& entry) {
	return std::wstring(entry.szExeFile) == L"IvAc.exe";
}

std::string utf16ToUtf8(const std::wstring& utf16Str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
	return conv.to_bytes(utf16Str);
}

BOOL CALLBACK enumWindowsProc(HWND hwnd, LPARAM lParam) {
	const auto& pids = *reinterpret_cast<std::vector<DWORD>*>(lParam);

	DWORD winId;
	GetWindowThreadProcessId(hwnd, &winId);

	for (DWORD pid : pids) {
		if (winId == pid) {
			std::wstring title(GetWindowTextLength(hwnd) + 1, L'\0');
			GetWindowTextW(hwnd, &title[0], title.size());

			/*std::cout << "Found window:\n";
			std::cout << "Process ID: " << pid << '\n';
			std::wcout << "Title: " << title << "\n\n";*/

			pidTitle.push_back(utf16ToUtf8(title));
		}
	}

	return TRUE;
}

int tamaño(const char* s)
{
	std::string sd = s;
	return sd.length();
}

bool estáIvAcAbierto(const wchar_t* proceso)
{
	bool exists = false;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry))
		while (Process32Next(snapshot, &entry))
			if (!_wcsicmp(entry.szExeFile, proceso))
				exists = true;

	CloseHandle(snapshot);
	return exists;
}

bool checkFir(const std::string& value, const std::vector<std::string>& array)
{
	return std::find(array.begin(), array.end(), value) != array.end();
}

void centrarstring(const char* s)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns, rows;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	int l = strlen(s);
	int pos = (int)((columns - l) / 2);
	for (int i = 0; i < pos; i++)
		std::cout << " ";

	std::cout << s;
}

int main()
{
	setlocale(LC_CTYPE, "Spanish");
	std::cout << std::endl;
	centrarstring(" |-------------------------------| "); std::cout << std::endl;
	centrarstring(" |  IVAC 1 DISCORD RICH PRESENCE | "); std::cout << std::endl;
	centrarstring(" |          Versión: 1.0         | "); std::cout << std::endl;
	centrarstring(" | Por @AlFaMoDzZ aka Álex Altea | "); std::cout << std::endl;
	centrarstring(" |-------------------------------| "); std::cout << std::endl;
	centrarstring(" | Tw: @alfamodzz Git: /alfamodz | "); std::cout << std::endl;
	centrarstring(" |        IVAO VID: 605126       | "); std::cout << std::endl;
	centrarstring(" |-------------------------------| "); std::cout << std::endl << std::endl << std::endl;

	if (!estáIvAcAbierto(L"IvAc.exe")) { centrarstring("No se ha podido detectar IvAc.exe. Revisa si está abierto."); std::cout << std::endl << std::endl; system("PAUSE"); return 0; }

	std::vector<DWORD> pid;

	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 9);
	PROCESSENTRY32W entry;
	entry.dwSize = sizeof(entry);

	if (!Process32FirstW(snap, &entry)) {
		return 0;
	}

	do {
		if (esIVAC(entry)) {
			pid.emplace_back(entry.th32ProcessID);
		}
	} while (Process32NextW(snap, &entry));

	EnumWindows(enumWindowsProc, reinterpret_cast<LPARAM>(&pid));

	for (auto i = pidTitle.begin(); i != pidTitle.end(); ++i) {
		//iterador += 1;
		//std::cout << iterador << " "<< *i << std::endl;
		std::string s = *i;
		if (s.find("IvAc") == 0) {
			títuloVentana = s;
			//std::cout << std::endl << std::endl << "el string " << s << "empieza con IvAc" << std::endl;
			break;
		}
	}
	// 18 bit          /
	//IvAc 1.2.4.225  - LEMD_OBS                  [104.5nm    0'00"   130┬░/310┬░    z9      lat:040┬░ 52' 04" lon:-03┬░ 45' 55"]
	//std::cout << "Se ha detectado automáticamente la posición: " << títuloVentana;
	char* filtro = new char[títuloVentana.size() + 1];
	títuloVentana.copy(filtro, títuloVentana.size() + 1);
	filtro[títuloVentana.size()] = '\0';

	//std::cout << filtro[18] << std::endl;
	char filtrado[20] = "";
	for (int i = 18; ; i++)
		if (filtro[i] == ' ') { break; }
		else { agregar(filtrado, filtro[i]); }

	//std::cout << std::endl<< std::endl << filtrado << std::endl;

	if (tamaño(filtrado) < 5) { centrarstring("No se ha podido detectar su indicativo. Revisa si estás conectado."); std::cout << std::endl << std::endl; system("PAUSE"); return 0; }

	std::string pene = filtrado;
	std::string m = pene.substr(0, 4);

	if (checkFir(m, LECM)) { fir = "lecm"; firNombre = "Madrid"; }
	if (checkFir(m, LECB)) { fir = "lecb"; firNombre = "Barcelona"; }
	if (checkFir(m, LECG)) { fir = "lecg"; firNombre = "Galicia"; }
	if (checkFir(m, LECL)) { fir = "lecl"; firNombre = "Levante"; }
	if (checkFir(m, LECP)) { fir = "lecp"; firNombre = "Palma"; }
	if (checkFir(m, LECS)) { fir = "lecs"; firNombre = "Sevilla"; }
	if (checkFir(m, GCCC)) { fir = "gccc"; firNombre = "Canarias"; }

	char* mFir = new char[fir.length() + 1];
	strcpy(mFir, fir.c_str());

	char* mFirNombre = new char[firNombre.length() + 1];
	strcpy(mFirNombre, firNombre.c_str());

	g_Discord->Init();
	g_Discord->Update(filtrado, mFir, mFirNombre);

	centrarstring("Activado. Cuando cierres IvAc, el programa se cerrará automáticamente. ¡Buen control!"); std::cout << std::endl << std::endl;

	while (estáIvAcAbierto(L"IvAc.exe")) {
		std::this_thread::sleep_for(std::chrono::seconds(20));
		g_Discord->Update(filtrado, mFir, mFirNombre);
	}
	return 0;
}