#include "vdb.h"

int main(int argc, char** argv) {
    
    regcomp(&name_pattern, "[A-Z][a-z]+(([A-Z][a-z]+)|(a-z)+)*", REG_EXTENDED);
    regcomp(&name_keyword_pattern, "name=[A-Za-z]+", REG_EXTENDED);
    regcomp(&date_pattern, "(19[0-9][0-9])|(20[0-1][0-9])|2020|2021", REG_EXTENDED);
    regcomp(&date_keyword_pattern, "date=(19[0-9][0-9])|(20[0-1][0-9])|2020|2021", REG_EXTENDED);
    regcomp(&tel_pattern, "36[0-9]{9}", REG_EXTENDED);
    regcomp(&tel_keyword_pattern, "tel=36[0-9]{9}", REG_EXTENDED);
    regcomp(&premium_keyword_pattern, "premium=(yes|no)", REG_EXTENDED);

    int idx = 1;

    while (idx < argc) {
     
        // ------------------------------------------------------------------------------- SEGÍTSÉG
        if (ARG_EQ("-h") || ARG_EQ("--help")) {
            print_help();
        }
        


        // --------------------------------------------------------------------------- TELJES LISTA
        else if (ARG_EQ("-l") || ARG_EQ("--list")) {
            file = fopen(datapath, "r");
            if (!file) {
                printf("[!] Failed to open the database file\n");
            } else {
                printf("\n# NAME\t\tDATE\tTEL\t\tPREMIUM\n");
                    // # NAME          DATE    TEL             PREMIUM
                printf("-----------------------------------------------\n");
                
                char * line = NULL;
                size_t len = 0;
                int n = 0;

                while ((getline(&line, &len, file)) != -1) {
                    printf("%d %s", n, line);
                    ++n;
                }
                printf("\n");
                fclose(file);
                free(line);
            }
            
        }



        // -------------------------------------------------------------------------------- BŐVÍTÉS
        else if (ARG_EQ("-a") || ARG_EQ("--add")) {

            char* name = NULL;
            int date = 0;
            char* tel = NULL;
            bool premium = false;
            bool already_given[4] = {false, false, false, false};
            
            ++idx;

            while (idx < argc && argv[idx][0] != '-') {
                if (!already_given[0] && MATCH(name_pattern, argv[idx])) {
                    name = argv[idx];
                    already_given[0] = true;                    
                }
                else if (!already_given[0] && MATCH(name_keyword_pattern, argv[idx])) {
                    name = argv[idx]+5;
                    already_given[0] = true;                    
                }
                else if (!already_given[1] && MATCH(date_pattern, argv[idx])) {
                    date = atoi(argv[idx]);
                    already_given[1] = true;                    
                }
                else if (!already_given[1] && MATCH(date_keyword_pattern, argv[idx])) {
                    date = atoi(argv[idx]+5);
                    already_given[1] = true;                    
                }
                else if (!already_given[2] && MATCH(tel_pattern, argv[idx])) {
                    tel = argv[idx];
                    already_given[2] = true;
                }
                else if (!already_given[2] && MATCH(tel_keyword_pattern, argv[idx])) {
                    tel = argv[idx]+4;
                    already_given[2] = true;
                }
                else if (!already_given[3] && ARG_EQ("$")) {
                    premium = true;
                    already_given[3] = true;
                }
                else if (!already_given[3] && MATCH(premium_keyword_pattern, argv[idx])) {
                    premium = !strcmp("yes", argv[idx]+8);
                    already_given[3] = true;
                }
                else {
                    printf("\nError processing parameter: \"%s\"\n", argv[idx]);
                    printf("Aborting...\n\n");
                    printf("Make sure you used enough parameters and that you didn't try to assign 2 values to the same attribute, e.g. by providing 2 phone numbers for the same record.\n");
                    printf("Use `vbd --help` to see the prerequisites for each parameter of new records.\n");
                    break;
                }
                ++idx;
            }

            if (!name || !date || !tel) {
                printf("[!] Failed to construct new record: not enough parameters.\n");
            } else {
                printf("\nNAME:\t %s\nDATE:\t %d\nTEL:\t %s\nPREMIUM: ",
                        name, date, tel);
                if(premium) printf("yes\n");
                else printf("no\n");

                Patient p = {name, date, tel, premium};
                
                bool already_exists = false;

                FILE* temp = fopen(datapath, "r");
                if (temp) {
                    char* p_string = tostring(p);
                    char * line = NULL;
                    size_t len = 0;
                    int n = 0;
                    while ((getline(&line, &len, temp)) != -1) {
                        if (EQ(line, p_string)) {
                            printf("\nAn identical record already exists in the database.\n");
                            already_exists = true;
                            break;
                        }
                    }
                    fclose(temp);
                    free(line);
                    free(p_string);
                }

                if (!already_exists) {
                    printf("\nAdding new record... ");
                    file = fopen(datapath, "a");
                    fprintPatient(file, &p);
                    fclose(file);
                    printf("Done.\n");
                }
            }
            --idx; // a ciklus elrontja az argumentum indexelését, enélkül kihagyná a paraméterek utáni argumentumot
        }



        // ------------------------------------------------------------------------------ MÓDOSÍTÁS
        else if (ARG_EQ("-m") || ARG_EQ("--modify")) {
            ++idx;
            int mod_idx = atoi(argv[idx]);

            file = fopen(datapath, "r");
            FILE* temp = fopen(".temp.data", "w");

            char * line = NULL;
            size_t len = 0;
            int n = 0;

            while ((getline(&line, &len, file)) != -1) {
                if (n != mod_idx) {
                    fprintf(temp, "%s", line);
                }
                else {
                    printf("\nThe following record is about to be modified:\n%d %s", n, line);
                    printf("Are you sure? (y/n) ");
                    
                    char c;
                    scanf("%c", &c);
                    
                    if (c != 'y' && c != 'Y') {
                        printf("Aborting... ");
                        fprintf(temp, "%s", line);
                    }
                    else {
                        Patient p;
                        parsePatient(line, &p);

                        bool already_given[4] = {false, false, false, false};
                        ++idx;
                        
                        while (idx < argc && argv[idx][0] != '-') {
                            if (!already_given[0] && MATCH(name_pattern, argv[idx])) {
                                p.name = argv[idx];
                                already_given[0] = true;                    
                            }
                            else if (!already_given[0] && MATCH(name_keyword_pattern, argv[idx])) {
                                p.name = argv[idx]+5;
                                already_given[0] = true;                    
                            }
                            else if (!already_given[1] && MATCH(date_pattern, argv[idx])) {
                                p.date = atoi(argv[idx]);
                                already_given[1] = true;                    
                            }
                            else if (!already_given[1] && MATCH(date_keyword_pattern, argv[idx])) {
                                p.date = atoi(argv[idx]+5);
                                already_given[1] = true;                    
                            }
                            else if (!already_given[2] && MATCH(tel_pattern, argv[idx])) {
                                p.tel = argv[idx];
                                already_given[2] = true;
                            }
                            else if (!already_given[2] && MATCH(tel_keyword_pattern, argv[idx])) {
                                p.tel = argv[idx]+4;
                                already_given[2] = true;
                            }
                            else if (!already_given[3] && ARG_EQ("$")) {
                                p.premium = true;
                                already_given[3] = true;
                            }
                            else if (!already_given[3] && MATCH(premium_keyword_pattern, argv[idx])) {
                                p.premium = !strcmp("yes", argv[idx]+8);
                                already_given[3] = true;
                            }
                            else {
                                printf("\nError processing parameter: \"%s\"\n", argv[idx]);
                                printf("Aborting...\n\n");
                                printf("Make sure you used enough parameters and that you didn't try to assign 2 values to the same attribute, e.g. by providing 2 phone numbers for the same record.\n");
                                printf("Use `vbd --help` to see the prerequisites for each parameter of new records.\n");
                                exit(1);
                            }
                            ++idx;
                        }
                        printf("Modifying record... ");
                        fprintPatient(temp, &p);
                    }
                }
                ++n;
            }

            fclose(file);
            fclose(temp);

            if (n < mod_idx) {
                printf("Error modifying record#%d: there are only %d records in the database.\n", mod_idx, n);
                remove(".temp.data");
            } else {
                remove(datapath);
                rename(".temp.data", datapath);

                printf("Done.\n");
            }

            --idx; // a ciklus elrontja az argumentum indexelését, enélkül kihagyná a paraméterek utáni argumentumot
        }
        


        // --------------------------------------------------------------------------------- TÖRLÉS
        else if (ARG_EQ("-r") || ARG_EQ("--remove")) {
            ++idx;
            int rem_idx = atoi(argv[idx]);

            file = fopen(datapath, "r");
            FILE* temp = fopen(".temp.data", "w");

            char * line = NULL;
            size_t len = 0;
            int n = 0;

            while ((getline(&line, &len, file)) != -1) {
                if (n != rem_idx) {
                    fprintf(temp, "%s", line);
                } else {
                    printf("The following record will be removed from the database:\n%d %s", n, line);
                    printf("Are you sure? (y/n) ");

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

            if (n <= rem_idx) {
                printf("\nError removing record #%d: there are only %d records in the database.\n", rem_idx, n);
                printf("(Keep in mind that indexing starts at zero.)\n");
            } else {
                fclose(file);
                fclose(temp);

                remove(datapath);
                rename(".temp.data", datapath);

                printf("Done.\n");
            }
        }
        


        // ------------------------------------------------------------------------------- NULLÁZÁS
        else if (ARG_EQ("--erase")) {
            char c;
            printf("\nAre you sure you want to erase all registration data? (y/n) ");
            scanf("%c", &c);
            if (c == 'y' || c == 'Y') {
                printf("Erasing data... ");
                file = fopen(datapath, "w");
                fclose(file);
                printf("Done.\n");
            } else {
                printf("Aborting...\n");
            }
        }



        // ismeretlen argumentum
        else printf("\n[!] Failed to process argument: \"%s\"; skipping...\n", argv[idx]);

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
