
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// include only for osx
#include <GLUT/glut.h>
// otherwise use this on UNIX & WINDOWS :
//#include<GL/glut.h>

// UNCOMMEND FOR VISUAL STUDIO:
// import the required libraries (note: only works for visual studio)
// #pragma comment(lib, "glut32.lib")
// #pragma comment(lib, "opengl32.lib")
// #pragma comment(lib, "glu32.lib")


GLfloat zoom=1.0f;
int n;												// animation angle
int rotor_speed = 10; 				// rotor animation speed
double drone_y_positon = 5.0;	// Drohnenhoehe
// Kamerabewegung :
double swipe_left_right = 4, swipe_up_down = 2, swipe_through_back = 12;

void quader(double l, double b, double t){
	GLdouble f[8][3]={
		{-l/2,-b/2,-t/2},{ l/2,-b/2,-t/2},
		{ l/2, b/2,-t/2},{-l/2, b/2,-t/2},
		{-l/2,-b/2, t/2},{ l/2,-b/2, t/2},
		{ l/2, b/2, t/2},{-l/2, b/2, t/2}};

	GLint in[18]={0,4,1,5,2,6,3,7,0,3,4,7,5,6,1,2,0,3};
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_DOUBLE, 0, f);
	glDrawElements(GL_QUAD_STRIP,4,GL_UNSIGNED_INT,&in[0]);
	glDrawElements(GL_QUAD_STRIP,4,GL_UNSIGNED_INT,&in[4]);
	glDrawElements(GL_QUAD_STRIP,10,GL_UNSIGNED_INT,&in[8]);
	glDisableClientState(GL_VERTEX_ARRAY);
}

// uqader aus triangle_strip
// z = leange, x = breite, y = hoehe
void uquader(double z2, double x2, double y2, double dz, double dy){
	if(dz >= z2) printf("ERROR on uqader : dz is too big!\n");
	if(dy >= y2) printf("ERROR on uqader : dy is too big!\n");
	glPushMatrix();
		// Da Parameter anders verstanden als vorgesehen, 90° verdrehen
		glRotated(90,0,1.0,0);
		double x = x2/2;
		double y = y2/2;
		double z = z2/2;

		// build 3 quader and intersect them
		// ************** box 1 *****************//
		glBegin(GL_TRIANGLE_STRIP);
			// *************  BOX FRONT *************
			// hinten unten
			glVertex3f( -x, -y, -z+dz );
			glVertex3f(  x, -y, -z+dz );
			// hinten oben
			glVertex3f( -x,  y, -z+dz );
			glVertex3f(  x,  y, -z+dz );
			// vorne oben
			glVertex3f( -x,  y, -z );
		 	glVertex3f(  x,  y, -z );
			// vorne unten
			glVertex3f( -x, -y, -z );
			glVertex3f(  x, -y, -z );
			// hinten unten
			glVertex3f(-x, -y, -z+dz );
			glVertex3f( x, -y, -z+dz );
			// Linke Seite
			glVertex3f( -x, -y, -z );
			glVertex3f( -x, -y, -z+dz );
			glVertex3f( -x,  y, -z );
			glVertex3f( -x,  y, -z+dz );
			// Rechte Seite
			glVertex3f( x, y, -z+dz);
			glVertex3f( x, y, -z);
			glVertex3f( x, -y, -z+dz);
			glVertex3f( x, -y, -z);
			// ************* BOX TOP : tunnel connecting 2 boxes  *************
			// init
			glVertex3f( x, y, -z+dz);
			glVertex3f( -x, y, -z+dz);
			// Dach
			glVertex3f( x, y, z);
			glVertex3f( -x, y, z);
			// Seite links
			glVertex3f( -x, y-dy, z);
			glVertex3f( -x, y, -z);
			glVertex3f( -x, y-dy, -z);
			// Seite rechts
			glVertex3f( x, y-dy, -z);
			glVertex3f( x, y, -z);
			glVertex3f( x, y-dy, z);
			glVertex3f( x, y, z);
			// Tunnel unten
			glVertex3f( x, y-dy, z);
			glVertex3f( -x, y-dy, z);
			glVertex3f( x, y-dy, -z);
			glVertex3f( -x, y-dy, -z);
			// *************  BOX BACK : mirrored box 1 *************
			// init
			glVertex3f( -x, y, z);
			glVertex3f( x, y, z);
			// hinten unten
			glVertex3f( -x, -y, z-dz );
			glVertex3f(  x, -y, z-dz );
			// hinten oben
			glVertex3f( -x,  y, z-dz );
			glVertex3f(  x,  y, z-dz );
			// vorne oben
			glVertex3f( -x,  y, z );
		 	glVertex3f(  x,  y, z );
			// // vorne unten
			glVertex3f( -x, -y, z );
			glVertex3f(  x, -y, z );
			// hinten unten
			glVertex3f(-x, -y, z-dz );
			glVertex3f( x, -y, z-dz );
			// Linke Seite
			glVertex3f( -x, -y, z );
			glVertex3f( -x, -y, z-dz );
			glVertex3f( -x,  y, z );
			glVertex3f( -x,  y, z-dz );
			// Rechte Seite
			glVertex3f( x, y, z-dz);
			glVertex3f( x, y, z);
			glVertex3f( x, -y, z-dz);
			glVertex3f( x, -y, z);
		glEnd();
	glPopMatrix();
}

