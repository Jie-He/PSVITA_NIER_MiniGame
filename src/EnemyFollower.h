// ============================================================
// Follower, follows the player
// ============================================================
#ifndef _E_FOLLOWER_
#define _E_FOLLOWER_
#include "Enemy.h"

class EnemyFollower : public Enemy{
    public:
        EnemyFollower(Actor _aType, vec2d _vLocation, 
                      short _sHealth=3, float _fMaxVel=5.0f, vMesh* _vModel = nullptr){
            aType       = _aType;
            vLocation   = _vLocation;
            sHealth     = _sHealth;
            fMaxVel     = _fMaxVel;
            vActorModel = _vModel;

            // Always face forward;
            vDirection.y = -128;
            vDirection.x = 0;

            bAlive = true;
        }

        void update(float fElapsedTime, vec2d vPlayerLoc) override{
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
        // Does no firing
        void firebt() override{
            // Do nothing
        };

        bool damage(short) override{
            sHealth--;
            if (sHealth <= 0) bAlive = false;
            return bAlive;
        };
};
#endif//_E_FOLLOWER;