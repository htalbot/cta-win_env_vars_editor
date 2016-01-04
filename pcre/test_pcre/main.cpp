#include <stdio.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include <map>
#include "pcrecpp.h"

#include <stdlib.h>
#include <locale.h>

// CHECK dies with a fatal error if condition is not true.  It is *not*
// controlled by NDEBUG, so the check will be executed regardless of
// compilation mode.  Therefore, it is safe to do things like:
//    CHECK_EQ(fp->Write(x), 4)
#define CHECK(condition) do {                           \
  if (!(condition)) {                                   \
    fprintf(stderr, "%s:%d: Check failed: %s\n",        \
            __FILE__, __LINE__, #condition);            \
    exit(1);                                            \
  }                                                     \
} while (0)

#define CHECK_EQ(a, b)   CHECK(a == b)


inline std::string narrow(std::wstring const& text)
{
    //~ std::locale const loc("");
    //~ wchar_t const* from = text.c_str();
    //~ std::size_t const len = text.size();
    //~ std::vector<char> buffer(len + 1);
    //~ std::use_facet<std::ctype<wchar_t> >(loc).narrow(from, from + len, '_', &buffer[0]);
    //~ return std::string(&buffer[0], &buffer[len]);

    int len = text.length();
    int sizeof_wchar_t = sizeof(wchar_t);

    char * str = new char[(len + 1) * sizeof_wchar_t];
    memset(str, 0, (len + 1) * sizeof_wchar_t);

    wcstombs(str, text.c_str(), len);

    return str;
}