// Alternative implementierung von uquader2 mit GL_POLYGON
// z = leange, x = breite, y = hoehe
void uquader2(double z2, double x2, double y2, double dz, double dy){
	if(dz >= z2) printf("ERROR on uqader : dz is too big!\n");
	if(dy >= y2) printf("ERROR on uqader : dy is too big!\n");
	glPushMatrix();
		// Da Parameter anders verstanden als vorgesehen, 90° verdrehen
		glRotated(90,0,1.0,0);
		double x = x2/2;
		double y = y2/2;
		double z = z2/2;
		// build 3 quader and intersect them
		// ************** box 1 *****************//
		glBegin(GL_POLYGON);
			// Rückseite
			glVertex3f(  x, -y, -z+dz );
			glVertex3f(  x,  y, -z+dz );
			glVertex3f( -x,  y, -z+dz );
			glVertex3f( -x, -y, -z+dz );
		glEnd();
		glBegin(GL_POLYGON);
			// Rechte Seite
			glVertex3f(  x, -y, -z );
			glVertex3f(  x,  y, -z );
			glVertex3f(  x,  y, -z+dz );
			glVertex3f(  x, -y, -z+dz );
		glEnd();
		glBegin(GL_POLYGON);
			// Linkse Seite
			glVertex3f(  -x, -y, -z );
			glVertex3f(  -x,  y, -z );
			glVertex3f(  -x,  y, -z+dz );
			glVertex3f(  -x, -y, -z+dz );
		glEnd();
		glBegin(GL_POLYGON);
			// Oben
			glVertex3f(  x, y, -z );
			glVertex3f(  x, y, -z+dz );
			glVertex3f(  -x, y, -z+dz );
			glVertex3f(  -x, y, -z );
		glEnd();
		glBegin(GL_POLYGON);
			// Unten
			glVertex3f(  x, -y, -z );
			glVertex3f(  x, -y, -z+dz );
			glVertex3f( -x, -y, -z+dz );
			glVertex3f( -x, -y, -z );
		glEnd();
		glBegin(GL_POLYGON);
			// Front
			glVertex3f(  x,  y, -z );
			glVertex3f(  x, -y, -z );
			glVertex3f( -x, -y, -z );
			glVertex3f( -x,  y, -z );
		glEnd();
		// ************** box 2, mirrored box1 on z *****************//
		glBegin(GL_POLYGON);
			// Rückseite --> Front
			glVertex3f(  x, -y, z-dz );
			glVertex3f(  x,  y, z-dz );
			glVertex3f( -x,  y, z-dz );
			glVertex3f( -x, -y, z-dz );
		glEnd();
		glBegin(GL_POLYGON);
			// Rechte Seite
			glVertex3f(  x, -y, z );
			glVertex3f(  x,  y, z );
			glVertex3f(  x,  y, z-dz );
			glVertex3f(  x, -y, z-dz );
		glEnd();
		glBegin(GL_POLYGON);
			// Linkse Seite
			glVertex3f(  -x, -y, z );
			glVertex3f(  -x,  y, z );
			glVertex3f(  -x,  y, z-dz );
			glVertex3f(  -x, -y, z-dz );
		glEnd();
		glBegin(GL_POLYGON);
			// Oben
			glVertex3f(  x, y, z );
			glVertex3f(  x, y, z-dz );
			glVertex3f(  -x, y, z-dz );
			glVertex3f(  -x, y, z );
		glEnd();
		glBegin(GL_POLYGON);
			// Unten
			glVertex3f(  x, -y, z );
			glVertex3f(  x, -y, z-dz );
			glVertex3f( -x, -y, z-dz );
			glVertex3f( -x, -y, z );
		glEnd();
		glBegin(GL_POLYGON);
			// Front --> Rückseite
			glVertex3f(  x,  y, z );
			glVertex3f(  x, -y, z );
			glVertex3f( -x, -y, z );
			glVertex3f( -x,  y, z );
		glEnd();
		// ************** box 3, tunnel connecting box1 & box2, >> dy << *****************//
		glBegin(GL_POLYGON);
			// Rechte Seite
			glVertex3f(  x, y, z );
			glVertex3f(  x, y, -z );
			glVertex3f(  x, y-dy, -z );
			glVertex3f(  x, y-dy, z );
		glEnd();
		glBegin(GL_POLYGON);
			// Linke Seite
			glVertex3f(  -x, y, z );
			glVertex3f(  -x, y, -z );
			glVertex3f(  -x, y-dy, -z );
			glVertex3f(  -x, y-dy, z );
		glEnd();
		glBegin(GL_POLYGON);
			// Oben
			glVertex3f( x, y, z );
			glVertex3f( x, y, -z );
			glVertex3f(-x, y, -z );
			glVertex3f(-x, y, z );
		glEnd();
		glBegin(GL_POLYGON);
			// Unten
			glVertex3f( x, y-dy, z );
			glVertex3f( x, y-dy, -z );
			glVertex3f(-x, y-dy, -z );
			glVertex3f(-x, y-dy, z );
		glEnd();
	glPopMatrix();
}

