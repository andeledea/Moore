#pragma once

#include "Asse.h"
#include "Laser.h"
#include "CHRocodile.h"
#include "IkOptical.h"

//Muove l'asse specificato chiedendo dove andare
void move(Asse& asse);

//misura a velocità costante
void measure(Asse& asseX, Asse& asseY, Laser &las, CHRocodile& CHR);

//cerca lo 0 del CHR e effettua la taratura a n punti
void calibrate(Asse& asse, CHRocodile& CHR);

//cerca lo 0 del CHR
void findCHRz(Asse& asse, CHRocodile& CHR, bool lr);

bool measFinish(bool dir, float sampleL, float startP, float pos);

void measScale(IkOptical& scale);