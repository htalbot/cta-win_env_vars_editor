#ifndef UTILS_H
#define UTILS_H

#include <Windows.h>
#include <vector>
#include <string>
#include <map>

#define MIN_FILE_COUNT_TO_KEEP 5

std::vector<std::wstring> parse(const std::wstring & wstr, wchar_t delim);
int get_dir(std::string dir, std::vector<std::string> &files);
std::string get_backup_dir();
bool keep_file(SYSTEMTIME & current_time, int file_year, int file_month, int file_day);
void cleanup_files(const std::string & dir);

#endif // UTILS_H
