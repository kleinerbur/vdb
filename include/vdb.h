#ifndef VDB_H
#define VDB_H

#include <stdio.h>

#include "patient.h"

void print_help() {
    printf("\
\n\
VDB v1.0 - Vaccination Database\n\
\n\
made by Kleinerbur\n\
pt46p3@inf.elte.hu\n\
\n\
Usage: vdb [-h|--help] [-l|--list]\n\
           [-a|--add name=<name> date=<date of birth> tel=<phone number> premium=yes/no]\n\
           [-m|--modify <index> name=<name> date=<date of birth> tel=<phone number> premium=yes/no]\n\
           [-r|--remove <index>]\n\
\n\
");
}

#endif // VDB_H