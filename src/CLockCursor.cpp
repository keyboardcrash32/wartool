#include "CLockCursor.h"

CLockCursor::CLockCursor()
{

}

CLockCursor::~CLockCursor()
{

}

void CLockCursor::SetWindow(HWND hwnd)
{
	m_hwnd = hwnd;
}

void CLockCursor::Lock() // TODO: optimize this code? - keyboardcrash
{
	RECT windowRect;

	GetClipCursor(&m_oldRect); // for uncliping
	GetClientRect(m_hwnd, &windowRect);

	MapWindowPoints(m_hwnd, nullptr, reinterpret_cast<POINT*>(&windowRect), 2);

	ClipCursor(&windowRect);

	cursorLocked = true;
}

void CLockCursor::Unlock()
{
	ClipCursor(&m_oldRect);

	cursorLocked = false;
}