#ifndef _BULLET_H_
#define _BULLET_H_

class Bullet{
    public:
        float bspeed{10};
        float blx, bly;
        float bvx, bvy;

        // Initial lx  ly  direction x y
        Bullet(float, float, float, float){};
        void update();
};
#endif//_BULLET_H_