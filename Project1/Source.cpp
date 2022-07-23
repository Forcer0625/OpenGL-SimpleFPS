/***************************
4108056036 洪郁修 第6-VI次作業12/22
***************************/
#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream> 
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include "glm.h"
#include "materials.h"
#include "sgi.h"

#define KEY1_DRAWARRAY			1
#define	KEY2_DRAWELEMENT		2
#define	KEY3_DRAMULTIWARRAY		3
#define	KEY4_DRAWMULTIELEMENT	4
#define MAX_CHAR 128
#define WIDTH 935
#define HEIGHT 311
using namespace std;
// Objects
class ObjLoader
{
    public:
	    struct vertex
	    {
		    float x;
		    float y;
		    float z;
	    };
        ObjLoader(string filename);
	    void Draw();
    private:
	    vector<vector<GLfloat>> v;
	    vector<vector<GLint>> f;
};
class Enemy
{
    public:
        int HealthPoints;
        int hp;
        bool ATTACK_ABLE;
        GLfloat speed;
        ObjLoader* loader;
        float size_factor;
        float hit_box_factor;
        float face_to; // 0~2pi
        float self_roate0; //0~360
        struct position
        {
            float x;
            float y;
            float z;
        };
        struct position pos;
        struct position old;
        Enemy(string name, int hp, float size_factor, float hit_box_factor, float self_roate0,int pox, int poy, int poz);
        void render();
        void attack();
        void move();
        void idle();
        bool hit_detection();
        bool objCollision(float posx, float posz,double range);
        bool objCollision(float posx, float posy, float posz, double range);
        void Collison(float posx, float posz, double range);
        void underAttack();
};
Enemy monkey = Enemy("monkey.obj", 67, 4.0, 2.0, 0.0,  0.0, 4.0, -13.0);
Enemy flower = Enemy("flower.obj", 200, 0.2, 100.0, -90.0, -40.0, 10.0, -90.0);
Enemy teapot = Enemy("teapot.obj", 123, 0.1, 80.0, 10.0, 51.0, 5.0, -40.0);
bool SHOW_HITBOX_FLAG = FALSE;
bool objATTACK_FLAG = FALSE;
float PlayerAttack = 15.0;
// Polygon Offset
GLfloat polyfactor = 0.0;
GLfloat polyunits = 1.0;
bool POLYGON_OFFSET_FLAG = FALSE;
bool ON_OFFSETING = FALSE;
// Snipe
bool SNIPE_FLAG = FALSE;
bool ON_SNIPING = FALSE;
static float snipe_offset = 0.0;
const float snipe_pos = 1.0;
// AA
bool AA_FLAG = FALSE;
// Fog
bool FOG_FLAG = FALSE;
// hit detection
const int hit_objs = 11;
bool hit_flag[hit_objs] = { TRUE };
// lighting
GLfloat light_position[] = { -0.0, 140.0, -50.0, 0.0 };
GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 0.2};
// gravity
static float gra = 9.8;
static float speed = 0.0;
static bool ON_JUMPING = FALSE;
static bool JUMP_CHEATING = FALSE;
// swing arm
static bool ON_SWING = FALSE;
static int robot_ang = 0;
// planet shooting
static bool ON_SHOOTING = FALSE;
static bool ON_RECOVER = FALSE;
const static float ShootingDistance = 50.0;
static float beforeShootingLoc[3];
static float shootingLocation = 0.0;
static float shootingSpeed = 0.0;
static int AttackAniColor = 10;
static int AttackAniColor_count = 0;
// Planet
float PI = 3.14159;
bool COLORFUL = 1;
static int DRAWMETHOD = 3;
static int year = 0;
static int day = 0;
static int marsyear = 0;
static int sunday = 0;
static int logoAngle = 0;

static bool FREECAM = 0;
static float camPosz = 30.0; //30
static float camPosx = 0.0;
static float camPosy = 2.0;
static float camLookx = 0.0;
static float camLooky = 0.0;
//static float camLookz = 0.0;
float r_r = 186.0 / 232.0;
float r_g = 17.0 / 232.0;
float r_b = 29.0 / 232.0;
float g = 121.0 / 276.0;
float b = 155.0 / 276.0;
static GLfloat* IconVertices = NULL;
const float ICONSCALE = 65.0;
const float ICONmovement = 30.0;


void floor();
void setMaterialv(const GLfloat* params);
void drawPlanet();
void drawPlanet0();
void swingAnimation();
void shootingPlanetAnimation();
void Animations();
void ShootingPush(float beforeShootingLoc_x, float beforeShooting_y, float beforeShooting_z);
void drawRobot_left();
void drawRobot_right();
void gravity();
void DrawCircle(float cx, float cy, float r, int num_segments);
void drawAxis();
void drawAxis(float locX, float LocY, float LocZ);
void drawAxis(float*);
bool hitInRange(float topLeft_x, float topLeft_y, float botRight_x, float botRitght_y);
void hitsFunc();
void iconsRecover();
static GLfloat colors[] = { 0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                            0, g, b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                                r_r, r_g, r_b,
                            r_r, r_g, r_b,
                            r_r, r_g, r_b,
                            r_r, r_g, r_b,
                            r_r, r_g, r_b,
                            r_r, r_g, r_b,
                            r_r, r_g, r_b,
                            r_r, r_g, r_b,
                            r_r, r_g, r_b,
                            r_r, r_g, r_b,
                            r_r, r_g, r_b,
                            r_r, r_g, r_b,
                            r_r, r_g, r_b,
                            r_r, r_g, r_b,
                            r_r, r_g, r_b,
                            r_r, r_g, r_b,
                            r_r, r_g, r_b,
                            r_r, r_g, r_b,
                            r_r, r_g, r_b,
                            r_r, r_g, r_b,
                            r_r, r_g, r_b,
                            r_r, r_g, r_b,
                            r_r, r_g, r_b,
                            r_r, r_g, r_b,
                            r_r, r_g, r_b,
                            r_r, r_g, r_b,
                            r_r, r_g, r_b,
                            r_r, r_g, r_b,
                            r_r, r_g, r_b,
                            r_r, r_g, r_b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                0, g, b,
                                    r_r, r_g, r_b,
                                    r_r, r_g, r_b,
                                    r_r, r_g, r_b,
                                    r_r, r_g, r_b,
                                    r_r, r_g, r_b,
                                    r_r, r_g, r_b,
                                    r_r, r_g, r_b,
                                    r_r, r_g, r_b, };
