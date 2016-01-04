/*
 * ====================================================================
 * Copyright (c) 2012 Hubert Talbot.  All rights reserved.
 *
 * CTA is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CTA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CTA.  If not, see <http://www.gnu.org/licenses/>.
 *
 * ====================================================================
 */

#include "unicode_stuff.h"
#include <stdlib.h>
#include <string.h>

std::string narrow(std::wstring const& text)
{
    int len = text.length();
    int sizeof_wchar_t = sizeof(wchar_t);

    char * str = new char[(len + 1) * sizeof_wchar_t];
    memset(str, 0, (len + 1) * sizeof_wchar_t);

    wcstombs(str, text.c_str(), len);

    return str;
}

std::wstring widen(std::string const& text)
{
    int len = text.length();
    wchar_t * wtext = new wchar_t[len + 1];
    memset(wtext, 0, (len + 1) * sizeof(wchar_t));
    mbstowcs(wtext, text.c_str(), len);
    std::wstring wstrText(wtext);
    delete [] wtext;

    return wstrText;
}


