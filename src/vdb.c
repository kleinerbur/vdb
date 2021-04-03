#include "vdb.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#define EQ(str1, str2) (!strcmp(str1, str2))
#define ARG_EQ(str) (EQ(argv[idx], str))

FILE* file;
char* datapath = "vdb.data";

int main(int argc, char** argv) {

    
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

            printf("#              NAME              DATE     TEL     PREMIUM\n");
            printf("---------------------------------------------------------\n");

            file = fopen(datapath, "r");
            while ((getline(&line, &len, file)) != -1) {
                printf("%d %s", n, line);
                ++n;
            }
            printf("\n");
            fclose(file);
        }

        // bővítés
        else if (ARG_EQ("-a") || ARG_EQ("--add")) {
            file = fopen(datapath, "a");
            fclose(file);
        }

        // módosítás
        else if (ARG_EQ("-m") || ARG_EQ("--modify")) {

        }
        
        // törlés
        else if (ARG_EQ("-r") || ARG_EQ("--remove")) {
            ++idx;
            int rem_idx = atoi(argv[idx]);
            printf("%d\n", rem_idx);

            file = fopen(datapath, "r");
            FILE* temp = fopen(".temp.data", "w");

            char * line = NULL;
            size_t len = 0;
            int n = 0;

            while ((getline(&line, &len, file)) != -1) {
                if (n != rem_idx)
                    fprintf(temp, "%s", line);
                ++n;
            }

            fclose(file);
            fclose(temp);
            remove(datapath);
            rename(".temp.data", datapath);
        }
        
        // nullázás
        else if (ARG_EQ("--erase")) {

        }

        // ismeretlen argumentum
        else {
            
        }

        ++idx;
    }

    return 0;
}