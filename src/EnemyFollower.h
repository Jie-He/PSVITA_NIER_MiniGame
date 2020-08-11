// ============================================================
// Follower, follows the player
// ============================================================
#ifndef _E_FOLLOWER_
#define _E_FOLLOWER_
#include "Enemy.h"

class EnemyFollower : public Enemy{
    private:
        bool do_damage(){
            sHealth--; 
            if (sHealth <= 0) bAlive = false;
            return bAlive;
        }

    public:
        EnemyFollower(Actor _aType, vec2d _vLocation, 
                      short _sHealth=3, float _fMaxVel=5.0f, vMesh* _vModel = nullptr, float _fAngV = 0.785398){
            aType       = _aType;
            vLocation   = _vLocation;
            sHealth     = _sHealth;
            fMaxVel     = _fMaxVel;
            fAngularV   = _fAngV;
            vActorModel = _vModel;

            // Always face forward;
            vDirection.y = -128;
            vDirection.x = 0;

            bAlive = true;
        }

        void update(float fElapsedTime, BaseActor& vPlayer) override{
            BaseActor::update(fElapsedTime);
    
            if (!bAlive) return;
            // set the vDirection to vPlayer
            point_player( fElapsedTime, vPlayer.getLocation());
            // Also set velocity to player
            vVelocity  = vDirection;
            // Scale the velocity to fMaxVel
            vVelocity *= fMaxVel;
        }
        // Does no firing
        void firebt() override{
            // Do nothing
        };

        bool damage(short k, Bullet& b) override{
            // Doesnt need Bullet info
            if (aType == Actor::FOLLOWER){
                do_damage();
            }else{ // A Hammer Head type
                // Calc bullet angle
                vec2d temp = b.vVel;
                // Calc the smallest radian
                float fTheta = std::acos(temp.dot(vDirection) / (temp.length() * vDirection.length()));
                
                // if the bullet enters at greater than 45 degrees
                // do damage
                if (std::abs(fTheta) <= 2.356194) 
                    do_damage();
            }
            return bAlive;
        };
};
#endif//_E_FOLLOWER_;