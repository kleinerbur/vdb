#ifndef VDB_PATIENT_H
#define VDB_PATIENT_H

#include <stdio.h>
#include <stdbool.h>

#ifdef CPLUSPLUS
extern 'C' {
#endif

typedef struct Patient {
    char* name;
    int dateOfBirth;
    char* phoneNumber;
    bool premium;
} Patient;

/* !
 * @function    printPatient
 * @abstract    Prints the patient's data to the standard output.
 * @param   *p  pointer to the Patient
 */  
void fprintPatient(FILE* file, Patient p) {
    fprintf(file,
            "%s\t%d\t%s",
            p.name,
            p.dateOfBirth, 
            p.phoneNumber);
    if (p.premium)
        fprintf(file, "\t*");
    fprintf(file, "\n");
}

#ifdef CPLUSPLUS
}
#endif

#endif // VDB_PATIENT_H