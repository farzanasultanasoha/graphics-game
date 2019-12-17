#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <bits/stdc++.h>
//#include <GL/glut.h>

#include <windows.h>
#include <glut.h>
#define pi (2*acos(0.0))

using namespace std;

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle = 0;


bool gameEnded = false;
double position[3][2];
int state[3][2];
double radius[3];
double dangerBallSpeed = 1;
double screenSize = 168; // to all sides from origin is 168
int numberOfElements = 3;


// Variables for the Bricks
double brickLength = 70, brickBreadth = 10;
double brickPosition[6][2];
double brickSpeed = 0.8;

// Variables for the User Ball
double userBallPosition[2];
double userBallRadius = 10;
double userBallSpeed = .5;

// Variable for Score
int score = 0;


struct point
{
	double x,y,z;
};

void push_pop(void)
{
    glPushMatrix();
        glRotatef(45, 0, 0, 1);
        glPushMatrix(); // Furthest Triangle, Draw first


            //glRotatef(45, 0, 0, 1);
            glTranslatef(-20, 0, 0);
            glScaled(2, 1, 0);
            glColor3f(0.0, 0.0, 1.0);
            glBegin(GL_POLYGON);
                glVertex2f(10, 10);
                glVertex2f(10, 0);
                glVertex2f(-10, 0);
            glEnd();
        glPopMatrix();

        glPushMatrix(); // Middle Triangle, Draw 2nd
            glColor3f(0.0, 1.0, 0.0);
            glBegin(GL_POLYGON);
                glVertex2f(10, 10);
                glVertex2f(10, 0);
                glVertex2f(-10, 0);
            glEnd();
        glPopMatrix();

        glPushMatrix(); // Nearest Triangle, Draw last
            glTranslatef(40, 10, 0);
            glColor3f(1.0, 0.0, 0.0);
            glBegin(GL_POLYGON);
                glVertex2f(10, 10);
                glVertex2f(10, 0);
                glVertex2f(-10, 0);
            glEnd();
        glPopMatrix();



    glPopMatrix();

}


void drawAxes()
{

		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();

}


void drawGrid()
{
	int i;

    glColor3f(0.6, 0.6, 0.6);	//grey
    glBegin(GL_LINES);{
        for(i=-8;i<=8;i++){

            if(i==0)
                continue;	//SKIP the MAIN axes

            //lines parallel to Y-axis
            glVertex3f(i*10, -90, 0);
            glVertex3f(i*10,  90, 0);

            //lines parallel to X-axis
            glVertex3f(-90, i*10, 0);
            glVertex3f( 90, i*10, 0);
        }
    }glEnd();

}


void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}

void drawBrick()
{
    double lengthX, lengthY;
    lengthX = brickLength / 2;
    lengthY = brickBreadth / 2;

    glBegin(GL_POLYGON);{
		glVertex3f(lengthX, lengthY, 0);
		glVertex3f(lengthX, -lengthY,0);
		glVertex3f(-lengthX, -lengthY,0);
		glVertex3f(-lengthX, lengthY,0);
	}glEnd();
}

void drawStar()
{
    //glClear( GL_COLOR_BUFFER_BIT );
    glBegin(GL_POLYGON);{

        glVertex3f(0, -5, 0);
        glVertex3f(-5, -2, 0);
        glVertex3f(-3, 4, 0);
        glVertex3f(3, 4, 0);
        glVertex3f(5, -2,0);

    }glEnd();

    return;
}



void drawCircle(float radius_x, float radius_y)
{
	int i = 0;
	float angle = 0.0;

	glBegin(GL_POLYGON);
    {
		for(i = 0; i < 100; i++)
		{
			angle = 2 * 3.1416 * i / 100;
			glVertex3f (cos(angle) * radius_x, sin(angle) * radius_y, 0);
		}

    }

	glEnd();
}


