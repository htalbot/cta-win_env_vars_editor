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

#ifndef _UNICODE_STUFF_H__
#define _UNICODE_STUFF_H__

#include <string>

std::string narrow(std::wstring const& text);
std::wstring widen(std::string const& text);

#endif // _UNICODE_STUFF_H__
