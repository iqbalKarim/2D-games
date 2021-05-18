
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD    16 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

typedef struct
{
	double x = 0, y = 0, dy, dx, angle;
}shuriken_t;
typedef struct
{
	bool show = false;
	double angle, x, y, rad, dir, radius;
	int r = 0, g = 0, b = 0;
}ninja_t;

ninja_t ninjas[6];
shuriken_t shuriken;

double x2, y2;
bool showGrids = true, projectile = false;
int angle = 0, displayAngle = 0, setting = 1, ninjaCount = 0, kills = 0;
/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void print(int x, int y, const char *string, void *font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void *font, const char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

void vprint2(int x, int y, float size, const char *string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}

void speedINIT()
{
	for (int i = 0; i < 6; i++)
	{
		ninjas[i].dir = (rand() % 10) + 1;
		if (ninjas[i].dir > 5)
			ninjas[i].dir -= 10;
		if (ninjas[i].dir == 0)
			ninjas[i].dir = -5;
	}
}
void hit()
{
	for (int i = 0; i < 6; i++)
	{
		if (ninjas[i].show)
		{
			double dx = ninjas[i].x - shuriken.x;
			double dy = ninjas[i].y - shuriken.y;
			double d = sqrt(dx*dx + dy * dy);
			if (d < 40)
			{
				ninjas[i].show = false;
				shuriken.x = 0;
				shuriken.y = 0;
				projectile = false;
				ninjaCount--;
				kills++;
				if (ninjaCount == 0)
				{
					setting = 1;
					speedINIT();
				}
			}
		}
	}
}
void shuriken1()
{
	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
	glVertex2f(15 * cos(angle * D2R) + shuriken.x, 15 * sin(angle * D2R) + shuriken.y);
	glVertex2f(15 * cos((angle + 90) * D2R) + shuriken.x, 15 * sin((angle + 90) * D2R) + shuriken.y);
	glVertex2f(15 * cos((angle + 180) * D2R) + shuriken.x, 15 * sin((angle + 180) * D2R) + shuriken.y);
	glVertex2f(15 * cos((angle + 270) * D2R) + shuriken.x, 15 * sin((angle + 270) * D2R) + shuriken.y);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(15 * cos(angle * D2R) + shuriken.x, 15 * sin(angle * D2R) + shuriken.y);
	glVertex2f(30 * cos((angle + 45) * D2R) + shuriken.x, 30 * sin((angle + 45) * D2R) + shuriken.y);
	glVertex2f(15 * cos((angle + 90) * D2R) + shuriken.x, 15 * sin((angle + 90) * D2R) + shuriken.y);

	glVertex2f(15 * cos((angle + 90) * D2R) + shuriken.x, 15 * sin((angle + 90) * D2R) + shuriken.y);
	glVertex2f(30 * cos((angle + 135) * D2R) + shuriken.x, 30 * sin((angle + 135) * D2R) + shuriken.y);
	glVertex2f(15 * cos((angle + 180) * D2R) + shuriken.x, 15 * sin((angle + 180) * D2R) + shuriken.y);

	glVertex2f(15 * cos((angle + 180) * D2R) + shuriken.x, 15 * sin((angle + 180) * D2R) + shuriken.y);
	glVertex2f(30 * cos((angle + 225) * D2R) + shuriken.x, 30 * sin((angle + 225) * D2R) + shuriken.y);
	glVertex2f(15 * cos((angle + 270) * D2R) + shuriken.x, 15 * sin((angle + 270) * D2R) + shuriken.y);

	glVertex2f(15 * cos((angle + 270) * D2R) + shuriken.x, 15 * sin((angle + 270) * D2R) + shuriken.y);
	glVertex2f(30 * cos((angle + 315) * D2R) + shuriken.x, 30 * sin((angle + 315) * D2R) + shuriken.y);
	glVertex2f(15 * cos(angle * D2R) + shuriken.x, 15 * sin(angle * D2R) + shuriken.y);
	glEnd();

	glColor3ub(100, 100, 100);
	glLineWidth(2);
	glBegin(GL_LINES);
	
	glVertex2f(15 * cos(angle * D2R) + shuriken.x, 15 * sin(angle * D2R) + shuriken.y);
	glVertex2f(30 * cos((angle + 45) * D2R) + shuriken.x, 30 * sin((angle + 45) * D2R) + shuriken.y);
	
	glVertex2f(30 * cos((angle + 45) * D2R) + shuriken.x, 30 * sin((angle + 45) * D2R) + shuriken.y);
	glVertex2f(15 * cos((angle + 90) * D2R) + shuriken.x, 15 * sin((angle + 90) * D2R) + shuriken.y);
	
	glVertex2f(15 * cos((angle + 90) * D2R) + shuriken.x, 15 * sin((angle + 90) * D2R) + shuriken.y);
	glVertex2f(30 * cos((angle + 135) * D2R) + shuriken.x, 30 * sin((angle + 135) * D2R) + shuriken.y);
	
	glVertex2f(30 * cos((angle + 135) * D2R) + shuriken.x, 30 * sin((angle + 135) * D2R) + shuriken.y);
	glVertex2f(15 * cos((angle + 180) * D2R) + shuriken.x, 15 * sin((angle + 180) * D2R) + shuriken.y);
	
	glVertex2f(15 * cos((angle + 180) * D2R) + shuriken.x, 15 * sin((angle + 180) * D2R) + shuriken.y);
	glVertex2f(30 * cos((angle + 225) * D2R) + shuriken.x, 30 * sin((angle + 225) * D2R) + shuriken.y);
	
	glVertex2f(30 * cos((angle + 225) * D2R) + shuriken.x, 30 * sin((angle + 225) * D2R) + shuriken.y);
	glVertex2f(15 * cos((angle + 270) * D2R) + shuriken.x, 15 * sin((angle + 270) * D2R) + shuriken.y);
	
	glVertex2f(15 * cos((angle + 270) * D2R) + shuriken.x, 15 * sin((angle + 270) * D2R) + shuriken.y);
	glVertex2f(30 * cos((angle + 315) * D2R) + shuriken.x, 30 * sin((angle + 315) * D2R) + shuriken.y);
	
	glVertex2f(30 * cos((angle + 315) * D2R) + shuriken.x, 30 * sin((angle + 315) * D2R) + shuriken.y);
	glVertex2f(15 * cos(angle * D2R) + shuriken.x, 15 * sin(angle * D2R) + shuriken.y);

	glEnd();

	glColor3f(0, 0.25, 0.25);
	circle(shuriken.x, shuriken.y, 3);
	circle(15 * cos(angle * D2R) + shuriken.x, 15 * sin(angle * D2R) + shuriken.y, 3);
	circle(15 * cos((angle + 90)* D2R) + shuriken.x, 15 * sin((angle + 90) * D2R) + shuriken.y, 3);
	circle(15 * cos((angle + 180) * D2R) + shuriken.x, 15 * sin((angle + 180) * D2R) + shuriken.y, 3);
	circle(15 * cos((angle + 270) * D2R) + shuriken.x, 15 * sin((angle + 270) * D2R) + shuriken.y, 3);
}
void namePlac(int localX, int localY)
{
	//DISPLAYS NAME AND ID ON TOP RIGHT CORNER
	glColor4f(1, 1, 1, 0.75);
	glRectf(localX, localY, localX + 177, localY + 50);
	glColor4f(0.25, 0.35, 0.5, 0.75);
	glRectf(localX + 5, localY, localX + 172, localY + 50);
	glColor3f(0, 0, 0);
	vprint(localX + 5, localY + 30, GLUT_BITMAP_HELVETICA_18, "Iqbal Karim");
	vprint(localX + 45, localY + 10, GLUT_BITMAP_HELVETICA_18, "21901251");
	glColor3f(1, 1, 1);
	if (setting != 2)
	{
		vprint(-30, -60, GLUT_BITMAP_HELVETICA_12, "Angle: %03d", displayAngle);
		vprint(-380, 320, GLUT_BITMAP_HELVETICA_12, "Press E to End the game.");
		vprint(-380, 335, GLUT_BITMAP_HELVETICA_12, "Press S to Toggle the grid.");
	}
	if (setting == 1)
		vprint(-380, 350, GLUT_BITMAP_HELVETICA_12, "Click Left Mouse Button to start game.");
	else if (setting == 0)
		vprint(-380, 350, GLUT_BITMAP_HELVETICA_12, "Click Left Mouse Button to shoot the Shuriken in the direction you want.");

	

}
ninja_t ninja(ninja_t obj)
{
	glBegin(GL_QUADS);
	glVertex2f(obj.x + 25, obj.y + 8);
	glVertex2f(obj.x + 23, obj.y + 15);

	glVertex2f(obj.x, obj.y - 13);
	glVertex2f(obj.x, obj.y - 15);

	glVertex2f(obj.x - 25, obj.y + 8);
	glVertex2f(obj.x - 23, obj.y + 15);

	glVertex2f(obj.x, obj.y - 13);
	glVertex2f(obj.x, obj.y - 15);
	glEnd();

	circle(obj.x - 24, obj.y + 11, 3.5);
	circle(obj.x + 24, obj.y + 11, 3.5);

	glColor3ub(obj.r,obj.g,obj.b);
	circle(obj.x, obj.y, obj.radius);
	
	glLineWidth(7);
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(obj.x + 15, obj.y + 13);
	glVertex2f(obj.x - 15, obj.y + 13);
	glEnd();
	circle(obj.x - 15, obj.y + 13, 3.5);
	circle(obj.x + 15, obj.y + 13, 3.5);

	glLineWidth(4);
	glColor3f(0.85, 0.85, 0.85);
	circle(obj.x, obj.y + 4, 4);

	circle(obj.x - 2, obj.y + 4, 4);
	circle(obj.x + 2, obj.y + 4, 4);

	circle(obj.x - 6, obj.y + 4, 3);
	circle(obj.x + 6, obj.y + 4, 3);

	circle(obj.x - 9, obj.y + 4, 3);
	circle(obj.x + 9, obj.y + 4, 3);

	circle(obj.x + 12, obj.y + 4, 2);
	circle(obj.x - 12, obj.y + 4, 2);

	glBegin(GL_LINES);
	glVertex2f(obj.x - 12, obj.y + 4);
	glVertex2f(obj.x + 12, obj.y + 4);
	glEnd();

	glColor3f(0, 0, 0);
	circle(obj.x + 6, obj.y + 4, 2);
	circle(obj.x - 6, obj.y + 4, 2);
	
	glColor3f(0, 0, 0);
	glBegin(GL_TRIANGLES);

	return obj;
}
void orbit(int rad)
{
	glColor3ub(0,0,0);
	double localY, localX;
	glLineWidth(1);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= 360; i++)
	{
		localX = rad * cos(i * D2R);
		localY = rad * sin(i * D2R);
		glVertex2f(localX, localY);
	}
	glEnd();
}
void grid()
{
	glColor3ub(0, 0, 0);
	glLineWidth(1);
	int localX, localY;
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0x0FF0);
	glBegin(GL_LINES);
	glVertex2f(-400, 0);
	glVertex2f(400, 0);
	glVertex2f(0, -400);
	glVertex2f(0, 400);
	glEnd();
	glDisable(GL_LINE_STIPPLE);
}
void resetProjectile()
{
	projectile = false;
	shuriken.y = 0;
	shuriken.x = 0;
}
void initializeNinja(double rad, double x2)
{
	ninjas[ninjaCount].show = true;
	ninjas[ninjaCount].rad = rad;
	ninjas[ninjaCount].y = rad;
	ninjas[ninjaCount].x = 0;
	ninjas[ninjaCount].angle = -90;
	ninjas[ninjaCount].r = rand() % 255;
	ninjas[ninjaCount].b = rand() % 255;
	ninjas[ninjaCount].g = rand() % 255;
	ninjas[ninjaCount].radius = (rand() % 5) + 18;
	ninjaCount++;
}