void set_mat1(){ // Beispiel 1 Materialien, kann genutzt oder veraendert werden
	GLfloat mat_ambient[]={0.21, 0.13, 0.05, 1};
	GLfloat mat_diffuse[]={0.71, 0.43, 0.18, 1};
	GLfloat mat_specular[]={0.39, 0.27, 0.17, 1};
	GLfloat mat_shininess=25.6;
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_specular);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,mat_shininess);
}

void set_mat2(){// Beispiel 2 Materialien, kann genutzt oder veraendert werden
	GLfloat mat_ambient[]={0.25, 0.21, 0.21, 1};
	GLfloat mat_diffuse[]={0.99, 0.83, 0.83, 1};
	GLfloat mat_specular[]={0.3, 0.3, 0.3, 1};
	GLfloat mat_shininess=11.3;

	glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialf(GL_FRONT,GL_SHININESS,mat_shininess);
}

void set_mat3(){// Beispiel 3 Materialien, selbst erstellt
	GLfloat mat_ambient[]={0.6, 0.6, 0.6, 1};
	GLfloat mat_diffuse[]={0.6, 0.6, 0.6, 1};
	GLfloat mat_specular[]={0.6, 0.6, 0.6, 1};
	GLfloat mat_shininess=128;

	glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialf(GL_FRONT,GL_SHININESS,mat_shininess);
}