float getv(int a) {
    return ((float)a / (float)WIDTH) - 0.5;
}
float getv_b(int b) {
    return (-((float)b - (float)HEIGHT) / (float)HEIGHT) - 0.5;
}
void setupPointers(void)
{
    static GLfloat vertices[] = { 0.022 - 0.5, 0.8 - 0.5, 0.0,
                        0.02 - 0.5,   0.78 - 0.5, 0.0,
                        0.114 - 0.5,   0.83 - 0.5, 0.0,
                        0.1137 - 0.5, 0.85 - 0.5, 0.0,
                        0.025 - 0.5, 0.8 - 0.5, 0.0,
                        0.1137 - 0.5, 0.85 - 0.5, 0.0,
                        0.096 - 0.5, 0.856 - 0.5, 0.0,
                        0.036 - 0.5, 0.815 - 0.5, 0.0,
                        0.02 - 0.5, 0.78 - 0.5, 0.0,
                        0.02 - 0.5, 0.765 - 0.5, 0.0,
                        0.07 - 0.5, 0.81 - 0.5, 0.0,
                        0.02 - 0.5, 0.8 - 0.5, 0.0,
                        0.02 - 0.5, 0.552 - 0.5, 0.0,
                        0.03 - 0.5, 0.56 - 0.5, 0.0,
                        0.035 - 0.5, 0.78 - 0.5, 0.0,
                        0.035 - 0.5, 0.78 - 0.5, 0.0,
                        0.032 - 0.5, 0.7 - 0.5, 0.0,
                        0.045 - 0.5, 0.725 - 0.5, 0.0,
                        0.045 - 0.5, 0.75 - 0.5, 0.0,
                        0.045 - 0.5, 0.75 - 0.5, 0.0,
                        0.045 - 0.5, 0.725 - 0.5, 0.0,
                        0.083 - 0.5, 0.77 - 0.5, 0.0,
                        0.083 - 0.5, 0.79 - 0.5, 0.0,
                        0.081 - 0.5, 0.82 - 0.5, 0.0,
                        0.083 - 0.5, 0.77 - 0.5, 0.0,
                        0.101 - 0.5, 0.77 - 0.5, 0.0,
                        0.092 - 0.5, 0.825 - 0.5, 0.0,
                        0.067 - 0.5, 0.81 - 0.5, 0.0,
                        0.069 - 0.5, 0.77 - 0.5, 0.0,
                        0.102 - 0.5, 0.605 - 0.5, 0.0,
                        0.10 - 0.5, 0.625 - 0.5, 0.0,
                        0.104 - 0.5, 0.648 - 0.5, 0.0,
                        0.095 - 0.5, 0.675 - 0.5, 0.0,
                        0.079 - 0.5, 0.775 - 0.5, 0.0,
                        0.078 - 0.5, 0.818 - 0.5, 0.0,
                        0.049 - 0.5, 0.737 - 0.5, 0.0,
                        0.032 - 0.5, 0.675 - 0.5, 0.0,
                        0.033 - 0.5, 0.658 - 0.5, 0.0,
                        0.041 - 0.5, 0.672 - 0.5, 0.0,
                        0.034 - 0.5, 0.645 - 0.5, 0.0,
                        0.035 - 0.5, 0.615 - 0.5, 0.0,
                        0.081 - 0.5, 0.668 - 0.5, 0.0,
                        0.083 - 0.5, 0.695 - 0.5, 0.0,
                        0.0668 - 0.5, 0.687 - 0.5, 0.0,
                        0.075 - 0.5, 0.725 - 0.5, 0.0,
                        0.072 - 0.5, 0.75 - 0.5, 0.0,
                        0.05 - 0.5, 0.702 - 0.5, 0.0,
                        0.065 - 0.5, 0.742 - 0.5, 0.0,
                        0.0915 - 0.5, 0.75 - 0.5, 0.0,
                        0.087 - 0.5, 0.74 - 0.5, 0.0,
                        0.0837 - 0.5, 0.68 - 0.5, 0.0,
                        0.079 - 0.5, 0.648 - 0.5, 0.0,
                        0.058 - 0.5, 0.6 - 0.5, 0.0,
                        0.0809 - 0.5, 0.6 - 0.5, 0.0,
                        0.0837 - 0.5, 0.625 - 0.5, 0.0,
                        0.0915 - 0.5, 0.65 - 0.5, 0.0,
                        0.1 - 0.5, 0.724 - 0.5, 0.0,
                        0.1 - 0.5, 0.6 - 0.5, 0.0,
                        0.058 - 0.5, 0.6 - 0.5, 0.0,
                        0.03 - 0.5, 0.59 - 0.5, 0.0,
                        0.03 - 0.5, 0.567 - 0.5, 0.0,
                        0.081 - 0.5, 0.575 - 0.5, 0.0,
                        0.1 - 0.5, 0.57 - 0.5, 0.0,
                        0.115 - 0.5, 0.585 - 0.5, 0.0,
                        0.107 - 0.5, 0.83 - 0.5, 0.0,
                        0.11 - 0.5, 0.562 - 0.5, 0.0,
                        0.1 - 0.5, 0.568 - 0.5, 0.0,
                        0.107 - 0.5, 0.52 - 0.5, 0.0,
                        0.112 - 0.5, 0.52 - 0.5, 0.0,
                        0.117 - 0.5, 0.55 - 0.5, 0.0,
                        0.114 - 0.5, 0.83 - 0.5, 0.0,
                        0.186 - 0.5, 0.83 - 0.5, 0.0,
                        0.183 - 0.5, 0.8 - 0.5, 0.0,
                        0.194 - 0.5, 0.77 - 0.5, 0.0,
                        0.208 - 0.5, 0.775 - 0.5, 0.0,
                        0.208 - 0.5, 0.8 - 0.5, 0.0,
                        0.186 - 0.5, 0.83 - 0.5, 0.0,
                        0.183 - 0.5, 0.8 - 0.5, 0.0,
                        0.194 - 0.5, 0.77 - 0.5, 0.0,
                        0.208 - 0.5, 0.775 - 0.5, 0.0,
                        0.208 - 0.5, 0.8 - 0.5, 0.0,
                        0.16 - 0.5, 0.74 - 0.5, 0.0,
                        0.158 - 0.5, 0.71 - 0.5, 0.0,
                        0.166 - 0.5, 0.7 - 0.5, 0.0,
                        0.195 - 0.5, 0.738 - 0.5, 0.0,
                        0.21 - 0.5, 0.745 - 0.5, 0.0,
                        0.225 - 0.5, 0.75 - 0.5, 0.0,
                        0.225 - 0.5, 0.774 - 0.5, 0.0,
                        0.216 - 0.5, 0.78 - 0.5, 0.0,
                        0.208 - 0.5, 0.775 - 0.5, 0.0,
                        0.194 - 0.5, 0.77 - 0.5, 0.0,
                        0.172 - 0.5, 0.745 - 0.5, 0.0,
                        0.172 - 0.5, 0.75 - 0.5, 0.0,
                        0.195 - 0.5, 0.738 - 0.5, 0.0,
                        0.173 - 0.5, 0.68 - 0.5, 0.0,
                        0.177 - 0.5, 0.65 - 0.5, 0.0,
                        0.195 - 0.5, 0.7 - 0.5, 0.0,
                        0.21 - 0.5, 0.745 - 0.5, 0.0,
                        0.195 - 0.5, 0.7 - 0.5, 0.0,
                        0.177 - 0.5, 0.65 - 0.5, 0.0,
                        0.215 - 0.5, 0.705 - 0.5, 0.0,
                        0.2255 - 0.5, 0.7 - 0.5, 0.0,
                        0.225 - 0.5, 0.72 - 0.5, 0.0,
                        0.219 - 0.5, 0.727 - 0.5, 0.0,
                        0.2255 - 0.5, 0.7 - 0.5, 0.0,
                        0.215 - 0.5, 0.65 - 0.5, 0.0,
                        0.205 - 0.5, 0.605 - 0.5, 0.0,
                        0.195 - 0.5, 0.6 - 0.5, 0.0,
                        0.207 - 0.5, 0.653 - 0.5, 0.0,
                        0.215 - 0.5, 0.705 - 0.5, 0.0,
                        0.195 - 0.5, 0.6 - 0.5, 0.0,
                        0.156 - 0.5, 0.595 - 0.5, 0.0,
                        0.16 - 0.5, 0.575 - 0.5, 0.0,
                        0.172 - 0.5, 0.565 - 0.5, 0.0,
                        0.219 - 0.5, 0.588 - 0.5, 0.0,
                        0.2415 - 0.5, 0.585 - 0.5, 0.0,
                        0.241 - 0.5, 0.605 - 0.5, 0.0,
                        0.233 - 0.5, 0.61 - 0.5, 0.0,
                        0.205 - 0.5, 0.605 - 0.5, 0.0,
                        0.267 - 0.5, 0.79 - 0.5, 0.0,
                        0.26 - 0.5, 0.75 - 0.5, 0.0,
                        0.275 - 0.5, 0.67 - 0.5, 0.0,
                        0.284 - 0.5, 0.67 - 0.5, 0.0,
                        0.277 - 0.5, 0.75 - 0.5, 0.0,
                        0.275 - 0.5, 0.77 - 0.5, 0.0,
                        0.273 - 0.5, 0.78 - 0.5, 0.0,
                        0.275 - 0.5, 0.77 - 0.5, 0.0,
                        0.287 - 0.5, 0.75 - 0.5, 0.0,
                        0.32 - 0.5, 0.79 - 0.5, 0.0,
                        0.329 - 0.5, 0.79 - 0.5, 0.0,
                        0.339 - 0.5, 0.79 - 0.5, 0.0,
                        0.353 - 0.5, 0.785 - 0.5, 0.0,
                        0.353 - 0.5, 0.8 - 0.5, 0.0,
                        0.349 - 0.5, 0.815 - 0.5, 0.0,
                        0.327 - 0.5, 0.81 - 0.5, 0.0,
                        0.317 - 0.5, 0.808 - 0.5, 0.0,
                        0.284 - 0.5, 0.79 - 0.5, 0.0,
                        0.273 - 0.5, 0.78 - 0.5, 0.0,
                        0.353 - 0.5, 0.785 - 0.5, 0.0,
                        0.327 - 0.5, 0.723 - 0.5, 0.0,
                        0.295 - 0.5, 0.675 - 0.5, 0.0,
                        0.3 - 0.5, 0.7 - 0.5, 0.0,
                        0.317 - 0.5, 0.73 - 0.5, 0.0,
                        0.327 - 0.5, 0.75 - 0.5, 0.0,
                        0.339 - 0.5, 0.79 - 0.5, 0.0,
                        0.317 - 0.5, 0.808 - 0.5, 0.0,
                        0.317 - 0.5, 0.845 - 0.5, 0.0,
                        0.322 - 0.5, 0.85 - 0.5, 0.0,
                        0.327 - 0.5, 0.845 - 0.5, 0.0,
                        0.327 - 0.5, 0.81 - 0.5, 0.0,
                        0.327 - 0.5, 0.75 - 0.5, 0.0,
                        0.327 - 0.5, 0.723 - 0.5, 0.0,
                        0.327 - 0.5, 0.44 - 0.5, 0.0,
                        0.317 - 0.5, 0.425 - 0.5, 0.0,
                        0.317 - 0.5, 0.73 - 0.5, 0.0,
                        0.32 - 0.5, 0.79 - 0.5, 0.0,
                        0.327 - 0.5, 0.723 - 0.5, 0.0,
                        0.339 - 0.5, 0.72 - 0.5, 0.0,
                        0.336 - 0.5, 0.695 - 0.5, 0.0,
                        0.295 - 0.5, 0.675 - 0.5, 0.0,
                        0.3 - 0.5, 0.7 - 0.5, 0.0,
                        0.394 - 0.5, 0.808 - 0.5, 0.0,
                        0.4 - 0.5, 0.8 - 0.5, 0.0,
                        0.4 - 0.5, 0.675 - 0.5, 0.0,
                        0.406 - 0.5, 0.678 - 0.5, 0.0,
                        0.406 - 0.5, 0.8 - 0.5, 0.0,
                        0.4 - 0.5, 0.823 - 0.5, 0.0,
                        0.406 - 0.5, 0.825 - 0.5, 0.0,
                        0.411 - 0.5, 0.8 - 0.5, 0.0,
                        0.411 - 0.5, 0.675 - 0.5, 0.0,
                        0.417 - 0.5, 0.685 - 0.5, 0.0,
                        0.417 - 0.5, 0.8 - 0.5, 0.0,
                        0.417 - 0.5, 0.82 - 0.5, 0.0,
                        0.422 - 0.5, 0.825 - 0.5, 0.0,
                        0.422 - 0.5, 0.69 - 0.5, 0.0,
                        0.428 - 0.5, 0.7 - 0.5, 0.0,
                        0.428 - 0.5, 0.82 - 0.5, 0.0,
                        0.438 - 0.5, 0.825 - 0.5, 0.0,
                        0.45 - 0.5, 0.83 - 0.5, 0.0,
                        0.45 - 0.5, 0.845 - 0.5, 0.0,
                        0.444 - 0.5, 0.85 - 0.5, 0.0,
                        0.45 - 0.5, 0.83 - 0.5, 0.0,
                        0.449 - 0.5, 0.805 - 0.5, 0.0,
                        0.448 - 0.5, 0.795 - 0.5, 0.0,
                        0.446 - 0.5, 0.77 - 0.5, 0.0,
                        0.445 - 0.5, 0.705 - 0.5, 0.0,
                        0.428 - 0.5, 0.7 - 0.5, 0.0,
                        0.428 - 0.5, 0.8 - 0.5, 0.0,
                        0.438 - 0.5, 0.8 - 0.5, 0.0,
                        0.438 - 0.5, 0.825 - 0.5, 0.0,
                        0.45 - 0.5, 0.83 - 0.5, 0.0,
                        0.449 - 0.5, 0.805 - 0.5, 0.0,
                        0.46 - 0.5, 0.803 - 0.5, 0.0,
                        0.457 - 0.5, 0.708 - 0.5, 0.0,
                        0.462 - 0.5, 0.71 - 0.5, 0.0,
                        0.467 - 0.5, 0.75 - 0.5, 0.0,
                        0.463 - 0.5, 0.76 - 0.5, 0.0,
                        0.469 - 0.5, 0.82 - 0.5, 0.0,
                        0.471 - 0.5, 0.85 - 0.5, 0.0,
                        0.36 - 0.5, 0.65 - 0.5, 0.0,
                        0.358 - 0.5, 0.62 - 0.5, 0.0,
                        0.365 - 0.5, 0.6 - 0.5, 0.0,
                        0.402 - 0.5, 0.6503 - 0.5, 0.0,
                        0.443 - 0.5, 0.68 - 0.5, 0.0,
                        0.492 - 0.5, 0.695 - 0.5, 0.0,
                        0.494 - 0.5, 0.715 - 0.5, 0.0,
                        0.487 - 0.5, 0.72 - 0.5, 0.0,
                        0.48 - 0.5, 0.715 - 0.5, 0.0,
                        0.462 - 0.5, 0.71 - 0.5, 0.0,
                        0.457 - 0.5, 0.708 - 0.5, 0.0,
                        0.445 - 0.5, 0.705 - 0.5, 0.0,
                        0.428 - 0.5, 0.7 - 0.5, 0.0,
                        0.422 - 0.5, 0.69 - 0.5, 0.0,
                        0.417 - 0.5, 0.685 - 0.5, 0.0,
                        0.411 - 0.5, 0.675 - 0.5, 0.0,
                        0.406 - 0.5, 0.678 - 0.5, 0.0,
                        0.4 - 0.5, 0.675 - 0.5, 0.0,
                        0.383 - 0.5, 0.61 - 0.5, 0.0,
                        0.391 - 0.5, 0.6 - 0.5, 0.0,
                        0.443 - 0.5, 0.64 - 0.5, 0.0,
                        0.393 - 0.5, 0.56 - 0.5, 0.0,
                        0.461 - 0.5, 0.65 - 0.5, 0.0,
                        0.483 - 0.5, 0.5 - 0.5, 0.0,
                        0.486 - 0.5, 0.57 - 0.5, 0.0,
                        0.487 - 0.5, 0.6 - 0.5, 0.0,
                        0.461 - 0.5, 0.65 - 0.5, 0.0,
                        0.483 - 0.5, 0.5 - 0.5, 0.0,
                        0.486 - 0.5, 0.57 - 0.5, 0.0,
                        0.487 - 0.5, 0.6 - 0.5, 0.0,
                        0.519 - 0.5, 0.71 - 0.5, 0.0,
                        0.522 - 0.5, 0.69 - 0.5, 0.0,
                        0.525 - 0.5, 0.677 - 0.5, 0.0,
                        0.534 - 0.5, 0.68 - 0.5, 0.0,
                        0.544 - 0.5, 0.7 - 0.5, 0.0,
                        0.557 - 0.5, 0.725 - 0.5, 0.0,
                        0.57 - 0.5, 0.73 - 0.5, 0.0,
                        0.588 - 0.5, 0.74 - 0.5, 0.0,
                        0.592 - 0.5, 0.765 - 0.5, 0.0,
                        0.574 - 0.5, 0.765 - 0.5, 0.0,
                        0.562 - 0.5, 0.755 - 0.5, 0.0,
                        0.562 - 0.5, 0.755 - 0.5, 0.0,
                        0.57 - 0.5, 0.83 - 0.5, 0.0,
                        0.574 - 0.5, 0.845 - 0.5, 0.0,
                        0.574 - 0.5, 0.765 - 0.5, 0.0,
                        0.57 - 0.5, 0.73 - 0.5, 0.0,
                        0.562 - 0.5, 0.67 - 0.5, 0.0,
                        0.555 - 0.5, 0.625 - 0.5, 0.0,
                        0.533 - 0.5, 0.605 - 0.5, 0.0,
                        0.549 - 0.5, 0.655 - 0.5, 0.0,
                        0.557 - 0.5, 0.725 - 0.5, 0.0,
                        0.574 - 0.5, 0.675 - 0.5, 0.0,
                        0.593 - 0.5, 0.63 - 0.5, 0.0,
                        0.608 - 0.5, 0.625 - 0.5, 0.0,
                        0.602 - 0.5, 0.65 - 0.5, 0.0,
                        0.608 - 0.5, 0.625 - 0.5, 0.0,
                        0.593 - 0.5, 0.63 - 0.5, 0.0,
                        0.574 - 0.5, 0.6 - 0.5, 0.0,
                        0.592 - 0.5, 0.6 - 0.5, 0.0,
                        0.602 - 0.5, 0.595 - 0.5, 0.0,
                        0.641 - 0.5, 0.84 - 0.5, 0.0,
                        0.646 - 0.5, 0.825 - 0.5, 0.0,
                        0.654 - 0.5, 0.855 - 0.5, 0.0,
                        0.659 - 0.5, 0.842 - 0.5, 0.0,
                        0.653 - 0.5, 0.82 - 0.5, 0.0,
                        0.659 - 0.5, 0.818 - 0.5, 0.0,
                        0.658 - 0.5, 0.79 - 0.5, 0.0,
                        0.653 - 0.5, 0.76 - 0.5, 0.0,
                        0.653 - 0.5, 0.7 - 0.5, 0.0,
                        0.641 - 0.5, 0.7 - 0.5, 0.0,
                        0.638 - 0.5, 0.8 - 0.5, 0.0,
                        0.653 - 0.5, 0.76 - 0.5, 0.0,
                        0.652 - 0.5, 0.83 - 0.5, 0.0,
                        0.675 - 0.5, 0.8 - 0.5, 0.0,
                        0.67 - 0.5, 0.825 - 0.5, 0.0,
                        0.683 - 0.5, 0.84 - 0.5, 0.0,
                        0.675 - 0.5, 0.855 - 0.5, 0.0,
                        0.6675 - 0.5, 0.82 - 0.5, 0.0,
                        0.667 - 0.5, 0.794 - 0.5, 0.0,
                        0.667 - 0.5, 0.794 - 0.5, 0.0,
                        0.661 - 0.5, 0.8 - 0.5, 0.0,
                        0.661 - 0.5, 0.819 - 0.5, 0.0,
                        0.6675 - 0.5, 0.82 - 0.5, 0.0,
                        0.679 - 0.5, 0.825 - 0.5, 0.0,
                        0.684 - 0.5, 0.825 - 0.5, 0.0,
                        0.683 - 0.5, 0.8 - 0.5, 0.0,
                        0.675 - 0.5, 0.8 - 0.5, 0.0,
                        0.662 - 0.5, 0.755 - 0.5, 0.0,
                        0.667 - 0.5, 0.725 - 0.5, 0.0,
                        0.678 - 0.5, 0.73 - 0.5, 0.0,
                        0.677 - 0.5, 0.75 - 0.5, 0.0,
                        0.668 - 0.5, 0.76 - 0.5, 0.0,
                        0.668 - 0.5, 0.76 - 0.5, 0.0,
                        0.677 - 0.5, 0.75 - 0.5, 0.0,
                        0.679 - 0.5, 0.78 - 0.5, 0.0,
                        0.675 - 0.5, 0.8 - 0.5, 0.0,
                        0.684 - 0.5, 0.83 - 0.5, 0.0,
                        0.686 - 0.5, 0.82 - 0.5, 0.0,
                        0.694 - 0.5, 0.81 - 0.5, 0.0,
                        0.704 - 0.5, 0.83 - 0.5, 0.0,
                        0.702 - 0.5, 0.85 - 0.5, 0.0,
                        0.704 - 0.5, 0.83 - 0.5, 0.0,
                        0.696 - 0.5, 0.808 - 0.5, 0.0,
                        0.683 - 0.5, 0.8 - 0.5, 0.0,
                        0.684 - 0.5, 0.775 - 0.5, 0.0,
                        0.683 - 0.5, 0.75 - 0.5, 0.0,
                        0.688 - 0.5, 0.74 - 0.5, 0.0,
                        0.698 - 0.5, 0.745 - 0.5, 0.0,
                        0.699 - 0.5, 0.8 - 0.5, 0.0,
                        0.6335 - 0.5, 0.695 - 0.5, 0.0,
                        0.618 - 0.5, 0.625 - 0.5, 0.0,
                        0.627 - 0.5, 0.6 - 0.5, 0.0,
                        0.641 - 0.5, 0.67 - 0.5, 0.0,
                        0.6335 - 0.5, 0.695 - 0.5, 0.0,
                        0.641 - 0.5, 0.67 - 0.5, 0.0,
                        0.711 - 0.5, 0.725 - 0.5, 0.0,
                        0.725 - 0.5, 0.725 - 0.5, 0.0,
                        0.714 - 0.5, 0.748 - 0.5, 0.0,
                        0.698 - 0.5, 0.745 - 0.5, 0.0,
                        0.688 - 0.5, 0.74 - 0.5, 0.0,
                        0.678 - 0.5, 0.73 - 0.5, 0.0,
                        0.667 - 0.5, 0.725 - 0.5, 0.0,
                        0.653 - 0.5, 0.7 - 0.5, 0.0,
                        0.641 - 0.5, 0.698 - 0.5, 0.0,
                        0.725 - 0.5, 0.725 - 0.5, 0.0,
                        0.711 - 0.5, 0.725 - 0.5, 0.0,
                        0.698 - 0.5, 0.695 - 0.5, 0.0,
                        0.722 - 0.5, 0.7 - 0.5, 0.0,
                        0.725 - 0.5, 0.713 - 0.5, 0.0,
                        0.667 - 0.5, 0.675 - 0.5, 0.0,
                        0.675 - 0.5, 0.66 - 0.5, 0.0,
                        0.685 - 0.5, 0.66 - 0.5, 0.0,
                        0.687 - 0.5, 0.68 - 0.5, 0.0,
                        0.684 - 0.5, 0.7 - 0.5, 0.0,
                        0.685 - 0.5, 0.66 - 0.5, 0.0,
                        0.675 - 0.5, 0.66 - 0.5, 0.0,
                        0.68 - 0.5, 0.645 - 0.5, 0.0,
                        0.682 - 0.5, 0.625 - 0.5, 0.0,
                        0.683 - 0.5, 0.58 - 0.5, 0.0,
                        0.678 - 0.5, 0.54 - 0.5, 0.0,
                        0.6875 - 0.5, 0.525 - 0.5, 0.0,
                        0.6875 - 0.5, 0.625 - 0.5, 0.0,
                        0.6875 - 0.5, 0.65 - 0.5, 0.0,
                        0.6875 - 0.5, 0.525 - 0.5, 0.0,
                        0.678 - 0.5, 0.54 - 0.5, 0.0,
                        0.652 - 0.5, 0.475 - 0.5, 0.0,
                        0.65 - 0.5, 0.44 - 0.5, 0.0,
                        0.667 - 0.5, 0.45 - 0.5, 0.0,
                        0.679 - 0.5, 0.5 - 0.5, 0.0,
                        0.6875 - 0.5, 0.65 - 0.5, 0.0,
                        0.68 - 0.5, 0.645 - 0.5, 0.0,
                        0.65 - 0.5, 0.615 - 0.5, 0.0,
                        0.64 - 0.5, 0.62 - 0.5, 0.0,
                        0.645 - 0.5, 0.6 - 0.5, 0.0,
                        0.682 - 0.5, 0.625 - 0.5, 0.0,
                        0.6875 - 0.5, 0.625 - 0.5, 0.0,
                        0.706 - 0.5, 0.6245 - 0.5, 0.0,
                        0.708 - 0.5, 0.64 - 0.5, 0.0,
                        0.7 - 0.5, 0.65 - 0.5, 0.0,
                        0.65 - 0.5, 0.44 - 0.5, 0.0,
                        0.652 - 0.5, 0.475 - 0.5, 0.0,
                        0.633 - 0.5, 0.458 - 0.5, 0.0,
                        0.617 - 0.5, 0.465 - 0.5, 0.0,
                        0.63 - 0.5, 0.445 - 0.5, 0.0,
                        getv(756), getv_b(28), 0.0,
                        getv(758), getv_b(36), 0.0,
                        getv(771), getv_b(42), 0.0,
                        getv(771), getv_b(33), 0.0,
                        getv(785), getv_b(46), 0.0,
                        getv(787), getv_b(36), 0.0,
                        getv(799), getv_b(55), 0.0,
                        getv(802), getv_b(37), 0.0,
                        getv(810), getv_b(63), 0.0,
                        getv(814), getv_b(38), 0.0,
                        getv(826), getv_b(62), 0.0,
                        getv(845), getv_b(36), 0.0,
                        getv(835), getv_b(66), 0.0,
                        getv(862), getv_b(34), 0.0,
                        getv(842), getv_b(71), 0.0,
                        getv(885), getv_b(28), 0.0,
                        getv(846), getv_b(77), 0.0,
                        getv(878), getv_b(46), 0.0,
                        getv(850), getv_b(84), 0.0,
                        getv(874), getv_b(66), 0.0,
                        getv(851), getv_b(93), 0.0,
                        getv(873), getv_b(80), 0.0,
                        getv(846), getv_b(108), 0.0,
                        getv(872), getv_b(95), 0.0,
                        getv(851), getv_b(121), 0.0,
                        getv(875), getv_b(111), 0.0,
                        getv(856), getv_b(135), 0.0,
                        getv(879), getv_b(129), 0.0,
                        getv(860), getv_b(150), 0.0,
                        getv(884), getv_b(143), 0.0,
                        getv(878), getv_b(157), 0.0,
                        getv(888), getv_b(152), 0.0,
                        getv(886), getv_b(161), 0.0,
                        getv(894), getv_b(164), 0.0,
                        getv(760), getv_b(44), 0.0,
                        getv(764), getv_b(60), 0.0,
                        getv(777), getv_b(51), 0.0,
                        getv(769), getv_b(47), 0.0,
                        getv(765), getv_b(71), 0.0,
                        getv(788), getv_b(57), 0.0,
                        getv(766), getv_b(82), 0.0,
                        getv(803), getv_b(67), 0.0,
                        getv(765), getv_b(90), 0.0,
                        getv(792), getv_b(80), 0.0,
                        getv(764), getv_b(102), 0.0,
                        getv(790), getv_b(94), 0.0,
                        getv(762), getv_b(115), 0.0,
                        getv(793), getv_b(105), 0.0,
                        getv(757), getv_b(134), 0.0,
                        getv(797), getv_b(113), 0.0,
                        getv(746), getv_b(161), 0.0,
                        getv(804), getv_b(119), 0.0,
                        getv(784), getv_b(148), 0.0,
                        getv(814), getv_b(122), 0.0,
                        getv(811), getv_b(144), 0.0,
                        getv(826), getv_b(122), 0.0,
                        getv(826), getv_b(144), 0.0,
                        getv(833), getv_b(119), 0.0,
                        getv(843), getv_b(145), 0.0,
                        getv(841), getv_b(114), 0.0,
                        getv(840), getv_b(145), 0.0,
                        getv(848), getv_b(129), 0.0,
                        getv(848), getv_b(147), 0.0,
                        getv(853), getv_b(139), 0.0,
                        getv(856), getv_b(149), 0.0,
                        getv(854), getv_b(143), 0.0,
                        getv(832), getv_b(78), 0.0,
                        getv(810), getv_b(63), 0.0,
                        getv(826), getv_b(62), 0.0,
                        getv(835), getv_b(66), 0.0,
                        getv(842), getv_b(71), 0.0,
                        getv(846), getv_b(77), 0.0,
                        getv(850), getv_b(84), 0.0,
                        getv(851), getv_b(93), 0.0,
                        getv(846), getv_b(108), 0.0,
                        getv(832), getv_b(78), 0.0,
                        getv(846), getv_b(108), 0.0,
                        getv(838), getv_b(96), 0.0,
                        getv(829), getv_b(84), 0.0,
                        getv(821), getv_b(74), 0.0,
                        getv(810), getv_b(63), 0.0,
                        getv(803), getv_b(67), 0.0,
                        getv(792), getv_b(80), 0.0,
                        getv(790), getv_b(94), 0.0,
                        getv(793), getv_b(105), 0.0,
                        getv(797), getv_b(113), 0.0,
                        getv(804), getv_b(119), 0.0,
                        getv(814), getv_b(122), 0.0,
                        getv(826), getv_b(122), 0.0,
                        getv(833), getv_b(119), 0.0,
                        getv(841), getv_b(114), 0.0,
                        getv(832), getv_b(99), 0.0,
                        getv(825), getv_b(89), 0.0,
                        getv(815), getv_b(78), 0.0,
                        getv(809), getv_b(72), 0.0,
                        getv(31), getv_b(276), 0.0,
                        getv(35), getv_b(276), 0.0,
                        getv(31), getv_b(238), 0.0,
                        getv(35), getv_b(238), 0.0,
                        getv(35), getv_b(238), 0.0,
                        getv(35), getv_b(246), 0.0,
                        getv(57), getv_b(268), 0.0,
                        getv(57), getv_b(276), 0.0,
                        getv(57), getv_b(276), 0.0,
                        getv(61), getv_b(276), 0.0,
                        getv(57), getv_b(238), 0.0,
                        getv(61), getv_b(238), 0.0,
                        getv(72), getv_b(255), 0.0,//a
                        getv(76), getv_b(257), 0.0,
                        getv(75), getv_b(250), 0.0,
                        getv(79), getv_b(253), 0.0,
                        getv(80), getv_b(248), 0.0,
                        getv(83), getv_b(252), 0.0,
                        getv(85), getv_b(247), 0.0,
                        getv(88), getv_b(253), 0.0,
                        getv(91), getv_b(249), 0.0,
                        getv(90), getv_b(254), 0.0,
                        getv(94), getv_b(252), 0.0,
                        getv(91), getv_b(258), 0.0,
                        getv(95), getv_b(257), 0.0,
                        getv(92), getv_b(276), 0.0,
                        getv(97), getv_b(276), 0.0,
                        getv(91), getv_b(258), 0.0,
                        getv(91), getv_b(262), 0.0,
                        getv(87), getv_b(259), 0.0,
                        getv(85), getv_b(264), 0.0,
                        getv(81), getv_b(260), 0.0,
                        getv(80), getv_b(264), 0.0,
                        getv(77), getv_b(262), 0.0,
                        getv(78), getv_b(266), 0.0,
                        getv(73), getv_b(264), 0.0,
                        getv(76), getv_b(268), 0.0,
                        getv(71), getv_b(269), 0.0,
                        getv(77), getv_b(271), 0.0,
                        getv(72), getv_b(273), 0.0,
                        getv(79), getv_b(273), 0.0,
                        getv(77), getv_b(276), 0.0,
                        getv(82), getv_b(273), 0.0,
                        getv(82), getv_b(277), 0.0,
                        getv(86), getv_b(272), 0.0,
                        getv(88), getv_b(275), 0.0,
                        getv(89), getv_b(271), 0.0,
                        getv(92), getv_b(271), 0.0,
                        getv(92), getv_b(267), 0.0,//
                        getv(105), getv_b(248), 0.0,//t
                        getv(105), getv_b(253), 0.0,
                        getv(117), getv_b(248), 0.0,
                        getv(117), getv_b(253), 0.0,
                        getv(108), getv_b(241), 0.0,
                        getv(112), getv_b(238), 0.0,
                        getv(108), getv_b(273), 0.0,
                        getv(112), getv_b(271), 0.0,
                        getv(111), getv_b(276), 0.0,
                        getv(117), getv_b(272), 0.0,
                        getv(117), getv_b(276), 0.0,//
                        getv(126), getv_b(242), 0.0,//i
                        getv(126), getv_b(238), 0.0,
                        getv(130), getv_b(242), 0.0,
                        getv(130), getv_b(238), 0.0,
                        getv(126), getv_b(276), 0.0,
                        getv(126), getv_b(248), 0.0,
                        getv(130), getv_b(276), 0.0,
                        getv(130), getv_b(248), 0.0,// 
                        getv(140), getv_b(261), 0.0,
                        getv(144), getv_b(259), 0.0,
                        getv(141), getv_b(254), 0.0,
                        getv(145), getv_b(256), 0.0,
                        getv(145), getv_b(250), 0.0,
                        getv(149), getv_b(253), 0.0,
                        getv(151), getv_b(247), 0.0,
                        getv(153), getv_b(252), 0.0,
                        getv(157), getv_b(248), 0.0,
                        getv(157), getv_b(253), 0.0,
                        getv(162), getv_b(250), 0.0,
                        getv(160), getv_b(256), 0.0,
                        getv(164), getv_b(255), 0.0,
                        getv(160), getv_b(260), 0.0,
                        getv(165), getv_b(260), 0.0,
                        getv(160), getv_b(265), 0.0,
                        getv(165), getv_b(267), 0.0,
                        getv(160), getv_b(268), 0.0,
                        getv(162), getv_b(273), 0.0,
                        getv(157), getv_b(272), 0.0,
                        getv(157), getv_b(276), 0.0,
                        getv(153), getv_b(273), 0.0,
                        getv(151), getv_b(277), 0.0,
                        getv(149), getv_b(272), 0.0,
                        getv(146), getv_b(275), 0.0,
                        getv(146), getv_b(270), 0.0,
                        getv(141), getv_b(270), 0.0,
                        getv(145), getv_b(266), 0.0,
                        getv(139), getv_b(266), 0.0,
                        getv(144), getv_b(263), 0.0,
                        getv(141), getv_b(254), 0.0,
                        getv(175), getv_b(248), 0.0,//n
                        getv(179), getv_b(248), 0.0,
                        getv(175), getv_b(276), 0.0,
                        getv(179), getv_b(276), 0.0,
                        getv(179), getv_b(256), 0.0,
                        getv(179), getv_b(251), 0.0,
                        getv(185), getv_b(253), 0.0,
                        getv(184), getv_b(248), 0.0,
                        getv(188), getv_b(253), 0.0,
                        getv(189), getv_b(247), 0.0,
                        getv(191), getv_b(253), 0.0,
                        getv(195), getv_b(249), 0.0,
                        getv(193), getv_b(256), 0.0,
                        getv(198), getv_b(256), 0.0,
                        getv(193), getv_b(276), 0.0,
                        getv(198), getv_b(276), 0.0, //
                        getv(72 + 136), getv_b(255), 0.0,//a+136
                        getv(76 + 136), getv_b(257), 0.0,
                        getv(75 + 136), getv_b(250), 0.0,
                        getv(79 + 136), getv_b(253), 0.0,
                        getv(80 + 136), getv_b(248), 0.0,
                        getv(83 + 136), getv_b(252), 0.0,
                        getv(85 + 136), getv_b(247), 0.0,
                        getv(88 + 136), getv_b(253), 0.0,
                        getv(91 + 136), getv_b(249), 0.0,
                        getv(90 + 136), getv_b(254), 0.0,
                        getv(94 + 136), getv_b(252), 0.0,
                        getv(91 + 136), getv_b(258), 0.0,
                        getv(95 + 136), getv_b(257), 0.0,
                        getv(92 + 136), getv_b(276), 0.0,
                        getv(97 + 136), getv_b(276), 0.0,
                        getv(91 + 136), getv_b(258), 0.0,
                        getv(91 + 136), getv_b(262), 0.0,
                        getv(87 + 136), getv_b(259), 0.0,
                        getv(85 + 136), getv_b(264), 0.0,
                        getv(81 + 136), getv_b(260), 0.0,
                        getv(80 + 136), getv_b(264), 0.0,
                        getv(77 + 136), getv_b(262), 0.0,
                        getv(78 + 136), getv_b(266), 0.0,
                        getv(73 + 136), getv_b(264), 0.0,
                        getv(76 + 136), getv_b(268), 0.0,
                        getv(71 + 136), getv_b(269), 0.0,
                        getv(77 + 136), getv_b(271), 0.0,
                        getv(72 + 136), getv_b(273), 0.0,
                        getv(79 + 136), getv_b(273), 0.0,
                        getv(77 + 136), getv_b(276), 0.0,
                        getv(82 + 136), getv_b(273), 0.0,
                        getv(82 + 136), getv_b(277), 0.0,
                        getv(86 + 136), getv_b(272), 0.0,
                        getv(88 + 136), getv_b(275), 0.0,
                        getv(89 + 136), getv_b(271), 0.0,
                        getv(92 + 136), getv_b(271), 0.0,
                        getv(92 + 136), getv_b(267), 0.0,// 
                        getv(242), getv_b(238), 0.0,
                        getv(247), getv_b(238), 0.0,
                        getv(242), getv_b(276), 0.0,
                        getv(247), getv_b(276), 0.0,
                        getv(298), getv_b(250), 0.0,
                        getv(304), getv_b(250), 0.0,
                        getv(298), getv_b(248), 0.0,
                        getv(302), getv_b(244), 0.0,
                        getv(297), getv_b(245), 0.0,
                        getv(298), getv_b(239), 0.0,
                        getv(294), getv_b(242), 0.0,
                        getv(294), getv_b(237), 0.0,
                        getv(292), getv_b(242), 0.0,
                        getv(291), getv_b(236), 0.0,
                        getv(288), getv_b(241), 0.0,
                        getv(285), getv_b(237), 0.0,
                        getv(285), getv_b(242), 0.0,
                        getv(281), getv_b(238), 0.0,
                        getv(282), getv_b(243), 0.0,
                        getv(275), getv_b(241), 0.0,
                        getv(278), getv_b(246), 0.0,
                        getv(271), getv_b(246), 0.0,
                        getv(276), getv_b(250), 0.0,
                        getv(269), getv_b(251), 0.0,
                        getv(275), getv_b(254), 0.0,
                        getv(269), getv_b(257), 0.0,
                        getv(275), getv_b(260), 0.0,
                        getv(270), getv_b(263), 0.0,
                        getv(276), getv_b(266), 0.0,
                        getv(272), getv_b(270), 0.0,
                        getv(279), getv_b(269), 0.0,
                        getv(278), getv_b(274), 0.0,
                        getv(283), getv_b(271), 0.0,
                        getv(283), getv_b(276), 0.0,
                        getv(288), getv_b(272), 0.0,
                        getv(290), getv_b(277), 0.0,
                        getv(292), getv_b(271), 0.0,
                        getv(296), getv_b(275), 0.0,
                        getv(296), getv_b(269), 0.0,
                        getv(302), getv_b(271), 0.0,
                        getv(298), getv_b(266), 0.0,
                        getv(304), getv_b(266), 0.0,
                        getv(299), getv_b(263), 0.0,
                        getv(304), getv_b(263), 0.0,
                        getv(175 + 138), getv_b(248), 0.0,//n+138
                        getv(179 + 138), getv_b(248), 0.0,
                        getv(175 + 138), getv_b(276), 0.0,
                        getv(179 + 138), getv_b(276), 0.0,
                        getv(179 + 138), getv_b(256), 0.0,
                        getv(179 + 138), getv_b(251), 0.0,
                        getv(185 + 138), getv_b(253), 0.0,
                        getv(184 + 138), getv_b(248), 0.0,
                        getv(188 + 138), getv_b(253), 0.0,
                        getv(189 + 138), getv_b(247), 0.0,
                        getv(191 + 138), getv_b(253), 0.0,
                        getv(195 + 138), getv_b(249), 0.0,
                        getv(193 + 138), getv_b(256), 0.0,
                        getv(198 + 138), getv_b(256), 0.0,
                        getv(193 + 138), getv_b(276), 0.0,
                        getv(198 + 138), getv_b(276), 0.0,//
                        getv(313), getv_b(238), 0.0,
                        getv(318), getv_b(238), 0.0,
                        getv(313), getv_b(276), 0.0,
                        getv(318), getv_b(276), 0.0,
                        getv(353), getv_b(247), 0.0,//u
                        getv(348), getv_b(247), 0.0,
                        getv(353), getv_b(270), 0.0,
                        getv(348), getv_b(270), 0.0,
                        getv(348), getv_b(270), 0.0,
                        getv(353), getv_b(270), 0.0,
                        getv(351), getv_b(275), 0.0,
                        getv(356), getv_b(272), 0.0,
                        getv(357), getv_b(276), 0.0,
                        getv(360), getv_b(272), 0.0,
                        getv(362), getv_b(275), 0.0,
                        getv(363), getv_b(271), 0.0,
                        getv(366), getv_b(272), 0.0,
                        getv(366), getv_b(269), 0.0,
                        getv(366), getv_b(247), 0.0,
                        getv(371), getv_b(247), 0.0,
                        getv(366), getv_b(276), 0.0,
                        getv(371), getv_b(276), 0.0,//
                        getv(175 + 207), getv_b(248), 0.0,//n382-175=+207
                        getv(179 + 207), getv_b(248), 0.0,
                        getv(175 + 207), getv_b(276), 0.0,
                        getv(179 + 207), getv_b(276), 0.0,
                        getv(179 + 207), getv_b(256), 0.0,
                        getv(179 + 207), getv_b(251), 0.0,
                        getv(185 + 207), getv_b(253), 0.0,
                        getv(184 + 207), getv_b(248), 0.0,
                        getv(188 + 207), getv_b(253), 0.0,
                        getv(189 + 207), getv_b(247), 0.0,
                        getv(191 + 207), getv_b(253), 0.0,
                        getv(195 + 207), getv_b(249), 0.0,
                        getv(193 + 207), getv_b(256), 0.0,
                        getv(198 + 207), getv_b(256), 0.0,
                        getv(193 + 207), getv_b(276), 0.0,
                        getv(198 + 207), getv_b(276), 0.0,//
                        getv(435), getv_b(257), 0.0,//g
                        getv(435), getv_b(252), 0.0,
                        getv(432), getv_b(254), 0.0,
                        getv(431), getv_b(249), 0.0,
                        getv(429), getv_b(252), 0.0,
                        getv(425), getv_b(247), 0.0,
                        getv(424), getv_b(252), 0.0,
                        getv(420), getv_b(249), 0.0,
                        getv(421), getv_b(254), 0.0,
                        getv(416), getv_b(254), 0.0,
                        getv(420), getv_b(258), 0.0,
                        getv(415), getv_b(262), 0.0,
                        getv(419), getv_b(266), 0.0,
                        getv(415), getv_b(269), 0.0,
                        getv(421), getv_b(270), 0.0,
                        getv(420), getv_b(274), 0.0,
                        getv(425), getv_b(272), 0.0,
                        getv(425), getv_b(276), 0.0,
                        getv(428), getv_b(272), 0.0,
                        getv(431), getv_b(275), 0.0,
                        getv(431), getv_b(270), 0.0,
                        getv(434), getv_b(272), 0.0,
                        getv(434), getv_b(267), 0.0,
                        getv(434), getv_b(248), 0.0,
                        getv(439), getv_b(248), 0.0,
                        getv(434), getv_b(279), 0.0,
                        getv(439), getv_b(279), 0.0,
                        getv(432), getv_b(281), 0.0,
                        getv(435), getv_b(285), 0.0,
                        getv(429), getv_b(284), 0.0,
                        getv(430), getv_b(288), 0.0,
                        getv(426), getv_b(285), 0.0,
                        getv(423), getv_b(288), 0.0,
                        getv(423), getv_b(283), 0.0,
                        getv(416), getv_b(284), 0.0,
                        getv(420), getv_b(279), 0.0,
                        getv(415), getv_b(279), 0.0,//
                        getv(463), getv_b(238), 0.0,
                        getv(468), getv_b(238), 0.0,
                        getv(463), getv_b(276), 0.0,
                        getv(468), getv_b(276), 0.0,
                        getv(489), getv_b(238), 0.0,
                        getv(494), getv_b(238), 0.0,
                        getv(489), getv_b(276), 0.0,
                        getv(494), getv_b(276), 0.0,
                        getv(468), getv_b(253), 0.0,
                        getv(489), getv_b(253), 0.0,
                        getv(468), getv_b(258), 0.0,
                        getv(489), getv_b(258), 0.0,
                        getv(526), getv_b(255), 0.0,//s
                        getv(521), getv_b(255), 0.0,
                        getv(525), getv_b(251), 0.0,
                        getv(520), getv_b(253), 0.0,
                        getv(521), getv_b(248), 0.0,
                        getv(518), getv_b(251), 0.0,
                        getv(516), getv_b(247), 0.0,
                        getv(513), getv_b(251), 0.0,
                        getv(510), getv_b(248), 0.0,
                        getv(510), getv_b(253), 0.0,
                        getv(506), getv_b(253), 0.0,
                        getv(509), getv_b(256), 0.0,
                        getv(505), getv_b(258), 0.0,
                        getv(511), getv_b(258), 0.0,
                        getv(509), getv_b(262), 0.0,
                        getv(514), getv_b(260), 0.0,
                        getv(514), getv_b(263), 0.0,
                        getv(519), getv_b(260), 0.0,
                        getv(519), getv_b(264), 0.0,
                        getv(523), getv_b(262), 0.0,
                        getv(522), getv_b(266), 0.0,
                        getv(528), getv_b(266), 0.0,
                        getv(523), getv_b(268), 0.0,
                        getv(527), getv_b(271), 0.0,
                        getv(522), getv_b(271), 0.0,
                        getv(523), getv_b(275), 0.0,
                        getv(519), getv_b(273), 0.0,
                        getv(517), getv_b(276), 0.0,
                        getv(513), getv_b(272), 0.0,
                        getv(509), getv_b(275), 0.0,
                        getv(510), getv_b(271), 0.0,
                        getv(505), getv_b(272), 0.0,
                        getv(508), getv_b(267), 0.0,
                        getv(505), getv_b(267), 0.0,//
                        getv(126 + 411), getv_b(242), 0.0,//i
                        getv(126 + 411), getv_b(238), 0.0,
                        getv(130 + 411), getv_b(242), 0.0,
                        getv(130 + 411), getv_b(238), 0.0,
                        getv(126 + 411), getv_b(276), 0.0,
                        getv(126 + 411), getv_b(248), 0.0,
                        getv(130 + 411), getv_b(276), 0.0,
                        getv(130 + 411), getv_b(248), 0.0,//
                        getv(175 + 377), getv_b(248), 0.0,//n+377
                        getv(179 + 377), getv_b(248), 0.0,
                        getv(175 + 377), getv_b(276), 0.0,
                        getv(179 + 377), getv_b(276), 0.0,
                        getv(179 + 377), getv_b(256), 0.0,
                        getv(179 + 377), getv_b(251), 0.0,
                        getv(185 + 377), getv_b(253), 0.0,
                        getv(184 + 377), getv_b(248), 0.0,
                        getv(188 + 377), getv_b(253), 0.0,
                        getv(189 + 377), getv_b(247), 0.0,
                        getv(191 + 377), getv_b(253), 0.0,
                        getv(195 + 377), getv_b(249), 0.0,
                        getv(193 + 377), getv_b(256), 0.0,
                        getv(198 + 377), getv_b(256), 0.0,
                        getv(193 + 377), getv_b(276), 0.0,
                        getv(198 + 377), getv_b(276), 0.0,//
                        getv(435 + 170), getv_b(257), 0.0,//g
                        getv(435 + 170), getv_b(252), 0.0,
                        getv(432 + 170), getv_b(254), 0.0,
                        getv(431 + 170), getv_b(249), 0.0,
                        getv(429 + 170), getv_b(252), 0.0,
                        getv(425 + 170), getv_b(247), 0.0,
                        getv(424 + 170), getv_b(252), 0.0,
                        getv(420 + 170), getv_b(249), 0.0,
                        getv(421 + 170), getv_b(254), 0.0,
                        getv(416 + 170), getv_b(254), 0.0,
                        getv(420 + 170), getv_b(258), 0.0,
                        getv(415 + 170), getv_b(262), 0.0,
                        getv(419 + 170), getv_b(266), 0.0,
                        getv(415 + 170), getv_b(269), 0.0,
                        getv(421 + 170), getv_b(270), 0.0,
                        getv(420 + 170), getv_b(274), 0.0,
                        getv(425 + 170), getv_b(272), 0.0,
                        getv(425 + 170), getv_b(276), 0.0,
                        getv(428 + 170), getv_b(272), 0.0,
                        getv(431 + 170), getv_b(275), 0.0,
                        getv(431 + 170), getv_b(270), 0.0,
                        getv(434 + 170), getv_b(272), 0.0,
                        getv(434 + 170), getv_b(267), 0.0,
                        getv(434 + 170), getv_b(248), 0.0,
                        getv(439 + 170), getv_b(248), 0.0,
                        getv(434 + 170), getv_b(279), 0.0,
                        getv(439 + 170), getv_b(279), 0.0,
                        getv(432 + 170), getv_b(281), 0.0,
                        getv(435 + 170), getv_b(285), 0.0,
                        getv(429 + 170), getv_b(284), 0.0,
                        getv(430 + 170), getv_b(288), 0.0,
                        getv(426 + 170), getv_b(285), 0.0,
                        getv(423 + 170), getv_b(288), 0.0,
                        getv(423 + 170), getv_b(283), 0.0,
                        getv(416 + 170), getv_b(284), 0.0,
                        getv(420 + 170), getv_b(279), 0.0,
                        getv(415 + 170), getv_b(279), 0.0,//
                        getv(633), getv_b(238), 0.0,//U
                        getv(638), getv_b(238), 0.0,
                        getv(633), getv_b(266), 0.0,
                        getv(638), getv_b(266), 0.0,
                        getv(637), getv_b(273), 0.0,
                        getv(641), getv_b(270), 0.0,
                        getv(643), getv_b(276), 0.0,
                        getv(647), getv_b(272), 0.0,
                        getv(651), getv_b(276), 0.0,
                        getv(654), getv_b(271), 0.0,
                        getv(659), getv_b(273), 0.0,
                        getv(659), getv_b(266), 0.0,
                        getv(664), getv_b(266), 0.0,
                        getv(659), getv_b(238), 0.0,
                        getv(664), getv_b(238), 0.0, //
                        getv(175 + 501), getv_b(248), 0.0,//n+501
                        getv(179 + 501), getv_b(248), 0.0,
                        getv(175 + 501), getv_b(276), 0.0,
                        getv(179 + 501), getv_b(276), 0.0,
                        getv(179 + 501), getv_b(256), 0.0,
                        getv(179 + 501), getv_b(251), 0.0,
                        getv(185 + 501), getv_b(253), 0.0,
                        getv(184 + 501), getv_b(248), 0.0,
                        getv(188 + 501), getv_b(253), 0.0,
                        getv(189 + 501), getv_b(247), 0.0,
                        getv(191 + 501), getv_b(253), 0.0,
                        getv(195 + 501), getv_b(249), 0.0,
                        getv(193 + 501), getv_b(256), 0.0,
                        getv(198 + 501), getv_b(256), 0.0,
                        getv(193 + 501), getv_b(276), 0.0,
                        getv(198 + 501), getv_b(276), 0.0,//
                        getv(126 + 584), getv_b(242), 0.0,//i+584
                        getv(126 + 584), getv_b(238), 0.0,
                        getv(130 + 584), getv_b(242), 0.0,
                        getv(130 + 584), getv_b(238), 0.0,
                        getv(126 + 584), getv_b(276), 0.0,
                        getv(126 + 584), getv_b(248), 0.0,
                        getv(130 + 584), getv_b(276), 0.0,
                        getv(130 + 584), getv_b(248), 0.0,//
                        getv(721), getv_b(248), 0.0,//v
                        getv(726), getv_b(248), 0.0,
                        getv(733), getv_b(276), 0.0,
                        getv(735), getv_b(270), 0.0,
                        getv(738), getv_b(276), 0.0,
                        getv(743), getv_b(248), 0.0,
                        getv(748), getv_b(248), 0.0,//
                        getv(760), getv_b(259), 0.0,//e
                        getv(760), getv_b(263), 0.0,
                        getv(777), getv_b(259), 0.0,
                        getv(781), getv_b(263), 0.0,
                        getv(776), getv_b(256), 0.0,
                        getv(779), getv_b(254), 0.0,
                        getv(774), getv_b(254), 0.0,
                        getv(774), getv_b(249), 0.0,
                        getv(769), getv_b(252), 0.0,
                        getv(768), getv_b(247), 0.0,
                        getv(765), getv_b(252), 0.0,
                        getv(762), getv_b(249), 0.0,
                        getv(761), getv_b(254), 0.0,
                        getv(757), getv_b(253), 0.0,
                        getv(760), getv_b(259), 0.0,
                        getv(754), getv_b(259), 0.0,
                        getv(760), getv_b(263), 0.0,
                        getv(754), getv_b(266), 0.0,
                        getv(761), getv_b(268), 0.0,
                        getv(759), getv_b(273), 0.0,
                        getv(763), getv_b(272), 0.0,
                        getv(764), getv_b(276), 0.0,
                        getv(768), getv_b(273), 0.0,
                        getv(772), getv_b(276), 0.0,
                        getv(774), getv_b(271), 0.0,
                        getv(780), getv_b(271), 0.0,
                        getv(776), getv_b(267), 0.0,
                        getv(781), getv_b(267), 0.0,//
                        getv(790), getv_b(248), 0.0,//r
                        getv(795), getv_b(248), 0.0,
                        getv(790), getv_b(276), 0.0,
                        getv(795), getv_b(276), 0.0,
                        getv(795), getv_b(257), 0.0,
                        getv(795), getv_b(251), 0.0,
                        getv(799), getv_b(252), 0.0,
                        getv(800), getv_b(247), 0.0,
                        getv(805), getv_b(254), 0.0,
                        getv(805), getv_b(249), 0.0,//
                        getv(526 + 306), getv_b(255), 0.0,//s
                        getv(521 + 306), getv_b(255), 0.0,
                        getv(525 + 306), getv_b(251), 0.0,
                        getv(520 + 306), getv_b(253), 0.0,
                        getv(521 + 306), getv_b(248), 0.0,
                        getv(518 + 306), getv_b(251), 0.0,
                        getv(516 + 306), getv_b(247), 0.0,
                        getv(513 + 306), getv_b(251), 0.0,
                        getv(510 + 306), getv_b(248), 0.0,
                        getv(510 + 306), getv_b(253), 0.0,
                        getv(506 + 306), getv_b(253), 0.0,
                        getv(509 + 306), getv_b(256), 0.0,
                        getv(505 + 306), getv_b(258), 0.0,
                        getv(511 + 306), getv_b(258), 0.0,
                        getv(509 + 306), getv_b(262), 0.0,
                        getv(514 + 306), getv_b(260), 0.0,
                        getv(514 + 306), getv_b(263), 0.0,
                        getv(519 + 306), getv_b(260), 0.0,
                        getv(519 + 306), getv_b(264), 0.0,
                        getv(523 + 306), getv_b(262), 0.0,
                        getv(522 + 306), getv_b(266), 0.0,
                        getv(528 + 306), getv_b(266), 0.0,
                        getv(523 + 306), getv_b(268), 0.0,
                        getv(527 + 306), getv_b(271), 0.0,
                        getv(522 + 306), getv_b(271), 0.0,
                        getv(523 + 306), getv_b(275), 0.0,
                        getv(519 + 306), getv_b(273), 0.0,
                        getv(517 + 306), getv_b(276), 0.0,
                        getv(513 + 306), getv_b(272), 0.0,
                        getv(509 + 306), getv_b(275), 0.0,
                        getv(510 + 306), getv_b(271), 0.0,
                        getv(505 + 306), getv_b(272), 0.0,
                        getv(508 + 306), getv_b(267), 0.0,
                        getv(505 + 306), getv_b(267), 0.0,//
                        getv(126 + 716), getv_b(242), 0.0,//i
                        getv(126 + 716), getv_b(238), 0.0,
                        getv(130 + 716), getv_b(242), 0.0,
                        getv(130 + 716), getv_b(238), 0.0,
                        getv(126 + 716), getv_b(276), 0.0,
                        getv(126 + 716), getv_b(248), 0.0,
                        getv(130 + 716), getv_b(276), 0.0,
                        getv(130 + 716), getv_b(248), 0.0,//
                        getv(105 + 753), getv_b(248), 0.0,//t
                        getv(105 + 753), getv_b(253), 0.0,
                        getv(117 + 753), getv_b(248), 0.0,
                        getv(117 + 753), getv_b(253), 0.0,
                        getv(108 + 753), getv_b(241), 0.0,
                        getv(112 + 753), getv_b(238), 0.0,
                        getv(108 + 753), getv_b(273), 0.0,
                        getv(112 + 753), getv_b(271), 0.0,
                        getv(111 + 753), getv_b(276), 0.0,
                        getv(117 + 753), getv_b(272), 0.0,
                        getv(117 + 753), getv_b(276), 0.0,//
                        getv(721 + 152), getv_b(248), 0.0,//v
                        getv(726 + 152), getv_b(248), 0.0,
                        getv(733 + 152), getv_b(276), 0.0,
                        getv(735 + 152), getv_b(270), 0.0,
                        getv(738 + 152), getv_b(276), 0.0,
                        getv(743 + 152), getv_b(248), 0.0,
                        getv(748 + 152), getv_b(248), 0.0,//
                        getv(889), getv_b(275), 0.0,
                        getv(884), getv_b(275), 0.0,
                        getv(888), getv_b(281), 0.0,
                        getv(882), getv_b(281), 0.0,
                        getv(883), getv_b(287), 0.0,
                        getv(876), getv_b(283), 0.0,
                        getv(876), getv_b(288), 0.0,
                        getv(769), getv_b(47), 0.0,
                        getv(786), getv_b(56), 0.0,
                        getv(765), getv_b(72), 0.0,
                        getv(764), getv_b(60), 0.0,
                        getv(760), getv_b(36), 0.0,
                        getv(784), getv_b(45), 0.0,
                        getv(787), getv_b(36), 0.0,
                        getv(771), getv_b(33), 0.0 };


    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(3, GL_FLOAT, 0, colors);
    IconVertices = vertices;
}
void drawUniversity_Logos() {

    float RR, GG, BB;
    RR = (float)(rand() % 256);
    GG = (float)(rand() % 256);
    BB = (float)(rand() % 256);
    GLfloat logos_mat[] = { RR/256.0, GG/256.0, BB/256.0, 1.0};//0.95, 0.325, 0.325 , 1.0 };
    setMaterialv(logos_mat);
    if (hit_flag[0])
    {
        //drawAxis(&IconVertices[363*3]);
        glPushMatrix();
            glTranslatef(0.38, 0.0, 0.0);
            //glRotatef(iconrotate, 0.0, 1.0, 0.0);
            glRotatef(logoAngle, 0.0, 1.0, 0.0);
            glTranslatef(-0.38, 0.0, 0.0);
            glDrawArrays(GL_QUAD_STRIP, 363, 34);
            glDrawArrays(GL_QUAD_STRIP, 397, 32);
            glDrawArrays(GL_TRIANGLE_FAN, 429, 9);
            glDrawArrays(GL_TRIANGLE_FAN, 438, 6);
            glDrawArrays(GL_POLYGON, 444, 14);
            glDrawArrays(GL_POLYGON, 1003, 4);
            glDrawArrays(GL_POLYGON, 1007, 4);
        glPopMatrix();
        //drawAxis(&IconVertices[395*3]);
    }
    GLfloat static words_mat[] = { 0.625, 0.625, 0.95 , 1.0 };
    setMaterialv(words_mat);
    if (hit_flag[1]) // 國
    {
        //drawAxis(&IconVertices[0*3]);
        glDrawArrays(GL_POLYGON, 0, 4);
        glDrawArrays(GL_POLYGON, 4, 4);
        glDrawArrays(GL_TRIANGLES, 8, 3);
        glDrawArrays(GL_POLYGON, 11, 4);
        glDrawArrays(GL_POLYGON, 15, 4);
        glDrawArrays(GL_POLYGON, 19, 4);
        glDrawArrays(GL_POLYGON, 23, 4);
        glDrawArrays(GL_POLYGON, 27, 8);
        glDrawArrays(GL_POLYGON, 35, 13);
        glDrawArrays(GL_POLYGON, 48, 9);
        glDrawArrays(GL_POLYGON, 57, 7);
        glDrawArrays(GL_POLYGON, 64, 7);
        //drawAxis(&IconVertices[68*3]);
    }
   
    
    if (hit_flag[2]) // 立
    {
        //drawAxis(&IconVertices[71*3]); // y
        //drawAxis(&IconVertices[81*3]); //x
        glDrawArrays(GL_POLYGON, 71, 5);
        glDrawArrays(GL_POLYGON, 76, 5);
        glDrawArrays(GL_POLYGON, 81, 4);
        glDrawArrays(GL_POLYGON, 93, 5);
        glDrawArrays(GL_POLYGON, 98, 6);
        glDrawArrays(GL_POLYGON, 104, 6);
        glDrawArrays(GL_POLYGON, 110, 9);
        //drawAxis(&IconVertices[115*3]);
    }
    if (hit_flag[3]) // 中
    {
        //drawAxis(&IconVertices[119*3]); //x
        glDrawArrays(GL_POLYGON, 119, 7);
        glDrawArrays(GL_POLYGON, 126, 12);
        glDrawArrays(GL_POLYGON, 138, 7);
        glDrawArrays(GL_POLYGON, 145, 11);
        glDrawArrays(GL_POLYGON, 156, 5);
        //drawAxis(&IconVertices[152*3]);//y
        //drawAxis(&IconVertices[131*3]); //x
    }
    if (hit_flag[4]) // 興
    {
        //drawAxis(&IconVertices[180*3]); //y
        //drawAxis(&IconVertices[199 * 3]);//x
        glDrawArrays(GL_POLYGON, 161, 6);
        glDrawArrays(GL_POLYGON, 167, 6);
        glDrawArrays(GL_POLYGON, 173, 8);
        glDrawArrays(GL_POLYGON, 181, 9);
        glDrawArrays(GL_POLYGON, 190, 9);
        glDrawArrays(GL_POLYGON, 199, 18);
        glDrawArrays(GL_POLYGON, 217, 4);
        glDrawArrays(GL_POLYGON, 221, 4);
        glDrawArrays(GL_POLYGON, 225, 4);
        //drawAxis(&IconVertices[226 * 3]);
    }
    if (hit_flag[5]) // 大
    {
        //drawAxis(&IconVertices[229*3]);//x
        //drawAxis(&IconVertices[241*3]); //y
        glDrawArrays(GL_POLYGON, 229, 11);
        glDrawArrays(GL_POLYGON, 240, 10);
        glDrawArrays(GL_POLYGON, 250, 4);
        //glDrawArrays(GL_POLYGON, 254, 5);
        //drawAxis(&IconVertices[254*3]);
    }
  
    if (hit_flag[6]) // 學
    {
        //drawAxis(&IconVertices[309* 3]); //x
        //drawAxis(&IconVertices[261 * 3]); //y
        glDrawArrays(GL_POLYGON, 259, 11);
        glDrawArrays(GL_POLYGON, 270, 8);
        glDrawArrays(GL_POLYGON, 278, 8);
        glDrawArrays(GL_POLYGON, 286, 5);
        glDrawArrays(GL_POLYGON, 291, 4);
        glDrawArrays(GL_POLYGON, 295, 5);
        glDrawArrays(GL_POLYGON, 300, 8);
        glDrawArrays(GL_POLYGON, 308, 4);
        glDrawArrays(GL_POLYGON, 312, 11);
        glDrawArrays(GL_POLYGON, 323, 5);
        glDrawArrays(GL_POLYGON, 328, 5);
        glDrawArrays(GL_POLYGON, 333, 9);
        glDrawArrays(GL_POLYGON, 342, 6);
        glDrawArrays(GL_POLYGON, 348, 10);
        glDrawArrays(GL_POLYGON, 358, 5);
        //drawAxis(&IconVertices[323 * 3]); //x
        //drawAxis(&IconVertices[345* 3]); //y
    }
    
    if (hit_flag[7]) // National
    {
        //drawAxis(&IconVertices[460*3]); //x
        glDrawArrays(GL_TRIANGLE_STRIP, 458, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 462, 8);
        glDrawArrays(GL_TRIANGLE_STRIP, 470, 15);
        glDrawArrays(GL_TRIANGLE_STRIP, 485, 22);
        glDrawArrays(GL_TRIANGLE_STRIP, 507, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 511, 7);
        glDrawArrays(GL_TRIANGLE_STRIP, 518, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 522, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 526, 31);
        glDrawArrays(GL_TRIANGLE_STRIP, 557, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 561, 12);
        glDrawArrays(GL_TRIANGLE_STRIP, 573, 15);
        glDrawArrays(GL_TRIANGLE_STRIP, 588, 22);
        glDrawArrays(GL_TRIANGLE_STRIP, 610, 4);
        //drawAxis(&IconVertices[613*3]);
    }
    
    if (hit_flag[8]) // Chung
    {
        //drawAxis(&IconVertices[635*3]); //x
        //drawAxis(&IconVertices[623*3]); //y
        glDrawArrays(GL_TRIANGLE_STRIP, 614, 40); //C
        glDrawArrays(GL_TRIANGLE_STRIP, 654, 4);  // h
        glDrawArrays(GL_TRIANGLE_STRIP, 658, 12);
        glDrawArrays(GL_TRIANGLE_STRIP, 670, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 674, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 678, 10);
        glDrawArrays(GL_TRIANGLE_STRIP, 688, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 692, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 696, 12);
        glDrawArrays(GL_TRIANGLE_STRIP, 708, 23);
        glDrawArrays(GL_TRIANGLE_STRIP, 731, 14);
        //drawAxis(&IconVertices[734*3]); //x
        //drawAxis(&IconVertices[738*3]); //y
    }
    
    if (hit_flag[9]) // Hsing
    {
        //drawAxis(&IconVertices[745*3]);
        glDrawArrays(GL_TRIANGLE_STRIP, 745, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 749, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 753, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 757, 34);
        glDrawArrays(GL_TRIANGLE_STRIP, 791, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 795, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 799, 4); // h
        glDrawArrays(GL_TRIANGLE_STRIP, 803, 12); // g
        glDrawArrays(GL_TRIANGLE_STRIP, 815, 23);
        glDrawArrays(GL_TRIANGLE_STRIP, 838, 14);
        //drawAxis(&IconVertices[841*3]); //x
        //drawAxis(&IconVertices[845*3]); //y
    }
    
    if (hit_flag[10])
    {
        //drawAxis(&IconVertices[852*3]);
        glDrawArrays(GL_TRIANGLE_STRIP, 852, 15);
        glDrawArrays(GL_TRIANGLE_STRIP, 867, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 871, 12);
        glDrawArrays(GL_TRIANGLE_STRIP, 883, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 887, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 891, 7);
        glDrawArrays(GL_TRIANGLE_STRIP, 898, 28);
        glDrawArrays(GL_TRIANGLE_STRIP, 926, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 930, 6);
        glDrawArrays(GL_TRIANGLE_STRIP, 936, 34);
        glDrawArrays(GL_TRIANGLE_STRIP, 970, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 974, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 978, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 982, 7);
        glDrawArrays(GL_TRIANGLE_STRIP, 989, 7);
        glDrawArrays(GL_TRIANGLE_STRIP, 996, 7);
        //drawAxis(&IconVertices[995*3]); //x
        //drawAxis(&IconVertices[1000*3]); //y
    }
   

    //glDisableClientState(GL_VERTEX_ARRAY);
    //glDisableClientState(GL_COLOR_ARRAY);
}
void SnipeUI();
void SnipeAnimation();
void drawString(const char* str)
{
    static int isFirstCall = 1;
    static GLuint lists;

    if (isFirstCall)
    { 
                          
        isFirstCall = 0;

       
        lists = glGenLists(MAX_CHAR);


       
        wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
    }
    
    for (; *str != '\0'; ++str)
    {
        glCallList(lists + *str);
    }
}
void polygonOffset();
void BeingAttack();
void init(void)
{
	/* selcet clearing color*/
	glClearColor(0.0, 0.0, 0.0, 0.0); //channel: RGBA
	glShadeModel(GL_SMOOTH);
    iconsRecover();
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 400.0);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 15.0);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    
	{
		GLfloat fogColor[4] = { 0.2, 0.2, 0.2, 0.5 };

		glFogi(GL_FOG_MODE, GL_EXP);
		glFogfv(GL_FOG_COLOR, fogColor);
		glFogf(GL_FOG_DENSITY, 0.03);
		glHint(GL_FOG_HINT, GL_DONT_CARE);
		glFogf(GL_FOG_START, 1.0);
		glFogf(GL_FOG_END, 5.0);
	}
	glClearColor(0.2, 0.2, 0.2, 0.5);  /* fog color */
    // Anti-Alias
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    // Depth
    glEnable(GL_DEPTH_TEST);
    // load object
    

	ShootingPush(camPosx, camPosy, camPosz);
	
}

