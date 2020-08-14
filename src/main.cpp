#include "../../VEngine/VEngine.h"
#include <memory>

#include "BaseActor.h"
#include "playerA.h"
#include "Enemy.h"
#include "EnemyFollower.h"
#include "EnemyShooter.h"
#include "EnemyParent.h"
#include "bullet.h"
#include "Wall.h"
// [TODO]
// struct SceneEntity
// {
	
// };


class MVEngine : public VEngine{

	private: 
		vCamera camMain = vCamera(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0.1f, 2.0f, 45.0f);
		vCamera camDevp = vCamera(200, 200, 740, 324, 0.1f, 100.0f,120.0f);
		
		// Store all the mesh we are going to use 
		vMesh vmPlayer;
		vMesh vmEnemy1;
		vMesh vmEnemy2; // Sphere . ... 75 tris.
		vMesh vmEnemy3; // Hammer Head
		vMesh vmGround;
		vMesh vmPlyBlt;

		// Generate the walls
		vMesh wall_WHT, wall_BLC, wall_RED;
		
		APlayer pPlayer = APlayer(Actor::PLAYER, vec2d(0.0f, -5.0f), 3, 16.0f, 0.05f, &vmEnemy1);

		std::vector<std::unique_ptr<Enemy>> enemies;
		std::vector<Wall> walls_back_side;
		std::vector<Wall> walls_front;

		inline std::unique_ptr<EnemyFollower> E_Follower(int a, int b) 					{return std::unique_ptr<EnemyFollower>(new EnemyFollower(Actor::FOLLOWER,   vec2d( a, b), 3, 1.0f, &vmEnemy1));			};
		inline std::unique_ptr<EnemyFollower> E_HammerHd(int a, int b) 					{return std::unique_ptr<EnemyFollower>(new EnemyFollower(Actor::HAMMERHEAD, vec2d( a, b), 3, 1.0f, &vmEnemy3));			};
		inline std::unique_ptr<EnemyShooter > E_ShooterG(int a, int b, ShootPattern c) 	{return std::unique_ptr<EnemyShooter> (new EnemyShooter (Actor::SHOTGUN,    vec2d( a, b), 5, 5.0f, 0.6f, c, &vmEnemy2));};
		inline std::unique_ptr<EnemyShooter > E_ShooterS(int a, int b, ShootPattern c) 	{return std::unique_ptr<EnemyShooter> (new EnemyShooter (Actor::SNIPER,     vec2d( a, b), 5, 5.0f, 0.6f, c, &vmEnemy2));};
		inline std::unique_ptr<EnemyParent  > E_PARENT  (int a, int b, ShootPattern c) 	{return std::unique_ptr<EnemyParent > (new EnemyParent  (Actor::SHOTGUN,    vec2d( a, b), 5, 5.0f, 0.6f, c, &wall_RED));};
		
		// Given a 2d vector, return the radians from anticlockwise
		float vec_to_rad(float ax, float ay){
			// Assuming the heading is always (0, 1)
			float vecMagtd = sqrtf(ax * ax + ay * ay);
			float cosTheta = ay / (vecMagtd);
			float rad = acosf(cosTheta);

			// if on the right, do 2pi - rad
			if (ax > 0)
				return 6.283185 - rad;

			return rad;
		}

		float vec_to_rad(vec2d& v){
			return vec_to_rad(v.x, -v.y);
		}

