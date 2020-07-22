#include "../../VEngine/VEngine.h"

#include "player.h"
#include "bullet.h"

class MVEngine : public VEngine{

	private: 
		vCamera camMain = vCamera(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0.1f, 10000.0f);
		vCamera camDevp = vCamera(200, 200, 740, 324, 0.1f, 100.0f, 60.0f);
		//std::vector<vMesh> scene;
		vMesh vmPlayer;
		vMesh vmEnemy1;
		vMesh vmGround;
		vMesh vmPlyBlt;
		Player pPlayer = Player(0, 0, 1.0f, 0.1f);

	// Given a 2d vector, return the radians from anticlockwise
	float vec_to_rad(float ax, float ay){
		// Assuming the heading is always (0, 1)
		float vecMagtd = sqrtf(ax * ax + ay * ay);
		float cosTheta = ay / (vecMagtd);
		float rad = acosf(cosTheta);

		// if on the right, do 2pi - rad
		if (ax > 0)
			return 2 * M_PI - rad;

		return rad;
	}


	void onCreate() override{
		// Turn on shatty lighting
		vecLight = vec3d(0.0f, -1000.0f, 0.0f);
		bLighting = true;
		// Change world colour
		world_colour = vec3d(76, 72, 60);
		
		#ifdef OPENCV
		vmPlayer.LoadMaterialFile("../res/main_pointer.mtl");
		vmPlayer.LoadFromObjectFile("../res/main_pointer.obj");

		vmEnemy1.LoadMaterialFile("../res/E_1.mtl");
		vmEnemy1.LoadFromObjectFile("../res/E_1.obj");

		vmGround.LoadMaterialFile("../res/Ground_Plane.mtl");
		vmGround.LoadFromObjectFile("../res/Ground_Plane.obj");

		vmPlyBlt.LoadFromObjectFile("../res/player_bullet.obj");
		#endif

		#ifdef PSVITA
		vmPlayer.LoadMaterialFile("app0:/res/main_pointer.mtl");
		vmPlayer.LoadFromObjectFile("app0:/res/main_pointer.obj");

		vmEnemy1.LoadMaterialFile("app0:/res/E_1.mtl");
		vmEnemy1.LoadFromObjectFile("app0:/res/E_1.obj");

		vmGround.LoadMaterialFile("app0:/res/Ground_Plane.mtl");
		vmGround.LoadFromObjectFile("app0:/res/Ground_Plane.obj");

		vmPlyBlt.LoadFromObjectFile("app0:/res/player_bullet.obj");
		#endif

		// Translate the mesh properly
		vmGround.ApplyTranslation(vec3d(0.0f, 0.5f, 0.0f));
		vmEnemy1.ApplyTranslation(vec3d(0.0f,-0.7f, 0.0f));
		vmPlayer.ApplyTranslation(vec3d(0.0f,-0.4f, 0.0f));
		vmPlyBlt.ApplyTranslation(vec3d(0.0f,-0.3f, 0.0f));

		camMain.ApplyTranslation(vec3d(0.0f, -10.0f, -3.0f));
		// Rotate forward 80 degress
		mat4x4 matRot = matMakeRotationX(-1.396263);
		camMain.ApplyRotation(matRot, camMain.getVecLocation());

		camDevp.ApplyTranslation(vec3d(12.0f, 0.0f, 0.0f));
		vec3d pointToLeft(-1.0f, 0.0f, 0.0f);
		pointToLeft += camDevp.getVecLocation();
		camDevp.PointAt(pointToLeft);
	}

	void update(float fElapsedTime) override{
		std::vector<vMesh> svv = {vmPlayer, vmEnemy1};
		std::vector<vMesh> sbl;
		vmEnemy1.ApplyTranslation(vec3d(0.0f, 0.0f, fElapsedTime/5));

		// format input for player
		int lx, ly, rx, ry;
		lx = ly = rx = ry = OFFSET_STICK;
		bool fire = false;

		#ifdef OPENCV
		// W key press
		if (keypress == 87 || keypress == 119) ly = 0;
		// S key press
		if (keypress == 83 || keypress == 115) ly = 256;
		// A key press
		if (keypress == 65 || keypress ==  97) lx = 0;
		// D key press
		if (keypress == 68 || keypress == 100) lx = 256;

		// IKJL for right stick
		// I key press
		if (keypress == 73 || keypress == 105) ry = 0;
		// K key press
		if (keypress == 75 || keypress == 107) ry = 256;
		// J key press
		if (keypress == 74 || keypress == 106) rx = 0;
		// L key press
		if (keypress == 76 || keypress == 108) rx = 256;

		// O key for fire
		if (keypress == 79 || keypress == 111) fire=true;
		
		std::cout << "." << std::endl;
		std::cout << pPlayer.dlx << " " << pPlayer.dly << std::endl;
		#endif

		#ifdef PSVITA
		fire = (ctrl.buttons & SCE_CTRL_RTRIGGER);
		lx = ctrl.lx;
		ly = ctrl.ly;
		rx = ctrl.rx;
		ry = ctrl.ry;
		#endif

		pPlayer.update(lx, ly, rx, ry, fire, fElapsedTime);
		short n = 0;
		for (auto blt : pPlayer.mag){
			if (blt.active){
				sbl.push_back(vmPlyBlt);
				sbl[n].setLocation(vec3d(blt.blx, vmPlyBlt.getVecLocation().y, -blt.bly));

				float fdirection = vec_to_rad(blt.bvx, -blt.bvy);

				mat4x4 matrot = matMakeRotationY(fdirection);
				sbl[n].ApplyRotation(matrot, sbl[n].getVecLocation());

				n++;
			}
		}


		vec3d vLocation(pPlayer.plx, svv[0].getVecLocation().y, -pPlayer.ply);
		svv[0].setLocation(vLocation);

		float rad = vec_to_rad(pPlayer.dlx, -pPlayer.dly);
		std::cout << rad << std::endl;
		mat4x4 matRot = matMakeRotationY(rad);
		svv[0].ApplyRotation(matRot, vLocation);

		// Draw the scene
		draw_mesh(camMain, vmGround);
		draw_scene(camMain, svv);
		draw_scene(camMain, sbl);
		
		
		draw_mesh(camDevp, vmGround);
		draw_scene(camDevp, svv);
		draw_scene(camDevp, sbl);
		
	}

};

int main(int argc, char *argv[]) {

	MVEngine mve;
	mve.start();

	#ifdef PSVITA
	sceKernelExitProcess(0);
	#endif 
	return 0;
}