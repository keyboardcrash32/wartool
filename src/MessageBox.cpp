#include "MessageBox.h"

std::string WC3MessageBoxFormatType(int type)
{
	std::string str;

	switch (type)
	{
	case WC3MB_WARN:
		str = "WC3MB_WARN";
		break;
	case WC3MB_ERROR:
		str = "WC3MB_ERROR";
		break;
	case WC3MB_QUESTION:
		str = "WC3MB_QUESTION";
		break;
	default:
		str = "Unknown: " + type;
		break;
	}

	return str;
}

void __fastcall HOOKED_WC3MessageBox(int type, const char* string, int a3, int a4, int a5, int a6, int a7)
{
#ifdef _DEBUG
	printf("WC3MessageBox (%s): %s\n", WC3MessageBoxFormatType(type).c_str(), string);
#endif

	ORIG_WC3MessageBox(type, string, a3, a4, a5, a6, a7);
}