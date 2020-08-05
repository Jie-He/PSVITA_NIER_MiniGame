#ifndef _BULLET_H_
#define _BULLET_H_

#include "../../VEngine/vector_matrix.h"
// Bullet variants
enum BClass{
    PLYBAR, // Player bullet type
    DESTBE, // Destoryable bullet ( enemy )
    UNDEST  // Undestoryable bullet
};

class Bullet{
    public:
        float bspeed;
        vec2d vPre; // Previous location
        vec2d vLoc;
        vec2d vVel;
        //float blx, bly;
        //float bvx, bvy;
        float unitx, unity;
        // temp 3 second life
        const float LIFETIME = 2.5f;
        float fLife;

        BClass mtype;

        int dmg;
        bool active{false};

        // Initial lx  ly  direction x y
        void init(float, float, float, float, int, BClass, float);
        void init(vec2d&, vec2d&, int, BClass, float);
        void update(float);
        bool intersect(Bullet&);
};
#endif//_BULLET_H_