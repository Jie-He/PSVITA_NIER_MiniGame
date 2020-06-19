#include "Bullet.h"
#include <math.h>

void Bullet::init(float _blx, float _bly, float _bvx, float _bvy, int _dmg, BClass bc){
    blx = _blx;
    bly = _bly;
    bvx = _bvx;
    bvy = _bvy;

    // Calculate the unit vector 
    float norm = sqrt(bvx * bvx + bvy * bvy);
    if (norm == 0.0f) norm = 1; // if i did some dumb thing
    norm = norm / 24;
    bvx /= norm;
    bvy /= norm;

    dmg = _dmg;
    mtype = bc;

    active = true;
}

void Bullet::update(){
    if (active){
        blx += bvx;
        bly += bvy;

        // if out of bound, then deactivate.
        if (blx > SCREEN_WIDTH || blx < 0) active = false;
        if (bly > SCREEN_HEIGHT|| bly < 0) active = false;
    }   
}