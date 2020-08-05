#ifndef _PLAYER_H_
#define _PLAYER_H_

//#include "defs.h"
#include "bullet.h"

//#include "soloud.h"
//#include "soloud_wav.h"

// Default stick offset;
const float OFFSET_STICK = 128.0f;
const float OFFSET_DEADZ =  50.0f;

class Player{
    public:
        float plx, ply; // current location
        float dlx{0}, dly{-128}; // current direction
	    float pvx{0}, pvy{0}; // velocity also the direction of point

	    int   health{10};   // health of player
        float max_vel;

	    float acceln;
        float frate;

        // set of bullets
        const int MSIZE = 25; // only allow player to have max 10 bullets
        Bullet mag[25];
        //int liveAmmo{0};    
        int lastFree{0};   

        // facing direction counter
        const float direction_change = 2.0f; // 2 seconds
        float direction_counter{0.0f}; 

        // Default constructor
        Player(float _plx, float _ply, float _max_vel, float _frate)
               : plx(_plx), ply(_ply), max_vel(_max_vel), frate(_frate){
                   //gSoloud.init();
                   //gWave.load("app0:/audio/laser.ogg");
                   acceln = _max_vel/3;
               };

        void update(int, int, int, int, bool, float);
        bool damage(int);

        void firebt();
    private: 
        float crate{0};
        //SoLoud::Soloud gSoloud;
        //SoLoud::Wav gWave;
        const int directionThresh = 180;
        // Normalise a vector, assuiming the other end is at 0, 0
        void normalise_vector(float, float, float&, float& , float);

};

#endif //_PLAYER_H_