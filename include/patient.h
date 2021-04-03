#ifndef VDB_PATIENT_H
#define VDB_PATIENT_H

#include <stdio.h>
#include <stdbool.h>

#ifdef CPLUSPLUS
extern 'C' {
#endif

typedef struct Patient {
    char name[30];
    int dateOfBirth;
    int phoneNumber;
    bool premium;
} Patient;

/* !
 * @function    printPatient
 * @abstract    Prints the patient's data to the standard output.
 * @param   *p  pointer to the Patient
 */  
void printPatient(Patient* p) {
    printf("%s\t%d\t%d",
            p->name,
            p->dateOfBirth, 
            p->phoneNumber);
    if (p->premium)
        printf("\t*");
    printf("\n");
}

#ifdef CPLUSPLUS
}
#endif

#endif // VDB_PATIENT_H