#include<GL/glut.h>

int help = 0;

//variable declarations
GLint  scene = 0, play_state = 0, still_state = 0;
GLfloat depth_tracker = 0;

//for sea
GLfloat const_adder = 0.05, wave_c = 0.1, plane_c = 5, ocean_color_blue = 0.572;
GLint negat_multiplier = -1;

//for submarine
GLfloat submarine_position_y = 0;

//for depth meter
GLint scale_state = 1;
GLfloat scale_c = 0.1;

void *font_pointer;

//function prototypes
void landscape();
void oceanLayerOne();
void submarine();
void oceanLayerTwo();
void depthScale(int);
void displayObject(GLfloat);
void depthStrip();
void shipTitanic();
void blueWhale();
void blueWhaleInSplash();
void fishes();
void scubaDiver();
void marianaTrench();

void writeText(float, float, float, char*);
void displayMenu(int);
void displaySelector(void);
void splashScreen(void);
void helpScreen();
void helpScreenInBetween();
void startAnimation(void);
void keyboardListener(unsigned char, int, int);
void sceneUpdater(int);

//function definitions
void writeText(float x, float y, float z, char *string)
{
	char *c;
	glRasterPos3f(x, y, z);

	for (c = string; *c != '\0'; c++)
		glutBitmapCharacter(font_pointer, *c);
}

void displayMenu(int m) {
	switch (m) {
	case 1:
		helpScreenInBetween();
		//play_state = 0;
		break;
	case 2: exit(0);
	}
}

void keyboardListener(unsigned char key, int x, int y)
{
	switch (key) {
	case 13:
		if (scene == 0)
		{
			scene = 1;
			displaySelector();
		}
		else if (scene == 1)									//ASCII value of enter key is 13
		{
			scene = 2;
			displaySelector();
		}

		if (help == 1) {
			help = 0;
			play_state = 1;
			displaySelector();
		}

		break;

	case 115:
		if (still_state == 1)									//ASCII of s is 115
		{
			still_state = 0;
			displaySelector();
		}
		else if (still_state == 0)
		{
			still_state = 1;
			displaySelector();
		}
		break;

	case 27:
		exit(0);											//ASCII value of esc is 27
		break;

	case 32:
		if (play_state == 0)								//ASCII value of spacebar key is 32
		{
			play_state = 1;
			displaySelector();
		}
		else if (play_state == 1)
		{
			play_state = 0;
			displaySelector();
		}

		if (help == 1) {
			help = 0;
			play_state = 1;
			displaySelector();
		}
		break;

	default:	break;
	}
}

void sceneUpdater(int value)
{
	//depth updater
	depth_tracker += 4;

	//sea wave movement and color transition
	if (plane_c < 500)
	{
		if (ocean_color_blue > 0.2)
		{
			ocean_color_blue -= 0.003;
		}

		plane_c = plane_c + 5;
		wave_c += const_adder;
		if (wave_c > 0.3)
			const_adder *= negat_multiplier;
		if (wave_c < 0.0)
			const_adder *= negat_multiplier;
	}

	//depth scale movement
	if (scale_state)
	{
		if ((scale_c > 1) || (scale_c < 0))
		{
			scale_c *= negat_multiplier;
		}
		scale_c += 0.1;
	}

	glutPostRedisplay();
}

void displaySelector(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0, 0.5, 1, 0);

	if (scene == 0)
		splashScreen();
	else if (scene == 1)
		helpScreen();
	else if (scene == 2)
		startAnimation();

	glutSwapBuffers();
}

