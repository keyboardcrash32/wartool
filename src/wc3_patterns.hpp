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

#ifndef WC3_PATTERNS_HPP_INCLUDED
#define WC3_PATTERNS_HPP_INCLUDED

#include "patterns.hpp"
#include "MemUtils.h"

namespace patterns
{
    namespace engine
    {
        PATTERNS(SetGameAreaFOV,
            "Pre-1.29",
            "51 D9 44 24 ?? 56 DC C8"
        );

        PATTERNS(WC3MessageBox,
			"Pre-1.29",
			"56 57 8B FA 85 FF 8B F1 0F 84 ?? ?? ?? ?? A1 ?? ?? ?? ??"
		);

		PATTERNS(NetEventGameStart,
			"Pre-1.29",
			"56 8B F1 57 B9 0D 00 00 00 E8 ?? ?? ?? ?? 83 7E ?? 01"
		);

		PATTERNS(OnPostPlayerLeave,
			"Pre-1.29",
			"6A FF 68 ?? ?? ?? ?? 64 A1 ?? ?? ?? ?? 50 83 EC 2C 53 55 56 57 A1 ?? ?? ?? ?? 33 C4 50 8D 44 24 ?? 64 A3 ?? ?? ?? ?? 8B F9 B9 0D 00 00 00"
		);
    }
}

#endif // WC3_PATTERNS_HPP_INCLUDED