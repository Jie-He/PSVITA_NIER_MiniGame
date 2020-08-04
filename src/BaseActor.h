// ============================================================
// Base Player/Enemy/Obstactle defn
// Does basic movement around the map at most
// Includes stuff for shooting
// ============================================================
#ifndef _BASE_ACTOR_H_
#define _BASE_ACTOR_H_
#include "../../VEngine/vector_matrix.h"
#include "bullet.h"

// Probably a good idea to keep track of the enemy type
enum Actor{
    // lol Names... errrr
    PLAYER,

    FOLLOWER, // Does noting but follow the player
    SNIPER,   // Spit one bullet each time
    SHOTGUN  // Spit three bullet at once ? or more
    // To be continued...
};

class BaseActor{

    protected:
        // Base information
        Actor  aType;
        short  sHealth;
        float  fFireRate;
        float  fReloadCount;
        float  fMaxVel;
        bool   bAlive;
        // Since its movement its 2D. lets just use the 2D vec
        vec2d  vLocation;
        vec2d  vDirection;
        // Maybe not
        vec2d  vVelocity;

    public:
        // Bullet stuff, 
        const int MAG_SIZE = 15; // [TODO Changing size]
        Bullet bMag[15];
        short nLastFreeMag{0};
    
    /**    BaseActor(Actor _aType, vec2d _vLocation, short _sHealth=3, float _fFireRate=1.0f, float _fMaxVel=5.0f)
                 : aType(_aType), sHealth(_sHealth), fFireRate(_fFireRate), fMaxVel(_fMaxVel){
                     // Default direction pointing forward
                     vDirection.x = 0; vDirection.y = -128;
                     bAlive = true;
                 }; **/
        BaseActor() {bAlive = true;};

        void update(float fElapsedTime){ 
            // Update the bullets even if dead.
            for (int i = 0; i < MAG_SIZE; i++) bMag[i].update(fElapsedTime);

            if (!bAlive) return;
            // update location
            vLocation.x += vVelocity.x * fElapsedTime;
            vLocation.y += vVelocity.y * fElapsedTime;

            // and the bullet
            // fire enabler
            fReloadCount = (fReloadCount >= fFireRate)? fFireRate : (fReloadCount + fElapsedTime);
            
        };        
        
        bool damage(short){
            sHealth--;
            if (sHealth <= 0) bAlive = false;
            return bAlive;
        };
        virtual void firebt(){};

        // Getters
        vec2d& getLocation() {return vLocation;};
        vec2d& getDirection(){return vDirection;};
    
        bool isAlive() {return bAlive;};
};

#endif//_BASE_ACTOR_H_