void set_bank(){ // Bank aus grossem Element und 3 kleine Sitzflächen
	// Anschauungsunterricht von Konrad-Zuse-Haus
	glPushMatrix(); // jede Bank neu erstellen
		glTranslated(0.95, 0.225, 0.275);  // von symmetrisch zu Eckpunkt mit  0,0,0 
		//glRotated(-90.0, 1.0, 0.0, 0.0); // um x-Achse drehen
		set_mat1(); // Matrialien setzen
		glEnable(GL_COLOR_MATERIAL);
		glColor3ub(193,131,106);
		uquader(1.8, 0.45, 0.55, 0.15, 0.15);
		// quader(1.9, 0.45, 0.55);
	glPopMatrix();
	
	glPushMatrix();
		glTranslated(0.95, 0.575, 0.1); // neue Koordinaten fuer Sitzflaeche 3 lange Quader
		set_mat2();
		glEnable(GL_COLOR_MATERIAL);
		glColor3ub(121,100,102);
		quader(1.9, 0.05, 0.05);
		glTranslated(0.0, 0.0, 0.175);// nur relative Verschiebung
		quader(1.9, 0.05, 0.05);
		glTranslated(0.0, 0.0, 0.175);// nur relative Verschiebung
		quader(1.9, 0.05, 0.05);
	glPopMatrix();
}

// Lampe aus Stab, Birne und Lampenschirm
void set_lamp(){
	glPushMatrix();
		// set_mat3();
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); // Default
		glEnable(GL_COLOR_MATERIAL);	// Farben einschalten
		// STAB
		glPushMatrix();
			glColor3ub(65, 47, 45);
			glTranslated(0,2,0);
			quader(0.1,4.0,0.1);
		glPopMatrix();

		// gelbe Birne
		glPushMatrix();
			glColor4f(1,1, 0, 0);	// Yellow
			glTranslated(0,4.0,0);
			glutSolidSphere(0.2,10,10);
		glPopMatrix();
			
		// Lampenschirm
		glPushMatrix();
			glColor3ub(65, 47, 45);
			glRotated(-90,1,0,0);
			glTranslated(0,0,4);
			glutSolidCone(0.5,0.4,10,10);
		glPopMatrix();

		glDisable(GL_COLOR_MATERIAL);	// Farben ausschalten
	glPopMatrix(); 

}

// Aus Torus, Kegel und Quadern Rotor in X-Y-Ebene erzeugen 
void set_rotor(){
	// 4 Rotoren
	glPushMatrix();
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); // Default
		glEnable(GL_COLOR_MATERIAL);	// Farben einschalten
		glColor3ub(217,66,20);
		// ********** ROTOR 1 ************
		glTranslated(0,0,-0.3);
		glutSolidTorus(0.1, 0.6, 20, 10);
			// Kegel	
		glutSolidCone(0.2, 0.2, 10, 10);
		glRotated(180, 1, 0, 0);
		glutSolidCone(0.2, 0.2, 10, 10);
			// Rototblätter
		glPushMatrix();
				glRotated(rotor_speed*n, 0, 0, 1);	// Rotorblaetter animieren	
				glColor3ub(9,56,68);		
				// 1 Blatt
				glPushMatrix();
				glTranslated(0.3,0,0); 
				quader(0.5,0.1,0.1);
				glPopMatrix();
				// 2 Blatt, 120 Grad
				glPushMatrix();
				glRotated(120,0,0,1);
				glTranslated(0.3,0,0); 
				quader(0.5,0.1,0.1);
				glPopMatrix();
				// 3 Blatt, 240 Grad
				glPushMatrix();
				glRotated(240,0,0,1);
				glTranslated(0.3,0,0); 
				quader(0.5,0.1,0.1);
				glPopMatrix();
		glPopMatrix();
		glDisable(GL_COLOR_MATERIAL);	// Farben ausschalten
	glPopMatrix();
}

