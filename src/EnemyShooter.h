// ============================================================
// Base enemy shooter
// ============================================================
#ifndef _E_SHOOTER_
#define _E_SHOOTER_
// Does the basic following
#include "BaseActor.h"

enum ShootPattern{
    SHOOT_DEST, // fire all destructable bullets
    SHOOT_UDET, //  ''   '' undestroyable bullets
    SHOOT_MIXED // fire one of each in turn
};

class EnemyShooter : public BaseActor{
    protected:
        ShootPattern spPattern;
        BClass shootingType;
    public:
        EnemyShooter(Actor _aType, vec2d _vlocation, short _sHealth=3,
                     float _fMaxVel=5.0f, float _fFireRate = 0.01f, ShootPattern _sp=SHOOT_DEST);
        void update(float fElapsedTime, vec2d vPlayerloc);
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
};

#endif//_E_SHOOTER_