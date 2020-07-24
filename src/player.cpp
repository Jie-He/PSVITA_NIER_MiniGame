#include "player.h"
#include "math.h"
#include <iostream>

void Player::update(int clx, int cly, int crx, int cry, bool fire, float fElapsedTime){

    // update the movement
    // Get moving velocity
    bool dx = (abs(clx - OFFSET_STICK) > OFFSET_DEADZ);
    bool dy = (abs(cly - OFFSET_STICK) > OFFSET_DEADZ);

    //if (dx) pvx += acceln * (clx - OFFSET_STICK) * fElapsedTime;
    //if (dy) pvy += acceln * (cly - OFFSET_STICK) * fElapsedTime;
    
    if (dx) pvx += (clx - OFFSET_STICK);
    if (dy) pvy += (cly - OFFSET_STICK);
    
    pvx = (abs(pvx) <= acceln)? 0.0f : pvx;
    pvy = (abs(pvy) <= acceln)? 0.0f : pvy;
    
    if (pvx != 0.0f && !dx) pvx -= ((pvx > 0)? acceln : -acceln) * fElapsedTime;
    if (pvy != 0.0f && !dy) pvy -= ((pvy > 0)? acceln : -acceln) * fElapsedTime;

    // Check max velocity bound
	pvx = (pvx > max_vel)?  max_vel : pvx;
	pvx = (pvx <-max_vel)? -max_vel : pvx;
	pvy = (pvy > max_vel)?  max_vel : pvy;
	pvy = (pvy <-max_vel)? -max_vel : pvy;

    // update player location
	plx += pvx * fElapsedTime;
	ply += pvy * fElapsedTime;	

    // now the direction
    bool dx2 = (abs(crx - OFFSET_STICK) > OFFSET_DEADZ);
    bool dy2 = (abs(cry - OFFSET_STICK) > OFFSET_DEADZ);

    // If either stick is above the deadzone offset, 
    // then change heading direction
    if (dx2 || dy2){
        dlx = crx - OFFSET_STICK;
        dly = cry - OFFSET_STICK;

        // reset direction counter if any
        direction_counter = 0;
    } else{
        // if dx or dy is not active for set amount of time
        // change to facing direction to moving direction

        direction_counter += fElapsedTime;

        if (direction_counter >= direction_change && (pvx || pvy)){
            direction_counter = direction_change;
            // Using a normalise function with factor 2.0f
            // factor is used to control the bullet spread
            normalise_vector(pvx, pvy, dlx, dly, 2.0f);            
        }

    }

    // check player bound
	plx = (plx < -20)? -20 : plx;
	ply = (ply < -20)? -20 : ply;
	plx = (plx >  20)?  20 : plx;
	ply = (ply >  20)?  20 : ply;


    // fire enabler
    crate = (crate >= frate)? frate : (crate + fElapsedTime); 
     // Checking fire button && update bullets
	for (int i = 0; i < MSIZE; i++) mag[i].update(fElapsedTime);

    if (fire) firebt();
}

void Player::firebt(){
    // if its re enabled, fire 
    // and mag has bullet
    if (crate == frate){

        // from the last free slot of mag, init another bullet
        int k = lastFree;
        
        while(mag[k].active){
            k++;
            if (k >= MSIZE) k = 0; 
            
            // if we made a loop and still not find it. then return
            if (k == lastFree) return;
        }
            
        // now we found a free bullet.
        float rp = -0.05f + (static_cast <float> (rand())) /( static_cast <float> (RAND_MAX/(0.1f)));
        float rq = -0.05f + (static_cast <float> (rand())) /( static_cast <float> (RAND_MAX/(0.1f)));

        mag[k].init(plx, ply, dlx + rp, dly + rq, 1, PLYBAR, 48.0f);
        // update last free
        lastFree = k;
        crate = 0.0f;
        //gSoloud.play(gWave); // play sound
    }
}

void Player::normalise_vector(float a, float b, float& a_out, float& b_out, 
                      float scale_factor = 1.0f){ // Optional scale factor to apply
    float common = std::sqrt( a * a + b * b );
    a_out = a / common * scale_factor;
    b_out = b / common * scale_factor;
};