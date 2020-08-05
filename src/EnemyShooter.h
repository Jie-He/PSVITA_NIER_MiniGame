// ============================================================
// Base enemy shooter
// ============================================================
#ifndef _E_SHOOTER_
#define _E_SHOOTER_
// Does the basic following
#include "Enemy.h"

enum ShootPattern{
    SHOOT_DEST, // fire all destructable bullets
    SHOOT_UDET, //  ''   '' undestroyable bullets
    SHOOT_MIXED // fire one of each in turn
};

class EnemyShooter : public Enemy{
    protected:
        ShootPattern spPattern;
        BClass shootingType;
    public:
        EnemyShooter(Actor _aType, vec2d _vLocation, short _sHealth=3,
                     float _fMaxVel=5.0f, float _fFireRate = 0.01f, ShootPattern _sp=SHOOT_DEST, vMesh* _vModel = nullptr){
        aType       = _aType;
        vLocation   = _vLocation;
        sHealth     = _sHealth;
        fMaxVel     = _fMaxVel;
        vActorModel = _vModel;

        // Always face forward;
        vDirection.y = -128;
        vDirection.x = 0;

        spPattern    = _sp;

        shootingType = DESTBE;
        if (spPattern == SHOOT_UDET) shootingType = UNDEST; 
        fFireRate = _fFireRate;
        bAlive = true;
        }
        
        void update(float fElapsedTime, vec2d vPlayerLoc) override{
            Enemy::update(fElapsedTime);
            
            if (!bAlive) return;
            // Set the vDirection (shooting direction) to vPlayerLoc
            vDirection.x = vPlayerLoc.x - vLocation.x;
            vDirection.y = vPlayerLoc.y - vLocation.y;
            vDirection = vecNormalise(vDirection);

            // and try to fire at the player
            firebt();

            // Well it doesnt need to follow the player
            // Move at random?
            vVelocity.x += (float)(rand() % 20 - 10) * fElapsedTime;
            vVelocity.y += (float)(rand() % 20 - 10) * fElapsedTime;
            vVelocity = vecNormalise(vVelocity);
        }

        void firebt() override{
            // If its still reloading then dont fire
            if (fFireRate != fReloadCount) return;
            
            int k = nLastFreeMag;
            
            while(bMag[k].active){
                k++;
                if (k >= MAG_SIZE) k = 0;
                // Maximum number of bullet has fired
                if (k == nLastFreeMag) return;
            }

            // Shoot at player direction with no rand()
            bMag[k].init(vLocation, vDirection, 1, shootingType, 24.0f);
            nLastFreeMag = k;
            fReloadCount = 0;
            // Change the bullet type if its mixed
            if (spPattern == SHOOT_MIXED){
                if(shootingType == UNDEST) shootingType = DESTBE;
                else                       shootingType = UNDEST;
            }
        };

        bool damage(short) override{
            sHealth--;
            if (sHealth <= 0) bAlive = false;
            return bAlive;
        };
};

#endif//_E_SHOOTER_