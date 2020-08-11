// ============================================================
// Updated player defintion with inheritance from BaseActor.h
// ============================================================
#ifndef _PLAYER_AH_
#define _PlAYER_AH_

#include "BaseActor.h"
#include "bullet.h"

// Default stick offset
const float OFFSET_STICK = 128.0f;
const float OFFSET_DEADZ =  50.0f;

class APlayer : public BaseActor{
    protected:
        fDelay fdDirectionDelay;
        float  fAcceleration;

    public:
        APlayer(){};
       ~APlayer(){}; // ... Do nothing?

        APlayer(Actor _aType, vec2d _vecLocation, short _sHealth=3,
                float _max_vel=5.0f, float _frate=1.0f, vMesh* _vModel=nullptr){        
            
            aType     = _aType;
            sHealth   = _sHealth;
            fMaxVel   = _max_vel;
            vLocation = _vecLocation;
            vDirection= vec2d(0.0f, 128.0f);

            #ifdef OPENCV
            std::cout << "Init Player with Max Vel: " << _max_vel << std::endl;
            #endif

            vVelocity = vec2d(0.0f, 0.0f);
            vActorModel = _vModel;

            fdDirectionDelay.nCNLimit = 2.0f;
            fdFireControl.nCNLimit    = _frate;

            fAcceleration = fMaxVel / 5;
            bAlive = true;
        }

        // Need to call parent update method
        void update(int, int, int, int, bool, float);
        void firebt() override;

        bool damage(short dmg) override{
            sHealth-=dmg;
            bAlive = sHealth > 0;
            return bAlive;
        }
};

#endif//_PLAYER_AH_