void splashScreen(void)
{
	font_pointer = (GLUT_BITMAP_TIMES_ROMAN_24);										//set font

	glClearColor(0.2, 0.2, 0.2, 0);														//set bg color
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1, 1, 1);																	//set text color
	writeText(480, 650, 0.0, "P.A  COLLEGE  OF  ENGINEERING");
	writeText(330, 600, 0.0, "DEPARTMENT  OF  COMPUTER  SCIENCE  AND  ENGINEERING");

	glColor3f(0, 0.5, 1);
	writeText(530, 520, 0.0, "\" DEEP  BLUE  OCEAN \"");
	writeText(545, 470, 0.0, "A MINI PROJECT ON");
	writeText(350, 440, 0.0, "GRAPHICAL REPRESENTATION OF DEPTH OF THE OCEAN");

	glColor3f(1, 1, 1);
	writeText(100, 280, 0.0, "CODED  BY :");
	writeText(100, 240, 0.0, "MUHAMMED  SHAHEER  [ 4PA14CS070 ]");
	writeText(100, 200, 0.0, "MUHAMMED  SHAHID     [ 4PA14CS063 ]");
	writeText(930, 280, 0.0, "GUIDES :");
	writeText(930, 240, 0.0, "Prof. MUHAMMAD  ASIF  RAIBAG");
	writeText(930, 200, 0.0, "Prof. RASHEL  SARKAR");

	glFlush();																			//flush buffer to screen
}

void helpScreen() {
	font_pointer = (GLUT_BITMAP_TIMES_ROMAN_24);										//set font

	glClearColor(0.204, 0.596, 0.859, 0);														//set bg color
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1, 1, 1);																	//set text color
	writeText(570, 650, 0.0, "HELP SCREEN");

	writeText(540, 520, 0.0, "START : ENTER KEY");
	writeText(485, 470, 0.0, "PLAY/PAUSE : SPACEBAR KEY");
	writeText(540, 420, 0.0, "EXIT : ESCAPE KEY");

	blueWhaleInSplash();

	glFlush();
}

void startAnimation(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	landscape();
	oceanLayerOne();
	submarine();
	oceanLayerTwo();

	depthScale(1);

	fishes();

	if (depth_tracker > 200)
	{
		//depth scale is displayed after reaching a particular depth
		if (still_state)
		{
			depthScale(0);
			depthStrip();
		}
		else
		{
			depthScale(1);
		}
	}

	if (play_state)
	{
		glutTimerFunc(500, sceneUpdater, 0);
	}

	displayObject(depth_tracker);

	if (help == 1) {
		play_state = 0;
		helpScreenInBetween();
	}

	glFlush();
}

void landscape()
{
	glPushMatrix();
	glTranslated(0, plane_c, 0);
	glScaled(40.0, 40.0, 0.0);

	//sky
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);
	glVertex2f(0, 7.5);
	glVertex2f(40, 7.5);

	glVertex2f(40.0, 25);
	glVertex2f(0.0, 25);
	glEnd();

	//mountains
	glBegin(GL_POLYGON);
	glColor3f(0.5, 0.5, 0);
	glVertex2f(-4, 7.5);
	glColor3f(0.2, 0.8, 0);
	glVertex2f(2, 9.5);
	glColor3f(0.0, 1, 0);
	glVertex2f(6, 7.5);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.5, 0.5, 0);
	glVertex2f(2, 7.5);
	glColor3f(0.2, 0.8, 0);
	glVertex2f(7, 11);
	glColor3f(0.0, 1, 0);
	glVertex2f(16, 7.5);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.5, 0.5, 0);
	glVertex2f(10, 7.5);
	glColor3f(0.2, 0.8, 0);
	glVertex2f(14, 9.5);
	glColor3f(0.0, 1, 0);
	glVertex2f(18, 7.5);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.5, 0.5, 0);
	glVertex2f(14, 7.5);
	glColor3f(0.2, 0.8, 0);
	glVertex2f(18, 10);
	glColor3f(0.0, 1, 0);
	glVertex2f(22, 7.5);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.5, 0.5, 0);
	glVertex2f(19, 7.5);
	glColor3f(0.2, 0.8, 0);
	glVertex2f(23, 9.5);
	glColor3f(0.0, 1, 0);
	glVertex2f(27, 7.5);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0, 1, 0);
	glVertex2f(23, 7.5);
	glVertex2f(27, 9);
	glVertex2f(31, 7.5);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.0, 1, 0);
	glVertex2f(27, 7.5);
	glColor3f(0.2, 0.8, 0);
	glVertex2f(32, 10.5);
	glColor3f(0.5, 0.5, 0.0);
	glVertex2f(38, 7.5);
	glEnd();

	//sun circle
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 0);
	glVertex2f(26.0, 12.0);
	glVertex2f(26.25, 12.55);

	glVertex2f(27.0, 13);
	glVertex2f(27.75, 12.55);

	glColor3f(0.9, 0.9, 0);
	glVertex2f(28.0, 12.0);
	glVertex2f(27.75, 11.45);

	glColor3f(0.9, 0.8, 0);
	glVertex2f(27.0, 11.0);
	glVertex2f(26.25, 11.45);
	glEnd();

	//sun rays
	glLineWidth(4.0);
	glColor3f(1, 1, 0);

	glBegin(GL_LINES);
	glVertex2f(25.5, 12.0);
	glVertex2f(26.0, 12.0);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(26.25, 12.55);
	glVertex2f(25.8, 13.0);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(27.0, 13.0);
	glVertex2f(27.0, 13.5);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(27.75, 12.55);
	glVertex2f(28.3, 13.0);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(28.0, 12.0);
	glVertex2f(28.5, 12.0);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(27.75, 11.45);
	glVertex2f(28.3, 11.0);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(27.0, 11.0);
	glVertex2f(27.0, 10.5);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(26.25, 11.45);
	glVertex2f(25.8, 11.0);
	glEnd();

	glPopMatrix();
}

