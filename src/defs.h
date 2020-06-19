#ifndef _DEF_H_
#define _DEF_H_

#include <stdio.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/ctrl.h>

//get the c++ library int
#include <stdio.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// import the vita2d library
#include <vita2d.h>

#include "debugScreen.h"
#define printf psvDebugScreenPrintf


// Def some colours
#define BLACK RGBA8(0, 0, 0, 0xFF)
#define WHITE RGBA8(0xFF, 0xFF, 0xFF, 0xFF)
#define CRIMSON RGBA8(0xFF, 0x24, 0, 0xFF)
#define SEAGREEN RGBA8(0x54, 0xFF, 0x9F, 0xFF)
#define AMBER RGBA8(0xFF, 0xBF, 0, 0xFF)

// font buffer
extern unsigned int basicfont_size;
extern unsigned char basicfont[];

// States of the game
enum GAME_STATE{
	RUNNING,
	PAUSED,
	ENDED
};

// Bullet variants
enum BClass{
    PLYBAR, // Player bullet type
    DESTBE, // Destoryable bullet ( enemy )
    UNDEST  // Undestoryable bullet
};

// Default stick offset;
const float OFFSET_STICK = 127.0f;
const float OFFSET_DEADZ =  60.0f;

#endif /* _DEF_H_ */
