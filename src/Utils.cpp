#include "Utils.h"
#include <dirent.h>
#include <vector>
#include <string>
#include <algorithm>
#include "pcrecpp.h"

#define MIN_FILE_COUNT_TO_KEEP 5

std::vector<std::wstring> parse(const std::wstring & wstr, wchar_t delim)
{
    std::vector<std::wstring> vect;

    std::wstring value;

    std::wstring::const_iterator it;
    for (it = wstr.begin(); it != wstr.end(); it++)
    {
        if (*it == delim)
        {
            if (value.length() > 0)
            {
                vect.push_back(value);
            }
            value = L"";
        }
        else
        {
            value += *it;
        }
    }

    if (value.length() > 0)
    {
        vect.push_back(value);
    }

    return vect;
}


int get_dir(std::string dir, std::vector<std::string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(std::string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}


std::string get_backup_dir()
{
    char * systdrive = getenv("SYSTEMDRIVE");
    std::string dir(systdrive);
    dir += "\\cta_back";

    return dir;
}


bool keep_file(SYSTEMTIME & current_time, int file_year, int file_month, int file_day)
{
    // To simplify, a month is considered to have 31 days.

    int days = 0;

    int year_diff = current_time.wYear - file_year;

    if (year_diff < 0)
    {
        // The year of the file is newer than the current year.
        // It is abnormal situation. So, let's keep the file (don't take
        // the chance to lose precious data).
        return true;
    }
    else
    {
        switch(year_diff)
        {
            case 0: // same year
                {
                    int month_diff = current_time.wMonth - file_month;
                    if (month_diff < 0)
                    {
                        // The month of the file is newer than the current month.
                        // It is abnormal situation. So, let's keep the file (don't take
                        // the chance to lose precious data).
                        return true;
                    }

                    if (month_diff == 0)
                    {
                        // Same year, same month, keep the file.
                        return true;
                    }

                    if (month_diff == 1)
                    {
                        days = current_time.wDay;
                        days += (31 - file_day);   //~31
                    }
                    else
                    {
                        // More than one month.
                        return false;
                    }
                }
                break;

            case 1: // The file year is the previous year of the current date.
                if (current_time.wMonth == 1) // January
                {
                    if (file_month == 12) // December
                    {
                        days = current_time.wDay;
                        days += (31 - file_day); //~31
                    }
                    else
                    {
                        // More than one month.
                        return false;
                    }
                }
                else
                {
                    // The file month is not january => more than ~31 days.
                    return false;
                }
                break;

            default:
                return false;
        };

        if (days <= 31) //~31
        {
            return true;
        }
    }

    return false;
}


void cleanup_files(const std::string & dir)
{
    // Keep at least 5 files in the last ~31 days.

    std::vector<std::string> files;
    if (get_dir(dir, files) == 0)
    {
        std::vector<std::string> vect;
        std::map<std::string, std::string> the_map;
        std::vector<std::string>::iterator it;
        for (it = files.begin(); it != files.end(); it++)
        {
            if (!pcrecpp::RE("env_vars-\\d{4}-\\d{2}-\\d{2}_\\d{2}h\\d{2}m\\d{2}s\\.cta", pcrecpp::UTF8()).FullMatch(*it))
            {
                continue;
            }

            size_t found1 = it->find("-");
            if (found1 != std::string::npos)
            {
                std::string year = it->substr(found1 + 1, 4);
                found1 = it->find("-");
                std::string month = it->substr(found1 + 1 + 5, 2);
                std::string day = it->substr(found1 + 1 + 5 + 3, 2);
                std::string hour = it->substr(found1 + 1 + 5 + 3 + 3, 2);
                std::string minute = it->substr(found1 + 1 + 5 + 3 + 3 + 3, 2);
                std::string second = it->substr(found1 + 1 + 5 + 3 + 3 + 3 + 3, 2);

                std::string key = year+month+day+hour+minute+second;
                vect.push_back(key);
                the_map[key] = *it;
            }
        }

        std::sort(vect.begin(), vect.end());

        if (vect.size() <= MIN_FILE_COUNT_TO_KEEP)
        {
            // Keep all files.
        }
        else
        {
            SYSTEMTIME st;
            GetLocalTime(&st);

            std::vector<std::string>::reverse_iterator it3;
            for (it3 = vect.rbegin(); it3 != vect.rend(); it3++)
            {
                std::string key = *it3;
                std::string year = key.substr(0, 4);
                std::string month = key.substr(0 + 4, 2);
                std::string day = key.substr(0 + 4 + 2, 2);
                int nYear = atoi(year.c_str());
                int nMonth = atoi(month.c_str());
                int nDay = atoi(day.c_str());

                if (!keep_file(st, nYear, nMonth, nDay))
                {
                    std::string complete_file_name(dir);
                    complete_file_name += "\\" + the_map[*it3];
                    remove(complete_file_name.c_str());
                }
            }
        }
    }
}