// drone = box with 2 wings and 4 rotors
void set_drone(){
	glPushMatrix();
		glColor3ub(82,115,107);

		// ******* BODY *******
		quader(0.7,0.7,0.5);

		glRotated(-n, 0, 0, 1);
	
		// ** 2 wings, crossed **
		glPushMatrix();	
			glColor3ub(9,56,68);
			glTranslated(0,0,-0.3);
			quader(3,0.1,0.1);
			glRotated(90,0,0,1);
			quader(3,0.1,0.1);
		glPopMatrix();

		// ******  ROTORS ***********
		glPushMatrix();	// add rotor 1
		glTranslated(2.1,0,0);
		set_rotor();
		glPopMatrix();

		glPushMatrix();	// add rotor 2
		glTranslated(-2.2,0,0);
		set_rotor();
		glPopMatrix();

		glPushMatrix();	// add rotor 3
		glTranslated(0,-2.1,0);
		set_rotor();
		glPopMatrix();
		
		glPushMatrix();	// add rotor 4
		glTranslated(0,+2.1,0);
		set_rotor();
		glPopMatrix();

	glPopMatrix();
}


int initApplication(){

	// Nebel
	glEnable(GL_FOG);
  float FogCol[3]={0.9,0.9,0.9};	// Nebelfarbe
  glFogfv(GL_FOG_COLOR,FogCol);		
  glFogi(GL_FOG_MODE, GL_LINEAR);	// Lineare Nebelentwicklung
  glFogf(GL_FOG_START, 15);				// Nebelvolumen
  glFogf(GL_FOG_END, 25);

  // Lichtfarben
	GLfloat model_ambient[]={ 1, 1, 1, 1.0};
	GLfloat light_intensity[]={ 1, 1, 0.95, 1};
	// SONNE, unendlich weit, seitlich
	GLfloat light_position[]={ 5, 5, 1, 0};
	GLfloat spot_direction[]={ 0, 0, 0, 0};
	
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// SONNE
	glEnable(GL_LIGHTING);	// Beleuchtung einschalten
	glLightfv(GL_LIGHT1,GL_DIFFUSE, light_intensity);
	glLightfv(GL_LIGHT1,GL_SPECULAR, light_intensity);
	glLightfv(GL_LIGHT1,GL_POSITION, light_position);
	glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION, spot_direction);
	glEnable(GL_LIGHT1);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); // Default
	glEnable(GL_COLOR_MATERIAL); 
	return 1;
}

