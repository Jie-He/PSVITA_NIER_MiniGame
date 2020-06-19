#ifndef _BULLET_H_
#define _BULLET_H_

#include "defs.h"

class Bullet{
    public:
        float bspeed{20};
        float blx, bly;
        float bvx, bvy;

        BClass mtype;

        int dmg;
        bool active{false};

        // Initial lx  ly  direction x y
        void init(float, float, float, float, int, BClass);
        void update();
};
#endif//_BULLET_H_