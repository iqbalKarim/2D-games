/*********
   CTIS164 - Template Source Program
----------
STUDENT : Iqbal Karim
SECTION : 2
HOMEWORK: 2
----------
PROBLEMS: none
----------
ADDITIONAL FEATURES:
1. birds generated with random speeds colors and x y axis everytime
2. birds animated
3. state machine game
4. trees in the background randomly assigned position
5. Cannon ball rotates when fired
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  1400
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD     16// Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define START 1
#define RUNNING 2
#define END 3
#define D2R 0.0174532
typedef struct
{
	int r, g, b;
	double x, y;
	bool appear;
	int dx;
}bird_t;
typedef struct
{
	int x = rand() % 1400 - 700, y = rand() % 10 - 170;
}tree_t;
bool birdFlap = false;
bird_t birds[20];
int xTrees[100], yTrees[100], state = START, sign = -1, count = 0;
bool activeTimer = true, shot = false;
int canonX = 0, canonY = -300;
int cannonballX = 800, cannonballY = 800, counter = 0, killed = 0;
double cannonballangle = 0;
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

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
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

void white()
{
	glColor3ub(255, 255, 255);
}
void yellow()
{
	glColor3ub(255, 235, 59);
}
void black()
{
	glColor3b(0, 0, 0);
}
void brown()
{
	glColor3ub(93, 64, 55);
}
void silver()
{
	glColor3ub(158, 158, 158);
}
void canon()
{
	black();
	circle(canonX, canonY + 30, 40);
	glBegin(GL_QUADS);
	glVertex2f(canonX + 40, canonY + 30);
	glVertex2f(canonX - 40, canonY + 30);
	glVertex2f(canonX - 20, canonY + 100);
	glVertex2f(canonX + 20, canonY + 100);
	glEnd();

	silver();
	glBegin(GL_LINES);
	glVertex2f(canonX - 33, canonY + 40);
	glVertex2f(canonX - 18, canonY + 90);
	glEnd();

	white();
	glLineWidth(2);
	circle_wire(canonX + 20, canonY, 15);
	circle_wire(canonX - 20, canonY, 15);
	black();
	circle(canonX + 20, canonY, 15);
	circle(canonX - 20, canonY, 15);
	brown();
	circle(canonX + 20, canonY, 13);
	circle(canonX - 20, canonY, 13);
	silver();
	circle(canonX - 20, canonY, 3);
	circle(canonX + 20, canonY, 3);
}
void canonball()
{
	glColor3ub(244, 208, 63);
	circle(cannonballX, cannonballY - 5, 20);
	glColor3ub(230, 126, 34);
	circle(cannonballX, cannonballY - 3, 20);
	circle(cannonballX + 10, cannonballY - 30, 6);
	circle(cannonballX - 10, cannonballY - 35, 8);
	glColor3ub(244, 208, 63);
	circle(cannonballX + 10, cannonballY - 30, 4);
	circle(cannonballX - 10, cannonballY - 35, 5);

	glColor3f(0.25, 0.25, 0.25);
	circle(cannonballX, cannonballY, 20);
	white();
	circle((9 * cos(cannonballangle) + 2) + cannonballX, (9 * sin(cannonballangle) + 2) + cannonballY, 10);
	glColor3f(0.25, 0.25, 0.25);
	circle(cannonballX, cannonballY, 15);

}
void tree(int x, int y)
{
	//Wood
	glBegin(GL_QUADS);
	glColor4ub(51, 25, 0, 0);
	glVertex2f(x + 10, y - 15);
	glVertex2f(x - 10, y - 15);
	glColor3ub(220, 118, 51);
	glVertex2f(x - 10, y);
	glColor3ub(135, 54, 0);
	glVertex2f(x + 10, y);
	glEnd();

	//Leaves. 5 layers of cones. triple coloured transition
	glBegin(GL_TRIANGLES);

	//5th trianble
	glColor3ub(11, 83, 69);
	glVertex2f(x, y + 65);
	glColor3ub(34, 153, 84);
	glVertex2f(x + 35, y);
	glColor3ub(11, 83, 69);
	glVertex2f(x - 35, y);

	//4th triangle
	glColor3ub(11, 83, 69);
	glVertex2f(x, y + 75);
	glColor3ub(35, 155, 86);
	glVertex2f(x + 32, y + 10);
	glColor3ub(11, 83, 69);
	glVertex2f(x - 32, y + 10);

	//3rd triangle
	glColor3ub(11, 83, 69);
	glVertex2f(x, y + 85);
	glColor3ub(34, 153, 84);
	glVertex2f(x + 29, y + 20);
	glColor3ub(11, 83, 69);
	glVertex2f(x - 29, y + 20);

	//2nd triangle
	glColor3ub(11, 83, 69);
	glVertex2f(x, y + 85 + 10);
	glColor3ub(34, 153, 84);
	glVertex2f(x + 27, y + 30);
	glColor3ub(11, 83, 69);
	glVertex2f(x - 27, y + 30);

	//1st triangle
	glColor3ub(11, 83, 69);
	glVertex2f(x, y + 105);
	glColor3ub(35, 155, 86);
	glVertex2f(x + 25, y + 40);
	glColor3ub(11, 83, 69);
	glVertex2f(x - 25, y + 40);

	glEnd();
}
void makeTrees(int x[], int y[])
{
	//loop to make trees. Change the for loop condition of j < 75 to desired number of trees from 0 - 100
	int j;
	for (j = 0; j < 100; j++)
	{
		tree(x[j], y[j]);
	}
}
void init_trees()
{
	//initializes the random positions of the trees
	int j;
	for (j = 0; j < 100; j++)
	{
		xTrees[j] = rand() % 1400 - 700;
		yTrees[j] = rand() % 10 - 170;
	}
}
void fence(int tempx, int tempy)
{
	int localx = tempx, localy = tempy;
	brown();
	glRectf(localx + 10, localy - 25, localx - 10, localy + 25);
	glBegin(GL_TRIANGLES);
	glVertex2f(localx + 10, localy + 25);
	glVertex2f(localx - 10, localy + 25);
	glVertex2f(localx, localy + 37);
	glEnd();

	glRectf(localx + 20, localy + 7, localx + 10, localy - 7);
	glBegin(GL_QUADS);
	glColor4f(0, 0, 0, 0.5);
	glVertex2f(localx + 10, localy + 7);
	glVertex2f(localx + 10, localy - 7);
	brown();
	glVertex2f(localx + 15, localy - 7);
	glVertex2f(localx + 15, localy + 7);
	glEnd();

	glBegin(GL_QUADS);
	glColor4ub(141, 110, 99, 125);
	glVertex2f(localx, localy + 37);
	glVertex2f(localx + 5, localy + 27);
	glColor4ub(141, 110, 99, 0);
	glVertex2f(localx + 3, localy);
	glVertex2f(localx, localy);
	glEnd();

	glLineWidth(1);
	black();
	double min = -5;
	for (int i = localy + 24; i > localy; i -= 1)
	{
		glBegin(GL_LINES);
		glColor4ub(0, 0, 0, 125);
		glVertex2f(localx - 10, i);
		glColor4ub(0, 0, 0, 0);
		glVertex2f(localx - min, i);
		glEnd();
		min += 0.2;
	}
	for (int i = localy; i >= localy - 25; i -= 1)
	{
		glBegin(GL_LINES);
		glColor4ub(0, 0, 0, 125);
		glVertex2f(localx - 10, i);
		glColor4ub(0, 0, 0, 0);
		glVertex2f(localx - min, i);
		glEnd();
		min -= 0.4;
	}
	glBegin(GL_TRIANGLES);
	glColor4ub(0, 0, 0, 125);
	glVertex2f(localx - 10, localy + 25);
	glVertex2f(localx, localy + 37);
	glColor4f(0, 0, 0, 0);
	glVertex2f(localx, localy + 25);
	glEnd();

	min = 0;
	for (int i = localx - 10; i < localx + 10; i += 1)
	{
		glBegin(GL_LINES);
		glColor4ub(0, 0, 0, 125);
		glVertex2f(i, localy - 25);
		glColor4ub(0, 0, 0, 0);
		glVertex2f(i, localy - min);
		glEnd();
		min += 1;
	}

	glLineWidth(1);
	glBegin(GL_LINES);
	black();
	glVertex2f(localx + 10, localy - 25);
	glVertex2f(localx + 10, localy + 25);
	glVertex2f(localx - 10, localy + 25);
	glVertex2f(localx - 10, localy - 25);
	glVertex2f(localx + 10, localy - 25);
	glVertex2f(localx - 10, localy - 25);
	glVertex2f(localx + 10, localy + 25);
	glVertex2f(localx, localy + 37);
	glVertex2f(localx - 10, localy + 25);
	glVertex2f(localx, localy + 37);
	glEnd();

	silver();
	circle(localx, localy, 3);
	white();
	circle(localx, localy, 2);
	silver();
	circle(localx + 1, localy, 1);


}
void checkHit()
{
	for (int i = 0; i <= 20; i++)
	{
		double dx = birds[i].x - cannonballX;
		double dy = birds[i].y - cannonballY;
		double d = sqrt(dx*dx + dy * dy);
		if (d <= 45)
		{
			cannonballY = 800;
			shot = false;
			black();
			glColor3f(1, 0, 0);
			killed++;
			birds[i].x = 750;
			birds[i].y = rand() % 250 + 100;
			birds[i].dx = rand() % 3 + 1;
			birds[i].r = rand() % 255;
			birds[i].g = rand() % 255;
			birds[i].b = rand() % 255;
		}
	}
}
void clouds()
{
	glColor4f(1, 1, 1, 0.05);
	circle(900, -50, 400);
	glColor3ub(52, 152, 219);
	circle(250, -50, 300);

	glColor4f(1, 1, 1, 0.05);
	circle(250, -50, 300);
	glColor3ub(52, 152, 219);
	circle(-50, -75, 250);

	glColor4f(1, 1, 1, 0.05);
	circle(-50, -75, 250);
	glColor3ub(52, 152, 219);
	circle(-375, 75, 175);

	glColor4f(1, 1, 1, 0.05);
	circle(-375, 75, 175);
	glColor3ub(52, 152, 219);
	circle(-600, 0, 200);

	glColor4f(1, 1, 1, 0.05);
	circle(-600, 0, 200);
	glColor3ub(52, 152, 219);
	circle(-875, -200, 400);


	glColor4f(1, 1, 1, 0.1);
	circle(-875, -200, 400);
	glColor3ub(52, 152, 219);
	circle(-450, -175, 225);

	glColor4f(1, 1, 1, 0.1);
	circle(-450, -175, 225);
	glColor3ub(52, 152, 219);
	circle(-50, -200, 300);

	glColor4f(1, 1, 1, 0.1);
	circle(-50, -200, 300);
	glColor3ub(52, 152, 219);
	circle(250, -150, 300);

	glColor4f(1, 1, 1, 0.1);
	circle(250, -150, 300);
	glColor3ub(52, 152, 219);
	circle(650, -200, 200);

	glColor4f(1, 1, 1, 0.1);
	circle(650, -200, 200);
	glColor3ub(52, 152, 219);
	circle(-750 - 80, -250, 300);


	glColor4f(1, 1, 1, 0.15);
	circle(-750 - 80, -250, 300);
	glColor3ub(52, 152, 219);
	circle(-470 - 80, -200, 150);

	glColor4f(1, 1, 1, 0.15);
	circle(-470 - 80, -200, 150);
	glColor3ub(52, 152, 219);
	circle(-100 - 80, -275, 275);

	glColor4f(1, 1, 1, 0.15);
	circle(-100 - 80, -275, 275);
	glColor3ub(52, 152, 219);
	circle(250 - 80, -200, 300);

	glColor4f(1, 1, 1, 0.15);
	circle(250 - 80, -200, 300);
	glColor3ub(52, 152, 219);
	circle(600 - 80, -200, 150);

	glColor4f(1, 1, 1, 0.15);
	circle(600 - 80, -200, 150);
	glColor3ub(52, 152, 219);
	circle(-750 + 50, -350, 300);

}
void backgroud()
{
	//sky
	glBegin(GL_QUADS);
	glColor3ub(52, 152, 219);
	glVertex2f(-700, -400);
	glVertex2f(700, -400);
	glVertex2f(700, 400);
	glVertex2f(-700, 400);
	glEnd();

	clouds();

	//transition shade
	glBegin(GL_QUADS);
	glColor4f(1, 1, 1, 0);
	glVertex2f(-700, -400);
	glVertex2f(700, -400);
	glColor4f(1, 1, 1, 0.3);
	glVertex2f(700, 400);
	glVertex2f(-700, 400);
	glEnd();

	//hills
	glColor3ub(34, 153, 84);
	circle(500, -1100, 1000);
	circle(-400, -800, 800);

	makeTrees(xTrees, yTrees);

	for (int i = -750; i < 700; i += 30)
		fence(i, -170);


}
void birdFunc(bird_t bird)
{
	if (birdFlap == true) //for when wings are down
	{
		//back wing: Based on two circles and a quad. All shapes are outlined
		glColor3ub(bird.r, bird.g, bird.b);
		circle(bird.x + 20, bird.y - 40, 5);
		circle(bird.x + 29, bird.y - 35, 5);

		glColor3f(0, 0, 0);
		circle_wire(bird.x + 20, bird.y - 40, 5);
		circle_wire(bird.x + 29, bird.y - 35, 5);

		glColor3f(0, 0, 0);
		glBegin(GL_LINES);
		glColor4f(0, 0, 0, 0);
		glVertex2f(bird.x + 16, bird.y);
		glColor3f(0, 0, 0);
		glVertex2f(bird.x + 6, bird.y - 45);
		glColor3f(0, 0, 0);
		glVertex2f(bird.x + 34, bird.y - 35);
		glColor4f(0, 0, 0, 0);
		glVertex2f(bird.x + 31, bird.y);
		glEnd();

		glColor3ub(bird.r, bird.g, bird.b);

		glBegin(GL_QUADS);
		glVertex2f(bird.x + 16, bird.y);
		glVertex2f(bird.x + 6, bird.y - 45);
		glVertex2f(bird.x + 34, bird.y - 35);
		glVertex2f(bird.x + 31, bird.y);
		glEnd();

		//beak: Made using triangle and outlined. Also has mouth
		glColor3ub(243, 156, 18); //Orange colour
		glBegin(GL_TRIANGLES);
		glVertex2f(bird.x - 25, bird.y - 1);
		glVertex2f(bird.x - 55, bird.y - 1);
		glVertex2f(bird.x - 30, bird.y + 18);
		glEnd();

		glLineWidth(2);
		glColor3ub(0, 0, 0);
		glBegin(GL_LINES);
		glVertex2f(bird.x - 25, bird.y - 1);
		glVertex2f(bird.x - 55, bird.y - 1);
		glVertex2f(bird.x - 55, bird.y - 1);
		glVertex2f(bird.x - 30, bird.y + 18);
		glEnd();

		//body: Base on  a circle and triangle. All shapes outlined. 
		glColor3f(0, 0, 0);
		circle_wire(bird.x, bird.y, 18);
		circle_wire(bird.x - 25, bird.y + 9, 10);

		glLineWidth(3);
		glBegin(GL_LINES);
		glVertex2f(bird.x, bird.y - 18);
		glVertex2f(bird.x + 60, bird.y - 18);
		glVertex2f(bird.x + 60, bird.y - 18);
		glVertex2f(bird.x + 7, bird.y + 17);
		glEnd();
		glLineWidth(2);

		glColor3ub(bird.r, bird.g, bird.b);
		glBegin(GL_TRIANGLES);
		glVertex2f(bird.x, bird.y - 18);
		glVertex2f(bird.x + 60, bird.y - 18);
		glVertex2f(bird.x + 7, bird.y + 17);
		glEnd();
		circle(bird.x, bird.y, 18);
		circle(bird.x - 25, bird.y + 9, 10);

		glColor3f(1, 1, 1);
		circle(bird.x - 27, bird.y + 13, 2);

		glLineWidth(2);
		glColor3ub(0, 0, 0);
		glBegin(GL_LINES);
		glVertex2f(bird.x - 54, bird.y);
		glVertex2f(bird.x - 45, bird.y + 3);
		glEnd();
		circle(bird.x - 27, bird.y + 13, 1);

		//Front wing: Based on 3 circles and 1 quad. All shaoes outlined. 
		glColor3ub(bird.r, bird.g, bird.b);
		circle(bird.x, bird.y - 45, 5);
		circle(bird.x + 9, bird.y - 40, 5);
		circle(bird.x + 18, bird.y - 35, 5);

		glColor3f(0, 0, 0);
		circle_wire(bird.x, bird.y - 45, 5);
		circle_wire(bird.x + 9, bird.y - 40, 5);
		circle_wire(bird.x + 18, bird.y - 35, 5);

		glColor3ub(bird.r, bird.g, bird.b);

		glBegin(GL_QUADS);
		glVertex2f(bird.x + 5, bird.y);
		glVertex2f(bird.x - 5, bird.y - 45);
		glVertex2f(bird.x + 23, bird.y - 35);
		glVertex2f(bird.x + 20, bird.y);
		glEnd();

		glColor3f(0, 0, 0);
		glBegin(GL_LINES);
		glColor4f(0, 0, 0, 0);
		glVertex2f(bird.x + 5, bird.y);
		glColor3f(0, 0, 0);
		glVertex2f(bird.x - 5, bird.y - 45);
		glColor3f(0, 0, 0);
		glVertex2f(bird.x + 23, bird.y - 35);
		glColor4f(0, 0, 0, 0);
		glVertex2f(bird.x + 20, bird.y);

		glColor4f(0, 0, 0, 0.5);
		glVertex2f(bird.x + 5, bird.y - 45);
		glColor4f(0, 0, 0, 0);
		glVertex2f(bird.x + 10, bird.y);

		glColor4f(0, 0, 0, 0.5);
		glVertex2f(bird.x + 15, bird.y - 45);
		glColor4f(0, 0, 0, 0);
		glVertex2f(bird.x + 14, bird.y);
		glEnd();
	}
	else if (birdFlap == false) //for when wings are up
	{
		//back wing: Based on 2 triangles and a quad. All shapes outlined. 
		glColor3ub(bird.r, bird.g, bird.b);
		circle(bird.x + 20, bird.y + 40, 5);
		circle(bird.x + 29, bird.y + 35, 5);

		glColor3f(0, 0, 0);
		circle_wire(bird.x + 20, bird.y + 40, 5);
		circle_wire(bird.x + 29, bird.y + 35, 5);

		glColor3f(0, 0, 0);
		glBegin(GL_LINES);
		glColor4f(0, 0, 0, 0);
		glVertex2f(bird.x + 16, bird.y);
		glColor3f(0, 0, 0);
		glVertex2f(bird.x + 6, bird.y + 45);
		glColor3f(0, 0, 0);
		glVertex2f(bird.x + 34, bird.y + 35);
		glColor4f(0, 0, 0, 0);
		glVertex2f(bird.x + 31, bird.y);
		glEnd();

		glColor3ub(bird.r, bird.g, bird.b);

		glBegin(GL_QUADS);
		glVertex2f(bird.x + 16, bird.y);
		glVertex2f(bird.x + 6, bird.y + 45);
		glVertex2f(bird.x + 34, bird.y + 35);
		glVertex2f(bird.x + 31, bird.y);
		glEnd();

		//beak: Based on triangle. All shapes outlined. 
		glColor3ub(243, 156, 18); //Orange colour
		glBegin(GL_TRIANGLES);
		glVertex2f(bird.x - 25, bird.y - 1);
		glVertex2f(bird.x - 55, bird.y - 1);
		glVertex2f(bird.x - 30, bird.y + 18);
		glEnd();

		glLineWidth(2);
		glColor3ub(0, 0, 0);
		glBegin(GL_LINES);
		glVertex2f(bird.x - 25, bird.y - 1);
		glVertex2f(bird.x - 55, bird.y - 1);
		glVertex2f(bird.x - 55, bird.y - 1);
		glVertex2f(bird.x - 30, bird.y + 18);
		glEnd();

		//body: Based on one circle and a triangle. All shapes outlined

		glColor3f(0, 0, 0);
		circle_wire(bird.x, bird.y, 18);
		circle_wire(bird.x - 25, bird.y + 9, 10);

		glLineWidth(3);
		glBegin(GL_LINES);
		glVertex2f(bird.x, bird.y - 18);
		glVertex2f(bird.x + 60, bird.y - 18);
		glVertex2f(bird.x + 60, bird.y - 18);
		glVertex2f(bird.x + 7, bird.y + 17);
		glEnd();
		glLineWidth(2);

		glColor3ub(bird.r, bird.g, bird.b);
		glBegin(GL_TRIANGLES);
		glVertex2f(bird.x, bird.y - 18);
		glVertex2f(bird.x + 60, bird.y - 18);
		glVertex2f(bird.x + 7, bird.y + 17);
		glEnd();
		circle(bird.x, bird.y, 18);
		circle(bird.x - 25, bird.y + 9, 10);

		glColor3f(1, 1, 1);
		circle(bird.x - 27, bird.y + 13, 2);

		glLineWidth(2);
		glColor3ub(0, 0, 0);
		glBegin(GL_LINES);
		glVertex2f(bird.x - 54, bird.y);
		glVertex2f(bird.x - 45, bird.y + 3);
		glEnd();
		circle(bird.x - 27, bird.y + 13, 1);

		//Front wing: Based on 3 circles and a quad. All shapes outlined
		glColor3ub(bird.r, bird.g, bird.b);
		circle(bird.x, bird.y + 45, 5);
		circle(bird.x + 9, bird.y + 40, 5);
		circle(bird.x + 18, bird.y + 35, 5);

		glColor3f(0, 0, 0);
		circle_wire(bird.x, bird.y + 45, 5);
		circle_wire(bird.x + 9, bird.y + 40, 5);
		circle_wire(bird.x + 18, bird.y + 35, 5);

		glColor3ub(bird.r, bird.g, bird.b);

		glBegin(GL_QUADS);
		glVertex2f(bird.x + 5, bird.y);
		glVertex2f(bird.x - 5, bird.y + 45);
		glVertex2f(bird.x + 23, bird.y + 35);
		glVertex2f(bird.x + 20, bird.y);
		glEnd();

		glColor3f(0, 0, 0);
		glBegin(GL_LINES);
		glColor4f(0, 0, 0, 0);
		glVertex2f(bird.x + 5, bird.y);
		glColor3f(0, 0, 0);
		glVertex2f(bird.x - 5, bird.y + 45);
		glColor3f(0, 0, 0);
		glVertex2f(bird.x + 23, bird.y + 35);
		glColor4f(0, 0, 0, 0);
		glVertex2f(bird.x + 20, bird.y);

		glColor4f(0, 0, 0, 0.5);
		glVertex2f(bird.x + 5, bird.y + 45);
		glColor4f(0, 0, 0, 0);
		glVertex2f(bird.x + 10, bird.y);

		glColor4f(0, 0, 0, 0.5);
		glVertex2f(bird.x + 15, bird.y + 45);
		glColor4f(0, 0, 0, 0);
		glVertex2f(bird.x + 14, bird.y);
		glEnd();
	}
}
void bringBird()
{
	for (int i = 0; i < 20; i++)
	{
		if (birds[i].x < -700)
		{
			birds[i].x = 750;
			birds[i].y = rand() % 250 + 100;
			birds[i].dx = rand() % 3 + 1;
		}
	}
}
void start()
{
	glColor3ub(0, 0, 0);
	circle(0, 0, 50);
	glColor3f(0.5, 0.5, 0.5);
	circle(0, 0, 40);
	glColor3ub(255, 255, 255);
	vprint(-30, -10, GLUT_BITMAP_HELVETICA_18, "START");
	glColor3ub(0, 0, 0);
	vprint(-50, -250, GLUT_BITMAP_HELVETICA_18, "HIT THE BIRDS!!");
	vprint(-40, -280, GLUT_BITMAP_HELVETICA_18, "GOODLUCK!!");
	vprint(-120, -310, GLUT_BITMAP_HELVETICA_18, "CLICK THE CIRCLE TO START");
}
void running()
{
	for (int i = 0; i < 20; i++)
	{
		if (birds[i].appear == true)
			birdFunc(birds[i]);
	}
	canonball();
	canon();
	if (shot == true)
		checkHit();
	black();
	if (shot == false)
		vprint(-680, -350, GLUT_BITMAP_HELVETICA_18, "PRESS SPACE TO SHOOT");
	vprint(-680, -330, GLUT_BITMAP_HELVETICA_18, "PRESS 'E' TO QUIT");
	vprint(-680, -370, GLUT_BITMAP_HELVETICA_18, "PRESS LEFT KEY TO MOVE LEFT");
	vprint(-680, -390, GLUT_BITMAP_HELVETICA_18, "PRESS RIGHT KEY TO MOVE RIGHT");
}
void namePlac()
{
	//DISPLAYS NAME AND ID ON TOP LEFT HAND CORNER
	glColor4f(1, 1, 1, 0.75);
	glRectf(495, 350, 680, 300);
	glColor4f(0.25, 0.35, 0.5, 0.75);
	glRectf(500, 350, 675, 300);
	glColor3f(0, 0, 0);
	vprint(505, 330, GLUT_BITMAP_HELVETICA_18, "Iqbal Karim Siddiqui");
	vprint(545, 310, GLUT_BITMAP_HELVETICA_18, "21901251");
}
void end()
{
	vprint(-130, 0, GLUT_BITMAP_HELVETICA_18, "YOU KILLED %d BIRDS.", killed);
	vprint(-150, 20, GLUT_BITMAP_HELVETICA_18, "YOU MURDEREERRRRRRR");
	vprint(-120, -300, GLUT_BITMAP_HELVETICA_18, "PRESS ESC TO EXIT");
}

void display() {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	backgroud();
	namePlac();
	if (state == START)
		start();
	else if (state == RUNNING)
		running();
	else
		end();

	glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);
	if (key == ' ' && shot == false && state == RUNNING)
	{
		shot = true;
		cannonballX = canonX;
		cannonballY = canonY + 50;
	}
	if (key == 'e' && state == RUNNING)
		state = END;
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
	if (key == GLUT_KEY_RIGHT)
	{
		canonX += 5;
	}
	if (key == GLUT_KEY_LEFT)
	{
		canonX -= 5;
	}
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{

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
	int x2 = x - winWidth / 2;
	int y2 = winHeight / 2 - y;
	int hyp = sqrtf(powf(x2, 2) + powf(y2, 2));
	if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)
	{
		if (state == START)
			if (hyp <= 50)
				state = RUNNING;
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



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);

	if (activeTimer)
	{
		if (state == RUNNING)
		{
			count++;
			if (count % 10 == 0)
			{
				bringBird();

				for (int i = 0; i < 20; i++)
				{
					birds[i].y += (sign * 10);
				}
				birdFlap = !(birdFlap);
				sign *= -1;
			}
			for (int i = 0; i < 20; i++)
				birds[i].x -= birds[i].dx;

			if (shot == true)
			{
				cannonballangle += 0.25;
				cannonballY += 5;
				if (cannonballY >= 450)
				{
					shot = false;
				}
			}
		}
	}

	glutPostRedisplay(); // display()

}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	srand(time(NULL));
	init_trees();
	for (int i = 0; i < 20; i++)
	{
		birds[i].x = rand() % 200 + 500;
		birds[i].appear = true;
		birds[i].y = rand() % 250 + 100;
		birds[i].dx = rand() % 3 + 1;
		birds[i].r = rand() % 255;
		birds[i].g = rand() % 255;
		birds[i].b = rand() % 255;
	}
}

void main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("HW-2: Hitting the target game (Iqbal Karim)");

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