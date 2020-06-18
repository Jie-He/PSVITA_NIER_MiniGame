#include "defs.h"
#include "player.h"
// Globa vals... for now
SceCtrlData   ctrl;
vita2d_font *font;
enum GAME_STATE GSTATE;

Player player(480.0f, 272.0f, 6.4f, 3.0f);; 

void menu(){
}

// function to update the player and bots
void update(){
	
	sceCtrlPeekBufferPositive(0, &ctrl, 1);

	player.update(ctrl.lx, ctrl.ly, ctrl.rx, ctrl.ry);

	// if x key pressed switch GSTATE between running and pause
	if (ctrl.buttons & SCE_CTRL_CROSS)
	{
		if (GSTATE == RUNNING)
			GSTATE =  PAUSED;
		else
			GSTATE =  RUNNING;
		sceKernelDelayThread(100000);
	}
}

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

void render(){
	// begin drawing
	vita2d_start_drawing();
	vita2d_clear_screen();

	//vita2d_font_draw_text(font, 350, 270, WHITE, 24, "Nier Automata - Hacking!");
	
	if (GSTATE == RUNNING)	drop_stats();

	vita2d_draw_fill_circle(player.plx, player.ply, 16, CRIMSON);
	vita2d_draw_line(player.plx, player.ply,
					 player.plx + 0.2f * player.dlx, 
					 player.ply + 0.2f * player.dly, WHITE);

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
	psvDebugScreenInit();
	printf("input test\n");
	printf("press Select+Start+L+R to stop\n");
	/* to enable analog sampling */
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);

	sceCtrlPeekBufferPositive(0, &ctrl, 1);

	// Init vita2d
	vita2d_init();
	vita2d_set_clear_color(BLACK);
	font = vita2d_load_font_mem(basicfont, basicfont_size);

	GSTATE = RUNNING;
	do{
		update();
		render();
		
		//printf("Buttons:%08X == ", ctrl.buttons);
		//printf("\e[m Stick:[%3i:%3i][%3i:%3i]\r\n", ctrl.lx,ctrl.ly, ctrl.rx,ctrl.ry);

		// Print some player information
		//printf("\ePlayer Location: [%d:%d]\r", player.plx, player.ply);
		//printf("  | - -Velocity: [%d:%d]\n", player.pvx, player.pvy);
		//printf("  | - - -Health: %d \n", player.health);
		//printf("  | - fire rate: %lf\n", player.frate );
	}while(1);

	free_vita2d();
	sceKernelExitProcess(0);
	return 0;
}