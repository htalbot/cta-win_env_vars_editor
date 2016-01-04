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

#ifndef __ENVVARVECT_H__
#define __ENVVARVECT_H__

#include "unicode_stuff.h"

#include <string>
#include <vector>

struct EnvVar
{
    EnvVar(const std::wstring & key, const std::wstring & value);
    bool operator== (const std::wstring & b) const;
    bool operator< (const EnvVar &) const;

    std::wstring key_;
    std::wstring value_;
};

class EnvVarVect: public std::vector<EnvVar>
{
public:
    enum enum_type
    {
        user,
        system
    };

    enum enum_size
    {
        max_size = 10000
    };

    EnvVarVect(enum_type type);
    EnvVarVect & operator=(const std::vector<EnvVar> &);

    bool get_value(const std::wstring & name, std::wstring & value);
    bool set_value(const std::wstring & name, const std::wstring & value);
    bool load(std::wstring & err_msg);
    bool reload(std::wstring & err_msg);
    bool save(std::wstring & err_msg);

protected:

    enum_type type_;
    bool loaded_;
};


class EnvVarVectSpecial: public std::vector<EnvVar>
{
public:
    EnvVarVectSpecial();

    bool get_value(const std::wstring & name, std::wstring & value);
};

#endif // __ENVVARVECT_H__