void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN && ON_SHOOTING == FALSE)
		{
			ShootingPush(camPosx, camPosy, camPosz);
			shootingSpeed = 0.75;
			AttackAniColor = 10;
			ON_SHOOTING = TRUE;
            monkey.ATTACK_ABLE = TRUE;
            flower.ATTACK_ABLE = TRUE;
            teapot.ATTACK_ABLE = TRUE;
		}
		glutPostRedisplay();
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
			JUMP_CHEATING = !JUMP_CHEATING;
		break;
	default:
		break;
	}
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 'w':
			camPosz -= 0.5;
			glutPostRedisplay();
			break;
		case 'W':
			camPosz -= 0.2;
			glutPostRedisplay();
			break;
		case 's':
			camPosz += 0.5;
			glutPostRedisplay();
			break;
		case 'S':
			camPosz += 0.2;
			glutPostRedisplay();
			break;
		case 'A':
			camPosx -= 0.2;
			glutPostRedisplay();
			break;
		case 'a':
			camPosx -= 0.5;
			glutPostRedisplay();
			break;
		case 'D':
			camPosx += 0.2;
			glutPostRedisplay();
			break;
		case 'd':
			camPosx += 0.5;
			glutPostRedisplay();
			break;
		case '8':
			camLooky += 0.4;
			glutPostRedisplay();
			break;
		case '2':
			camLooky -= 0.4;
			glutPostRedisplay();
			break;
		case '4':
			camLookx -= 0.4;
			glutPostRedisplay();
			break;
		case '6':
			camLookx += 0.4;
			glutPostRedisplay();
			break;
		case '5':
			camLookx = 0.0;
			camLooky = 0.0;
			glutPostRedisplay();
			break;
		case ' ':
			if (ON_JUMPING && !JUMP_CHEATING)
				break;
			ON_JUMPING = TRUE;
			speed = 40.0;
			break;
		case 'x':
		case 'X':
			if (!ON_SWING)
			{
				ON_SWING = TRUE;
				robot_ang++;
			}
			break;
        case 'f':
        case 'F':
            FOG_FLAG = !FOG_FLAG;
            if (FOG_FLAG)
                glEnable(GL_FOG);
            else
                glDisable(GL_FOG);
            glutPostRedisplay();
            break;
        case 'c':
        case 'C':
            AA_FLAG = !AA_FLAG;
            if (AA_FLAG)
            {
                glEnable(GL_LINE_SMOOTH);
                glEnable(GL_POINT_SMOOTH);
            }
            else
            {
                glDisable(GL_POINT_SMOOTH);
                glDisable(GL_LINE_SMOOTH);
            }
            glutPostRedisplay();
            break;
        case 'r':
        case 'R':
            monkey.hp = monkey.HealthPoints;
            flower.hp = flower.HealthPoints;
            teapot.hp = teapot.HealthPoints;
            if (ON_SHOOTING)
                break;
            iconsRecover();
            glutPostRedisplay();
            break;
        case 'b':
        case 'B':
            SNIPE_FLAG = !SNIPE_FLAG;
            if (!SNIPE_FLAG)
                snipe_offset = 0.0;
            glutPostRedisplay();
            break;
        case 'P':
        case 'p':
            if (ON_OFFSETING)
                break;
            ON_OFFSETING = TRUE;
            if (polyfactor <= 0.0)
                POLYGON_OFFSET_FLAG = FALSE;
            glutPostRedisplay();
            break;
        case 'z':
        case 'Z':
            SHOW_HITBOX_FLAG = !SHOW_HITBOX_FLAG;
            glutPostRedisplay();
            break;
		case 27:
			exit(0);
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
    gluLookAt(camPosx, camPosy, camPosz - snipe_offset, camPosx + camLookx, camPosy + camLooky, camPosz - 40.0, 0.0, 1.0, 0.0);
   

    /* start drawing */
	floor();
	
	glPushMatrix();
        glTranslatef(0.0, ICONmovement, 0.0);
        glScalef(ICONSCALE, ICONSCALE, ICONSCALE);
        setupPointers();
        drawUniversity_Logos();
	glPopMatrix();
    hitsFunc();

    monkey.render();
    flower.render();
    teapot.render();

	// robot
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(polyfactor, polyunits);
    drawRobot_left();
    drawRobot_right();
    if (ON_OFFSETING && !POLYGON_OFFSET_FLAG)
    {
        glPushMatrix();
            //glTranslatef(camPosx, camPosy, camPosz);
            glRasterPos3f(camPosx-0.3f, camPosy -0.8, camPosz-2.5f);
            glColor3f(1.0f, 0.0f, 0.0f);
            drawString("Attack Up!");
        glPopMatrix();
    }
        
    glDisable(GL_POLYGON_OFFSET_FILL);
	
    // planet
	if(ON_RECOVER)
		drawPlanet0();
	drawPlanet();	

    if(SNIPE_FLAG)
        SnipeUI();
    if (objATTACK_FLAG)
    {
        objATTACK_FLAG = 0;
        BeingAttack();
    }
    //glColor3f(1.0f, 0.0f, 0.0f);
    
    //drawString("Hello, World!");
    
	glutSwapBuffers();
	glFlush();

}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(100.0, (GLfloat)w / (GLfloat)h, 0.1, 220.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camPosx, camPosy, camPosz, camPosx + camLookx, camPosy + camLooky, camPosz - 1.0, 0.0, 1.0, 0.0);	
}

