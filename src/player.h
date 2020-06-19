#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "defs.h"

class Player{
    public:
        float plx, ply; // current location
        float dlx{0}, dly{-127}; // current direction
	    float pvx{0}, pvy{0}; // velocity also the direction of point

	    int   health{10};   // health of player
        float max_vel;

	    const float acceln = 0.5f;
        float frate;

        // Default constructor
        Player(float _plx, float _ply, float _max_vel, float _frate)
               : plx(_plx), ply(_ply), max_vel(_max_vel), frate(_frate){};

        void update(int, int, int, int);
        bool damage(int);

        void firebt();
};

#endif //_PLAYER_H_