void oceanLayerOne()
{
	glPushMatrix();
	glTranslated(0, plane_c, 0.0);
	glScaled(40.0, 40.0, 0.0);

	glColor3f(0.0, 0.0, ocean_color_blue);
	//wave top
	glBegin(GL_POLYGON);
	glVertex2f(-4, wave_c + 7.6);
	glVertex2f(-3, wave_c + 7.7);
	glVertex2f(-2, wave_c + 7.9);
	glVertex2f(-2, wave_c + 8.1);
	glVertex2f(-2, wave_c + 8.15);
	glVertex2f(-1, wave_c + 8.1);
	glVertex2f(0, wave_c + 7.9);
	glVertex2f(1, wave_c + 7.7);
	glVertex2f(2, wave_c + 7.5);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(0, wave_c + 7.6);
	glVertex2f(1, wave_c + 7.7);
	glVertex2f(2, wave_c + 7.9);
	glVertex2f(3, wave_c + 8.1);
	glVertex2f(4, wave_c + 8.15);
	glVertex2f(5, wave_c + 8.1);
	glVertex2f(6, wave_c + 7.9);
	glVertex2f(7, wave_c + 7.7);
	glVertex2f(8, wave_c + 7.5);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(6, wave_c + 7.6);
	glVertex2f(7, wave_c + 7.7);
	glVertex2f(8, wave_c + 7.9);
	glVertex2f(9, wave_c + 8.1);
	glVertex2f(10, wave_c + 8.15);
	glVertex2f(11, wave_c + 8.1);
	glVertex2f(12, wave_c + 7.9);
	glVertex2f(13, wave_c + 7.7);
	glVertex2f(14, wave_c + 7.5);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(13, wave_c + 7.6);
	glVertex2f(14, wave_c + 7.7);
	glVertex2f(15, wave_c + 7.9);
	glVertex2f(16, wave_c + 8.1);
	glVertex2f(17, wave_c + 8.15);
	glVertex2f(18, wave_c + 8.1);
	glVertex2f(19, wave_c + 7.9);
	glVertex2f(20, wave_c + 7.7);
	glVertex2f(21, wave_c + 7.5);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(20, wave_c + 7.6);
	glVertex2f(21, wave_c + 7.7);
	glVertex2f(22, wave_c + 7.9);
	glVertex2f(23, wave_c + 8.1);
	glVertex2f(24, wave_c + 8.15);
	glVertex2f(25, wave_c + 8.1);
	glVertex2f(26, wave_c + 7.9);
	glVertex2f(27, wave_c + 7.7);
	glVertex2f(28, wave_c + 7.5);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(26, wave_c + 7.6);
	glVertex2f(27, wave_c + 7.7);
	glVertex2f(28, wave_c + 7.9);
	glVertex2f(29, wave_c + 8.1);
	glVertex2f(30, wave_c + 8.15);
	glVertex2f(31, wave_c + 8.1);
	glVertex2f(32, wave_c + 7.9);
	glVertex2f(33, wave_c + 7.7);
	glVertex2f(34, wave_c + 7.5);
	glEnd();

	//plane bottom
	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 0.1);
	glVertex2f(0, -13);
	glVertex2f(40, -13);
	glColor3f(0.0, 0.0, ocean_color_blue);
	glVertex2f(40, 7.9);
	glVertex2f(0, 7.9);
	glEnd();

	glPopMatrix();
}

