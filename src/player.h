#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "defs.h"
#include "bullet.h"

class Player{
    public:
        float plx, ply; // current location
        float dlx{0}, dly{-127}; // current direction
	    float pvx{0}, pvy{0}; // velocity also the direction of point

	    int   health{10};   // health of player
        float max_vel;

	    const float acceln{0.5f};
        int frate;

        // set of bullets
        const int msize = 15; // only allow player to have max 10 bullets
        Bullet mag[15];
        //int liveAmmo{0};    
        int lastFree{0};    

        // Default constructor
        Player(float _plx, float _ply, float _max_vel, int _frate)
               : plx(_plx), ply(_ply), max_vel(_max_vel), frate(_frate){
                   // init the bullets
                   for(int i = 0; i < msize; i++) mag[i] = Bullet();
               };

        void update(int, int, int, int);
        bool damage(int);

        void firebt();
    private: 
        int crate{0};
};

#endif //_PLAYER_H_