void specialKeyListener(int key, int x,int y){
	double temp = 5;

	switch(key){
//		case GLUT_KEY_UP:		//down arrow key
//			pos.x+=l.x;
//			pos.y+=l.y;
//			pos.z+=l.z;
//			break;
		case GLUT_KEY_DOWN:		// up arrow key
//			pos.x-=l.x;
//			pos.y-=l.y;
//			pos.z-=l.z;
            userBallPosition[1] -= 10;
			break;

		case GLUT_KEY_RIGHT :
//			pos.x+=r.x;
//			pos.y+=r.y;
//			pos.z+=r.z;
            userBallPosition[0] += temp;
            if(userBallPosition[0] > screenSize - userBallRadius)
                userBallPosition[0] -= temp;
			break;
		case GLUT_KEY_LEFT :
//			pos.x-=r.x;
//			pos.y-=r.y;
//			pos.z-=r.z;
            userBallPosition[0] -= temp;
            if(userBallPosition[0] < -(screenSize - userBallRadius))
                userBallPosition[0] += temp;
			break;

//		case GLUT_KEY_PAGE_UP:
//		    pos.x+=u.x;
//			pos.y+=u.y;
//			pos.z+=u.z;
//			break;
//		case GLUT_KEY_PAGE_DOWN:
//            pos.x-=u.x;
//			pos.y-=u.y;
//			pos.z-=u.z;
//			break;
//
//		case GLUT_KEY_INSERT:
//			break;
//
//		case GLUT_KEY_HOME:
//			break;
//		case GLUT_KEY_END:
//			break;

		default:
			break;
	}
}

double calculateDistance(double x1, double y1, double x2, double y2)
{
	int x,y;		//temporary variables
	float distance;

	x = x2 - x1;
	y =y2 - y1;
	distance = sqrt((x*x)+(y*y));
//	printf("Distance: %.2f",distance);
	return distance;
}

int generateRandomNumber(int range1, int range2)
{
    srand(time(NULL));
    return (rand() % range2 * 2) + range1;
}

void checkIfGameEnded()
{
    double dist = calculateDistance(userBallPosition[0], userBallPosition[1], position[0][0], position[0][1]);

    if (dist <= userBallRadius + radius[0] || userBallPosition[1] < -(screenSize + userBallRadius))
    {
        cout << "End Game" << endl;
        gameEnded = true;
    }
}

void scoreGame()
{
    for (int i = 1; i < 3; i++)
    {
        double dist = calculateDistance(userBallPosition[0], userBallPosition[1], position[i][0], position[i][1]);
        if (dist <= userBallRadius + radius[i])
        {
            score++;
            radius[0] += 6;
            cout << score <<endl;

            if (i == 1)
            {
                // star A
                position[1][0] = generateRandomNumber(-150, 1); // along x-axis
                position[1][1] = 150; // along y-axis
            }
            else {
                // star B
                position[2][0] = generateRandomNumber(-1, 150); // along x-axis
                position[2][1] = 150; // along y-axis
            }
        }
    }
}


void moveBall()
{
    bool userBallOnBrick = false;
    double lengthX, lengthY;
    lengthX = brickLength / 2;
    lengthY = brickBreadth / 2;

    //userBallPosition[1] -= userBallSpeed;


    if (userBallPosition[1] >= screenSize - userBallRadius)
        userBallPosition[1] = screenSize - userBallRadius;

    for (int i = 0; i < 6; i++)
    {
        double rightCornerX, leftCornerX, horizon;

        leftCornerX = brickPosition[i][0] - lengthX - userBallRadius;
        rightCornerX = brickPosition[i][0] + lengthX + userBallRadius;
        horizon = brickPosition[i][1] + lengthY;

        if (userBallPosition[0] >= leftCornerX && userBallPosition[0] <= rightCornerX && userBallPosition[1] > (horizon + 1) && userBallPosition[1] <= (horizon + userBallRadius))
        {
            userBallPosition[1] += brickSpeed;
            userBallOnBrick = true;
        }

    }
    if (userBallOnBrick == false)
            userBallPosition[1] -= userBallSpeed;

    return;
}

void initRadius()
{
    radius[0] = 20; // Radius of Danger Ball
    radius[1] = radius[2] = 5;  // Radius of the Stars

    return;
}

void initState()
{
    // state of the elements along x-axis
    for (int i = 0; i < numberOfElements; i++)
        state[i][0] = generateRandomNumber(-100, 100);

    // state of the elements along y-axis

    state[0][1] = 10; // Danger Ball

    // the stars
    state[0][1] = -10;
    state[0][1] = -10;

    return;
}

