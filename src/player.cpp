#include "player.h"

void Player::update(int clx, int cly, int crx, int cry){

    // update the movement
    // Get moving velocity
	pvx = (pvx + (abs(clx - OFFSET_STICK) > OFFSET_DEADZ)? clx - OFFSET_STICK:0);
	pvx = (abs(pvx) < 0.2f)? 0 : pvx;
	pvx = (pvx != 0)? pvx - (abs(pvx) / pvx) * acceln : 0;

    pvy = (pvy + (abs(cly - OFFSET_STICK) > OFFSET_DEADZ)? cly - OFFSET_STICK:0);
	pvy = (abs(pvy) < 0.2f)? 0 : pvy;
	pvy = (pvy != 0)? pvy - (abs(pvy) / pvy) * acceln : 0;

    // Check max velocity bound
	pvx = (pvx > max_vel)?  max_vel : pvx;
	pvx = (pvx <-max_vel)? -max_vel : pvx;
	pvy = (pvy > max_vel)?  max_vel : pvy;
	pvy = (pvy <-max_vel)? -max_vel : pvy;

    // update player location
	plx += pvx;
	ply += pvy;	

    // now the direction
    bool dx(abs(crx - OFFSET_STICK) > OFFSET_DEADZ);
    bool dy(abs(cry - OFFSET_STICK) > OFFSET_DEADZ);

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