void reshape(int w, int h)	{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0,w*1.0/h, 0.1, 40.0);	// Sichtweite : 40
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Kameraverschiebung durch Tastendruck, [W] [A] [S] [D] [1] [2]
	gluLookAt(swipe_left_right, swipe_up_down, swipe_through_back, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

// KEY entspricht dem ASCII Zeichen der Taste
void keyboard(unsigned char key, int x, int y)	{
	switch (key) {
		case 119:	// W & w
		case 87: swipe_through_back -= 0.2; break;
		case 65: // A & a
		case 97: swipe_left_right -= 0.2; break;
		case 83: // S & s
		case 115: swipe_through_back += 0.2; break;
		case 100: 	// D & d
		case 68:  swipe_left_right += 0.2; break; 	

		case 101:		// E & e
		case 69:	drone_y_positon += 0.028; break;
		case 113:	// Q & q
		case 81: drone_y_positon -= 0.028; break;

		case 49: swipe_up_down += 0.5;break; // Taste 1
		case 50: swipe_up_down -= 0.5;break; // Taste 2
		case 51: rotor_speed++; break; 			 // Taste 3
		case 52: rotor_speed--; break;			 // Taste 4

	}
	// printf("key : %i and rotor_speed = %i\n",key,rotor_speed);
	// Grenzwerte fuer Manipulation
	if (zoom<1.0) zoom=1.0;
	if (drone_y_positon > 8.0) drone_y_positon = 8.0;
	if (drone_y_positon < 1.0) drone_y_positon = 1.0;
	if (rotor_speed > 60) rotor_speed = 60;	// 60 = FPS, stillstand
	if (rotor_speed < 1) rotor_speed = 1;
	if (swipe_up_down < -1) swipe_up_down = -1;
	glutPostRedisplay();
}

void changeView() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(swipe_left_right, swipe_up_down, swipe_through_back, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void display()	{
	n++;							// animation angle ++
	if(n == 360) n=0; // avoid infinity 
	changeView();

	GLfloat ebene[4][3]={ // Grundflaeche
		{-30.0f, 0.0f,-15.0f}, { 30.0f,0.0f,-15.0f},
		{ -30.0f, 0.0f,15.0f},{ 30.0f,0.0f, 15.0f}};
	GLfloat light_position1[]={ -20.0f, 20.0f, -20.0f, 0.0};

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	float angle;

	glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
	glDisable(GL_LIGHTING);

	// Skybox, Kugel ueber die aktuelle Szene, hellblau
	glPushMatrix();
		// bewegt sich mit der kamera
		glTranslatef(swipe_left_right, swipe_up_down, swipe_through_back);
		glColor4ub(15,170,255,100);
		glutSolidSphere(25,30,30);
	glPopMatrix();

	glPushMatrix();	
			// Bodenflaeche
			glTranslatef(0.0f, -3.0f, 0.0f); // nach unten verschoben
			glColor4ub(143, 163, 112, 255);
			glBegin(GL_QUAD_STRIP);
					glVertex3fv(ebene[0]);
					glVertex3fv(ebene[1]);
					glVertex3fv(ebene[2]);
					glVertex3fv(ebene[3]);
			glEnd();

			// Licht
			glEnable(GL_LIGHTING);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			// Teapot
			glPushMatrix();
					set_mat3();
					glTranslated(-2.5, 0.5, 3);
					glRotated(n,0,1,0);
					glutSolidTeapot(0.5);
			glPopMatrix();

			// Bank ausgeben
			// Bank2 ausgeben, Drehwinkel: ITMZ: 190 -> 910 % 360 = 190  ...
			glPushMatrix();
					glRotated(190, 0.0f, 1.0f, 0.0f);
					set_bank();
			glPopMatrix();

			glPushMatrix();
					glTranslated(3.0, 0.0, 0.0);	// (laengs, hoch, tief) = (x, y, z)
					glRotated(190, 0.0f, 1.0f, 0.0f);	// (winkel, x, y, z)
					set_bank();
			glPopMatrix();

				// Bank2 ausgeben
			glPushMatrix();
					glTranslated(-3.0, 0.0, 0.0);	// (laengs, hoch, tief)
					glRotated(190, 0.0f, 1.0f, 0.0f);
					set_bank();
			glPopMatrix();
			
			// Lampe
			glPushMatrix();
					glTranslated(-5,0,2);
					glRotated(45, 0,1,0);
					set_lamp();
			glPopMatrix();

			// *********** bewegliche Teile ******************
			glDisable(GL_LIGHTING);

			glPushMatrix(); 
					glRotated(90, 1.0, 0.0, 0.0);
					angle = glutGet(GLUT_ELAPSED_TIME)/1000.0f *15;
					glRotated(angle, 0.0, 0.0, 1.0);

					/*  Hier Drohe zusammensetzen und Parameter auswerten*/
					glTranslatef(0, -4, - drone_y_positon);
					set_drone();
			glPopMatrix(); // Time

	glPopMatrix(); // Gesamtszene

	glutSwapBuffers();
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	//initialize glut and parse command line arguments @ see glut documentation for more details
	glutInit(&argc, argv);
	//setting up the window display mode
	glutInitDisplayMode(
	GLUT_RGBA |   //use a rgba target framebuffer
	GLUT_DEPTH |  //create a depth (z-)buffer
	GLUT_DOUBLE |  //create a front and back-buffer
	GLUT_MULTISAMPLE); //create a multi-sample framebuffer (use for fullscreen antialiasing)
	//setting up window pos and size
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 600);
	//create the window
	//from this point we can use the opengl api, because
	//glut has created a valid opengl context for us
	glutCreateWindow( "Hausaufgabe 2");
	//init the application and all required resources
	if (!initApplication())
		return EXIT_FAILURE;
	//register the required callback
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	// glutMouseFunc(mouse);
	//show the window
	glutShowWindow();
	//enter mainloop which handle all window events
	glutMainLoop();
	return EXIT_SUCCESS;

}
