/* 
 * File:   main.cpp
 * Author: lekkas
 *
 * Created on December 28, 2010, 4:47 AM
 */

#include <GL/glut.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <cmath>
#include "node.h"
#include <unistd.h>
#include <signal.h>
#include <sstream>

#define PI 3.14159265
#define DELAY           15
#define RADIUS          10      // size of node
#define BCAST_RADIUS    100     // maximum signal propagation distance
#define W               850.0
#define H               650.0

using namespace std;

int frames = 0;
float color = 0.0;
int R = 0;          // broadcast ring radius -- animation
int beacon_x = 0;
int beacon_y = 0;
int bcast_counter = 0;  // for logging (printing) the sequence of broadcasts.

vector<node*> nodes;
vector<node*> ideal_MAC_bcast_q;

void log_bcast(int nodeid) {
    stringstream o;
    
    o << nodes.at(nodeid)->getBcast_seq();
    const char *text = o.str().c_str();

    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2i(nodes.at(nodeid)->getX()-2, nodes.at(nodeid)->getY()-2);
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *text);
        text++;
    }
}

static void _Timer(int value) {
        // Choose the next node from IDEAL_MAC_BCAST Queue and
        // for each neighbor a of nodeid do a->broadcast_receive()
        // and change their color to green.(dark green if it is a gateway)
        // Then reset the timer.
   if(frames < 50) {
        R += 2;
        frames++;
        color += 0.02f;

        glutPostRedisplay();
        glutTimerFunc(DELAY, _Timer, 0);
    }
   else {  // end of animation, reset and schedule next node to broadcast
        frames = 0;
        R = 0;
        color = 0;

        // Pick next available node for broadcast
        while(ideal_MAC_bcast_q.size() > 0) {
            node *a = ideal_MAC_bcast_q.back();
            ideal_MAC_bcast_q.pop_back();
            
            /*
             * If forward list of specific node is empty, proceed to the next one
             */
            if( a->forward.empty() )
                continue;

            beacon_x = a->getX();
            beacon_y = a->getY();
     
            for(unsigned int i = 0; i < a->neighbors.size(); i++) {
                // Invoke the broadcast_receive routine in each neighbor
                a->neighbors.at(i)->broadcast_receive(a, ideal_MAC_bcast_q);
                if(!a->neighbors.at(i)->isGateway())
                    a->neighbors.at(i)->setRGB(0.0, 1.0, 0.0);
                else
                    a->neighbors.at(i)->setRGB(0.0, 0.6, 0.0);
            }

            a->broadcasted = true; // this node has successfully broadcasted
            bcast_counter++;
            a->setBcast_seq(bcast_counter);
            
            sleep(1);
            glutTimerFunc(DELAY, _Timer, 0);
            break;
        }
   }
}

/*
 * Found this on some programming related forum and I cannot remember the original author.
 */
void glCircle3i(GLint x, GLint y, GLint radius) {
	glPushMatrix();
	glLoadIdentity();
	//glColor4ub(r, g, b, a);
	glDisable(GL_TEXTURE_2D);
	glLineWidth(1.0f);
	glBegin(GL_POLYGON);
	float xmod, ymod, step = PI / (4*radius); //don't compute much more than is necessary for a solid circle
	for(float theta = 0; theta <= PI/4; theta += step)
	{
		xmod = (cos(theta) * radius);
		ymod = (sin(theta) * radius);
		glVertex2f(x + xmod, y - ymod); //create solids by vertices
		glVertex2f(x + xmod, y + ymod); //and using glpolygon for filling
		glVertex2f(x - xmod, y - ymod);
		glVertex2f(x - xmod, y + ymod);
		glVertex2f(x + ymod, y - xmod); //reflect points through the center and over a 45 degree (PI/4) diagonal
		glVertex2f(x + ymod, y + xmod); //for 1/2 loop time
		glVertex2f(x - ymod, y - xmod);
		glVertex2f(x - ymod, y + xmod);
	}
	glEnd();
	glEnable(GL_TEXTURE_2D);
	glPopMatrix();
}

/*
 * As with the glCircle3i() function, I found this online and cannot remember the original author.
 */
void glHCircle3i(GLint x, GLint y, GLint radius) {
    float angle;
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_TEXTURE_2D);
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
    for(int i = 0; i < 100; i++) {
        angle = i*2*M_PI/100;
        glVertex2f(x + (cos(angle) * radius), y + (sin(angle) * radius));
    }
    glEnd();
    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
}

void OpenGLInit(void) {
    glClearColor (1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, W, 0.0, H);
}

void display(void) {
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
     
     unsigned int i;
     
     // draw beacon if in animation mode
     if(frames != 0) {
         glColor3f(color, color, color);
         glHCircle3i(beacon_x, beacon_y, R);
     }

     // draw lines (links)
     for(i = 0; i < nodes.size(); i++){
         node *a, *b;
         a = nodes.at(i);
         for(unsigned int j = 0; j < a->neighbors.size(); j++) {
             b = a->neighbors.at(j);
             glColor3f(0.0, 0.0, 0.0);
             glLineWidth(1.0f);
             glBegin(GL_LINES);
                glVertex2i(a->getX(), a->getY()); // origin of the line
                glVertex2i(b->getX(), b->getY()); // end of the line
             glEnd();
         }
     }

     // draw nodes
     for(i = 0; i < nodes.size(); i++) {
        glColor3f(nodes.at(i)->getR(), nodes.at(i)->getG(), nodes.at(i)->getB());
        glCircle3i(nodes.at(i)->getX(), nodes.at(i)->getY(), RADIUS);

        stringstream o;
        o << "(" << nodes.at(i)->getX() << "," << nodes.at(i)->getY() << ")";
        const char *text = o.str().c_str();

        glColor3f(0.0, 0.0, 0.0);
        glRasterPos2i(nodes.at(i)->getX()-RADIUS, nodes.at(i)->getY()-RADIUS*2);
        while (*text) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *text);
            text++;
        }
        
        // last but not least, display bcast seq number if applicable
        if(nodes.at(i)->broadcasted) 
            log_bcast(i);
     }
     
     glutSwapBuffers();
}

