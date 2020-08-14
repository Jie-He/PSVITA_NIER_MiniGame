// A virtual class for all enemies to inherit
#ifndef _E_ENEMY_
#define _E_ENEMY_
#include <memory>
#include "BaseActor.h"

class Enemy : public BaseActor{
    protected:
        float fAngularV;
        float fSinAV;
        float fCosAV;

        // Errrrrr, Pointer to Smart Pointer
        std::unique_ptr<Enemy>* eParent;
        int nLiveChildCount;

        virtual void call_parent_on_born(){ nLiveChildCount++; };

    public:
        void update(float fElapsedTime){ BaseActor::update(fElapsedTime);};
        void point_player(float fElapsedTime, vec2d& vloc){
            // Experiment, vector point to player
            vec2d temp = vloc - vLocation;
            // Calc the smallest radian
            float fTheta = std::acos(temp.dot(vDirection) /\
                           (temp.length() * vDirection.length()));
            // A negative angle would only affect fSinAV. so multiply by this factor
            float sign   = (temp.cross(vDirection) > 0)? -1 : 1;

            // Update by fElapsedTime
            fCosAV = cosf(fAngularV * fElapsedTime);
            fSinAV = sinf(fAngularV * fElapsedTime) * sign;

            vDirection = vec2d(vDirection.x * fCosAV - vDirection.y * fSinAV,
                               vDirection.x * fSinAV + vDirection.y * fCosAV);
            
            // Normalise it before sending it out
            vDirection = vecNormalise(vDirection);
        }

        void set_parent(std::unique_ptr<Enemy>* ep){ eParent = ep; (*ep)->call_parent_on_born();};

        virtual void update(float fElapsedTime, BaseActor& vPlayer){};
        virtual void firebt(){};
        // shall supply a bullet for shield and things etc.
        virtual bool damage(short, Bullet& b){ return false;};
        // For child enemies to call to parent when dead.
        virtual void call_parent_on_dead(){ nLiveChildCount--; };
    
};

#endif//_E_ENEMY_