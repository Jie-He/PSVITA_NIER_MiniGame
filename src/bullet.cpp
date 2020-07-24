#include "bullet.h"
#include <math.h>

void Bullet::init(float _blx, float _bly, float _bvx, float _bvy, int _dmg, BClass bc, float speedFactor){
    fLife = 0.0f;

    blx = _blx;
    bly = _bly;
    bvx = _bvx;
    bvy = _bvy;

    // Calculate the unit vector 
    float norm = sqrt(bvx * bvx + bvy * bvy);
    if (norm == 0.0f) norm = 1; // if i did some dumb thing
    unitx = bvx / norm;
    unity = bvy / norm;
    bvx *= speedFactor/norm;
    bvy *= speedFactor/norm;

    blx += unitx;
    bly += unity;

    dmg = _dmg;
    mtype = bc;

    active = true;
}

void Bullet::update(float fElapsedTIme){
    if (active){
        blx += bvx * fElapsedTIme;
        bly += bvy * fElapsedTIme;

        // if out of bound, then deactivate.
        //if (blx > SCREEN_WIDTH || blx < 0) active = false;
        //if (bly > SCREEN_HEIGHT|| bly < 0) active = false;
        fLife += fElapsedTIme;
        if (fLife >= LIFETIME)
            active = false;
    }   
}