		void onCreate() override{
			// Turn on shatty lighting
			vecLight = vec3d(0.0f, -1000.0f, 0.0f);
			bLighting = true;
			// Change world colour
			world_colour = vec3d(76, 72, 60);

			#ifdef OPENCV
			vmPlayer.LoadFromObjectFile("../res/main_pointer.obj"	);
			vmEnemy1.LoadFromObjectFile("../res/E_1.obj"			);
			vmEnemy2.LoadFromObjectFile("../res/sphere.obj"			);
			vmEnemy3.LoadFromObjectFile("../res/E_HammerHead.obj"	);
			vmGround.LoadFromObjectFile("../res/Ground_Plane.obj"	);
			vmPlyBlt.LoadFromObjectFile("../res/player_bullet.obj"	);
			wall_WHT.LoadFromObjectFile("../res/cubeX8.obj"         );
			wall_BLC.LoadFromObjectFile("../res/cubeX8.obj"         );
			wall_RED.LoadFromObjectFile("../res/cubeX8.obj"         );
			#endif

			#ifdef PSVITA
			vmPlayer.LoadFromObjectFile("app0:/res/main_pointer.obj" );
			vmEnemy1.LoadFromObjectFile("app0:/res/E_1.obj"			 );
			vmEnemy2.LoadFromObjectFile("app0:/res/sphere.obj"		 );
			vmEnemy3.LoadFromObjectFile("app0:/res/E_HammerHead.obj" );
			vmGround.LoadFromObjectFile("app0:/res/Ground_Plane.obj" );
			vmPlyBlt.LoadFromObjectFile("app0:/res/player_bullet.obj");
			wall_WHT.LoadFromObjectFile("app0:/res/cubeX8.obj"       );
			wall_BLC.LoadFromObjectFile("app0:/res/cubeX8.obj"       );
			wall_RED.LoadFromObjectFile("app0:/res/cubeX8.obj"       );
			#endif

			// Translate the mesh properly along the y axis
			vmGround.ApplyTranslation(vec3d(0.0f, 0.5f, 0.0f));
			vmEnemy1.ApplyTranslation(vec3d(0.0f,-0.7f, 0.0f));
			vmPlayer.ApplyTranslation(vec3d(0.0f,-0.4f, 0.0f));
			vmPlyBlt.ApplyTranslation(vec3d(0.0f,-0.3f, 0.0f));

			camMain.ApplyTranslation(vec3d(0.0f, -36.0f, -25.0f));
			// Rotate forward 60 degress
			mat4x4 matRot = matMakeRotationX(-(60.0f / 180.0f * 3.14159));
			camMain.ApplyRotation(matRot, camMain.getVecLocation());

			camDevp.ApplyTranslation(vec3d(22.0f, -2.0f, 0.0f));
			vec3d pointToLeft(-1.0f, 0.0f, 0.0f);
			pointToLeft += camDevp.getVecLocation();
			camDevp.PointAt(pointToLeft);


			enemies.push_back(E_PARENT(-5, -5, ShootPattern::SHOOT_MIXED));

			for (int i = 0; i < 3; i++){
				enemies.push_back(E_Follower(-5 + i * 5, -5 + i * 5));
				enemies.push_back(E_HammerHd(-3 + i * 3, -3 + i * 3));
			}

			for (int i = 0; i < 2; i++){
				enemies.push_back(E_ShooterG(-2+i*4, -2+i*4, ShootPattern::SHOOT_DEST));
			}
				enemies.push_back(E_ShooterS(-2, -2, ShootPattern::SHOOT_MIXED));

			for (int i = 2; i < 7; i+=2){
				enemies[i]->set_parent(&enemies[0]);
			}
			
			// Set up the demo walls
			// Set colour first
			wall_WHT.setColour(255, 255, 255);
			wall_BLC.setColour( 21,  21,  21);
			wall_RED.setColour(224,   0,   0);

			// Generate the front set thats 22 of them
			int w = 0;
			for (w = 0; w < 22; w++){
				walls_front.push_back(Wall(WALL_TYPE::WHT_WALL, wall_WHT, vec3d(-20.0f + w * 2, -0.8f, -20.8f)));
			}
			// Generate the back set of walls
			for (w = 0; w < 22; w++){
				walls_back_side.push_back(Wall(WALL_TYPE::WHT_WALL, wall_RED, vec3d(-20.0f + w * 2, -0.8f,  20.8f)));
			}
		}