void initPosition()
{
    // Danger Ball
    position[0][0] = generateRandomNumber(-150, 150); // along x-axis
    position[0][1] = -150; // along y-axis

    // star A
    position[1][0] = generateRandomNumber(-150, 1); // along x-axis
    position[1][1] = 150; // along y-axis

    // star B
    position[2][0] = generateRandomNumber(-1, 150); // along x-axis
    position[2][1] = 150; // along y-axis


    // For Bricks

        // along x-axis                            // along y-axis
    brickPosition[0][0] = screenSize / 2, brickPosition[0][1] = (screenSize / 3) * 2;
    brickPosition[1][0] = -brickPosition[0][0], brickPosition[1][1] = brickPosition[0][1];
    brickPosition[2][0] = -0, brickPosition[2][1] = screenSize / 3;

    brickPosition[3][0] = screenSize / 2, brickPosition[3][1] = -screenSize / 3;
    brickPosition[4][0] = brickPosition[1][0], brickPosition[4][1] = brickPosition[3][1];
    brickPosition[5][0] = 0, brickPosition[5][1] = -brickPosition[0][1];

    // For User Ball
    userBallPosition[0] = 0, userBallPosition[1] = screenSize;

    return;
}

void initElements()
{
    // For Danger Ball
    glPushMatrix();
    glColor3ub(255, 0, 0);
    glTranslatef(position[0][0], position[0][1], 0);
    drawCircle(radius[0], radius[0]);
    glPopMatrix();

    // User Ball
    glPushMatrix();
    glColor3ub(135,206,235);
    glTranslatef(userBallPosition[0], userBallPosition[1], 0);
    drawCircle(userBallRadius, userBallRadius);
    glPopMatrix();

    // For the Stars
    glPushMatrix();
    glColor3ub(0, 255, 0);
    glTranslatef(position[1][0], position[1][1], 0);
    glRotatef(angle,0,0,1);
    drawStar();
    glPopMatrix();

    glPushMatrix();
    glColor3ub(0, 255, 0);
    glTranslatef(position[2][0], position[2][1], 0);
    glRotatef(-angle,0,0,1);
    drawStar();
    glPopMatrix();



    // For Bricks
    for (int i = 0; i < 6; i++)
    {
        glPushMatrix();
        glColor3ub(255,255,255);
        glTranslatef(brickPosition[i][0], brickPosition[i][1], 0);
        drawBrick();
        glPopMatrix();
    }



    return;
}

void initAll()
{
    initRadius();
    initPosition();
    initState();

    return;
}


void rapid_roll_hybrid()
{
    initElements();
    scoreGame();
    checkIfGameEnded();

}

void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);



	//add objects
	//drawAxes();
	//drawGrid();
	//drawCircle(50,50);
    //drawSS();
    //draw_rec();
    //push_pop();
    //simple_trans();
    //rec_animation();



    rapid_roll_hybrid();




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void stateX()
{
    for (int i = 0; i < numberOfElements; i++)
    {
        double temp = screenSize - radius[i];

        if(state[i][0] < 0 && position[i][0] >= temp)
            state[i][0] = 1;

        if(state[i][0] >= 0 && position[i][0] <= -temp)
            state[i][0] = -1;

        if(state[i][0] < 0)
            position[i][0] += dangerBallSpeed;

        else position[i][0] -= dangerBallSpeed;
    }

    return;
}


void stateY()
{
    for (int i = 0; i < numberOfElements; i++)
    {
        double temp = screenSize - radius[i];

        if(state[i][1] < 0 && position[i][1] >= temp)
            state[i][1] = 1;

        if(state[i][1] >= 0 && position[i][1] <= -temp)
            state[i][1] = -1;

        if(state[i][1] < 0)
            position[i][1] += dangerBallSpeed;

        else position[i][1] -= dangerBallSpeed;
    }

    return;
}

void moveBricks()
{
    for (int i = 0; i < 6; i++)
    {
        brickPosition[i][1] += brickSpeed;

        if (brickPosition[i][1] > screenSize)
            brickPosition[i][1] = -brickPosition[i][1];
    }
    return;
}



void animate()
{
    angle+=10;
    stateX();
    stateY();
    moveBricks();
    moveBall();

	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	//clear the screen
	glClearColor(0,0,0,0);


	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
	//gluOrtho2D(-620.0, 620.0, -340.0, 340.0);
}


int main(int argc, char **argv){

	initAll();
	glutInit(&argc,argv);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(700, 50);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Rapid Roll X");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutSpecialFunc(specialKeyListener);


    glutMainLoop();		//The main loop of OpenGL
	//glClear ( GL_COLOR_BUFFER_BIT );

	return 0;
}