void idle()
{
	Animations();
	gravity();
	shootingPlanetAnimation();
	swingAnimation();
    SnipeAnimation();
    polygonOffset();
    monkey.idle();
    flower.idle();
    teapot.idle();
    
    monkey.Collison(camPosx, camPosz, 0.0);
    flower.Collison(camPosx, camPosz, 0.0);
    teapot.Collison(camPosx, camPosz, 0.0);

    flower.Collison(monkey.pos.x, monkey.pos.z, 0.0);
    flower.Collison(teapot.pos.x, teapot.pos.z, 0.0);

    teapot.Collison(monkey.pos.x, monkey.pos.z, 0.0);
    teapot.Collison(flower.pos.x, flower.pos.z, 0.0);

    monkey.Collison(flower.pos.x, flower.pos.z, 0.0);
    monkey.Collison(teapot.pos.x, teapot.pos.z, 0.0);

	glutPostRedisplay();
}

int main(int argc, char* argv[])
{

	// 	set seed
	srand(time(NULL));
	// Initialize GLUT
	glutInit(&argc, argv);
	// Set up some memory buffers for our display
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	// Set the window size
	glutInitWindowSize(WIDTH, HEIGHT);
	//glutInitWindowPosition(-200,60);
	// Create the window with the title "Hello,GL"
	glutCreateWindow("NCHU LOGO");
	init();
	glewInit();

	glutDisplayFunc(display);
	//glutIdleFunc(&mydisplay);
	// Very important!  This initializes the entry points in the OpenGL driver so we can 
	// call all the functions in the API.
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glDepthMask(GL_TRUE);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}

