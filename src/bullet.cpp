#include "bullet.h"
#include <math.h>

void Bullet::init(vec2d& _loc, vec2d& _dir, int _dmg, BClass bc, float speedFactor){
    //init(_loc.x, _loc.y, dir.x, dir.y, dmg, bc, speedFactor);
    fLife = 0.0f;

    vLoc = _loc;
    vVel = _dir;

    // Calculate the unit vector 
    vVel = vecNormalise(vVel);
    // Shift the bullet forward a little
    vPre = _loc;
    vLoc += vVel;
    // Scale the bullet speed up
    vVel *= speedFactor;
    
    dmg = _dmg;
    mtype = bc;

    active = true;        
}

void Bullet::init(float _blx, float _bly, float _bvx, float _bvy, int _dmg, BClass bc, float speedFactor){
    vec2d _loc = vec2d(_blx, _bly);
    vec2d _dir = vec2d(_bvx, _bvy);
    init(_loc, _dir, _dmg, bc, speedFactor);
}

void Bullet::update(float fElapsedTIme){
    if (active){
        vPre = vLoc;
        vLoc += (vVel * fElapsedTIme);
        // if out of bound, then deactivate.
        if (vLoc.y > 20 || vLoc.y < -20) active = false;
        if (vLoc.x > 20 || vLoc.x < -20) active = false;
        fLife += fElapsedTIme;
        if (fLife >= LIFETIME)
            active = false;
    } 
}
// On GEEK4GEEK
// Check if two line segements are intersecting

// Check if point q lies on line p - r
bool onSegment(vec2d& p, vec2d& q, vec2d& r) 
{ 
    using namespace std;
    if (q.x <= max(p.x, r.x) && q.x <= min(p.x, r.x) && 
        q.y <= max(p.y, r.y) && q.y <= min(p.y, r.y)) 
       return true; 
   
  
    return false; 
} 

// Determine the orientation of the 3 points
int orientation(vec2d& p, vec2d& q, vec2d& r){ 
    int val = (q.y - p.y) * (r.x - q.x) - 
              (q.x - p.x) * (r.y - q.y); 
  
    if (val == 0) return 0;  // colinear 
  
    return (val > 0)? 2: 1; // clock or counterclock wise 
} 

// If the path of two bullets intersects
bool Bullet::intersect(Bullet& target){
    
    int o1 = orientation(vLoc, vPre, target.vLoc); 
    int o2 = orientation(vLoc, vPre, target.vPre); 
    int o3 = orientation(target.vLoc, target.vPre, vLoc); 
    int o4 = orientation(target.vLoc, target.vPre, vPre); 
  
    if (o1 != o2 && o3 != o4) 
        return true; 
  
    if (o1 == 0 && onSegment(vLoc, vPre, target.vLoc)) return true; 
  
    if (o2 == 0 && onSegment(vLoc, vPre, target.vPre)) return true; 
  
    if (o3 == 0 && onSegment(target.vLoc, target.vPre, vLoc)) return true; 
  
    if (o4 == 0 && onSegment(target.vLoc, target.vPre, vPre)) return true; 
  
    return false; 
}