void submarine()
{
	glPushMatrix();
	glTranslated(0, submarine_position_y, 0);
	glScaled(40.0, 40.0, 0.0);
	glBegin(GL_POLYGON);
	//outer circle
	glColor3f(0.8, 0.6, 0);
	glVertex2f(14, 7);
	glVertex2f(14.5, 8.3);

	glColor3f(0.5, 0.6, 0);
	glVertex2f(16, 9);
	glVertex2f(17.5, 8.3);

	glColor3f(0.5, 0.6, 0);
	glVertex2f(18, 7);
	glVertex2f(17.5, 5.8);

	glColor3f(0.6, 0.6, 0);
	glVertex2f(16, 5);
	glVertex2f(14.5, 5.8);
	glEnd();
	//inner circle
	glBegin(GL_POLYGON);
	glColor3f(0.4, 0.4, 0);

	glVertex2f(14.5, 7);
	glVertex2f(15, 7.8);

	glColor3f(0.5, 0.6, 0);
	glVertex2f(16, 8.5);
	glVertex2f(17, 7.8);

	glColor3f(0.8, 0.8, 0);
	glVertex2f(17.5, 7);
	glVertex2f(17, 6.3);

	glColor3f(0.4, 0.4, 0);
	glVertex2f(16, 5.5);
	glVertex2f(15, 6.3);
	glEnd();

	//inner circle glass
	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.4, 1);

	glVertex2f(15, 7);
	glVertex2f(15.2, 7.6);

	glColor3f(0.0, 0.2, 1);
	glVertex2f(16, 8);
	glVertex2f(16.8, 7.6);

	glColor3f(0.0, 0.3, 0.8);
	glVertex2f(17, 7);
	glVertex2f(16.7, 6.4);

	glVertex2f(16, 6);
	glVertex2f(15.4, 6.3);

	glEnd();
	glPopMatrix();
}

void oceanLayerTwo()
{
	glPushMatrix();
	glTranslated(0, plane_c, 0.0);
	glScaled(40.0, 40.0, 0.0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);


	//plane bottom
	glBegin(GL_POLYGON);
	glColor4f(0.0, 0.0, ocean_color_blue, 0.3);
	glVertex2f(0, -13);
	glVertex2f(40, -13);

	glVertex2f(40, 7.5);
	glVertex2f(0, 7.5);
	glEnd();

	glPopMatrix();
}

