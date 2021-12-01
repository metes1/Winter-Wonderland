#ifdef __APPLE__
#  define GL_SILENCE_DEPRECATION
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include <math.h>
#include <vector>
#include <time.h> 
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "ResourceLoader.h"
#include "Model.h"
#include "Camera.h"
#include "Terrain.h"
#include "particleSystem.h"
#include "Particle.h"

//global variables
Terrain land;
particleSystem snowSystem;
Camera cam;
float verts[8][3] = { {0,-100,256}, {0,256,256}, {256,256,256}, {256,-100,256}, {0,-100,0}, {0,256,0}, {256,256,0}, {256,-100,0} };
bool snowfall = false;
bool wind = false;
int sky = 0;


float TERRAIN_SIZE = 400; 
int maxx; int width; int height; 
int ew = 0;
int tab = 0; 
float scaleRand = 10;
float treeColour[4] = {0.2, 0.6, 0.2, 1};
float blu[4] = {0.137255, 0.137255, 0.556863,1};
bool y = true;

GLUquadric* qobj;
bool moo = false;

float light_pos[4] = {100, 10, 100, 1};
float amb[4] = {0.5, 0.5, 0.5, 1};
float diff[4] = {1, 1, 1, 1};
float spec[4] = {0.3, 0.3, 0.3, 1};
float m_amb[4] = {0.0f, 0.2f, 0.0f, 1.0f};
float m_diff[4] = {0.0f, 0.5f, 0.0f, 1.0f};
float m_spec[4] = {0.0f, 0.05f, 0.0f, 1.0f};
float s_mat[4] = {1, 1, 1, 1};

float purp[4] = {0.53,  0.12, 0.47,1};
float green1[4] = {0.137255,0.556863, 0.137255,1};
float di[4] = {0.32,0.49, 0.46,1};
float mv = 0;
float mvv =0; 
float org[4] = {1.0f, 0.5f , 0.5f};
float black[4] = {0.36f, 0.25f , 0.20f};
float black1[4] = {0.0f, 0.0f , 0.0f};

GLuint textures [3]; 

//an array for image data
GLubyte* img_backD;
GLubyte* img_topD;
GLubyte* img_bottomD;
GLubyte* img_rightD;
GLubyte* img_leftD;
GLubyte* img_frontD;

GLubyte* img_backN;
GLubyte* img_topN;
GLubyte* img_bottomN;
GLubyte* img_rightN;
GLubyte* img_leftN;
GLubyte* img_frontN;

GLuint skyImages[12];
int widthSky1, heightSky1, maxSky1;

//draws skybox faces
void drawPolygon(int a, int b, int c, int d, float v[8][3]){
    glBegin(GL_POLYGON);
    
    glTexCoord2f(0, 0);
    glVertex3fv(v[a]);
    
    glTexCoord2f(0, 1);
    glVertex3fv(v[b]);
    
    glTexCoord2f(1, 1);
    glVertex3fv(v[c]);
    
    glTexCoord2f(1, 0);
    glVertex3fv(v[d]);
    glEnd();
}

/* cube - takes an array of 8 vertices, and draws 6 faces
 *  with drawPolygon, making up a box
 */
void drawSkyBox(float v[8][3])
{
    glDisable(GL_LIGHTING);

    glBindTexture(GL_TEXTURE_2D, skyImages[0+sky]);
    drawPolygon(3, 2, 1, 0, v);

    glBindTexture(GL_TEXTURE_2D, skyImages[1+sky]);
    drawPolygon(7, 6, 5, 4, v);
    
    glBindTexture(GL_TEXTURE_2D, skyImages[2+sky]);
    drawPolygon(4, 5, 1, 0, v);
   
    glBindTexture(GL_TEXTURE_2D, skyImages[3+sky]);
    drawPolygon(7, 6, 2, 3, v);
    
    glBindTexture(GL_TEXTURE_2D, skyImages[4+sky]);
    drawPolygon(2, 1, 5, 6, v);

    glBindTexture(GL_TEXTURE_2D, skyImages[5+sky]);
    drawPolygon(4, 0, 3, 7, v);

    glEnable(GL_LIGHTING);
    glBindTexture(GL_TEXTURE_2D, NULL);

}

