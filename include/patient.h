#ifndef VDB_PATIENT_H
#define VDB_PATIENT_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct Patient {
    char* name;
    int date;
    char* tel;
    bool premium;
} Patient;

// Prints the patient's data in the given file.
void fprintPatient(FILE* file, Patient* p) {
    fprintf(file,
            "%s\t%d\t%s\t",
            p->name,
            p->date, 
            p->tel);
    if (p->premium) {
        fprintf(file, "*\n");
    } else {
        fprintf(file, " \n");
    }
}

// Parses patient data from a string
void parsePatient(const char* line, Patient* p) {
    char* temp = strdup(line);

    char* token = strtok(temp, "\t");
    p->name = strdup(token);
    
    token = strtok(NULL, "\t");
    p->date = atoi(token);
    
    token = strtok(NULL, "\t");
    p->tel = strdup(token);
    
    token = strtok(NULL, "\t");
    p->premium = token == "*";
    
    free(temp);
}

// Converts a patient record to a string (to be used for checking if the record is already in the database)
char* tostring(Patient p) {
    int n = strlen(p.name) + strlen(p.tel) + /*date*/ 4 + /*premium*/ 1 + /*tabs*/ 3 + /*newline*/ 1 + /*null terminator*/ 1;
    char* result = malloc(sizeof(char) * n);

    sprintf(result, "%s\t%d\t%s\t", p.name, p.date, p.tel);

    if (p.premium){
        strcat(result, "*\n");
    } else {
        strcat(result, " \n");
    }
    
    return result;
}

#endif // VDB_PATIENT_H