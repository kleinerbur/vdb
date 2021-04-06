#ifndef VDB_PATIENT_H
#define VDB_PATIENT_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#ifdef CPLUSPLUS
extern 'C' {
#endif

typedef struct Patient {
    char* name;
    int date;
    char* tel;
    bool premium;
} Patient;

void fprintPatient(FILE* file, Patient p) {
    fprintf(file,
            "%s\t%d\t%s",
            p.name,
            p.date, 
            p.tel);
    if (p.premium)
        fprintf(file, "\t*");
    fprintf(file, "\n");
}

Patient parsePatient(const char* line) {
    char* temp = strdup(line);
    char* token = strtok(temp, "\t");
    char** result = {malloc(sizeof(char)*51),
                     malloc(sizeof(char)*5),
                     malloc(sizeof(char)*12),
                     malloc(sizeof(char)*2)};
    int i = 0;
    while(token) {
        printf("-- %s\n", token);
        result[i] = strdup(token);
        token = strtok(NULL, "\t");
        ++i;
    }
    Patient p = {result[0], atoi(result[1]), result[2], result[3]=="*"};
    return p;
}

#ifdef CPLUSPLUS
}
#endif

#endif // VDB_PATIENT_H