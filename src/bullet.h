#ifndef _BULLET_H_
#define _BULLET_H_

//#include "defs.h"
// Bullet variants
enum BClass{
    PLYBAR, // Player bullet type
    DESTBE, // Destoryable bullet ( enemy )
    UNDEST  // Undestoryable bullet
};

class Bullet{
    public:
        float bspeed{20};
        float blx, bly;
        float bvx, bvy;
        float unitx, unity;
        // temp 3 second life
        const float LIFETIME = 1.5f;
        float fLife;

        BClass mtype;

        int dmg;
        bool active{false};

        // Initial lx  ly  direction x y
        void init(float, float, float, float, int, BClass, float);
        void update(float);
};
#endif//_BULLET_H_