		inline bool in_range(float ax, float ay, float bx, float by, float r){
			float dx = ax - bx;
			float dy = ay - by;
			float range = sqrtf(dx * dx + dy * dy);
			return (range <= r);
		}

		inline bool in_range(vec2d& p1, vec2d& p2, float r){
			return in_range(p1.x, p1.y, p2.x, p2.y, r);
		}	

		void append_scene_object(std::vector<vMesh>& mesh, BaseActor& actor){
				vMesh enemy_mesh = *actor.getActorModel();
				vec3d vLocation = vec3d(actor.getLocation().x, enemy_mesh.getVecLocation().y,
								-actor.getLocation().y);
				enemy_mesh.setLocation(vLocation);
				float rad = vec_to_rad(actor.getDirection());
				mat4x4 matRot = matMakeRotationY(rad);
				enemy_mesh.ApplyRotation(matRot, vLocation);
				mesh.push_back(enemy_mesh);
		}

		void update(float fElapsedTime) override{
			std::vector<vMesh> svv; // All Actor objects here
			std::vector<vMesh> sbl; // All bullets here
		
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
			#endif

			#ifdef PSVITA
			fire = (ctrl.buttons & SCE_CTRL_RTRIGGER);
			lx = ctrl.lx;
			ly = ctrl.ly;
			rx = ctrl.rx;
			ry = ctrl.ry;
			#endif

			pPlayer.update(lx, ly, rx, ry, fire, fElapsedTime);
			for (auto& e : enemies){
				e->update(fElapsedTime, pPlayer);
				//e->update(fElapsedTime, vec2d(0, 0));
			}

			// Do player mesh
			vMesh player_mesh = vmPlayer;
			vec3d vLocation(pPlayer.getLocation().x, vmPlayer.getVecLocation().y, -pPlayer.getLocation().y);
			player_mesh.setLocation(vLocation);

			float rad = vec_to_rad(pPlayer.getDirection());
			mat4x4 matRot = matMakeRotationY(rad);
			player_mesh.ApplyRotation(matRot, vLocation);
			// Push to scene array
			svv.push_back(player_mesh);

			// Do the same for the enemy 
			for (auto& e : enemies){
				// Check bullet if its not follower
				if (e->getActorType() != Actor::FOLLOWER){
					// Update the bullet even if it the enemy is dead
					for (auto& blt : e->bMag){
						if (blt.active){
							sbl.push_back(vmPlyBlt);
							if(blt.mtype == BClass::DESTBE) sbl[sbl.size()-1].setColour(255, 151,  87);
							if(blt.mtype == BClass::UNDEST) sbl[sbl.size()-1].setColour( 73,  74, 138);
							sbl[sbl.size()-1].setLocation(vec3d(blt.vLoc.x, vmPlyBlt.getVecLocation().y, -blt.vLoc.y));
							float fdirection = vec_to_rad(blt.vVel.x, -blt.vVel.y);

							mat4x4 matrot = matMakeRotationY(fdirection);
							sbl[sbl.size()-1].ApplyRotation(matrot, sbl[sbl.size()-1].getVecLocation());
						}
					}
				}

				if (!e->isAlive()) continue;
				append_scene_object(svv, *e);
			}
			
			// Check player bullet collision
			for (auto& blt : pPlayer.bMag){
				if (blt.active){
					sbl.push_back(vmPlyBlt);
					sbl[sbl.size()-1].setLocation(vec3d(blt.vLoc.x, vmPlyBlt.getVecLocation().y, -blt.vLoc.y));
					float fdirection = vec_to_rad(blt.vVel.x, -blt.vVel.y);

					mat4x4 matrot = matMakeRotationY(fdirection);
					sbl[sbl.size()-1].ApplyRotation(matrot, sbl[sbl.size()-1].getVecLocation());

					for(auto& e : enemies){
						// if this enemy has bullets
						if (e->getActorType() != Actor::FOLLOWER){
							for(auto& ebullet : e->bMag){
								if(!ebullet.active) continue;

								if (in_range(blt.vLoc, ebullet.vLoc, 1.0f)){
									blt.active = false;
									if (ebullet.mtype != BClass::UNDEST) ebullet.active = false;
									break;
								}
							}
						}

						if (!e->isAlive()) continue;
						if (!blt.active) break;
							// check hit
						if (in_range(blt.vLoc, e->getLocation(), 1.5f)){

							blt.active = false;
							e->damage(blt.dmg, blt);
						}
					}			
				}
			}

			// Check player enemy collision
			for (auto& e : enemies){
				if (e->getActorType() != Actor::FOLLOWER){
					for(auto& ebullet : e->bMag){
						if(!ebullet.active) continue;
						if (in_range(pPlayer.getLocation(), ebullet.vLoc, 1.0f)){
							ebullet.active = false;
							break;
						}
					}
				}
		// [TODO]

				if (!e->isAlive()) continue;
				bool bCollid = in_range(pPlayer.getLocation(), e->getLocation(), 1.5f);
				if(bCollid){
					if(pPlayer.damage(1)){
					}
				}
			}

			// Collect the ground wall
			std::vector<vMesh*> walls_b;
			std::vector<vMesh*> walls_f;
			for (auto& w : walls_back_side){
				if (w.isAlive()) walls_b.push_back(w.getMesh());
			}
			for (auto& w : walls_front){
				if (w.isAlive()) walls_f.push_back(w.getMesh());
			}


			// Draw the scene
			draw_mesh(camMain, vmGround);
			draw_scene(camMain, walls_b);
			draw_scene(camMain, svv);
			draw_scene(camMain, sbl);
			draw_scene(camMain, walls_f);
			
			#ifdef OPENCV
			camDevp.SetLocation(vec3d(camDevp.getVecLocation().x, camDevp.getVecLocation().y, -pPlayer.getLocation().y));
			draw_scene(camDevp, walls_b);
			draw_scene(camDevp, svv);
			draw_scene(camDevp, sbl);
			draw_scene(camDevp, walls_f);
			#endif

			#ifdef OPENCV
			vec3d pt1, pt2, pt3;

			for (auto& blt : pPlayer.bMag){
				if (!blt.active) continue;
				
				pt1 = project_point( vec3d(blt.vLoc.x, 0.0f, -blt.vLoc.y), camDevp);
				pt2 = project_point( vec3d(blt.vPre.x, 0.0f, -blt.vPre.y), camDevp);
				pt3 = project_point( vec3d(blt.vLoc.x + 0.8f, 0.0f, -blt.vLoc.y + 0.8f), camDevp);
				pt3 = pt3 - pt1;
				float l = sqrtf(pt3.x * pt3.x + pt3.y * pt3.y);

				//cv::line(canvas, cv::Point(pt1.x, pt1.y), cv::Point(pt2.x, pt2.y),
				//		cv::Scalar(0,0,255), 2);
				//cv::circle(canvas, cv::Point(pt1.x, pt1.y), l, cv::Scalar(255,0,0));
			
			}

			//Player healt information
			char buff[20];
			sprintf(buff, "Health: %d", pPlayer.getHealth());
			pt1 = project_point( vec3d(pPlayer.getLocation().x, 0.0f, -pPlayer.getLocation().y), camMain);
			cv::putText(canvas, buff, cv::Point(pt1.x + 5, pt1.y - 5),
						cv::FONT_HERSHEY_DUPLEX, 0.5, CV_RGB(255,255,255), 1);

			// Print enemy health info
			for (auto& e : enemies){
				if (!e->isAlive()) continue;
				pt1 = project_point( vec3d(e->getLocation().x, 0.0f, -e->getLocation().y), camMain);
				sprintf(buff, "Health: %d", e->getHealth());
				cv::putText(canvas, buff, cv::Point(pt1.x + 5, pt1.y - 5),
						cv::FONT_HERSHEY_DUPLEX, 0.5, CV_RGB(255,  0,255), 1);
			}
			#endif
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