void myMenu(int selection) {
    int nodeid = selection - 2;
	switch(selection) {
		case 1: // Redraw screen and RESET animation variables
                    for(unsigned int i = 0; i < nodes.size(); i++) {
                        if(nodes.at(i)->isGateway())
                            nodes.at(i)->setRGB(0.6, 0.0, 0.0);
                        else
                            nodes.at(i)->setRGB(0.0, 0.0, 0.0);
                        nodes.at(i)->gotMessage = false;
                        nodes.at(i)->broadcasted = false;
                        nodes.at(0)->setBcast_seq(0);
                        bcast_counter = 0;
                        nodes.at(i)->forward.clear();
                    }
                    glutPostRedisplay();
                    break;
		default: // A node has been chosen to broadcast
                    beacon_x = nodes.at(nodeid)->getX();
                    beacon_y = nodes.at(nodeid)->getY();
                    node *a = nodes.at(nodeid);

                    a->gotMessage = true;   // Obviously, the node that broadcasts
                                            // has the message
                    a->broadcasted = true;
                    bcast_counter++;
                    a->setBcast_seq(bcast_counter);
                    
                    if(!a->isGateway())
                        a->setRGB(0.0, 1.0, 0.0);
                    else
                        a->setRGB(0.0, 0.6, 0.0);

                    // for each neighbor a of nodeid do a->broadcast_receive()
                    // and change their color to green.(dark green if it is a gateway)
                    for(unsigned int i = 0; i < a->neighbors.size(); i++) {
                        a->neighbors.at(i)->broadcast_receive(a, ideal_MAC_bcast_q);
                        if(!a->neighbors.at(i)->isGateway())
                            a->neighbors.at(i)->setRGB(0.0, 1.0, 0.0);
                        else
                            a->neighbors.at(i)->setRGB(0.0, 0.6, 0.0);
                    }
                    glutTimerFunc(DELAY, _Timer, 0); /* register callback for a timer */
                    break;
	}
}

void Tokenize(const string& line, vector<double>& tokens) {
    char delimeter[] = " ";
    char *ptr, *str, *end = NULL;
    double d;

    str = new char[line.length()+1];    // create char array. Strtok works with c strings.
    str[line.length()] = '\0';
    strcpy(str, line.c_str());          // copy c_string represantation of line to str array

    ptr = strtok(str, delimeter);       // string tokenizer loop
    while(ptr != NULL) {
        if(strlen(ptr) > 0) {           // ptr contains token.
            d = strtod(ptr, &end);      // extract double from string.
            if(end == ptr && d == 0)    // See strtod Reference for more info.
                throw string("Wrong file format.");
            else
                tokens.push_back(d);
        }
        ptr = strtok(NULL, " ");
    }
    delete [] str;                      // free str memory.
}

int parseFile(string s) {
    if(s.empty())
        s.assign("./node_configs/default.txt");
        //ifstream myfile ("nodes.txt");

    ifstream myfile (s.c_str());

    string line;
    vector<double> toks;    

    try {
        if (myfile.is_open()) {
            while( !getline (myfile, line).eof() ) {

                Tokenize(line, toks);
                if(toks.size() != 2)
                    throw string("Wrong file format.");

                nodes.push_back(new node((int)toks.at(0), (int)toks.at(1)));   

                line.clear();
                toks.clear();
            }
            myfile.close();
        }
    } catch (string e) {
        cout << "Caught exception: " << e.c_str() << endl;
        return 1;
    }
    return 0;
}

void findNeighs(void) {
    node *a, *b;
    for(unsigned int i = 0; i < nodes.size(); i++) {
        for(unsigned int j = 0; j < nodes.size(); j++) {
            a = nodes.at(i);
            b = nodes.at(j);
            if(a == b ) continue;

            if ( a->dist(a, b) <= BCAST_RADIUS)
                a->neighbors.push_back(b);
            }
    }
    // now prepare each node's key
    for(unsigned int i = 0; i < nodes.size(); i++) {
        a = nodes.at(i);
        a->k.deg = a->neighbors.size();
        a->k.x = a->getX();
        a->k.y = a->getY();
    }
}

void setInternalStatus() {
    node *a;
    for(unsigned int i = 0; i < nodes.size(); i++) {
        a = nodes.at(i);
        a->internal_status();
    }
}
int main(int argc, char** argv) {

    if(argc > 1)
        parseFile(argv[1]);
    else
        parseFile("");
    
    findNeighs();
    // Neighbors have now been found and communication links have been established.
    setInternalStatus();    // see paper
    
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (850, 650);
    glutInitWindowPosition (50, 50);
    glutCreateWindow ("Dominating sets");
    glutDisplayFunc(display);
    OpenGLInit();
        
    glutCreateMenu(myMenu);
    glutAddMenuEntry("Redraw", 1);
    unsigned int i;
    for(i = 0; i < nodes.size(); i++) {
        stringstream o;
        o << "Broadcast (" << nodes.at(i)->getX() << "," << nodes.at(i)->getY() << ")";
        glutAddMenuEntry(o.str().c_str(), i+2);
    }
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutMainLoop();
    return 0;
}