//draws terrain
void drawTerrain(){
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);

    //add texture
    if(ew == 1)
    	glBindTexture(GL_TEXTURE_2D, textures[1]);
    else 
        glBindTexture(GL_TEXTURE_2D, textures[0]);

	for (int j = 0; j < land.tSize-1; j++){
		glBegin(GL_QUAD_STRIP);
		for (int i = 0; i < land.tSize-1; i++){
			glNormal3f(land.normals[i][j].dx, land.normals[i][j].dy, land.normals[i][j].dz);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(i, land.heightMap[i][j], j);
            glNormal3f(land.normals[i][j+1].dx, land.normals[i][j+1].dy, land.normals[i][j+1].dz);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(i, land.heightMap[i][j+1], j+1);
            glNormal3f(land.normals[i+1][j].dx, land.normals[i+1][j].dy, land.normals[i+1][j].dz);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(i+1, land.heightMap[i+1][j], j);
            glNormal3f(land.normals[i+1][j+1].dx, land.normals[i+1][j+1].dy, land.normals[i+1][j+1].dz);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(i+1, land.heightMap[i+1][j+1], j+1);		
		}
		glEnd();
	}

	//add tree models
    Model tempr = Model("/pine.obj", true, scaleRand, 50, land.heightMap[50][50]+5, 50, false, purp);
    tempr.drawModel();
    Model temprrr =  Model("/pine.obj", true, scaleRand, 200, land.heightMap[200][200]+5, 200, false, green1);
    temprrr.drawModel();
	Model pen =  Model("/pen.obj", true, 2, 200, land.heightMap[200][50]+5, 50, false, di);
    pen.drawModel(); 
    Model pen1 =  Model("/pen.obj", true, 2, 50, land.heightMap[50][200]+5, 200, false, blu);
    pen1.drawModel(); 
}

//draws snowman
void drawman() {
   
    int mod = 50;
    tab +=1; 
    int calc = tab % mod; 
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, s_mat);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, s_mat);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, s_mat);

	glPushMatrix();
    glTranslatef(150+mv, land.heightMap[150+int(mv)][150+int(mvv)]+4, 150+mvv);
    glutSolidSphere(5, 10, 10);
    glTranslatef(0.0f, 10.0f, 0.0f);
    glutSolidSphere(5, 10, 10);
    glTranslatef(0.0f, 0.0f, 5.0f);
    glPushMatrix();
	if(snowfall) {
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, black1);
	    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, black1);
	    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,black1); 
	    glTranslatef(0.0f, 3.0f, 0.0f);
	    glutSolidSphere(0.5, 5, 5);
		glTranslatef(3.0f, 0.0f, 0.0f);
    	glutSolidSphere(0.5, 5, 5);
		glTranslatef(0.0f, -3.0f, 0.0f);
        glutSolidSphere(0.5, 5, 5);
        glTranslatef(-1.5f, -1.0f, 0.0f);
        glutSolidSphere(0.5, 5, 5);
        glTranslatef(-1.5f, 0.0f, 0.0f);
		glutSolidSphere(0.5, 5, 5);
        glTranslatef(-1.5f, 1.0f, 0.0f);
        glutSolidSphere(0.5, 5, 5);
        glPopMatrix();
	}
	else {
    	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, black1);
	    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, black1);
	    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,black1); 
    
        glTranslatef(0.0f, 3.0f, 0.0f);
		glutSolidSphere(0.5, 5, 5);
		glTranslatef(3.0f, 0.0f, 0.0f);
    	glutSolidSphere(0.5, 5, 5);
		glTranslatef(0.0f, -5.0f, 0.0f);
        glutSolidSphere(0.5, 5, 5);
        glTranslatef(-1.5f, 1.0f, 0.0f);
        glutSolidSphere(0.5, 5, 5);
        glTranslatef(-1.5f, 0.0f, 0.0f);
        glutSolidSphere(0.5, 5, 5);
        glTranslatef(-1.5f, -1.0f, 0.0f);
        glutSolidSphere(0.5, 5, 5);
        glPopMatrix();
	}
  	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, org);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, org);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,org);
    glRotatef(0.0f,1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(1.0f, 1.0f, 0.0f);
	glutSolidCone(0.8f,5.0f,10,2);
    glPopMatrix();

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, black);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, black);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,black); 

    qobj = gluNewQuadric();
	gluQuadricNormals(qobj, GLU_SMOOTH);
	glTranslatef(0.0f, -5.0f, -5.0f);
	if(snowfall) {
    	if(moo) {
     		if(5 < calc > 0) { 
				glRotatef(90.0f,0.0f, 1.0f, 0.0f); 
  			} 
  		}
  	else {
    	glRotatef(90.0f,0.0f, 1.0f, 0.0f); 
   	}
  	}
	else {
    	if(moo) {
    		if(30 < calc > 0) { 
 				glRotatef(90.0f,0.0f, 1.0f, 0.0f);
 			}
 		}
 		else {
    		glRotatef(90.0f,0.0f, 1.0f, 0.0f); 
 		}
    }

  	gluCylinder(qobj, 0.5f,0.5f,10.0f,32,32);
	glTranslatef(0.0f, 0.0f, -10.0f);
	gluCylinder(qobj, 0.5f,0.5f,10.0f,32,32);
	glPopMatrix();
}