void display() {
	glClearColor(0, 0.25, 0.25, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	namePlac(215, 340);

	if (setting != 2)
	{
		glColor3f(0, 0, 0);
		if (showGrids)
		{
			orbit(350);
			orbit(300);
			orbit(250);
			orbit(200);
			orbit(150);
			orbit(100);
			grid();
		}

		for (int i = 0; i < 6; i++)
			if (ninjas[i].show)
				ninjas[i] = ninja(ninjas[i]);

		glColor3f(1, 1, 1);
		if (setting == 0)
			hit();
		shuriken1();
	}

	else
	{
		vprint(-50, 0, GLUT_BITMAP_HELVETICA_12, "Well played");
		vprint(-75, -15, GLUT_BITMAP_HELVETICA_12, "You eliminated %d ninjas.", kills);
		vprint(-60, -50, GLUT_BITMAP_HELVETICA_12, "Press ESC to exit.");
	}
	glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);
	if (key == 's')
		showGrids = !showGrids;
	if (key == 'd' && setting == 1 && ninjaCount != 0)
		setting = 0;
	if (key == 'e')
		setting = 2;
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = true; break;
	case GLUT_KEY_DOWN: down = true; break;
	case GLUT_KEY_LEFT: left = true; break;
	case GLUT_KEY_RIGHT: right = true; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
	x2 = x - winWidth / 2;
	y2 = winHeight / 2 - y;
	if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN && projectile == false && setting == 0)
	{
		shuriken.angle = atan2(y2 - shuriken.y, x2 - shuriken.x) / D2R;
		shuriken.dy = 8*sin(shuriken.angle * D2R);
		shuriken.dx = 8*cos(shuriken.angle * D2R);
		projectile = !projectile;
	}
	else if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN && setting == 1)
	{
		for (int i = 0; i < 6; i++)
		{
		
			int num = rand() % 6;
			if (num == 0)
				initializeNinja(100, 0);
			else if (num == 1)
				initializeNinja(150, 0);
			else if (num == 2)
				initializeNinja(200, 0);
			else if (num == 3)
				initializeNinja(250, 0);
			else if (num == 4)
				initializeNinja(300, 0);
			else if (num == 5)
				initializeNinja(350, 0);
			setting = 0;
		}
	}
	glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function   
	glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
	// Write your codes here.

	x2 = x - winWidth / 2;
	y2 = winHeight / 2 - y;
	if (projectile == false)
	{
		angle = atan2(y2 - shuriken.y, x2 - shuriken.x) / D2R;
		if (angle > 0)
			displayAngle = angle;
		else
			displayAngle = 360 + angle;
	}
	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {
	if (projectile == true)
	{
		angle += 10;
		shuriken.x += shuriken.dx;
		shuriken.y += shuriken.dy;
		if (shuriken.x > 400 || shuriken.x < -400 || shuriken.y > 400 || shuriken.y < -400)
			resetProjectile();
	}
	
	if (setting == 0)
		for (int i = 0; i < 6; i++)
			if (ninjas[i].show)
			{
				ninjas[i].angle = ninjas[i].angle + (ninjas[i].dir * 0.0002);
				ninjas[i].x = ninjas[i].rad * cos(ninjas[i].angle / D2R);
				ninjas[i].y = ninjas[i].rad * sin(ninjas[i].angle / D2R);
			}

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	glutPostRedisplay(); // display()

}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	srand(time(NULL));
	for (int i = 0; i < 6; i++)
	{
		ninjas[i].dir = (rand() % 10) + 1;
		if (ninjas[i].dir > 5)
			ninjas[i].dir -= 10;
		if (ninjas[i].dir == 0)
			ninjas[i].dir = -5;
	}
}

void main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("OOP PROJECT (Ahmed Saud)");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}