#include "vdb.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <regex.h>

#define EQ(str1, str2) (!strcmp(str1, str2))
#define ARG_EQ(str) (EQ(argv[idx], str))
#define MATCH(pattern, str) (!regexec(&pattern, str, 0, NULL, 0))

FILE* file;
char* datapath = "vdb.data";

int main(int argc, char** argv) {

    regex_t name_pattern;
    regex_t name_keyword_pattern;
    regex_t date_pattern;
    regex_t date_keyword_pattern;
    regex_t tel_pattern;
    regex_t tel_keyword_pattern;
    regex_t premium_keyword_pattern;
    
    regcomp(&name_pattern, "[A-Z][a-z]+(([A-Z][a-z]+)|(a-z)+)*", REG_EXTENDED);
    regcomp(&name_keyword_pattern, "name=[A-Za-z]+", REG_EXTENDED);
    regcomp(&date_pattern, "(19[0-9][0-9])|(20[0-1][0-9])|2020|2021", REG_EXTENDED);
    regcomp(&date_keyword_pattern, "date=(19[0-9][0-9])|(20[0-1][0-9])|2020|2021", REG_EXTENDED);
    regcomp(&tel_pattern, "36[0-9]{9}", REG_EXTENDED);
    regcomp(&tel_keyword_pattern, "tel=36[0-9]{9}", REG_EXTENDED);
    regcomp(&premium_keyword_pattern, "premium=(yes|no)", REG_EXTENDED);

    int idx = 1;
    while (idx < argc) {
        
        // segítség
        if (ARG_EQ("-h") || ARG_EQ("--help")) {
            print_help();
        }
        
        // teljes lista készítése
        else if (ARG_EQ("-l") || ARG_EQ("--list")) {
            char * line = NULL;
            size_t len = 0;
            int n = 0;

            printf("\n# NAME\t\tDATE\tTEL\t\tPREMIUM\n");
                 // # NAME          DATE    TEL             PREMIUM
            printf("-----------------------------------------------\n");

            file = fopen(datapath, "r");
            if (file == 0) return 1;
            while ((getline(&line, &len, file)) != -1) {
                printf("%d %s", n, line);
                ++n;
            }
            printf("\n");
            fclose(file);
        }

        // bővítés
        else if (ARG_EQ("-a") || ARG_EQ("--add")) {

            char* name;
            int date;
            char* tel;
            bool premium = false;

            bool require[4] = {true, true, true, true}; // TODO: currently unused
            ++idx;

            while (idx < argc && argv[idx][0] != '-') {

                if (require[0] && MATCH(name_pattern, argv[idx])) {
                    name = argv[idx];
                    require[0] = false;                    
                    ++idx;
                }

                else if (require[0] && MATCH(name_keyword_pattern, argv[idx])) {
                    name = argv[idx]+5;
                    require[0] = false;                    
                    ++idx;
                }

                else if (require[1] && MATCH(date_pattern, argv[idx])) {
                    date = atoi(argv[idx]);
                    require[1] = false;                    
                    ++idx;
                }

                else if (require[1] && MATCH(date_keyword_pattern, argv[idx])) {
                    date = atoi(argv[idx]+5);
                    require[1] = false;                    
                    ++idx;
                }

                else if (require[2] && MATCH(tel_pattern, argv[idx])) {
                    tel = argv[idx];
                    require[2] = false;
                    ++idx;
                }

                else if (require[2] && MATCH(tel_keyword_pattern, argv[idx])) {
                    tel = argv[idx]+4;
                    require[2] = false;
                    ++idx;
                }

                else if (require[3] && ARG_EQ("$")) {
                    premium = true;
                    require[3] = false;
                    ++idx;
                }

                else if (require[3] && MATCH(premium_keyword_pattern, argv[idx])) {
                    premium = !strcmp("yes", argv[idx]+8);
                    require[3] = false;
                    ++idx;
                    printf("Premium: %d\n", premium);
                }

                else {
                    printf("\nError processing parameter: \"%s\"\n", argv[idx]);
                    printf("Aborting...\n\n");
                    printf("Make sure you used enough parameters and that you didn't try to assign 2 values to the same attribute, e.g. by providing 2 phone numbers for the same record.\n");
                    printf("Use `vbd --help` to see the prerequisites for each parameter of new records.\n");
                    exit(1);
                }
            }
            
            printf("NAME: %s\nDATE: %d\nTEL: %s\nPREMIUM: %d\n",
                    name, date, tel, premium);
            Patient p = {name, date, tel, premium};
            file = fopen(datapath, "a");
            fprintPatient(file, p);
            fclose(file);
        }

        // módosítás
        else if (ARG_EQ("-m") || ARG_EQ("--modify")) {
            ++idx;
            int mod_idx = atoi(argv[idx]);

            file = fopen(datapath, "r");
            FILE* temp = fopen(".temp.data", "w");

            char * line = NULL;
            size_t len = 0;
            int n = 0;

            while ((getline(&line, &len, file)) != -1) {
                if (n != mod_idx)
                    fprintf(temp, "%s", line);
                else {
                    printf("Are you sure you want to modify the following record? (y/n)\n%d %s", n, line);
                    char c;
                    scanf("%c", &c);
                    if (c != 'y' && c != 'Y') {
                        printf("Aborting... ");
                        fprintf(temp, "%s", line);
                    } else {
                        Patient p = parsePatient(line);
                        ++idx;
                        while (idx < argc && argv[idx][0] != '-') {

                            if (MATCH(name_pattern, argv[idx])) {
                                p.name = argv[idx];
                                ++idx;
                            }

                            else if (MATCH(name_keyword_pattern, argv[idx])) {
                                p.name = argv[idx]+5;
                                ++idx;
                            }

                            else if (MATCH(date_pattern, argv[idx])) {
                                p.date = atoi(argv[idx]);
                                ++idx;
                            }

                            else if (MATCH(date_keyword_pattern, argv[idx])) {
                                p.date = atoi(argv[idx]+5);
                                ++idx;
                            }

                            else if (MATCH(tel_pattern, argv[idx])) {
                                p.tel = argv[idx];
                                ++idx;
                            }

                            else if (MATCH(tel_keyword_pattern, argv[idx])) {
                                p.tel = argv[idx]+4;
                                ++idx;
                            }

                            else if (ARG_EQ("$")) {
                                p.premium = true;
                                ++idx;
                            }

                            else if (MATCH(premium_keyword_pattern, argv[idx])) {
                                p.premium = !strcmp("yes", argv[idx]+8);
                                ++idx;
                            }

                            else {
                                printf("\nError processing parameter: \"%s\"\n", argv[idx]);
                                printf("Aborting...\n\n");
                                printf("Make sure you used enough parameters and that you didn't try to assign 2 values to the same attribute, e.g. by providing 2 phone numbers for the same record.\n");
                                printf("Use `vbd --help` to see the prerequisites for each parameter of new records.\n");
                                exit(1);
                            }
                        }
                        fprintPatient(temp, p);
                    }
                }
                ++n;
            }

            if (n < mod_idx) {
                printf("Error modifying record#%d: there are only %d records in the database.\n", mod_idx, n);
            }
            else {
                fclose(file);
                fclose(temp);

                remove(datapath);
                rename(".temp.data", datapath);

                printf("Done.\n");
            }
        }
        
        // törlés
        else if (ARG_EQ("-r") || ARG_EQ("--remove")) {
            ++idx;
            int rem_idx = atoi(argv[idx]);

            file = fopen(datapath, "r");
            FILE* temp = fopen(".temp.data", "w");

            char * line = NULL;
            size_t len = 0;
            int n = 0;

            while ((getline(&line, &len, file)) != -1) {
                if (n != rem_idx)
                    fprintf(temp, "%s", line);
                else {
                    printf("Are you sure you want to remove the following record from the database? (y/n)\n%d %s", n, line);
                    char c;
                    scanf("%c", &c);
                    if (c != 'y' && c != 'Y') {
                        printf("Aborting... ");
                        fprintf(temp, "%s", line);
                    } else {
                        printf("Removing record... ");
                    }
                }
                ++n;
            }

            if (n < rem_idx) {
                printf("Error removing record#%d: there are only %d records in the database.\n", rem_idx, n);
            }
            else {
                fclose(file);
                fclose(temp);

                remove(datapath);
                rename(".temp.data", datapath);

                printf("Done.\n");
            }
        }
        
        // nullázás
        else if (ARG_EQ("--erase")) {
            char c;
            printf("\nAre you sure you want to erase all registration data? (y/n) ");
            scanf("%c", &c);
            if (c == 'y' || c == 'Y') {
                printf("Erasing data... ");
                remove(datapath);
                printf("Done.\n");
            } else {
                printf("Aborting...\n");
            }
        }

        // ismeretlen argumentum
        else {
            
        }

        ++idx;
    }

    regfree(&name_pattern);
    regfree(&name_keyword_pattern);
    regfree(&date_pattern);
    regfree(&date_keyword_pattern);
    regfree(&tel_pattern);
    regfree(&tel_keyword_pattern);
    regfree(&premium_keyword_pattern);
    return 0;
}