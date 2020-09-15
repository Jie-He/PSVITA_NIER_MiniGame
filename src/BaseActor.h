// ============================================================
// Base Player/Enemy/Obstactle defn
// Does basic movement around the map at most
// Includes stuff for shooting
// ============================================================
#ifndef _BASE_ACTOR_H_
#define _BASE_ACTOR_H_

// Get vec2d && some utils
#include "../../VEngine/vector_matrix.h"
#include "../../VEngine/vMesh.h"
#include "bullet.h"

// Probably a good idea to keep track of the enemy type
enum Actor{
    // Yh. Player.
    PLAYER,

    // Both using EnemyFollower class
    FOLLOWER,   // Does noting but follow the player
    HAMMERHEAD, // Same as Follower but can only be killed from behind

    // Both using EnemyShooter class
    SNIPER,     // Spit one bullet each time
    SHOTGUN     // Spit three bullet at once ? or more
    // To be continued...
    
    // This Aircraft carrier thingy. spawns child enemies initially
    // Cant not be damaged until all children are destoryed
};

// A delay handler
// e.g. allow shooting every one second
template <class T>
struct Delay{
    T nCounter;
    T nCNLimit;  // should be larger than counter

    // Default 0 and 1 as values.
    Delay(){nCounter=0; nCNLimit=1;};
    Delay(T _nc, T _nl): nCounter(_nc), nCNLimit(_nl){};
    Delay(const Delay& t) {nCounter = t.nCounter; nCNLimit=t.nCNLimit;};
    
    bool tick(T cycle){
        nCounter = std::min(nCounter + cycle, nCNLimit);
        return (nCounter == nCNLimit);
    }

    bool isEnable() {return nCounter == nCNLimit;};
    void reset()    {nCounter = 0;};
};

typedef Delay<float> fDelay;

class BaseActor{

    protected:
        // Base information
        Actor  aType;
        short  sHealth;
        //float  fFireRate;
        //float  fReloadCount;
        fDelay fdFireControl;
        float  fMaxVel;
        bool   bAlive;
        // Since its movement its 2D. lets just use the 2D vec
        vec2d  vLocation;
        vec2d  vDirection;
        vec2d  vVelocity;
        vMesh* vActorModel;

    public:
        // Bullet stuff, 
        const int MAG_SIZE = 64; // [TODO Changing size]
        Bullet bMag[64];
        short nLastFreeMag{0};
    
        BaseActor() {bAlive = true; vActorModel = nullptr;};
       ~BaseActor(){};
        void update(float fElapsedTime){ 
            // Update the bullets even if dead.
            for (auto& b : bMag) b.update(fElapsedTime);

            if (!bAlive) return;
            // update location
            vLocation.x += vVelocity.x * fElapsedTime;
            vLocation.y += vVelocity.y * fElapsedTime;

            // TEMP check bound
            vLocation.x = (vLocation.x < -20)? -20 : vLocation.x;
            vLocation.y = (vLocation.y < -20)? -20 : vLocation.y;
            vLocation.x = (vLocation.x >  20)?  20 : vLocation.x;
            vLocation.y = (vLocation.y >  20)?  20 : vLocation.y;

            // fire enabler
            //fReloadCount = (fReloadCount >= fFireRate)? fFireRate : (fReloadCount + fElapsedTime);
            fdFireControl.tick(fElapsedTime);
        };        
        
        virtual bool damage(short){ return false;};
        virtual void firebt(){};

        // Getters
        vec2d& getLocation()    {return vLocation;};
        vec2d& getDirection()   {return vDirection;};
        vMesh* getActorModel()  {return vActorModel;};
        Actor& getActorType()   {return aType;};
        bool   isAlive()        {return bAlive;};
        short  getHealth()      {return sHealth;};
};

#endif//_BASE_ACTOR_H_