//draws snow particles
void drawSnow(){
    //draw each snow particle

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, s_mat);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, s_mat);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, s_mat);

    list<Particle>::iterator it;
    for (it = snowSystem.particles.begin(); it != snowSystem.particles.end(); it++){
        glPushMatrix();
            glColor3f(1, 1, 1);
            glTranslatef(it->position.px, it->position.py, it->position.pz);
            glutSolidSphere(0.2,4,4);
        glPopMatrix();

        //snow lays on terrain
        if (it->position.px < 256 && it->position.px > 0 && it->position.pz < 256 && it->position.pz > 0)
            if (it->position.py <= land.heightMap[int(it->position.px)][int(it->position.pz)]) {
                it->speed = 0.0f;  
            }
    }
}

//displays 2d overview of terrain heights
void display2D() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    
        for (int z = 0; z < land.tSize-1; z++) {
            for (int x = 0; x < land.tSize-1; x++) {
                glPointSize(1.0); //size of point
                float y2D = land.heightMap[x][z];
                float color = (y2D-(land.minHeight))/(land.maxHeight);
                glColor3f(color, color, color); //color of point
                glBegin(GL_POINTS); //draws point on screen
                    glVertex3f(x, z, 0.0f);
                glEnd();
            }
        }
    //switch our buffers for a smooth animation
    glutSwapBuffers();
    glutPostRedisplay();
}

void view2D()
{
    glutInitWindowPosition(700,50);
    glutInitWindowSize(land.tSize-1, land.tSize-1);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutCreateWindow("2D Terrain Overview");    //creates the window
    glutDisplayFunc(display2D);

    glClearColor(0, 0, 0, 0);
    glColor3f(1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(0, land.tSize-1, land.tSize-1, 0);
}

void handleKeyboard(unsigned char key, int _x, int _y) {
	switch(key){
        //enable snowfall
        case 's':
        case 'S':
            snowfall = !snowfall;
            break;
        //enable wind
        case 'w':
        case 'W':
            wind = !wind;
            break;
        //enable wind
        case 'n':
        case 'N':
            if (sky == 0) {
            	sky = 6; //night time
            	light_pos[1] = 80; //lighting is less bright
            }
            else {
            	sky = 0; //day time
            	light_pos[1] = 10;
            }
            break;
        //move snowman forward
        case 't':
        case 'T':
            mvv +=1;
            moo = true;
            break;
        //move snowman backward
    	case 'g':
        case 'G':
            mvv -=1;
            moo = true;
            break;
        //move snowman right
    	case 'h':
        case 'H':
            mv +=1;
            moo = true;
            break;
        //move snowman left
        case 'f':
        case 'F':
            mv -=1;
            moo = true;
            break;
        //texture
        case 'c':
        case 'C':
	        if(ew < 1)
	         ew +=1;
	            else {
	                ew = 0;
	            }
	        break;
        //displays 2D terrain overview
        case 'p':
        case 'P':
            view2D();
            break;
		//quits program
        case 27: //ESC key
        case 'q':
        case 'Q':
            exit(0);
            break;
	}
}

//rotate/zoom camera using arrow keys
void specialKey(int key, int x, int y)
{
    switch(key)
    {
        //zoom in
        case GLUT_KEY_UP: {
            cam.zoomIn();
            break;
        }
        //zoom out
        case GLUT_KEY_DOWN: {
            cam.zoomOut();
            break;
        }
        //rotates camera about y axis
        case GLUT_KEY_RIGHT:
            cam.rightRotate();
            break;

        case GLUT_KEY_LEFT:
            cam.leftRotate();
            break;
    }
}

void handleReshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(cam.fovY, 1, 1, 2000);
    glMatrixMode(GL_MODELVIEW);
}

