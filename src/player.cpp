#include "player.h"

//void Player::update(int clx, int cly, int crx, int cry, float fElapsedTime){
void Player::update(SceCtrlData ctrl, float fElapsedTime){
    // update the movement
    // Get moving velocity
    bool dx = (abs(ctrl.lx - OFFSET_STICK) > OFFSET_DEADZ);
    bool dy = (abs(ctrl.ly - OFFSET_STICK) > OFFSET_DEADZ);

    if (dx) pvx += acceln * (ctrl.lx - OFFSET_STICK) * fElapsedTime;
    if (dy) pvy += acceln * (ctrl.ly - OFFSET_STICK) * fElapsedTime;

    pvx = (abs(pvx) <= acceln)? 0.0f : pvx;
    pvy = (abs(pvy) <= acceln)? 0.0f : pvy;
    
    if (pvx != 0.0f) pvx -= ((pvx > 0)? acceln : -acceln) * fElapsedTime;
    if (pvy != 0.0f) pvy -= ((pvy > 0)? acceln : -acceln) * fElapsedTime;

    // Check max velocity bound
	pvx = (pvx > max_vel)?  max_vel : pvx;
	pvx = (pvx <-max_vel)? -max_vel : pvx;
	pvy = (pvy > max_vel)?  max_vel : pvy;
	pvy = (pvy <-max_vel)? -max_vel : pvy;

    // update player location
	plx += pvx;
	ply += pvy;	

    // now the direction
    bool dx2 = (abs(ctrl.rx - OFFSET_STICK) > OFFSET_DEADZ);
    bool dy2 = (abs(ctrl.ry - OFFSET_STICK) > OFFSET_DEADZ);

    // If either stick is above the deadzone offset, 
    // then change heading direction
    if (dx2 || dy2){
        dlx = ctrl.rx - OFFSET_STICK;
        dly = ctrl.ry - OFFSET_STICK;

        // reset direction counter if any
        direction_counter = 0;
    } else{
        // if dx or dy is not active for set amount of time
        // change to facing direction to moving direction

        direction_counter += fElapsedTime;

        if (direction_counter >= direction_change && (dx || dy)){
            direction_counter = direction_change;
            // Using a normalise function with factor 20.0f
            // factor is used to control the bullet spread
            normalise_vector(pvx, pvy, dlx, dly, 100.0f);            
        }

    }

    // check player bound
	plx = (plx < 0)? 0 : plx;
	ply = (ply < 0)? 0 : ply;
	plx = (plx > SCREEN_WIDTH )? SCREEN_WIDTH  : plx;
	ply = (ply > SCREEN_HEIGHT)? SCREEN_HEIGHT : ply;

    // fire enabler
    crate = (crate > frate)? frate : (crate + 1); 

    // Checking fire button && update bullets
	for (int i = 0; i < MSIZE; i++) mag[i].update();
    // if fire button pressed. which is the R button
	if (ctrl.buttons & SCE_CTRL_RTRIGGER) firebt();
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
        // add some spread to the bullet direction
        float rp = -3.0f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(6.0f)));
        float rq = -3.0f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(6.0f)));
        mag[k].init(plx + rq, ply + rp, dlx + rp, dly + rq, 1, PLYBAR);
        // update last free
        lastFree = k;
        // reset shooting interval
        crate = 0;
    }
}