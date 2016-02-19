#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>      // (or others, depending on the system in use)


class scrPt {
public:
	GLint x, y;
	scrPt *next;
};

class polygon {
public:
	scrPt *points;
	int size;
};


#define width  450
#define height 450


scrPt leftbottom, righttop;
polygon p[5];
int translate = 0, resize = 0;
int prev_x, prev_y;
int win1, win2;
double ratio = 4.5;



void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);  // Set display-window color to white.

	glMatrixMode(GL_PROJECTION);       // Set projection parameters.
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);
}


void drawfunc1(void)
{

	glClear(GL_COLOR_BUFFER_BIT);  // Clear display window.


	glColor3f(1.0, 0.0, 1.0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRecti(leftbottom.x, leftbottom.y, righttop.x, righttop.y);

	for (int i = 0; i < 5; i++) {
		
		glBegin(GL_LINE_LOOP);
		
		for (int j = 0; j < p[i].size; j++) {
			glVertex2f(p[i].points[j].x, p[i].points[j].y);
		}
		
		glEnd();
	}

	glFlush();     // Process all OpenGL routines as quickly as possible.

}

void drawfunc2(void)
{

	glClear(GL_COLOR_BUFFER_BIT);  // Clear display window.


	glColor3f(0.0, 1.0, 1.0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	int lx = leftbottom.x;
	int hx = righttop.x;
	int ly = leftbottom.y;
	int hy = righttop.y;

	for (int i = 0; i < 5; i++) 
	{
		for (int j = 0; j < p[i].size; j++) 
		{
			int x1, y1, x2, y2;

			if (j != p[i].size - 1)
			{
				x1 = p[i].points[j].x;
				y1 = p[i].points[j].y;
				x2 = p[i].points[j + 1].x;
				y2 = p[i].points[j + 1].y;
			}
			else
			{
				x1 = p[i].points[p[i].size - 1].x;
				y1 = p[i].points[p[i].size - 1].y;
				x2 = p[i].points[0].x;
				y2 = p[i].points[0].y;
			}

			if (x1 < lx && x2 < lx || x1 > hx && x2 > hx || y1 < ly && y2 < ly || y1 > hy && y2 > hy) 
			{
				continue;
			}
			else
			{
				glBegin(GL_LINES);
				glVertex2f((x1 - leftbottom.x) * ratio, (y1 - leftbottom.y) * ratio);
				glVertex2f((x2 - leftbottom.x) * ratio, (y2 - leftbottom.y) * ratio);
				glEnd();
			}
		}
	}

	glFlush();     // Process all OpenGL routines as quickly as possible.

}


void MousePress(int button, int state, int x, int y)
{

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		translate = 1;
		prev_x = x;
		prev_y = y;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		resize = 1;
		prev_x = x;
		prev_y = y;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		translate = 0;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		resize = 0;
	}
}

void MouseDrag(int x, int y)
{
	int tx, ty;

	if (translate) {
		tx = x - prev_x;
		ty = prev_y - y;
		prev_x = x;
		prev_y = y;
		leftbottom.x += tx;
		leftbottom.y += ty;
		righttop.x += tx;
		righttop.y += ty;

		drawfunc1();
		glutSetWindow(win2);
		drawfunc2();
		glutSetWindow(win1);
	}

	if (resize) {
		tx = x - prev_x;
		ty = prev_y - y;
		prev_x = x;
		prev_y = y;
		righttop.x += tx;
		if (righttop.x < leftbottom.x + 5) // make sure your rectangle maintain a correct order of vertices
			righttop.x = leftbottom.x + 5;
		righttop.y += ty;
		if (righttop.y < leftbottom.y + 5) // make sure your rectangle maintain a correct order of vertices
			righttop.y = leftbottom.y + 5;


		drawfunc1();
		glutSetWindow(win2);
		drawfunc2();
		glutSetWindow(win1);
	}

	glutPostRedisplay();
}

void winReshapeFcn(int newWidth, int newHeight)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)newWidth, 0.0, (GLdouble)newHeight);

	glClear(GL_COLOR_BUFFER_BIT);
}


void main(int argc, char** argv)
{

	glutInit(&argc, argv);                         // Initialize GLUT.
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);   // Set display mode.
	glutInitWindowSize(width, height);      // Set display-window width and height.

											//first window
	glutInitWindowPosition(50, 100);   // Set top-left display-window position.
	win1 = glutCreateWindow("Window One\n"); // Create display window.
	init();                            // Execute initialization procedure.

	leftbottom.x = 150;
	leftbottom.y = 150;
	righttop.x = 250;
	righttop.y = 250;

	p[0].size = 3;
	p[0].points = new scrPt[p[0].size];
	p[0].points[0].x = 10;
	p[0].points[0].y = 10;
	p[0].points[1].x = 90;
	p[0].points[1].y = 170;
	p[0].points[2].x = 170;
	p[0].points[2].y = 90;

	p[1].size = 4;
	p[1].points = new scrPt[p[1].size];
	p[1].points[0].x = 10;
	p[1].points[0].y = 250;
	p[1].points[1].x = 130;
	p[1].points[1].y = 250;
	p[1].points[2].x = 130;
	p[1].points[2].y = 400;
	p[1].points[3].x = 10;
	p[1].points[3].y = 400;

	p[2].size = 3;
	p[2].points = new scrPt[p[2].size];
	p[2].points[0].x = 440;
	p[2].points[0].y = 440;
	p[2].points[1].x = 360;
	p[2].points[1].y = 280;
	p[2].points[2].x = 280;
	p[2].points[2].y = 360;

	p[3].size = 5;
	p[3].points = new scrPt[p[3].size];
	p[3].points[0].x = 200;
	p[3].points[0].y = 200;
	p[3].points[1].x = 250;
	p[3].points[1].y = 200;
	p[3].points[2].x = 280;
	p[3].points[2].y = 250;
	p[3].points[3].x = 225;
	p[3].points[3].y = 300;
	p[3].points[4].x = 170;
	p[3].points[4].y = 250;

	p[4].size = 4;
	p[4].points = new scrPt[p[4].size];
	p[4].points[0].x = 430;
	p[4].points[0].y = 200;
	p[4].points[1].x = 390;
	p[4].points[1].y = 200;
	p[4].points[2].x = 390;
	p[4].points[2].y = 100;
	p[4].points[3].x = 430;
	p[4].points[3].y = 100;

	glRecti(10, 200, 30, 100);

	glBegin(GL_TRIANGLES);

	glVertex2f(0.0f, 0.0f);
	glVertex2f(100.0f, 300.0f);
	glVertex2f(300.0f, 150.0f);

	glEnd();

	glutDisplayFunc(drawfunc1);       // Send graphics to display window.
	glutReshapeFunc(winReshapeFcn);
	glutMouseFunc(MousePress);
	glutMotionFunc(MouseDrag);

	win2 = glutCreateWindow("Window Two\n"); // Create display window.
	glutPositionWindow(550, 100);
	init();                            // Execute initialization procedure.
	glutDisplayFunc(drawfunc2);       // Send graphics to display window.
	glutReshapeFunc(winReshapeFcn);

	glutMainLoop();                    // Display everything and wait.
}