int main(int argc, char * argv[])
{
    printf("\n");

    //~ $test =~ s/(.*)\\$/$1/g;
    pcrecpp::RE exp_no_ending_backslash("(.*)\\\\$");
    std::string with_ending_back_slash = "C:\\dpo\\tools\\scripts\\";
    std::string with_no_ending_back_slash;
    if (exp_no_ending_backslash.FullMatch(with_ending_back_slash, &with_no_ending_back_slash))
    {
        std::cout << "out = " << with_no_ending_back_slash << std::endl;
    }
    else
    {
        std::cout << "no match" << std::endl;
    }


    //~ $test =~ s/\%(.*)\%/$ENV{$1}/g;
    //~ std::string env_var_to_expand("%ACE_ROOT%\\bin");
    //~ pcrecpp::RE re_env("\\%(.*)\\%");
    //~ if (re_env.Replace("repertoire", &env_var_to_expand))
    //~ {
        //~ std::cout << "env_var_to_expand = " << env_var_to_expand << std::endl;
    //~ }
    //~ else
    //~ {
        //~ std::cout << "no env" << std::endl;
    //~ }

    //~ $test =~ s/\//\\/g;  # Windows style
    //~ std::string with_back("C:/dpo/tools\\scripts/");
    //~ pcrecpp::RE re_substitute_slashby_backslash("/");
    //~ if (re_substitute_slashby_backslash.GlobalReplace("\\\\", &with_back))
    //~ {
        //~ std::cout << "with_back = " << with_back << std::endl;
    //~ }
    //~ else
    //~ {
        //~ std::cout << "no match with_back" << std::endl;
    //~ }

    //~ if ($test =~ /^([a-zA-Z]\:|\\\\[^\/\\:*?"<>|]+\\[^\/\\:*?"<>|]+)(\\[^\/\\:*?"<>|]+)+$/)
    //~ pcrecpp::RE re_format("^([a-zA-Z]\\:|\\\\\\\\[^\\/\\\\:*?\"<>|]+\\\\[^\\/\\\\:*?\"<>|]+)(\\\\[^\\/\\\\:*?\"<>|]+)+$");
    //~ std::string dir("D:\\machine\\dpo");
    //~ if (re_format.FullMatch(dir))
    //~ {
        //~ std::cout << "bon format" << std::endl;
    //~ }
    //~ else
    //~ {
        //~ std::cout << "mauvais format" << std::endl;
    //~ }

    //~ {
        //~ if (!(-d $test) && !(-f $test))
        //~ {
            //~ $$background_color_ref->Set(255, 255, 0, Wx::wxALPHA_OPAQUE);
            //~ $$text_color_ref->Set(0, 0, 0, Wx::wxALPHA_OPAQUE);
            //~ return 1;
        //~ }
    //~ }

    std::map<std::string, std::string> mmm;
    mmm["AAAAA"] = "11111";
    mmm["BBBBB"] = "22222";
    mmm["CCCCC"] = "33333";

    printf("mmm[AAAAA] = %s\n", mmm["AAAAA"].c_str());

    const char * delim(";");
    std::string value("%AAAAA%\\%BBBBB%\\%CCCCC%");
    char * tok = strtok(const_cast<char *>(value.c_str()), delim);
    while (tok != NULL)
    {
        std::string strTok = tok;

        printf("strTok = %s\n", strTok.c_str());

        pcrecpp::RE re_env_var("%(.*?)%");
        std::string str_env_var_id;
        while (re_env_var.FullMatch(strTok, &str_env_var_id))
        {
            std::wstring wstrValue = L"YY\\\\";
            std::string strValue(narrow(wstrValue));

            //~ printf("replace %s with %s\n", str_env_var_id.c_str(), mmm[str_env_var_id].c_str());
            printf("replace %s with %s\n", str_env_var_id.c_str(), strValue.c_str());
            //~ pcrecpp::RE("%(.*?)%").GlobalReplace(mmm[str_env_var_id].c_str(), &strTok);
            pcrecpp::RE("%(.*?)%", pcrecpp::UTF8()).Replace(strValue.c_str(), &strTok);
            printf("replacement = %s\n", strTok.c_str());
        }

        printf("===> strTok = %s\n", strTok.c_str());

        tok = strtok(NULL, delim);
    }

    printf("/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n");
    {
        std::string strTok = "helloZZworldZZ";
        printf("strTok = %s\n", strTok.c_str());

        std::string strBackslash("sakqasdasd\\AAAAA\\BBBBB\\");

        pcrecpp::RE("(\\\\)").GlobalReplace("\\\\\\\\", &strBackslash);

        pcrecpp::StringPiece input(strTok);  // Wrap in a StringPiece
        std::string word;
        while (pcrecpp::RE("(ZZ)").FindAndConsume(&input, &word))
        {
            pcrecpp::RE("(ZZ)").Replace(strBackslash.c_str(), &strTok);
            input = strTok;
        }
        printf("===> strTok = %s\n", strTok.c_str());

/*
        pcrecpp::RE re_zz("(ZZ)");
        std::string zz;
        while (re_zz.FullMatch(strTok, &zz))
        {
            printf("222222\n");
            //~ std::wstring wstrValue = L"\\\\";
            //~ std::string strBackslash(narrow(wstrValue));
            std::string strBackslash("YY");

            printf("replace %s with %s\n", zz.c_str(), strBackslash.c_str());
            pcrecpp::RE("(ZZ)").Replace(strBackslash.c_str(), &strTok);
            printf("replacement = %s\n", strTok.c_str());
        }

        printf("===> strTok = %s\n", strTok.c_str());

        tok = strtok(NULL, delim);
*/
    }


    //~ {
        //~ std::string contents("%AAAAA%\\%BBBBB%\\%CCCCC%");
        //~ printf("***************************************************************: %s\n", contents.c_str());

        //~ pcrecpp::StringPiece input(contents);  // Wrap in a StringPiece
        //~ std::string word;
        //~ while (pcrecpp::RE("%(.*?)%").FindAndConsume(&input, &word))
        //~ {
            //~ pcrecpp::RE("%(.*?)%").Replace(mmm[word], &contents);
            //~ input = contents;
        //~ }

        //~ printf("====> %s\n", contents.c_str());
    //~ }

    //~ std::string strTok("abc\\");
    //~ while(pcrecpp::RE("(.*)\\\\$").FullMatch(strTok, &strTok));
    //~ printf("strTok = %s\n", strTok.c_str());

    //~ std::string unix_style("abc/def/xyz");
    //~ printf("unix_style = %s\n", unix_style.c_str());
    //~ pcrecpp::RE("/").GlobalReplace("\\\\", &unix_style);
    //~ printf("windows_style = %s\n", unix_style.c_str());

    //~ printf ("UUUUUUUUUUUUUUUUTTTTTTTTTTTTTFFFFFFFFFFFFFFF888888888888888\n");
//~ printf("Testing UTF-8 handling\n");

    //~ // Three Japanese characters (nihongo)
    //~ const unsigned char utf8_string[] = {
         //~ 0xe6, 0x97, 0xa5, // 65e5
         //~ 0xe6, 0x9c, 0xac, // 627c
         //~ 0xe8, 0xaa, 0x9e, // 8a9e
         //~ 0
    //~ };
    //~ const unsigned char utf8_pattern[] = {
         //~ '.',
         //~ 0xe6, 0x9c, 0xac, // 627c
         //~ '.',
         //~ 0
    //~ };

    //~ std::wstring wstr = (wchar_t *)utf8_string;
    //~ printf("utf8_string           = %s\n", utf8_string);
    //~ printf("utf8_string (wstring) = %s\n", wstr.c_str());

    //~ int count;
    //~ int len = wstr.length();
    //~ for (int i = 0; i < len; i++)
    //~ {
        //~ ++count;
    //~ }

    //~ printf("--------------------------->len = %d, count = %d\n", len, count);


    //~ // Both should match in either mode, bytes or UTF-8
    //~ pcrecpp::RE re_test1(".........");
    //~ CHECK(re_test1.FullMatch(utf8_string));
    //~ CHECK(re_test1.FullMatch((unsigned char *)wstr.c_str()));
    //~ pcrecpp::RE re_test2("...", pcrecpp::UTF8());
    //~ CHECK(re_test2.FullMatch(utf8_string));

    //~ // Check that '.' matches one byte or UTF-8 character
    //~ // according to the mode.
    //~ string ss;
    //~ pcrecpp::RE re_test3("(.)");
    //~ CHECK(re_test3.PartialMatch(utf8_string, &ss));

    //~ CHECK_EQ(ss, string("\xe6"));
    //~ pcrecpp::RE re_test4("(.)", pcrecpp::UTF8());
    //~ CHECK(re_test4.PartialMatch(utf8_string, &ss));
    //~ CHECK_EQ(ss, string("\xe6\x97\xa5"));
    //~ printf("ss = %s\n", ss.c_str());

    //~ // Check that string matches itself in either mode
    //~ pcrecpp::RE re_test5(utf8_string);
    //~ CHECK(re_test5.FullMatch(utf8_string));
    //~ pcrecpp::RE re_test6(utf8_string, pcrecpp::UTF8());
    //~ CHECK(re_test6.FullMatch(utf8_string));

    //~ // Check that pattern matches string only in UTF8 mode
    //~ pcrecpp::RE re_test7(utf8_pattern);
    //~ CHECK(!re_test7.FullMatch(utf8_string));
    //~ pcrecpp::RE re_test8(utf8_pattern, pcrecpp::UTF8());

    return 0;
}