void depthScale(int running)
{
	glPushMatrix();
	glScaled(40.0, 40.0, 0.0);
	if (running)
		glTranslated(0, scale_c, 0);
	else
		glTranslated(0, 1, 0);

	glColor3f(0, 0, 0);

	glLineWidth(4.0);
	glBegin(GL_LINES);

	//vertical
	glVertex2f(0, -5);
	glVertex2f(0, 20);
	glEnd();

	//horizontal
	glBegin(GL_LINES);

	glVertex2f(0, -0.5);
	glVertex2f(1, -0.5);
	glVertex2f(0, -1);
	glVertex2f(1, -1);
	glVertex2f(0, -1.5);
	glVertex2f(1, -1.5);

	glVertex2f(0, 0);
	glVertex2f(2, 0);

	glVertex2f(0, 0.5);
	glVertex2f(1, 0.5);
	glVertex2f(0, 1);
	glVertex2f(1, 1);
	glVertex2f(0, 1.5);
	glVertex2f(1, 1.5);

	glVertex2f(0, 2);
	glVertex2f(2, 2);

	glVertex2f(0, 2.5);
	glVertex2f(1, 2.5);
	glVertex2f(0, 3);
	glVertex2f(1, 3);
	glVertex2f(0, 3.5);
	glVertex2f(1, 3.5);

	glVertex2f(0, 4);
	glVertex2f(2, 4);

	glVertex2f(0, 4.5);
	glVertex2f(1, 4.5);
	glVertex2f(0, 5);
	glVertex2f(1, 5);
	glVertex2f(0, 5.5);
	glVertex2f(1, 5.5);

	glVertex2f(0, 6);
	glVertex2f(2, 6);

	glVertex2f(0, 6.5);
	glVertex2f(1, 6.5);
	glVertex2f(0, 7);
	glVertex2f(1, 7);
	glVertex2f(0, 7.5);
	glVertex2f(1, 7.5);

	glVertex2f(0, 8);
	glVertex2f(2, 8);

	glVertex2f(0, 8.5);
	glVertex2f(1, 8.5);
	glVertex2f(0, 9);
	glVertex2f(1, 9);
	glVertex2f(0, 9.5);
	glVertex2f(1, 9.5);

	glVertex2f(0, 10);
	glVertex2f(2, 10);

	glVertex2f(0, 10.5);
	glVertex2f(1, 10.5);
	glVertex2f(0, 11);
	glVertex2f(1, 11);
	glVertex2f(0, 11.5);
	glVertex2f(1, 11.5);

	glVertex2f(0, 12);
	glVertex2f(2, 12);

	glVertex2f(0, 12.5);
	glVertex2f(1, 12.5);
	glVertex2f(0, 13);
	glVertex2f(1, 13);
	glVertex2f(0, 13.5);
	glVertex2f(1, 13.5);

	glVertex2f(0, 14);
	glVertex2f(2, 14);

	glVertex2f(0, 14.5);
	glVertex2f(1, 14.5);
	glVertex2f(0, 15);
	glVertex2f(1, 15);
	glVertex2f(0, 15.5);
	glVertex2f(1, 15.5);

	glVertex2f(0, 16);
	glVertex2f(2, 16);

	glVertex2f(0, 16.5);
	glVertex2f(1, 16.5);
	glVertex2f(0, 17);
	glVertex2f(1, 17);
	glVertex2f(0, 17.5);
	glVertex2f(1, 17.5);

	glVertex2f(0, 18);
	glVertex2f(2, 18);

	glVertex2f(0, 18.5);
	glVertex2f(1, 18.5);
	glVertex2f(0, 19);
	glVertex2f(1, 19);
	glVertex2f(0, 19.5);
	glVertex2f(1, 19.5);

	glEnd();

	glPopMatrix();
}

void depthStrip()
{
	glPushMatrix();
	glScaled(40, 40, 0);
	glColor3f(0, 0.2, 0.1);
	glBegin(GL_POLYGON);
	glVertex2f(0, 5);
	glVertex2f(35, 5);
	glVertex2f(35, 4.5);
	glVertex2f(0, 4.5);
	glEnd();
	glPopMatrix();
}

void helpScreenInBetween() {

	help = 1;

	font_pointer = (GLUT_BITMAP_TIMES_ROMAN_24);										//set font

	glClearColor(0.204, 0.596, 0.859, 0);														//set bg color
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1, 1, 1);																	//set text color
	writeText(570, 650, 0.0, "HELP SCREEN");

	writeText(540, 520, 0.0, "START : ENTER KEY");
	writeText(485, 470, 0.0, "PLAY/PAUSE : SPACEBAR KEY");
	writeText(540, 420, 0.0, "EXIT : ESCAPE KEY");

	blueWhaleInSplash();

	glFlush();
}

