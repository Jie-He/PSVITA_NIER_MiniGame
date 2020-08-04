#include "EnemyShooter.h"

EnemyShooter::EnemyShooter(Actor _aType, vec2d _vLocation, short _sHealth,
                           float _fMaxVel, float _fFireRate, ShootPattern _sp){
    aType       = _aType;
    vLocation   = _vLocation;
    sHealth     = _sHealth;
    fMaxVel     = _fMaxVel;
    // Always face forward;
    vDirection.y = -128;
    vDirection.x = 0;

    spPattern    = _sp;

    shootingType = DESTBE;
    if (spPattern == SHOOT_UDET) shootingType = UNDEST; 
    fFireRate = _fFireRate;
    bAlive = true;
}

void EnemyShooter::update(float fElapsedTime, vec2d vPlayerLoc){
    BaseActor::update(fElapsedTime);
    if (!bAlive) return;
    // Set the vDirection (shooting direction) to vPlayerLoc
    vDirection.x = vPlayerLoc.x - vLocation.x;
    vDirection.y = vPlayerLoc.y - vLocation.y;
    vDirection = vecNormalise(vDirection);

    // and try to fire at the player
    firebt();

    // Well it doesnt need to follow the player
    // Move at random?
    vVelocity.x = 0.01f;
    vVelocity.y = 0.01f;

}

// void EnemyShooter::firebt(){
//     // If its still reloading then dont fire
//     if (fFireRate != fReloadCount) return;
    
//     int k = nLastFreeMag;
    
//     while(bMag[k].active){
//         k++;
//         if (k >= MAG_SIZE) k = 0;
//         // Maximum number of bullet has fired
//         if (k == nLastFreeMag) return;
//     }

//     // Shoot at player direction with no rand()
//     bMag[k].init(vLocation, vDirection, 1, shootingType, 24.0f);
//     nLastFreeMag = k;
//     fReloadCount = 0;
//     // Change the bullet type if its mixed
//     if (spPattern == SHOOT_MIXED){
//         if(shootingType == UNDEST) shootingType = DESTBE;
//         else                       shootingType = UNDEST;
//     }
// }