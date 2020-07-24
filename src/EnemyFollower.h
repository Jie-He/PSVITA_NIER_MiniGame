// ============================================================
// Follower, follows the player
// ============================================================
#ifndef _E_FOLLOWER_
#define _E_FOLLOWER_
#include "BaseActor.h"

class EnemyFollower : public BaseActor{
    public:
        EnemyFollower(Actor _aType, vec2d _vLocation, short _sHealth=3, float _fMaxVel=5.0f);

        void update(float fElapsedTime, vec2d vPlayerLoc);
        // Does no firing
        void firebt() override{
            // Do nothing
        };
};
#endif//_E_FOLLOWER;