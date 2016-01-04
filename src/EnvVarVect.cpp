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

#include "EnvVarVect.h"
#include <algorithm>
#include <Windows.h>
#include <stdio.h>

#include <iostream>
using namespace std;


EnvVar::EnvVar(const std::wstring & key, const std::wstring & value)
: key_(key)
, value_(value)
{
}

bool EnvVar::operator== (const std::wstring & b) const
{
    return (this->key_ == b);
}

bool EnvVar::operator< (const EnvVar & rhs) const
{
    return (this->key_ < rhs.key_);
}

EnvVarVect::EnvVarVect(enum_type type)
: type_(type)
, loaded_(false)
{
}

EnvVarVect & EnvVarVect::operator=(const std::vector<EnvVar> & rhs)
{
    this->clear();
    std::vector<EnvVar>::const_iterator it;
    for (it = rhs.begin(); it != rhs.end(); it++)
    {
        this->push_back(*it);
    }

    return *this;
}

bool EnvVarVect::get_value(const std::wstring & name, std::wstring & value)
{
    std::vector<EnvVar>::iterator it = std::find(begin(), end(), name);

    if (it == end())
    {
        return false;
    }

    value = it->value_;
    return true;
}


bool EnvVarVect::set_value(const std::wstring & name, const std::wstring & value)
{
    std::vector<EnvVar>::iterator it = std::find(begin(), end(), name);

    if (it == end())
    {
        return false;
    }

    it->value_ = value;
    return true;
}


bool EnvVarVect::load(std::wstring & err_msg)
{
    if (loaded_)
    {
        return true;
    }

    HKEY hBaseKey;
    HKEY hKey = 0;

    std::wstring wstr;
    std::wstring wstr_err_key;
    if (type_ == user)
    {
        hBaseKey = HKEY_CURRENT_USER;
        wstr_err_key = L"HKEY_CURRENT_USER\\Environment";
        wstr = L"Environment";
    }
    else
    {
        hBaseKey = HKEY_LOCAL_MACHINE;
        wstr_err_key = L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment";
        wstr = L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment";
    }

    LONG rc = RegOpenKeyExW(hBaseKey, wstr.c_str(), 0, KEY_QUERY_VALUE, &hKey);
    if(rc != ERROR_SUCCESS)
    {
        err_msg = L"RegOpenKeyExW() failed: " + wstr_err_key;
        return false;
    }

    DWORD i = 0;
    while(rc == ERROR_SUCCESS)
    {
        WCHAR name[max_size];
        DWORD name_length = max_size;
        LPBYTE data = new BYTE[max_size];
        memset(data, 0, max_size);
        DWORD data_size = max_size;
        rc = RegEnumValueW(hKey, i, name, &name_length, NULL, NULL, (LPBYTE)data, &data_size);
        if (rc == ERROR_SUCCESS)
        {
            EnvVar var(name, (LPWSTR)data);
            this->push_back(var);
        }
        else
        {
            if (rc != ERROR_NO_MORE_ITEMS)
            {
                err_msg = L"RegEnumValueW() failed: " + wstr_err_key;
                delete [] data;
                RegCloseKey(hKey);
                return false;
            }
        }

        delete [] data;

        ++i;
    }

    RegCloseKey(hKey);

    loaded_ = true;

    return true;
}


bool EnvVarVect::reload(std::wstring & err_msg)
{
    loaded_ = false;
    this->clear();

    return load(err_msg);
}