//frames per second
void FPS(int val){
    //continues to add snow when system isn't paused
    if (snowfall){
        snowSystem.add();
    }

    //updates snow system
    snowSystem.update(wind);
    glutTimerFunc(17, FPS, 0); // 1sec = 1000, 60fps = 1000/60 = ~17
}

//loads texture files
GLubyte* LoadPPM(char* file, int* width, int* height, int* maxx)
{
    GLubyte* img;
    FILE *fd;
    int n, m;
    int  k, nm;
    char c;
    int i;
    char b[100];
    float s;
    int red, green, blue;
    
    fd = fopen(file, "r");
    fscanf(fd,"%[^\n] ",b);
    fscanf(fd, "%c",&c);
    while(c == '#')
    {
        fscanf(fd, "%[^\n] ", b);
        fscanf(fd, "%c",&c);
    }
    ungetc(c,fd);
    fscanf(fd, "%d %d %d", &n, &m, &k);
    
    nm = n*m;
    
    img = (GLubyte*)(malloc(3*sizeof(GLuint)*nm));
    
    s=255.0/k;
    
    
    for(i=0;i<nm;i++)
    {
        fscanf(fd,"%d %d %d",&red, &green, &blue );
        img[3*nm-3*i-3]=red*s;
        img[3*nm-3*i-2]=green*s;
        img[3*nm-3*i-1]=blue*s;
    }
    
    *width = n;
    *height = m;
    *maxx = k;
    
    return img;
}

//textures for terrain
void init2() {
    glEnable(GL_TEXTURE_2D);
    glGenTextures(3, textures);
    GLubyte* img_data = LoadPPM("marble.ppm", &width, &height, &maxx);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);

    GLubyte* img_data1 = LoadPPM("star.ppm", &width, &height, &maxx);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data1);

    GLubyte* img_data2 = LoadPPM("carpet.ppm", &width, &height, &maxx);
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data2);
    
}