void displayObject(GLfloat curr_depth) {
	if ((curr_depth > 400) && (curr_depth < 600)) {
		still_state = 1;
		scubaDiver();
	}
	else if ((curr_depth > 700) && (curr_depth < 800)) {
		glPushMatrix();
		glScaled(40, 40, 0);
	
		glColor3f(1, 1, 1);
		writeText(2.5, 8.5, 0.0, "200 metres");
		writeText(2.5, 7.5, 0.0, "Epipelagic Zone!");
		glPopMatrix();
	}
	else if ((curr_depth > 800) && (curr_depth < 1000)) {
		still_state = 1;
		blueWhale();
	}
	else if ((curr_depth > 1050) && (curr_depth < 1150)) {
		glPushMatrix();
		glScaled(40, 40, 0);

		glColor3f(1, 1, 1);
		writeText(2.5, 8.5, 0.0, "~1000 metres");
		writeText(2.5, 7.5, 0.0, "Mesopelagic Zone!");
		glPopMatrix();
	}
	else if ((curr_depth > 1200) && (curr_depth < 1400)) {
		still_state = 1;
		shipTitanic();
	}
	else if ((curr_depth > 1450) && (curr_depth < 1550)) {
		glPushMatrix();
		glScaled(40, 40, 0);

		glColor3f(1, 1, 1);
		writeText(2.5, 8.5, 0.0, "4000 metres");
		writeText(2.5, 7.5, 0.0, "Bathypelagic Zone!");
		glPopMatrix();
	}
	else if ((curr_depth > 1600) && (curr_depth < 1700)) {
		glPushMatrix();
		glScaled(40, 40, 0);

		glColor3f(1, 1, 1);
		writeText(2.5, 8.5, 0.0, "6000 metres");
		writeText(2.5, 7.5, 0.0, "Abyssotelagic Zone!");
		glPopMatrix();
	}
	else if ((curr_depth > 1750) && (curr_depth < 1900)) {
		glPushMatrix();
		glScaled(40, 40, 0);

		glColor3f(1, 1, 1);
		writeText(2.5, 8.5, 0.0, "> 6000 metres");
		writeText(2.5, 7.5, 0.0, "Hadalpelagic Zone!");
		glPopMatrix();
	}
	else if (curr_depth > 2000) {
		if (submarine_position_y > -130) {
			submarine_position_y -= 4;
		}
		else {
			play_state = 0;
		}
		marianaTrench();
	}
	else {
		still_state = 0;
	}
}

void scubaDiver()
{
	glPushMatrix();
	glScaled(40.0, 40.0, 0.0);

	//text
	glColor3f(1, 1, 1);
	writeText(2.5, 5.5, 0.0, "40 metres");
	writeText(2.5, 3.5, 0.0, "Maximum depth for recreational scuba divers");

	//head
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 0);
	glVertex2f(22.0, 9.0);
	glVertex2f(22.25, 9.55);

	glVertex2f(23.0, 10);
	glVertex2f(23.75, 9.55);

	glVertex2f(24.0, 9.25);
	glVertex2f(23.75, 8.45);

	glVertex2f(23.0, 8.0);
	glVertex2f(22.25, 8.45);
	glEnd();

	//body
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 0);
	glVertex2f(24, 9.8);
	glVertex2f(24.2, 10);
	glVertex2f(28, 9);
	glVertex2f(28, 7);
	glVertex2f(24.2, 8);
	glVertex2f(23.8, 8.2);
	glEnd();

	//hands
	glBegin(GL_POLYGON);
	glVertex2f(24, 8.3);
	glVertex2f(25, 8);
	glVertex2f(25.5, 6.5);
	glVertex2f(25, 6.5);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(25, 7);
	glVertex2f(25, 6.5);
	glVertex2f(23.8, 6.6);
	glVertex2f(23.8, 6.8);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(23.8, 6.6);
	glVertex2f(23.3, 6.5);
	glVertex2f(23.3, 6.9);
	glVertex2f(23.8, 6.8);
	glEnd();

	//thighs
	glBegin(GL_POLYGON);
	glVertex2f(28, 9);
	glVertex2f(31, 8.5);
	glVertex2f(31, 7);
	glVertex2f(28, 7);
	glEnd();

	//lower leg
	glBegin(GL_POLYGON);
	glVertex2f(31, 8.5);
	glVertex2f(32, 10);
	glVertex2f(32.5, 10);
	glVertex2f(32, 7.4);
	glVertex2f(31, 7);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(31, 8.5);
	glVertex2f(33.5, 8.7);
	glVertex2f(33.5, 8.2);
	glVertex2f(31, 7);
	glEnd();

	//fish tail
	glBegin(GL_POLYGON);
	glVertex2f(32, 10);
	glVertex2f(32.3, 11.4);
	glVertex2f(32.5, 11);
	glVertex2f(34, 12);
	glVertex2f(32.5, 10);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(33.5, 8.7);
	glVertex2f(34, 8.9);
	glVertex2f(34.2, 8.7);
	glVertex2f(33.5, 8.2);
	glEnd();

	//cylinder
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);
	glVertex2f(25, 9.8);
	glVertex2f(28, 9);
	glVertex2f(28.5, 9.8);
	glVertex2f(25.5, 10.7);
	glEnd();

	glPopMatrix();
}