void akdo(GLint ind[], int size)
{
	for (int i = 0; i < size; i++)	ind[i] += size;
}
void drawAxis() {
	//x-axis
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(600.0, 0.0, 0.0);
	glEnd();
	//y-axis
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 600.0, 0.0);
	glEnd();
	//z-axis
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 600.0);
	glEnd();
}
void DrawCircle(float cx, float cy, float r, int num_segments)
{
	glBegin(GL_LINE_LOOP);
		for (int ii = 0; ii < num_segments; ii++)
		{
			float theta = 2.0 * 3.1415926 * float(ii) / float(num_segments);//get the current angle

			float x = r * cosf(theta);//calculate the x component
			float y = r * sinf(theta);//calculate the y component

			glVertex3f(x + cx, y + cy,0.0);//output vertex

		}
	glEnd();
}
void setMaterialv(const GLfloat* params)
{
	glMaterialfv(GL_FRONT, GL_SPECULAR, params);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, params);
}
void drawPlanet0()
{

	
	glEnable(GL_BLEND);
	float blend_alpha = (shootingLocation-(ShootingDistance/2) )/ (ShootingDistance/2);
	GLfloat planet_sun_mat_specular[] = { 227.0 / 255.0, 179.0 / 255.0, 30.0 / 255.0, blend_alpha };
	GLfloat planet_earth_mat_specular[] = { 112.0 / 255.0, 128.0 / 255.0, 144.0 / 255.0, blend_alpha };
	GLfloat planet_mars_mat_specular[] = { 193.0 / 255.0, 68.0 / 255.0, 14.0 / 255.0, blend_alpha };
	GLfloat planet_moon_mat_specular[] = { 0.15, 0.45, 0.75, blend_alpha };
	GLfloat planet_nocolor_mat_specular[] = { 1.0, 1.0, 1.0, blend_alpha };
	GLfloat static planet_mat_shininess[] = { 120.0 };

	glMaterialfv(GL_FRONT, GL_SHININESS, planet_mat_shininess);
	glPushMatrix();
			// Trace My camera
			glTranslatef(camPosx, camPosy+(1.0-ShootingDistance/shootingLocation), camPosz);
			//to Top RIght and Turn to camera
			glTranslatef(1.3, 0.625, -2.5);
			//glTranslatef(0.0, 0.0, -shootingLocation);
			//glRotatef(shootingLocation*10, 0.0, 0.0, 1.0);
			glRotatef(30 ,1.0,0.0,1.0);
			// draw Mars
			glPushMatrix();
				glRotatef((GLfloat)marsyear, 0.0, 1.0, 0.0);
				glTranslatef(1.52, 0.0, 0.0);
				glRotatef((GLfloat)day, 0.0, 1.0, 0.0);
				
				glColor4f(193.0 / 255.0, 68.0 / 255.0, 14.0 / 255.0, blend_alpha);
				setMaterialv(planet_mars_mat_specular);
				
				glRotatef(90, 1.0, 0.0, 0.0);
				glutSolidSphere(0.2*0.532, 10, 8);    /* draw smaller planet */
			glPopMatrix();
			// draw Earth
			glPushMatrix();
				glRotatef((GLfloat)year, 0.0, 1.0, 0.0);
				glTranslatef(1.0, 0.0, 0.0);
				glRotatef((GLfloat)day, 0.0, 1.0, 0.0);
				glPushMatrix();
					glRotatef((GLfloat)year, 0.0, 1.0, 0.0);
					glTranslatef(0.25, 0.0, 0.0);
					glRotatef((GLfloat)year, 0.0, 1.0, 0.0);
					
					glColor4f(112.0/255.0, 128.0/255.0, 144.0/255.0, blend_alpha);
					setMaterialv(planet_earth_mat_specular);
				
					glRotatef(90, 1.0, 0.0, 0.0);
					glutSolidSphere(0.08, 10, 8);    /* draw Moon */
				glPopMatrix();
				
				glColor4f(0.15, 0.45, 0.75, blend_alpha);
				setMaterialv(planet_moon_mat_specular);
			
				glRotatef(90, 1.0, 0.0, 0.0);
				glutSolidSphere(0.2, 10, 8);    /* draw smaller planet */
				glColor4f(112.0/255.0, 128.0/255.0, 144.0/255.0, shootingLocation / ShootingDistance);
				setMaterialv(planet_nocolor_mat_specular);
				DrawCircle(0.0, 0.0, 0.25, 80); // Moon
			glPopMatrix();
			glPushMatrix();
				/* draw sun */
				glRotatef((GLfloat)sunday/10, 0.0, 1.0, 0.0);
				glRotatef(90, 1.0, 0.0, 0.0);
				 
				glColor4f(227.0 / 255.0, 179.0 / 255.0, 30.0 / 255.0, blend_alpha);
				setMaterialv(planet_sun_mat_specular);
				glutSolidSphere(0.5, 36, 36);
				
				glColor4f(1.0, 1.0, 1.0, shootingLocation / ShootingDistance);
				setMaterialv(planet_nocolor_mat_specular);
				DrawCircle(0.0, 0.0, 1.52, 200); // Mar
				DrawCircle(0.0, 0.0, 1.0, 200); // Earth
			glPopMatrix();
			
		glPopMatrix();
		glDisable(GL_BLEND);
}
void drawPlanet()
{
	GLfloat static planet_sun_mat_specular[] = { 227.0 / 255.0, 179.0 / 255.0, 30.0 / 255.0, 1.0 };
	GLfloat static planet_earth_mat_specular[] = { 112.0 / 255.0, 128.0 / 255.0, 144.0 / 255.0, 1.0 };
	GLfloat static planet_mars_mat_specular[] = { 193.0 / 255.0, 68.0 / 255.0, 14.0 / 255.0, 1.0 };
	GLfloat static planet_moon_mat_specular[] = { 0.15, 0.45, 0.75, 1.0 };
	GLfloat static planet_nocolor_mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat static planet_circle_mat_specualr[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat static planet_mat_shininess[] = { 120.0 };
	
	glMaterialfv(GL_FRONT, GL_SHININESS, planet_mat_shininess);
	
	glPushMatrix();
			// Trace My camera
			if(ON_SHOOTING)
			{
				
				glTranslatef(beforeShootingLoc[0], beforeShootingLoc[1], beforeShootingLoc[2]);
				glEnable(GL_LINE_STIPPLE);
				glLineStipple(1, 0x7FE0);  /*  dashed  */
				glColor3f(227.0 / 255.0, 179.0 / 255.0, 30.0 / 255.0);
				setMaterialv(planet_sun_mat_specular);
				glBegin(GL_LINES);
					glVertex3f(1.3, 0.625, -1.5);
					glVertex3f(1.3, 0.625, -shootingLocation-2.5);
				glEnd();
				glDisable(GL_LINE_STIPPLE);
			}
			else
				glTranslatef(camPosx, camPosy, camPosz);
			//to Top RIght and Turn to camera
			glTranslatef(1.3, 0.625, -2.5);
			glTranslatef(0.0, 0.0, -shootingLocation);
			glRotatef(shootingLocation*10, 0.0, 0.0, 1.0);
            //printf("Planet: %.2f, %.2f, %.2f\n",beforeShootingLoc[0], beforeShootingLoc[1], beforeShootingLoc[2]-shootingLocation);
			glRotatef(30 ,1.0,0.0,1.0);
			// draw Mars
			glPushMatrix();
				glRotatef((GLfloat)marsyear, 0.0, 1.0, 0.0);
				glTranslatef(1.52, 0.0, 0.0);
				glRotatef((GLfloat)day, 0.0, 1.0, 0.0);

				if (COLORFUL)
				{
					glColor3f(193.0 / 255.0, 68.0 / 255.0, 14.0 / 255.0);
					setMaterialv(planet_mars_mat_specular);
				}
				else
				{
					glColor3f(1.0, 1.0, 1.0);
					setMaterialv(planet_nocolor_mat_specular);
				}
					
				glRotatef(90, 1.0, 0.0, 0.0);
				glutSolidSphere(0.2*0.532, 10, 8);    /* draw smaller planet */
			glPopMatrix();
			// draw Earth
			glPushMatrix();
				glRotatef((GLfloat)year, 0.0, 1.0, 0.0);
				glTranslatef(1.0, 0.0, 0.0);
				glRotatef((GLfloat)day, 0.0, 1.0, 0.0);
				glPushMatrix();
					glRotatef((GLfloat)year, 0.0, 1.0, 0.0);
					glTranslatef(0.25, 0.0, 0.0);
					glRotatef((GLfloat)year, 0.0, 1.0, 0.0);

					if (COLORFUL)
					{
						glColor3f(112.0 / 255.0, 128.0 / 255.0, 144.0 / 255.0);
						setMaterialv(planet_earth_mat_specular);
					}
					else
					{
						glColor3f(1.0, 1.0, 1.0);
						setMaterialv(planet_nocolor_mat_specular);
					}
						
					glRotatef(90, 1.0, 0.0, 0.0);
					glutSolidSphere(0.08, 10, 8);    /* draw Moon */
				glPopMatrix();
				if (COLORFUL)
				{
					glColor3f(0.15, 0.45, 0.75);
					setMaterialv(planet_moon_mat_specular);
				}
				else
				{
					glColor3f(1.0, 1.0, 1.0);
					setMaterialv(planet_nocolor_mat_specular);
				}
				glRotatef(90, 1.0, 0.0, 0.0);
				glutSolidSphere(0.2, 10, 8);    /* draw smaller planet */
				glColor3f(1.0, 1.0, 1.0);
				setMaterialv(planet_nocolor_mat_specular);
				DrawCircle(0.0, 0.0, 0.25, 80); // Moon
			glPopMatrix();
			glPushMatrix();
				/* draw sun */
				glRotatef((GLfloat)sunday/10, 0.0, 1.0, 0.0);
				glRotatef(90, 1.0, 0.0, 0.0);
				if (ON_SHOOTING && shootingLocation > ShootingDistance - shootingSpeed*2)
					glScalef(4.0, 4.0, 4.0);
				if (COLORFUL)
				{
					glColor3f(227.0 / 255.0, 179.0 / 255.0, 30.0 / 255.0);
					setMaterialv(planet_sun_mat_specular);
				}
				else
				{
					glColor3f(1.0, 1.0, 1.0);
					setMaterialv(planet_nocolor_mat_specular);
				}
				glutSolidSphere(0.5, 36, 36);
				glColor3f(1.0, 1.0, 1.0);
				setMaterialv(planet_nocolor_mat_specular);
				DrawCircle(0.0, 0.0, 1.52, 200); // Mar
				DrawCircle(0.0, 0.0, 1.0, 200); // Earth
			glPopMatrix();
		glPopMatrix();

		
}
void drawRobot_left()
{
	GLfloat static normal_mat_specular[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat static finger0_mat_specular[] = { 0.7, 0.0, 0.0, 0.7 };
	GLfloat static finger1_mat_specular[] = { 0.7, 0.7, 0.0, 0.7 };
	GLfloat static finger2_mat_specular[] = { 0.0, 0.7, 0.0, 0.7 };
	GLfloat static finger3_mat_specular[] = { 0.0, 0.45, 0.7, 0.7 };

	GLfloat static mat_shininess[] = { 20.0 };
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glPushMatrix();
		// Trace My camera
		glTranslatef(camPosx-0.8, camPosy-0.5, camPosz);
		glRotatef(robot_ang<<1, 1.0, 0.0, 0.0);
		glRotatef(-30, 0.0, 0.0, 1.0);
		//shoulder
		glPushMatrix();
			glTranslatef(-0.75, -0.3, -0.625);
			glRotatef( -30,1.0,0.0,0.0);
			glScalef(0.2, 0.2, 0.75);
			glColor3f(128.0/255.0, 128.0/255.0, 128.0/255.0);
			setMaterialv(normal_mat_specular);
			glutSolidCube(1.0);
		glPopMatrix();
		//elbow
		glPushMatrix();
			glTranslatef(-0.75, -0.3-0.2, -1.35);
			glScalef(0.2, 0.2, 0.75);
			glColor3f(128.0/255.0, 128.0/255.0, 128.0/255.0);
			setMaterialv(normal_mat_specular);
			glutSolidCube(1.0);
			//drawAxis();
		glPopMatrix();
		glPushMatrix();
			//glRotatef(-90, 0.0, 0.0, 1.0);
			//glRotatef(180, 0.0, 1.0, 0.0);
			//finger 0
			glPushMatrix();
				glTranslatef(-0.95, -0.3-0.2+0.05, -1.35-0.375);
				setMaterialv(finger0_mat_specular);
				glPushMatrix();
					glScalef( 0.5/4, 0.2/4, 0.2/4);
					glColor3f(128.0/255.0, 128.0/255.0, 128.0/255.0);
					glutSolidCube(1.0);
				glPopMatrix();
				glPushMatrix();
					glTranslatef(-0.1,0.1,0.0);
					glScalef(0.2/4, 0.75/4, 0.2/4 );
					glColor3f(128.0/255.0, 128.0/255.0, 128.0/255.0);
					glutSolidCube(1.0);
				glPopMatrix();
				//drawAxis();
			glPopMatrix();
			//finger 1
			glPushMatrix();
				glTranslatef(-0.875, -0.3-0.2+0.05, -1.35-0.5);
				setMaterialv(finger1_mat_specular);
				
				glPushMatrix();
					glScalef(0.2/4, 0.2/4, 0.75/4);
					glColor3f(128.0/255.0, 128.0/255.0, 128.0/255.0);
					glutSolidCube(1.0);
				glPopMatrix();
				glPushMatrix();
					glTranslatef(-0.0,0.1,-0.1);
					glScalef(0.2/4, 0.75/4, 0.2/4 );
					glColor3f(128.0/255.0, 128.0/255.0, 128.0/255.0);
					glutSolidCube(1.0);
				glPopMatrix();
			glPopMatrix();
			//finger 2
			glPushMatrix();
				glTranslatef(-0.80, -0.3-0.2+0.05, -1.35-0.5);
				setMaterialv(finger2_mat_specular);
				
				glPushMatrix();
					glScalef(0.2/4, 0.2/4, 0.75/4);
					glColor3f(128.0/255.0, 128.0/255.0, 128.0/255.0);
					glutSolidCube(1.0);
				glPopMatrix();
				glPushMatrix();
					glTranslatef(-0.0,0.1,-0.1);
					glScalef(0.2/4, 0.75/4, 0.2/4 );
					glColor3f(128.0/255.0, 128.0/255.0, 128.0/255.0);
					glutSolidCube(1.0);
				glPopMatrix();
			glPopMatrix();
			//finger 3
			glPushMatrix();
				glTranslatef(-0.75+0.025, -0.3-0.2+0.05, -1.35-0.5);
				setMaterialv(finger3_mat_specular);
				
				glPushMatrix();
					glScalef(0.2/4, 0.2/4, 0.75/4);
					glColor3f(128.0/255.0, 128.0/255.0, 128.0/255.0);
					glutSolidCube(1.0);
				glPopMatrix();
				glPushMatrix();
					glTranslatef(-0.0,0.1,-0.1);
					glScalef(0.2/4, 0.75/4, 0.2/4 );
					glColor3f(128.0/255.0, 128.0/255.0, 128.0/255.0);
					glutSolidCube(1.0);
				glPopMatrix();
				//drawAxis();
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}
void drawRobot_right()
{
	GLfloat static normal_mat_specular[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat static finger0_mat_specular[] = { 0.7, 0.0, 0.0, 0.7 };
	GLfloat static finger1_mat_specular[] = { 0.7, 0.7, 0.0, 0.7 };
	GLfloat static finger2_mat_specular[] = { 0.0, 0.7, 0.0, 0.7 };
	GLfloat static finger3_mat_specular[] = { 0.0, 0.45, 0.7, 0.7 };

	GLfloat static mat_shininess[] = { 20.0 };
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glPushMatrix();
		// Trace My camera
		glTranslatef(camPosx+0.8, camPosy-0.5, camPosz);
		glRotatef(40, 0.0, 0.0, 1.0);
		//shoulder
		glPushMatrix();
			glTranslatef(0.75, -0.3, -0.625);
			glRotatef( -30,1.0,0.0,0.0);
			glScalef(0.2, 0.2, 0.75);
			glColor3f(128.0/255.0, 128.0/255.0, 128.0/255.0);
			setMaterialv(normal_mat_specular);
			glutSolidCube(1.0);
		glPopMatrix();
		//elbow
		glPushMatrix();
			glTranslatef(0.75, -0.3-0.2, -1.35);
			glScalef(0.2, 0.2, 0.75);
			glColor3f(128.0/255.0, 128.0/255.0, 128.0/255.0);
			setMaterialv(normal_mat_specular);
			glutSolidCube(1.0);
			//drawAxis();
		glPopMatrix();
		glPushMatrix();
			//glRotatef(-90, 1.0, 0.0, 0.0);
			//glRotatef(180, 0.0, 1.0, 0.0);
			//finger 0
			glPushMatrix();
				glTranslatef(0.95, -0.3-0.2+0.05, -1.35-0.375);
				setMaterialv(finger0_mat_specular);
				glPushMatrix();
					glScalef( 0.5/4, 0.2/4, 0.2/4);
					glColor3f(128.0/255.0, 128.0/255.0, 128.0/255.0);
					glutSolidCube(1.0);
				glPopMatrix();
				glPushMatrix();
					glTranslatef(0.1,0.1,0.0);
					glScalef(0.2/4, 0.75/4, 0.2/4 );
					glColor3f(128.0/255.0, 128.0/255.0, 128.0/255.0);
					glutSolidCube(1.0);
				glPopMatrix();
				//drawAxis();
			glPopMatrix();
			//finger 1
			glPushMatrix();
				glTranslatef(0.75+0.125, -0.3-0.2+0.05, -1.35-0.5);
				setMaterialv(finger1_mat_specular);
				glPushMatrix();
					glScalef(0.2/4, 0.2/4, 0.75/4);
					glColor3f(128.0/255.0, 128.0/255.0, 128.0/255.0);
					glutSolidCube(1.0);
				glPopMatrix();
				glPushMatrix();
					glTranslatef(0.0,0.1,-0.1);
					glScalef(0.2/4, 0.75/4, 0.2/4 );
					glColor3f(128.0/255.0, 128.0/255.0, 128.0/255.0);
					glutSolidCube(1.0);
				glPopMatrix();
			glPopMatrix();
			//finger 2
			glPushMatrix();
				glTranslatef(0.75+0.05, -0.3-0.2+0.05, -1.35-0.5);
				setMaterialv(finger2_mat_specular);
				glPushMatrix();
					glScalef(0.2/4, 0.2/4, 0.75/4);
					glColor3f(128.0/255.0, 128.0/255.0, 128.0/255.0);
					glutSolidCube(1.0);
				glPopMatrix();
				glPushMatrix();
					glTranslatef(0.0,0.1,-0.1);
					glScalef(0.2/4, 0.75/4, 0.2/4 );
					glColor3f(128.0/255.0, 128.0/255.0, 128.0/255.0);
					glutSolidCube(1.0);
				glPopMatrix();
			glPopMatrix();
			//finger 3
			glPushMatrix();
				glTranslatef(0.75-0.025, -0.3-0.2+0.05, -1.35-0.5);
				setMaterialv(finger3_mat_specular);
				glPushMatrix();
					glScalef(0.2/4, 0.2/4, 0.75/4);
					glColor3f(128.0/255.0, 128.0/255.0, 128.0/255.0);
					glutSolidCube(1.0);
				glPopMatrix();
				glPushMatrix();
					glTranslatef(0.0,0.1,-0.1);
					glScalef(0.2/4, 0.75/4, 0.2/4 );
					glColor3f(128.0/255.0, 128.0/255.0, 128.0/255.0);
					glutSolidCube(1.0);
				glPopMatrix();
				//drawAxis();
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}
void ShootingPush(float beforeShootingLoc_x, float beforeShootingLoc_y, float beforeShootingLoc_z)
{
	beforeShootingLoc[0] = beforeShootingLoc_x;
	beforeShootingLoc[1] = beforeShootingLoc_y;
	beforeShootingLoc[2] = beforeShootingLoc_z;
}
void shootingPlanetAnimation()
{
	if (ON_SHOOTING)
	{
		shootingLocation += shootingSpeed;
		AttackAniColor_count++;
		AttackAniColor_count %= AttackAniColor;
		if (AttackAniColor_count == 0)
			COLORFUL = !COLORFUL;
		if (shootingLocation > ShootingDistance / 10)
		{
			// recover animation
			ON_RECOVER = TRUE;
			if (shootingLocation > ShootingDistance * 0.8 / 1.0)
				AttackAniColor = 2;
			else if (shootingLocation > ShootingDistance * 0.7 / 1.0)
				AttackAniColor = 4;
			else if (shootingLocation > ShootingDistance * 0.6 / 1.0)
				AttackAniColor = 6;
			else if (shootingLocation > ShootingDistance * 0.5 / 1.0)
				AttackAniColor = 8;
		}

		if (shootingLocation > ShootingDistance)
		{
			shootingLocation = 0;
			shootingSpeed = 0.0;
			COLORFUL = 1;
			ON_SHOOTING = FALSE;
			ON_RECOVER = FALSE;
		}
	}
}
void gravity()
{
	if (!ON_JUMPING)
		return;

	camPosy += speed / 40.0;
	speed -= gra / 20.0;
	if (camPosy < 2.0)
	{
		camPosy = 2.0;
		speed = 0.0;
		ON_JUMPING = FALSE;
	}
}
void Animations()
{
	day = (day + 3) % 360;
	year = (year + 2) % 360;
	marsyear = (marsyear + 1) % 360;
	sunday = (sunday + 3) % 3600;

	logoAngle = (logoAngle + 3) % 360;
}
void swingAnimation()
{
	if (robot_ang == 0)
		ON_SWING = FALSE;
	if (ON_SWING)
		(++robot_ang) %= 180;
}
void floor()
{

	static float floor_size = 800;
	static float squareColors[] = {
			122.0/255.0, 64.0/255.0, 52.0/255.0,
			122.0/255.0, 64.0/255.0, 52.0/255.0,
			122.0/255.0, 64.0/255.0, 52.0/255.0,
			122.0/255.0, 64.0/255.0, 52.0/255.0,
		};
	static float squareCoords[] = {
		-floor_size,	0.0f,	-floor_size,   // top left
		-floor_size,	0.0f,	floor_size,   // bottom left
		floor_size,		0.0f,	floor_size,   // bottom right
		floor_size,		0.0f,	-floor_size, }; // top right
	GLfloat static floor_mat[] = {
		219.0/255.0, 155.0/255.0, 93.6/255.0
	};
	setMaterialv(floor_mat);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glPushMatrix();
		glTranslatef(camPosx, 0, camPosz);
		glColorPointer(3, GL_FLOAT, 0, squareColors);
		glVertexPointer(3, GL_FLOAT, 0, squareCoords);
		glDrawArrays(GL_POLYGON, 0, 4);
	glPopMatrix();
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}
void drawAxis(float LocX, float LocY, float LocZ)
{
    glPushMatrix();
        glTranslatef(LocX, LocY, LocZ);
        drawAxis();
    glPopMatrix();
}
void drawAxis(float* Locs)
{
    drawAxis(Locs[0], Locs[1], Locs[2]);
}
bool hitInRange(float topLeft_x, float topLeft_y, float botRight_x, float botRight_y)
{
    topLeft_x *= ICONSCALE;
    topLeft_y *= ICONSCALE;
    botRight_x *= ICONSCALE;
    botRight_y *= ICONSCALE;
    topLeft_y += ICONmovement;
    botRight_y += ICONmovement;
    //printf("%.2f, %.2f, %.2f, %.2f\n", topLeft_x, topLeft_y, botRight_x, botRight_y);
    //drawAxis(topLeft_x, topLeft_y, 0.0);
    //drawAxis(botRight_x, botRight_y, 0.0);
    //printf("Planet: %.2f, %.2f, %.2f\n", beforeShootingLoc[0], beforeShootingLoc[1], beforeShootingLoc[2]-shootingLocation);
    if (ON_SHOOTING && beforeShootingLoc[2] > 0.0)
        if (beforeShootingLoc[0] > topLeft_x && beforeShootingLoc[0] < botRight_x && beforeShootingLoc[1] > botRight_y && beforeShootingLoc[1] < topLeft_y)
            if (beforeShootingLoc[2] - shootingLocation < 0.0)
                return TRUE;
    
    return FALSE;
}
void hitsFunc()
{
    // 國
    if (hitInRange(IconVertices[0], IconVertices[1], IconVertices[68*3], IconVertices[68*3+1]))
        hit_flag[1] = FALSE;
    // 立
     if (hitInRange(IconVertices[81*3], IconVertices[71*3+1], IconVertices[115*3], IconVertices[115*3+1]))
        hit_flag[2] = FALSE;
    // 中
    if (hitInRange(IconVertices[119*3], IconVertices[119*3+1], IconVertices[131*3], IconVertices[152*3+1]))
        hit_flag[3] = FALSE;
    // 興
    if (hitInRange(IconVertices[199*3], IconVertices[180*3+1], IconVertices[226*3], IconVertices[226*3+1]))
        hit_flag[4] = FALSE;
    // 大
    if (hitInRange(IconVertices[229*3], IconVertices[241*3+1], IconVertices[254*3], IconVertices[254*3+1]))
        hit_flag[5] = FALSE;
    // 學
    if (hitInRange(IconVertices[309*3], IconVertices[261*3+1], IconVertices[323*3], IconVertices[345*3+1]))
        hit_flag[6] = FALSE;
    // logo
    if (hitInRange(IconVertices[363*3], IconVertices[363*3+1], IconVertices[395*3], IconVertices[395*3+1]))
        hit_flag[0] = FALSE;
    // National
    if (hitInRange(IconVertices[460*3], IconVertices[460*3+1], IconVertices[613*3], IconVertices[613*3+1]))
        hit_flag[7] = FALSE;
    // Chung
    if (hitInRange(IconVertices[635*3], IconVertices[623*3+1], IconVertices[734*3], IconVertices[738*3+1]))
        hit_flag[8] = FALSE;
    // Hsing
    if (hitInRange(IconVertices[745*3], IconVertices[745*3+1], IconVertices[841*3], IconVertices[845*3+1]))
        hit_flag[9] = FALSE;
    // University
    if (hitInRange(IconVertices[852*3], IconVertices[852*3+1], IconVertices[995*3], IconVertices[1000*3+1]))
        hit_flag[10] = FALSE;
    
}
void iconsRecover()
{
    for (int i = 0; i < hit_objs; i++)
        hit_flag[i] = 1;
}
void SnipeUI()
{
    if (snipe_pos < 0.5)
        return;
    //glDisable(GL_BLEND);
    glDisable(GL_LIGHTING);
    glPushMatrix();
        glTranslatef(camPosx, camPosy, camPosz-0.2-snipe_offset);
        glColor4f(0.0, 0.0, 0.0, snipe_offset);
        glBegin(GL_POLYGON);
            glVertex3f(0.01 * (snipe_pos / (snipe_offset)), 0.0005, 0.0);
            glVertex3f(0.2 * (snipe_pos / (snipe_offset)), 0.0005, 0.0);
            glVertex3f(0.2 * (snipe_pos / (snipe_offset)), -0.0005, 0.0);
            glVertex3f(0.01 * (snipe_pos / (snipe_offset)), -0.0005, 0.0);
        glEnd();
        glBegin(GL_POLYGON);
            glVertex3f(-0.2 * (snipe_pos / (snipe_offset)), 0.0005, 0.0);
            glVertex3f(-0.01 * (snipe_pos / (snipe_offset)), 0.0005, 0.0);
            glVertex3f(-0.01 * (snipe_pos / (snipe_offset)), -0.0005, 0.0);
            glVertex3f(-0.2 * (snipe_pos / (snipe_offset)), -0.0005, 0.0);
        glEnd();
        glBegin(GL_POLYGON);
            glVertex3f(0.0005, 0.01 * (snipe_pos / (snipe_offset)), 0.0);
            glVertex3f(0.0005, 0.2 * (snipe_pos / (snipe_offset)), 0.0);
            glVertex3f(-0.0005, 0.2 * (snipe_pos / (snipe_offset)), 0.0);
            glVertex3f(-0.0005, 0.01 * (snipe_pos / (snipe_offset)), 0.0);
        glEnd();
        glBegin(GL_POLYGON);
            glVertex3f(0.0005, -0.2 * (snipe_pos / (snipe_offset)), 0.0);
            glVertex3f(0.0005, -0.01 * (snipe_pos / (snipe_offset)),  0.0);
            glVertex3f(-0.0005, -0.01 * (snipe_pos / (snipe_offset)),  0.0);
            glVertex3f(-0.0005, -0.2 * (snipe_pos / (snipe_offset)), 0.0);
        glEnd();
        //for(float r=0.2; r>0; r-=0.05)
       
        glColor4f(1.0, 0.0, 0.0, snipe_offset);
        DrawCircle(0.0, 0.0, 0.2, 60);
        DrawCircle(0.0, 0.0, 0.01 * (snipe_pos / (snipe_offset)), 20);
       
    glPopMatrix();
    glEnable(GL_LIGHTING);
    //glEnable(GL_BLEND);
}
void SnipeAnimation()
{
    if (!SNIPE_FLAG)
        return;
    snipe_offset += 0.03;
    if (snipe_offset > snipe_pos)
    {
        snipe_offset = snipe_pos;
        ON_SNIPING = FALSE;
    }
        
}
void polygonOffset()
{
    if (!ON_OFFSETING)
        return;

    if (!POLYGON_OFFSET_FLAG)
    {
        polyfactor += 1.0;
        polyunits += 1.0;
        if (polyfactor >= 20.0)
        {
            ON_OFFSETING = FALSE;
            POLYGON_OFFSET_FLAG = TRUE;
        }
        PlayerAttack = 30.0;
    }
    else
    {
        polyfactor -= 1.0;
        polyunits -= 1.0;
        if(polyfactor <= 0.0)
        {
            ON_OFFSETING = FALSE;
            POLYGON_OFFSET_FLAG = FALSE;
        }
        PlayerAttack = 15.0;
    }
        

}
ObjLoader::ObjLoader(string filename)
{

    ifstream file(filename);
    string line;
    while (getline(file, line))
    {

        if (line.substr(0, 1) == "v")
        {

            vector<GLfloat> Point;
            GLfloat x, y, z;
            istringstream s(line.substr(2));
            s >> x; s >> y; s >> z;
            Point.push_back(x);
            Point.push_back(y);
            Point.push_back(z);
            v.push_back(Point);

        }
        else if (line.substr(0, 1) == "f")
        {

            vector<GLint> vIndexSets;
            int points[20];
            GLint ind[3];
            istringstream vtns(line.substr(2));
            char ss[64];
            size_t len = line.copy(ss, line.length() - 2, 2);
            ss[len] = '\0';

            char* token = strtok(ss, " /");
            len = 0;
            while (token)
            {
                points[len++] = atoi(token);
                //cout << "token:" << token << endl;
                //cout << points[len - 1] << endl;
                token = strtok(NULL, " /");
            }
            if (len >= 6)
            {
                ind[0] = points[0];
                ind[1] = points[2];
                ind[2] = points[4];
            }
            else
            {
                ind[0] = points[0];
                ind[1] = points[1];
                ind[2] = points[2];
            }
            vIndexSets.push_back(ind[0] - 1);
            vIndexSets.push_back(ind[1] - 1);
            vIndexSets.push_back(ind[2] - 1);
            //cout << endl;
            //for (int i = 0; i < 3; i++) cout << ind[i] << endl;
            f.push_back(vIndexSets);
        }
    }
    file.close();
}
void ObjLoader::Draw()
{
    GLfloat static obj_mat[] = { 0.7, 0.2, 0.6, 1.0 };
    setMaterialv(obj_mat);
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < f.size(); i++) {

        GLfloat VN[3];

        vertex a, b, c, normal;

        if ((f[i]).size() != 3) {

            cout << "ERRER::THE SIZE OF f IS NOT 3!" << endl;
        }
        else {

            GLint firstVertexIndex = (f[i])[0];
            GLint secondVertexIndex = (f[i])[1];
            GLint thirdVertexIndex = (f[i])[2];

            a.x = (v[firstVertexIndex])[0];
            a.y = (v[firstVertexIndex])[1];
            a.z = (v[firstVertexIndex])[2];

            b.x = (v[secondVertexIndex])[0];
            b.y = (v[secondVertexIndex])[1];
            b.z = (v[secondVertexIndex])[2];

            c.x = (v[thirdVertexIndex])[0];
            c.y = (v[thirdVertexIndex])[1];
            c.z = (v[thirdVertexIndex])[2];


            GLfloat vec1[3], vec2[3], vec3[3];
            //(x2-x1,y2-y1,z2-z1)
            vec1[0] = a.x - b.x;
            vec1[1] = a.y - b.y;
            vec1[2] = a.z - b.z;

            //(x3-x2,y3-y2,z3-z2)
            vec2[0] = a.x - c.x;
            vec2[1] = a.y - c.y;
            vec2[2] = a.z - c.z;

            //(x3-x1,y3-y1,z3-z1)
            vec3[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
            vec3[1] = vec2[0] * vec1[2] - vec2[2] * vec1[0];
            vec3[2] = vec2[1] * vec1[0] - vec2[0] * vec1[1];

            GLfloat D = sqrt(pow(vec3[0], 2) + pow(vec3[1], 2) + pow(vec3[2], 2));

            VN[0] = vec3[0] / D;
            VN[1] = vec3[1] / D;
            VN[2] = vec3[2] / D;

            glNormal3f(VN[0], VN[1], VN[2]);

            glVertex3f(a.x, a.y, a.z);
            glVertex3f(b.x, b.y, b.z);
            glVertex3f(c.x, c.y, c.z);
        }
    }
    glEnd();
}
Enemy::Enemy(string name, int hp, float size_factor,float hit_box_factor, float self_roate0, int pox, int poy, int poz)
{
    this->loader = new ObjLoader(name);
    this->ATTACK_ABLE = FALSE;
    this->old.x = this->pos.x = pox;
    this->old.y = this->pos.y = poy;
    this->old.z = this->pos.z = poz;
    if (hp > 0)
        this->hp = this->HealthPoints = hp;
    else
        this->hp = this->HealthPoints = 20;
    this->speed = 0.08;
    this->face_to = 0;
    this->size_factor = size_factor;
    this->hit_box_factor = hit_box_factor;
    this->self_roate0 = self_roate0;
}
void Enemy::render()
{
    glPushMatrix();
        glTranslatef(pos.x, pos.y, pos.z);
        glRotatef(face_to * 180 / 3.14, 0.0, 1.0, 0.0);
        // Rendering HP
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glPushMatrix();
            int len = hit_box_factor * size_factor * 0.7;
            glTranslatef(0.0, len ,0.0);
            // base: red
            glColor4f(0.8, 0.2, 0.2, 0.5);
            glBegin(GL_POLYGON);
                glVertex3f(-len, 0.0, 0.0);
                glVertex3f(-len, len*0.1, 0.0);
                glVertex3f(len, len*0.1, 0.0);
                glVertex3f(len, 0.0, 0.0);
            glEnd();
            // status: green
            glColor4f(0.2, 0.8, 0.2, 1.0);
            glBegin(GL_POLYGON);
                glVertex3f(-len, 0.0, 0.1);
                glVertex3f(-len, len*0.1, 0.1);
                glVertex3f(2* len * hp / HealthPoints - len, len*0.1, 0.1);
                glVertex3f(2* len * hp / HealthPoints - len, 0.0, 0.1);
            glEnd();
        glPopMatrix();
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glRotatef(self_roate0, 1.0, 0.0, 0.0);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glScalef(size_factor, size_factor, size_factor);
        loader->Draw();
        if (SHOW_HITBOX_FLAG)
            glutWireCube(hit_box_factor);
    glPopMatrix();
}
void Enemy::attack()
{
    objATTACK_FLAG = TRUE;
}
void Enemy::move()
{
    float dir[2]; // x z
    float dis = sqrt(pow(camPosx - pos.x, 2) + pow(camPosz - pos.z, 2));
    dir[0] = (camPosx - pos.x) / dis;
    dir[1] = (camPosz - pos.z) / dis;
    // trun my face to
    face_to = atan2f((camPosx - pos.x), (camPosz - pos.z));
    // movement
    pos.x += speed * dir[0];
    pos.z += speed * dir[1];
        
}
void Enemy::idle()
{
    if (hp <= 0)
        return;
    move();
    if (ON_SHOOTING && hit_detection())
    {
        underAttack();

        float dir[2]; // x z
        float dis = sqrt(pow(camPosx - beforeShootingLoc[0], 2) + pow(camPosz - beforeShootingLoc[2] + shootingLocation, 2));
        dir[0] = (beforeShootingLoc[0] - pos.x) / dis;
        dir[1] = (beforeShootingLoc[2]-shootingLocation - pos.z) / dis;

        pos.x -= dir[0] * speed * 5;
        pos.z -= dir[1] * speed * 5;
    }
        
}
bool Enemy::hit_detection()
{
    float posx = beforeShootingLoc[0];
    float posy = beforeShootingLoc[1];
    float posz = beforeShootingLoc[2] - shootingLocation;
    double distance = sqrt(pow(posx - pos.x, 2) + pow(posz - pos.z, 2) + pow(posy - pos.y, 2));
    if ( (distance - (double)size_factor * (double)hit_box_factor) / 2 <= 0.0)
        return TRUE;
    return FALSE;
            
    return FALSE;
}
bool Enemy::objCollision(float posx, float posz, double range)
{
    double distance = sqrt(pow(posx - pos.x, 2) + pow(posz - pos.z, 2));
    if (range >= (distance-(double)size_factor*(double)hit_box_factor)/2)
        return TRUE;
    return FALSE;
}
void Enemy::Collison(float posx, float posz, double range)
{
    if (objCollision(posx, posz, range))
    {
        float dir[2]; // x z
        float dis = sqrt(pow(camPosx - pos.x, 2) + pow(camPosz - pos.z, 2));
        dir[0] = (posx - pos.x) / dis;
        dir[1] = (posz - pos.z) / dis;
        
        pos.x -= dir[0] * speed * 3;
        pos.z -= dir[1] * speed * 3;

        if(posx==camPosx && posz==camPosz && !ON_JUMPING && hp > 0)
            attack();

        glutPostRedisplay();
    }
}
void BeingAttack()
{
    objATTACK_FLAG = FALSE;
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_DEPTH_TEST);
    glPushMatrix();
        glTranslatef(camPosx, camPosy, camPosz);
        //drawAxis();
        glColor4f(0.8, 0.2, 0.2, 0.1);
        /*glBegin(GL_POLYGON);
            glVertex3f(-10.0 ,-10.0, 0.0);
            glVertex3f(-10.0, 10.0, 0.0);
            glVertex3f(10.0, 10.0, 0.0);
            glVertex3f(10.0, -10.0, 0.0);
        glEnd();*/
        glutSolidCube(3.0);
    glPopMatrix();
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
}
void Enemy::underAttack()
{
    if (ATTACK_ABLE)
    {
        hp -= PlayerAttack;
        ATTACK_ABLE=FALSE;
    }
    if (hp <= 0)
        hp = 0;
}