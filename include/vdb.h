#ifndef VDB_H
#define VDB_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <regex.h>

#include "patient.h"

#define EQ(str1, str2) (!strcmp(str1, str2))
#define ARG_EQ(str) (EQ(argv[idx], str))
#define MATCH(pattern, str) (!regexec(&pattern, str, 0, NULL, 0))

FILE* file;
char* datapath = "vdb.data";

regex_t name_pattern;
regex_t name_keyword_pattern;
regex_t date_pattern;
regex_t date_keyword_pattern;
regex_t tel_pattern;
regex_t tel_keyword_pattern;
regex_t premium_keyword_pattern;

void print_help() {
    printf("\
\n\
VDB v1.0 - Vaccination Database\n\
\n\
made by Bence Bur\n\
pt46p3@inf.elte.hu\n\
github.com/kleinerbur\n\
\n\
Usage:\n\
vdb [-h|--help] [-l|--list] \
[-a|--add name=\"<name>\" date=<date of birth> tel=<phone number> premium=<yes/no>] \
[-m|--modify <index> name=\"<name>\" date=<date of birth> tel=<phone number> premium=<yes/no>] \
[-r|--remove <index>] \
[--erase]\n\
\n\
Prerequisites for parameters when adding / modifying records:\n\
    - name: Make sure you use parentheses around the name.\n\
    - date: Only dates between 1900 and 2021 are accepted.\n\
            If you were born before that, congratulations:\n\
            you have the privilege to lie on this form.\n\
    - tel: Only phone numbers starting with 36 are accepted, without any dashes,\n\
           spaces, or any other whitespace characters.\n\
    - premium: If you use the 'premium=' keyword, make sure to only write 'yes'\n\
               or 'no'; this is case sensitive. By default, new records are set\n\
               to not premium. Adding a '$' character without the keyword will \n\
               also set the record to premium.\n\
Using the keywords isn't necessary, but it can make it easier to check that you\n\
didn't leave one out accidentally. The order of the parameters is flexible, but\n\
make sure you include ONE of each when adding a new record to the database.\n\
When modifying a record, specify only the parameters you want to change ONCE.\n\
\n\
If one of the parameters cannot be processed, an error message will be printed\n\
to the console and the program will stop without processing the rest of the\n\
arguments.\n\
\n\
");
}

#endif // VDB_H