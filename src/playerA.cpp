#include "playerA.h"

void APlayer::update(int clx, int cly, int crx, int cry, bool fire, float fElapsedTime){
    // update the movement
    // Get moving direction
    clx -= OFFSET_STICK;
    cly -= OFFSET_STICK;

    // if the analogue stick is above dead zone..
    bool valid = (sqrtf(clx * clx + cly * cly) >= OFFSET_DEADZ);


    //vVelocity.x = vVelocity.y = 0;
    if(valid){ // Update the velocity to input value according to fAcceleration
        vVelocity.x += (clx / OFFSET_STICK) * fAcceleration * 2;
        vVelocity.y += (cly / OFFSET_STICK) * fAcceleration * 2;

        if (vVelocity.length() > fMaxVel){
            vVelocity = vecNormalise(vVelocity);
            vVelocity *= fMaxVel;
        }
    }

    // Handling direction
    crx -= OFFSET_STICK;
    cry -= OFFSET_STICK;

    valid = (sqrtf(crx * crx + cry * cry) >= OFFSET_DEADZ);
    if (valid){
        vDirection.x = crx;
        vDirection.y = cry;

        // Reset direction delay since we just updated it
        fdDirectionDelay.reset();
    }else{
        // Tick the Direction Delay
        if(fdDirectionDelay.tick(fElapsedTime) && !fire){
            // If we are also moving. i.e have some velocity
            if(vVelocity.x || vVelocity.y){
                vDirection = vecNormalise(vVelocity);
                vDirection *= 128.0f;
            }
        }
    }    

    // Call base update
    BaseActor::update(fElapsedTime);

    // Deceleration
    // [TODO]
    if (vVelocity.length() < fAcceleration / 2){
        vVelocity.x = vVelocity.y = 0;
    }
    
    if (vVelocity.x != 0.0f) vVelocity.x -= (((vVelocity.x > 0)? fAcceleration : -fAcceleration)/2);
    if (vVelocity.y != 0.0f) vVelocity.y -= (((vVelocity.y > 0)? fAcceleration : -fAcceleration)/2);
    
    // Update bullet
    for(auto& b : bMag) b.update(fElapsedTime);
    if(fire) firebt();
}

void APlayer::firebt(){
    // if its re enabled, fire 
    // and mag has bullet
    if (!fdFireControl.isEnable()) return;
    fdFireControl.reset();

    // from the last free slot of mag, init another bullet
    int k = nLastFreeMag;
    
    while(bMag[k].active){
        k++;
        if (k >= MAG_SIZE) k = 0;
        // Maximum number of bullet has fired
        if (k == nLastFreeMag) return;
    }
    nLastFreeMag = k;        
    // now we found a free bullet.
    float rp = -5.0f + (static_cast <float> (rand())) /( static_cast <float> (RAND_MAX/(10.0f)));
    float rq = -5.0f + (static_cast <float> (rand())) /( static_cast <float> (RAND_MAX/(10.0f)));

    bMag[k].init(vLocation.x, vLocation.y, vDirection.x + rp, vDirection.y + rq,
                 1, BClass::PLYBAR, 50.0f);
    //gSoloud.play(gWave); // play sound
}