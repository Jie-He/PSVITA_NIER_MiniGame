#include "defs.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"

#include <chrono>
#include <ctime>

// Globa vals... for now
SceCtrlData   ctrl;
vita2d_font  *font;
enum GAME_STATE GSTATE;

Player player(480.0f, 272.0f, 8.0f, 6);
//Enemy  enemy1(300.0f, 200.0f, 8.0f, 6);

void menu(){
}

// function to update the player and bots
void update(float fElapsedTime){
	
	sceCtrlPeekBufferPositive(0, &ctrl, 1);

	//player.update(ctrl.lx, ctrl.ly, ctrl.rx, ctrl.ry, fElapsedTime);
	player.update(ctrl, fElapsedTime);

	// if x key pressed switch GSTATE between running and pause
	if (ctrl.buttons & SCE_CTRL_CROSS)
	{
		if (GSTATE == RUNNING)
			GSTATE =  PAUSED;
		else
			GSTATE =  RUNNING;
		sceKernelDelayThread(500000);
	}
}

// Draw some text to render, for debugging
// Cause alot of lag btw..
void drop_stats(){
	char buff[30];
	vita2d_font_draw_text(font, 10, 10, CRIMSON, 11, "Player stats: ");
	sprintf(buff, "PVX: %6.3f", player.pvx);
	vita2d_font_draw_text(font, 10, 25, WHITE, 11, buff);
	
	sprintf(buff, "PVY: %6.3f", player.pvy);
	vita2d_font_draw_text(font, 10, 40, WHITE, 11, buff);

	sprintf(buff, "PLX: %6.3f", player.plx);
	vita2d_font_draw_text(font, 10, 55, WHITE, 11, buff);

	sprintf(buff, "PLY: %6.3f", player.ply);
	vita2d_font_draw_text(font, 10, 70, WHITE, 11, buff);

	vita2d_font_draw_text(font, 128, 10, CRIMSON, 11, "Analogue stick: ");
	sprintf(buff, "OLX, LX: %d", ctrl.lx);
	vita2d_font_draw_text(font, 128, 25, WHITE, 11, buff);
	
	sprintf(buff, "OLY, LY: %d", ctrl.ly);
	vita2d_font_draw_text(font, 128, 40, WHITE, 11, buff);

	sprintf(buff, "ORX, RX :%d", ctrl.rx);
	vita2d_font_draw_text(font, 128, 55, WHITE, 11, buff);

	sprintf(buff, "ORY, RY: %d", ctrl.ry);
	vita2d_font_draw_text(font, 128, 70, WHITE, 11, buff);
}

// Draw game objects with Vita2D graphics. 
// Update later with Crappy Engine 3D
void render(float fFPS){
	// begin drawing
	vita2d_start_drawing();
	vita2d_clear_screen();

	// Display FPS
	char buff[16];
	sprintf(buff, "FPS: %6.3f", fFPS);
	vita2d_font_draw_text(font, 860, 20, WHITE, 18, buff);
	
	if (GSTATE == RUNNING)	drop_stats();

	if (ctrl.buttons & SCE_CTRL_RTRIGGER) // if shoot. change the circle colour for now
			vita2d_draw_fill_circle(player.plx, player.ply, 16, CRIMSON);
	else	vita2d_draw_fill_circle(player.plx, player.ply, 16, WHITE);

	vita2d_draw_line(player.plx, player.ply,
					 player.plx + 0.2f * player.dlx, 
					 player.ply + 0.2f * player.dly, WHITE);

	// draw bullets
	for (int i = 0; i < player.MSIZE; i++){
		if (player.mag[i].active){
			vita2d_draw_fill_circle(player.mag[i].blx,player.mag[i].bly, 4, CRIMSON);
		}
	}

	// end drawing
	vita2d_end_drawing();
	vita2d_swap_buffers();
}

// Free all resources loaded by vita2d
void free_vita2d(){
	vita2d_fini();;
	vita2d_free_font(font); 
}

int main(int argc, char *argv[]) {
	//psvDebugScreenInit();
	//printf("input test\n");
	//printf("press Select+Start+L+R to stop\n");
	/* to enable analog sampling */
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
	// Init vita2d
	vita2d_init();
	vita2d_set_clear_color(BLACK);
	font = vita2d_load_font_mem(basicfont, basicfont_size);

	GSTATE = RUNNING;

	// maybe get an clock ticking?
	auto cTimeStamp = std::chrono::system_clock::now();
    auto cTimeCurrent = std::chrono::system_clock::now();

    float fElapsedTime = 0.0f; // in seconds?
    float fFPS = 0.0f;
	do{
 		// calc elasped time since last loop
        cTimeCurrent = std::chrono::high_resolution_clock::now();
        // elapsed in seconds
        fElapsedTime = (std::chrono::duration<float>(cTimeCurrent - cTimeStamp).count()); 
        // reset time stamp
        cTimeStamp = cTimeCurrent;
        // Conversion && FPS
        fFPS = 1 / fElapsedTime;

		update(fElapsedTime);
		render(fFPS);
	}while(1);

	free_vita2d();
	sceKernelExitProcess(0);
	return 0;
}