void fishes() {
	glPushMatrix();
	glTranslated((depth_tracker - 600)*(-1) + 5, depth_tracker - 400, 0);
	glScaled(40.0, 40.0, 0.0);

	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);

	glVertex2f(30, 10);
	glVertex2f(30.3, 10.4);
	glVertex2f(31, 10);
	glVertex2f(31.3, 9.9);
	glVertex2f(31, 10);
	glVertex2f(31.3, 10.1);
	glVertex2f(30.3, 9.6);

	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);

	glVertex2f(30 - 2, 10 - 1);
	glVertex2f(30.3 - 2, 10.4 - 1);
	glVertex2f(31 - 2, 10 - 1);
	glVertex2f(31.3 - 2, 9.9 - 1);
	glVertex2f(31 - 2, 10 - 1);
	glVertex2f(31.3 - 2, 10.1 - 1);
	glVertex2f(30.3 - 2, 9.6 - 1);

	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);

	glVertex2f(30 + 1, 10 + 2);
	glVertex2f(30.3 + 1, 10.4 + 2);
	glVertex2f(31 + 1, 10 + 2);
	glVertex2f(31.3 + 1, 9.9 + 2);
	glVertex2f(31 + 1, 10 + 2);
	glVertex2f(31.3 + 1, 10.1 + 2);
	glVertex2f(30.3 + 1, 9.6 + 2);

	glEnd();

	glPopMatrix();
}

void blueWhale()
{
	//function to draw blue whale
	glPushMatrix();
	glScaled(40, 40, 0);

	//text
	glColor3f(1, 1, 1);
	writeText(2.5, 5.5, 0.0, "500 metres");
	writeText(2.5, 3.5, 0.0, "The deepest blue whales can dive");

	//body
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 0);
	glVertex2f(20, 10);
	glVertex2f(22, 7);
	glVertex2f(25, 6);
	glVertex2f(29, 6.2);
	glVertex2f(32, 7.5);
	glVertex2f(30, 8.5);
	glVertex2f(25, 10);
	glEnd();

	//mouth
	glBegin(GL_LINE_STRIP);
	glColor3f(1, 1, 1);
	glVertex2f(20.2, 9.8);
	glVertex2f(21, 9.6);
	glVertex2f(23, 9.4);
	glEnd();

	//eye
	glBegin(GL_POLYGON);
	glVertex2f(23.6, 9.5);
	glVertex2f(23.7, 9.5);
	glVertex2f(23.7, 9.6);
	glVertex2f(23.6, 9.6);
	glEnd();

	//tail down
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 0);
	glVertex2f(32, 7.5);
	glVertex2f(29, 7.5);
	glVertex2f(33.5, 6);
	glEnd();

	//tail up
	glBegin(GL_POLYGON);
	glVertex2f(32, 7.5);
	glVertex2f(29, 7.5);
	glVertex2f(33, 9);
	glEnd();

	//fins
	glBegin(GL_LINE_STRIP);
	glColor3f(1, 1, 1);
	glVertex2f(24, 7.5);
	glVertex2f(25, 7);
	glVertex2f(27, 7.5);
	glVertex2f(25, 8);
	glEnd();

	glPopMatrix();
}

void blueWhaleInSplash()
{
	//function to draw blue whale
	glPushMatrix();
	glScaled(20, 20, 0);

	//body
	glBegin(GL_POLYGON);
	glColor3f(0.125, 0.502, 0.725);
	glVertex2f(20, 10);
	glVertex2f(22, 7);
	glVertex2f(25, 6);
	glVertex2f(29, 6.2);
	glVertex2f(32, 7.5);
	glVertex2f(30, 8.5);
	glVertex2f(25, 10);
	glEnd();

	//mouth
	glBegin(GL_LINE_STRIP);
	glColor3f(1, 1, 1);
	glVertex2f(20.2, 9.8);
	glVertex2f(21, 9.6);
	glVertex2f(23, 9.4);
	glEnd();

	//eye
	glBegin(GL_POLYGON);
	glVertex2f(23.6, 9.5);
	glVertex2f(23.7, 9.5);
	glVertex2f(23.7, 9.6);
	glVertex2f(23.6, 9.6);
	glEnd();

	//tail down
	glBegin(GL_POLYGON);
	glColor3f(0.125, 0.502, 0.725);
	glVertex2f(32, 7.5);
	glVertex2f(29, 7.5);
	glVertex2f(33.5, 6);
	glEnd();

	//tail up
	glBegin(GL_POLYGON);
	glVertex2f(32, 7.5);
	glVertex2f(29, 7.5);
	glVertex2f(33, 9);
	glEnd();

	//fins
	glBegin(GL_LINE_STRIP);
	glColor3f(1, 1, 1);
	glVertex2f(24, 7.5);
	glVertex2f(25, 7);
	glVertex2f(27, 7.5);
	glVertex2f(25, 8);
	glEnd();

	glPopMatrix();
}

