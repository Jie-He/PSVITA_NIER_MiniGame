// ============================================================
// Wall definition
// Three types: Black, White, Red
// - Black wall can be destoryed
// - White wall is solid
// - Read  wall is solid and inflict damage to player 
// ============================================================
#ifndef _WALL_H_
#define _WALL_H_

#include "../../VEngine/vMesh.h"

// Defining the three wall types
enum WALL_TYPE{
    BLC_WALL,
    WHT_WALL,
    RED_WALL
};

class Wall{
    protected:
        WALL_TYPE wtp;
        short sHealth;
        vMesh vWallMh;
        vec2d vLocatn;

    public:

        Wall(){};
       ~Wall(){};
        Wall(WALL_TYPE _wtp, vMesh _vWallMh, vec3d _vLocatn, short _sHealth=3.0f)
            : wtp(_wtp), sHealth(_sHealth), vWallMh(_vWallMh) {

            vWallMh.ApplyTranslation(_vLocatn);
            // Extract the 2D coord, Y is ignore since its a 2D based game
            vLocatn = vec2d(_vLocatn.x, _vLocatn.z);
        };
        
        bool isAlive() { return (    sHealth >  0); };
        // Only react to player ?
        bool damage()  { return ((--sHealth) <= 0); };
        
        // Get the wall mesh
        vMesh* getMesh() { return &vWallMh; };
};
#endif//_WALL_H_