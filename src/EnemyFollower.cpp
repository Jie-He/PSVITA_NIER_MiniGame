#include "EnemyFollower.h"

EnemyFollower::EnemyFollower(Actor _aType, vec2d _vLocation, 
                             short _sHealth, float _fMaxVel){
    aType       = _aType;
    vLocation   = _vLocation;
    sHealth     = _sHealth;
    fMaxVel     = _fMaxVel;
    // Always face forward;
    vDirection.y = -128;
    vDirection.x = 0;

    bAlive = true;
};

void EnemyFollower::update(float fElapsedTime, vec2d vPlayerLoc){    
    // Call base update to move
    BaseActor::update(fElapsedTime);
    
    if (!bAlive) return;
    // set the vDirection to vPlayer
    vDirection.x = vPlayerLoc.x - vLocation.x;
    vDirection.y = vPlayerLoc.y - vLocation.y;
    vDirection = vecNormalise(vDirection);
    // Also set velocity to player
    vVelocity.x  = vDirection.x;
    vVelocity.y  = vDirection.y;
    // Scale the velocity to fMaxVel
    vVelocity.x *= fMaxVel;
    vVelocity.y *= fMaxVel;


}