void shipTitanic()
{
	glPushMatrix();
	glScaled(40, 40, 0);
	glColor3f(0, 0, 0);
	//text
	glColor3f(1, 1, 1);
	writeText(2.5, 5.5, 0.0, "3,800 metres");
	writeText(2.5, 3.5, 0.0, "Depth at which titanic was found");

	//basement
	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2f(23, 6);
	glVertex2f(30, 6);
	glVertex2f(32, 9);
	glVertex2f(21, 9);
	glEnd();

	//level1
	glBegin(GL_POLYGON);
	glVertex2f(31, 9);
	glVertex2f(24, 9);
	glVertex2f(24.5, 10);
	glVertex2f(30.5, 10);
	glEnd();

	//level2
	glBegin(GL_POLYGON);
	glVertex2f(30, 10);
	glVertex2f(25, 10);
	glVertex2f(25.5, 10.5);
	glVertex2f(29.5, 10.5);
	glEnd();

	//chimney
	glBegin(GL_POLYGON);
	glVertex2f(30, 9);
	glVertex2f(30.8, 9);
	glVertex2f(30.8, 11);
	glVertex2f(30, 11);
	glEnd();

	//window
	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	glVertex2f(24.5, 9);
	glVertex2f(25.5, 9);
	glVertex2f(25.5, 9.6);
	glVertex2f(24.5, 9.6);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(26, 9);
	glVertex2f(27, 9);
	glVertex2f(27, 9.6);
	glVertex2f(26, 9.6);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(27.5, 9);
	glVertex2f(28.5, 9);
	glVertex2f(28.5, 9.6);
	glVertex2f(27.5, 9.6);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(29, 9);
	glVertex2f(30, 9);
	glVertex2f(30, 9.6);
	glVertex2f(29, 9.6);
	glEnd();
	glPopMatrix();

}

void marianaTrench()
{
	glPushMatrix();
	glScaled(40.0, 40.0, 0);

	//text
	glColor3f(1, 1, 1);
	writeText(2.5, 5.5, 0.0, "10,994 metres");
	writeText(2.5, 4, 0.0, "Mariana Trench");
	writeText(2.5, 3, 0, "Deepest known point in ocean");

	//deepest part
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 0);
	glVertex2f(0, 0);
	glColor3f(0.1, 0.1, 0.1);
	glVertex2f(6, 2);
	glColor3f(0, 0, 0);
	glVertex2f(7, 0);
	glColor3f(0.1, 0.1, 0.1);
	glVertex2f(8, 3);
	glColor3f(0, 0, 0);
	glVertex2f(15, 1);
	glColor3f(0.1, 0.1, 0.1);
	glVertex2f(20, 2);
	glColor3f(0, 0, 0);
	glVertex2f(40, 0);
	glEnd();
	glPopMatrix();
}

void main()
{

	glutInitWindowSize(1366, 728);								//my resolution
	glutInitWindowPosition(0, 0);								//window at top left position
	glutCreateWindow("Depth of the Ocean");						//window title

	glutDisplayFunc(displaySelector);
	glutKeyboardFunc(keyboardListener);

	int menuid;
	menuid = glutCreateMenu(displayMenu);
	glutAddMenuEntry("Help Screen", 1);
	glutAddMenuEntry("Exit", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	gluOrtho2D(0.0, 1366.0, 0.0, 728.0);						//set viewport to full window size
	glutMainLoop();
}

