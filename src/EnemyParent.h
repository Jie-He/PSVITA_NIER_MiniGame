// ============================================================
// Enemy Parent
// Before damage can be done to this enemy,
// all its child enemies must be destroyed.
// ============================================================
#ifndef _E_PARENT_
#define _E_PARENT_
// Inherits shooter
#include "EnemyShooter.h"

class EnemyParent : public EnemyShooter{
    // Only need to override the take damage function and
    protected:
        // Gonna increase health for every child added too
        void call_parent_on_born() override {nLiveChildCount++; sHealth++;
            #ifdef OPENCV
                std::cout << "Child: " << nLiveChildCount << " reporting..." << std::endl;
            #endif  
        };
    public:
        EnemyParent(Actor _aType, vec2d _vLocation, short _sHealth=3,
                     float _fBSpeed=2.0f, float _fFireRate = 0.01f, 
                     ShootPattern _sp=SHOOT_DEST, vMesh* _vModel = nullptr, float _fAngV=0.785398)
                     : EnemyShooter(_aType, _vLocation, _sHealth, _fBSpeed, _fFireRate, _sp, _vModel, _fAngV)
                     {};
            
       ~EnemyParent(){};

        bool damage(short, Bullet) {
            #ifdef OPENCV
            std::cout << "Calling at Parent thou" << std::endl;
            #endif

            // If there are still children then just return true
            if (!nLiveChildCount) {
                // How about also increasing fire rate?
                fdFireControl.nCNLimit *= 0.5f; // 2x faster?
                return bAlive;
            }

            sHealth--; 
            if (sHealth <= 0) bAlive = false;
            return bAlive;
        };
        void call_parent_on_dead() override {nLiveChildCount--;
        #ifdef OPENCV
        std::cout << "One child died..." << std::endl;
        #endif    
        };

   // Override take damage
};

#endif//_E_PARENT_