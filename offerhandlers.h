#ifndef __OFFERS_HEADER__
#define __OFFERS_HEADER__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smokers.h"

void release_offers(CigaretteMaterial* offers) {
    offers[0] = (CigaretteMaterial) (rand() % 3);
    offers[1] = (CigaretteMaterial) (rand() % 3);
    while (offers[0] == offers [1]) {
        offers[1] = (CigaretteMaterial) (rand() % 3);
    }
        
    FILE* offers_file;
    offers_file = fopen("offers.bin", "wb+");
    fwrite(offers, sizeof(CigaretteMaterial), 2, offers_file);
    fclose(offers_file);
}

void look_offers(CigaretteMaterial* offers) {
    FILE* offers_file;
    offers_file = fopen("offers.bin", "rb");
    fread(offers, sizeof(CigaretteMaterial), 2, offers_file);
    fclose(offers_file);
}
#endif