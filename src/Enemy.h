// A virtual class for all enemies to inherit
#ifndef _E_ENEMY_
#define _E_ENEMY_
#include "BaseActor.h"

class Enemy : public BaseActor{
    public:
        void update(float fElapsedTime){ BaseActor::update(fElapsedTime);};
        virtual void update(float fElapsedTime, vec2d vPlayerLoc){};
        virtual void firebt(){};
        virtual bool damage(short){ return false;};
};

#endif//_E_ENEMY_