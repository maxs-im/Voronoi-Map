#include <iostream>
#include "Voronoi.h"
#include <GL/glut.h>
#include <ctime>

#define WIDTH 1366
#define HIGHT 768

Voronoi *task;

void make_display();
void display();
void reshape(int, int);
void mouse(int, int, int, int);
void keyboard(unsigned char, int, int); 
void paint(int, int, const std::vector< std::pair<double, double> > &);
void refreshion();

void main ()
{
	srand( time(0) );
	task= new Voronoi(WIDTH, HIGHT);			// initialize(construct) our class
	make_display();
} // main

void make_display()
{
	int argc = 0;
	glutInit( &argc, nullptr);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);	
	glutCreateWindow("Voronoi");	

	glutFullScreen();
	glClear(GL_COLOR_BUFFER_BIT);

	glutDisplayFunc(display);				
	glutReshapeFunc(reshape);				// window resizing, setting projection parameters

	glutKeyboardFunc(keyboard);				// connection keyboard
	glutMouseFunc(mouse);					// connection mouse

	glutMainLoop();
} // make_display
void display()
{
	glFlush();
} // display
void reshape(int width, int height)
{
	glViewport( 0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D( 0, width, height, 0);			// for the mouse's coordinates
	glPointSize(3.0);						// initialize size of our point
	refreshion();
} // reshape
void keyboard(unsigned char key, int x, int y)
{
			// exit from the program
	if(key == 27) { 
		delete task;
		exit(1);
	}
			// delete previous point
	if(key == 8) {
		 size_t number_sites= task->points.size();
		if(number_sites > 1) {
			task->points.erase(task->points.begin());
			refreshion();
		}
		if(number_sites == 1) {
			task->points.erase(task->points.begin());
			glClear(GL_COLOR_BUFFER_BIT);
			//glFlush();
		}
	}
} // keyboard
void mouse(int button, int state, int x, int y)
{
				// click left to add new point
	if((button == GLUT_LEFT_BUTTON) & (state == GLUT_DOWN)) {
		task->add_point(x, y);
		 std::vector< std::pair<double, double> > mass;
		mass= task->take_polygon(0);
		paint(x, y, mass);
	}
				// click right to refresh
	if((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN))
		refreshion();		// rewrite our diagram (refresh)
} // mouse
void paint(int x, int y, const std::vector< std::pair<double, double> > &mass)				
{		
	std::cout<< std::endl <<x << " "<< y<< std::endl;			// (*) write our points to the console
							//random our colour
	 double colourR= (double) rand()/RAND_MAX,
			colourG= (double) rand()/RAND_MAX,
			colourB= (double) rand()/RAND_MAX;
	glColor3d(colourR, colourG, colourB);

							// draw our VORONOI polygon for each vertex
	glBegin(GL_POLYGON);		
		 size_t size_mass= mass.size();
		for(size_t i= 0; i < size_mass; i++) {
			glVertex2d(mass[i].first, mass[i].second);
			std::cout<< "("<<mass[i].first<< ";"<< mass[i].second<< ") ";
		}
	glEnd();
							// draw our vertex(site)
	glBegin(GL_POINTS);
		glColor3f(1.0, 1.0, 0.0);
		glVertex2i(x, y);
    glEnd();
} // paint
void refreshion()
{
	std::cout<< "\n\n\t\t REFRESH";			// in the console we would see all the polygons for each site
	
	task->refresh();
	 std::vector< std::pair<double, double> > mass;
	 size_t size_points= task->points.size();
	for(size_t i= 0; i < size_points; i++) {
		mass= task->take_polygon(i);
		paint(task->points[i].val.x, task->points[i].val.y, mass);
	}
} // refreshion