bool EnvVarVect::save(std::wstring & err_msg)
{
    if (!loaded_)
    {
        return true;
    }

    HKEY hBaseKey;
    HKEY hKey = 0;

    std::wstring wstr;
    std::wstring wstr_err_key;
    if (type_ == user)
    {
        hBaseKey = HKEY_CURRENT_USER;
        wstr_err_key = L"HKEY_CURRENT_USER\\Environment";
        wstr = L"Environment";
    }
    else
    {
        hBaseKey = HKEY_LOCAL_MACHINE;
        wstr_err_key = L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment";
        wstr = L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment";
    }


    LONG rc = RegOpenKeyExW(hBaseKey, L"", 0, KEY_QUERY_VALUE, &hKey);
    if(rc != ERROR_SUCCESS)
    {
        err_msg = L"RegOpenKeyExW() failed: " + wstr_err_key;
        return false;
    }

    // delete keys .../Environment from registry
    rc = RegDeleteKeyW(hKey, wstr.c_str());
    if (rc != ERROR_SUCCESS)
    {
        err_msg = L"RegDeleteKeyW() failed: " + wstr;
        RegCloseKey(hKey);
        return false;
    }

    // Create new keys
    HKEY hNewKey;
    DWORD dwDisposition;
    rc = RegCreateKeyExW(hKey,
                        wstr.c_str(),
                        0,
                        NULL,
                        REG_OPTION_NON_VOLATILE,
                        KEY_ALL_ACCESS,
                        NULL,
                        &hNewKey,
                        &dwDisposition );

    if ( rc != ERROR_SUCCESS )
    {
        err_msg = L"RegCreateKeyExW() failed: " + wstr;
        RegCloseKey(hKey);
        return false;
    }

    bool err(false);
    EnvVarVect::iterator it;
    for (it = this->begin(); it != this->end(); it++)
    {
        // Why should we not use REG_EXPAND_SZ?
        DWORD dwRc = RegSetValueExW(hNewKey, it->key_.c_str(), 0, REG_EXPAND_SZ, (BYTE*)it->value_.c_str(), 2 *(it->value_.length() + 1));
        if (dwRc != ERROR_SUCCESS)
        {
            err = true;
            err_msg = L"RegCreateKeyExW() failed: " + wstr + L"\n";
        }
    }

    RegCloseKey(hNewKey);
    RegCloseKey(hKey);

    DWORD dwReturnValue;
    SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM) L"Environment", SMTO_ABORTIFHUNG, 5000, &dwReturnValue);

    if (err)
    {
        return false;
    }

    return true;
}


EnvVarVectSpecial::EnvVarVectSpecial()
{
    std::vector<std::wstring> special;

    special.push_back(L"ALLUSERSPROFILE");
    special.push_back(L"PROGRAMDATA");
    special.push_back(L"APPDATA");
    special.push_back(L"COMPUTERNAME");
    special.push_back(L"COMMONPROGRAMFILES");
    special.push_back(L"COMMONPROGRAMFILES(x86)");
    special.push_back(L"SystemRoot");
    special.push_back(L"COMSPEC");
    special.push_back(L"HOMEDRIVE");
    special.push_back(L"HOMEPATH");
    special.push_back(L"LOCALAPPDATA");
    special.push_back(L"LOGONSERVER");
    special.push_back(L"PATH");
    special.push_back(L"PATHEXT");
    special.push_back(L"PROGRAMFILES");
    special.push_back(L"PROGRAMFILES(X86)");
    special.push_back(L"PROMPT");
    special.push_back(L"SystemDrive");
    special.push_back(L"USERPROFILE");
    special.push_back(L"TEMP");
    special.push_back(L"TMP");
    special.push_back(L"USERDOMAIN");
    special.push_back(L"USERDATA");
    special.push_back(L"USERNAME");
    special.push_back(L"WINDIR");
    special.push_back(L"PUBLIC");
    special.push_back(L"PROGRAMDATA");
    special.push_back(L"PSModulePath");

    std::vector<std::wstring>::iterator it;
    for (it = special.begin(); it != special.end(); it++)
    {
        std::wstring value;

        char * path;
        path = getenv(const_cast<char *>(narrow(*it).c_str()));
        if (path != NULL)
        {
            value = widen(path);
        }

        EnvVar var(*it, (LPWSTR)value.c_str());
        this->push_back(var);
    }
}


bool EnvVarVectSpecial::get_value(const std::wstring & name, std::wstring & value)
{
    std::vector<EnvVar>::iterator it = std::find(begin(), end(), name);

    if (it == end())
    {
        return false;
    }

    value = L"";

    char * path;
    path = getenv(const_cast<char *>(narrow(it->key_).c_str()));
    if (path != NULL)
    {
        value = widen(path);
    }

    return true;
}
