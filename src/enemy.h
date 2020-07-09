#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "player.h"

class Enemy : public Player{
    public:
        /**
        // Default constructor
        Enemy(float _plx, float _ply, float _max_vel, int _frate)
             : plx(_plx), ply(_ply), max_vel(_max_vel), frate(_frate){
                // init the bullets
                for(int i = 0; i < MSIZE; i++) mag[i] = Bullet();
        }; **/

        void update(SceCtrlData ctrl, float fElapsedTime){
            // Not gonna need ctrl, since its a bot. 
            // maybe to cheat on player's actions
        }

        void damage(int);
    private:
        void firebt();
};

#endif //_ENEMY_H_