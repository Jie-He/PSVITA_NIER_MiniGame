#include "player.h"

void Player::update(int clx, int cly, int crx, int cry){

    // update the movement
    // Get moving velocity
    bool dx = (abs(clx - OFFSET_STICK) > OFFSET_DEADZ);
    bool dy = (abs(cly - OFFSET_STICK) > OFFSET_DEADZ);

    if (dx) pvx += acceln * (clx - OFFSET_STICK);
    if (dy) pvy += acceln * (cly - OFFSET_STICK);

    pvx = (abs(pvx) <= acceln)? 0.0f : pvx;
    pvy = (abs(pvy) <= acceln)? 0.0f : pvy;
    
    if (pvx != 0.0f) pvx -= ((pvx > 0)? acceln : -acceln);
    if (pvy != 0.0f) pvy -= ((pvy > 0)? acceln : -acceln);

    // Check max velocity bound
	pvx = (pvx > max_vel)?  max_vel : pvx;
	pvx = (pvx <-max_vel)? -max_vel : pvx;
	pvy = (pvy > max_vel)?  max_vel : pvy;
	pvy = (pvy <-max_vel)? -max_vel : pvy;

    // update player location
	plx += pvx;
	ply += pvy;	

    // now the direction
    dx = (abs(crx - OFFSET_STICK) > OFFSET_DEADZ);
    dy = (abs(cry - OFFSET_STICK) > OFFSET_DEADZ);

    if (dx || dy){
        dlx = crx - OFFSET_STICK;
        dly = cry - OFFSET_STICK;
    }

    // check player bound
	plx = (plx < 0)? 0 : plx;
	ply = (ply < 0)? 0 : ply;
	plx = (plx > SCREEN_WIDTH )? SCREEN_WIDTH  : plx;
	ply = (ply > SCREEN_HEIGHT)? SCREEN_HEIGHT : ply;

   
}
