#ifndef __SMOKERS_SHEADER__
#define __SMOKERS_SHEADER__
#include <stdbool.h>
#include <string.h>

typedef enum cigarette_enum { PAPER, MATCHES, TOBACCO } CigaretteMaterial;

typedef struct smoker_type {
    int pid;
    char name[10];
    CigaretteMaterial material;
} Smoker;

void create_smoker(Smoker* smoker, int pid, const char* name, CigaretteMaterial material) {
    smoker->pid = pid;
    strcpy(smoker->name, name);
    smoker->material = material;
}

char* material_name(CigaretteMaterial material) {
    switch (material)
    {
        case PAPER: return "Paper"; break;
        case MATCHES: return "Matches"; break;
        case TOBACCO: return "Tobacco"; break;
        default: return "Unknown"; break;
    }
}
#endif