//initializes and loads skybox images
void init() {
    glMatrixMode(GL_TEXTURE);
    img_frontD = LoadPPM("SkyBox/frontD.ppm", &widthSky1, &heightSky1, &maxSky1);
    img_backD = LoadPPM("SkyBox/backD.ppm", &widthSky1, &heightSky1, &maxSky1);
    img_leftD = LoadPPM("SkyBox/leftD.ppm", &widthSky1, &heightSky1, &maxSky1);
    img_rightD = LoadPPM("SkyBox/rightD.ppm", &widthSky1, &heightSky1, &maxSky1);
    img_topD = LoadPPM("SkyBox/topD.ppm", &widthSky1, &heightSky1, &maxSky1);
    img_bottomD = LoadPPM("SkyBox/bottomD.ppm", &widthSky1, &heightSky1, &maxSky1);

    img_frontN = LoadPPM("SkyBox/frontN.ppm", &widthSky1, &heightSky1, &maxSky1);
    img_backN = LoadPPM("SkyBox/backN.ppm", &widthSky1, &heightSky1, &maxSky1);
    img_leftN = LoadPPM("SkyBox/leftN.ppm", &widthSky1, &heightSky1, &maxSky1);
    img_rightN = LoadPPM("SkyBox/rightN.ppm", &widthSky1, &heightSky1, &maxSky1);
    img_topN = LoadPPM("SkyBox/topN.ppm", &widthSky1, &heightSky1, &maxSky1);
    img_bottomN = LoadPPM("SkyBox/bottomN.ppm", &widthSky1, &heightSky1, &maxSky1);

    
    glEnable(GL_TEXTURE_2D);
    
    glGenTextures(12, skyImages);
    
    glBindTexture(GL_TEXTURE_2D, skyImages[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthSky1, heightSky1, 0, GL_RGB, GL_UNSIGNED_BYTE, img_frontD);   
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, skyImages[1]);   
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthSky1, heightSky1, 0, GL_RGB, GL_UNSIGNED_BYTE, img_backD);    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, skyImages[2]);   
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthSky1, heightSky1, 0, GL_RGB, GL_UNSIGNED_BYTE, img_leftD);   
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, skyImages[3]);   
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthSky1, heightSky1, 0, GL_RGB, GL_UNSIGNED_BYTE, img_rightD);   
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, skyImages[4]);   
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthSky1, heightSky1, 0, GL_RGB, GL_UNSIGNED_BYTE, img_topD);   
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, skyImages[5]);   
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthSky1, heightSky1, 0, GL_RGB, GL_UNSIGNED_BYTE, img_bottomD);   
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, skyImages[6]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthSky1, heightSky1, 0, GL_RGB, GL_UNSIGNED_BYTE, img_frontN);   
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, skyImages[7]);   
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthSky1, heightSky1, 0, GL_RGB, GL_UNSIGNED_BYTE, img_backN);    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, skyImages[8]);   
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthSky1, heightSky1, 0, GL_RGB, GL_UNSIGNED_BYTE, img_leftN);   
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, skyImages[9]);   
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthSky1, heightSky1, 0, GL_RGB, GL_UNSIGNED_BYTE, img_rightN);   
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, skyImages[10]);   
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthSky1, heightSky1, 0, GL_RGB, GL_UNSIGNED_BYTE, img_topN);   
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, skyImages[11]);   
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthSky1, heightSky1, 0, GL_RGB, GL_UNSIGNED_BYTE, img_bottomN);   
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glShadeModel(GL_SMOOTH);
    glClearColor(0,0,0,0);
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    glEnable(GL_BLEND);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 1, 2000);

}

//displays the scene
void handleDisplay(void){
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(cam.fovY, 1, 1, 2000);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(cam.eye.dx, cam.eye.dy, cam.eye.dz, cam.lookAt.dx, cam.lookAt.dy, cam.lookAt.dz, cam.up.dx, cam.up.dy, cam.up.dz);

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

    if (y)
    	init2();
    y = false;

    drawSkyBox(verts);
    drawTerrain();
    drawman();
    drawSnow();
    glutSwapBuffers();
    glutPostRedisplay();
}


//prints instructions to console
void instruct()
{
    cout << "---------------------------------------------------------\n";
    cout << "                 Keyboard Controls         \n";
    cout << "---------------------------------------------------------\n\n";
    cout << " Left-Arrow   => Rotate scene left on y-axis\n";
    cout << " Right-Arrow  => Rotate scene right on y-axis\n";
    cout << " Up-Arrow Key => Zoom In\n";
    cout << " Down-Arrow   => Zoom Out\n";
    cout << " T            => Move Snowman forward\n";
    cout << " G            => Move Snowman backward\n";
    cout << " F            => Move Snowman left\n";
    cout << " H            => Move Snowman right\n";
    cout << " N            => Toggle night time mode\n";
    cout << " S            => Toggle snowfall\n";
    cout << " W            => Toggle wind\n";
    cout << " C            => Toggle textures\n";
    cout << " P            => Show 2D Overview pixel map\n";
    cout << " q/ESC        => Quit program\n";
    cout << "--------------------------------------------------------\n";
}

int main(int argc, char** argv) {
	srand(time(0)); //number randomizer

    glutInit(&argc, argv);
    glutInitWindowSize(800,600);
    glutInitWindowPosition(200,50);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Landscape");
    instruct();
    init2();
    init();

    glutKeyboardFunc(handleKeyboard);
    glutSpecialFunc(specialKey);
    glutReshapeFunc(handleReshape);
    glutDisplayFunc(handleDisplay);
    glutTimerFunc(0, FPS, 0);

    glutMainLoop();

    return 0;
}