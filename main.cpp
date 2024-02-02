#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>
#include <vecmath.h>
using namespace std;

// Globals
    // Here are some colors you might use - feel free to add more
GLfloat diffColors[6][4] = { {0.0, 0.0, 1.0, 1.0},  // Bright blue
                            {1.0, 0.0, 0.0, 1.0},  // Bright red
                            {0.0, 1.0, 0.0, 1.0},  // Bright green
                            {0.0, 1.0, 1.0, 1.0},  // Bright cyan
                            {1.0, 0.0, 1.0, 1.0},  // Bright magenta
                            {0.0, 1.0, 0.5, 1.0} };  // Bright turquoise
    // for color index
int colorIndex = 0;

// Light position
GLfloat Lt0pos[] = {1.0f, 1.0f, 5.0f, 1.0f};
// for rotation index
float rotationAngle = 0.0f;
bool isWhiteBackground = false;

// This is the list of points (3D vectors)
vector<Vector3f> vecv;

// This is the list of normals (also 3D vectors)
vector<Vector3f> vecn;

// This is the list of faces (indices into vecv and vecn)
vector<vector<unsigned> > vecf;
bool isTeapot = false; // to toggle teapot object

// You will need more global variables to implement color and position changes


// These are convenience functions which allow us to call OpenGL 
// methods on Vec3d objects
inline void glVertex(const Vector3f &a) 
{ glVertex3fv(a); }

inline void glNormal(const Vector3f &a) 
{ glNormal3fv(a); }


// This function is called whenever a "Normal" key press is received.
void keyboardFunc( unsigned char key, int x, int y )
{
    switch ( key )
    {
    case 27: // Escape key
        exit(0);
        break;
    case 'c':
        // add code to change color here
        colorIndex = (colorIndex + 1) % 6; // Cycle through the colors
        cout << "Color changed to " << diffColors[colorIndex] << "." << endl;
        break;
    case 'r':
        // increase rotation angle
         rotationAngle += 10.0f; 
        break;
    case 'b':
        isWhiteBackground = !isWhiteBackground;  // Flip the boolean value
        if (isWhiteBackground) {
            // change background color to white
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            cout << "Background color changed to white." << endl;
        } else {
            // change background color to dark
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            cout << "Background color changed to dark." << endl;
        }
        break;
    case 't':
        isTeapot = !isTeapot;  // Flip the boolean value
        cout << "Toggled to " << (isTeapot ? "teapot" : "original image") << "." << endl;
        break;
    default:
        cout << "Unhandled key press " << key << "." << endl;        
    }

	// this will refresh the screen so that the user sees the color change
    glutPostRedisplay();
}

// This function is called whenever a "Special" key press is received.
// Right now, it's handling the arrow keys.
void specialFunc( int key, int x, int y )
{
// In your switch-case structure
switch (key) {
    case GLUT_KEY_UP:
        Lt0pos[1] += 0.5; // Move light up
        cout << "Light position: " << Lt0pos[0] << ", " << Lt0pos[1] << ", " << Lt0pos[2] << endl;
        break;
    case GLUT_KEY_DOWN:
        Lt0pos[1] -= 0.5; // Move light down
        cout << "Light position: " << Lt0pos[0] << ", " << Lt0pos[1] << ", " << Lt0pos[2] << endl;
        break;
    case GLUT_KEY_LEFT:
        Lt0pos[0] -= 0.5; // Move light to the left
        cout << "Light position: " << Lt0pos[0] << ", " << Lt0pos[1] << ", " << Lt0pos[2] << endl;
        break;
    case GLUT_KEY_RIGHT:
        Lt0pos[0] += 0.5; // Move light to the right
        cout << "Light position: " << Lt0pos[0] << ", " << Lt0pos[1] << ", " << Lt0pos[2] << endl;
        break;
}

	// this will refresh the screen so that the user sees the light position
    glutPostRedisplay();
}
void loadInput()
{
    string line;
    while (getline(cin, line)) {
        stringstream ss(line);
        string prefix;
        ss >> prefix;

        if (prefix == "v") {
            Vector3f v;
            ss >> v[0] >> v[1] >> v[2];
            vecv.push_back(v);
        } else if (prefix == "vn") {
            Vector3f vn;
            ss >> vn[0] >> vn[1] >> vn[2];
            vecn.push_back(vn);
        } else if (prefix == "f") {
            vector<unsigned int> face;
            while (ss.good()) {
                cout << ss.str() << endl;
                unsigned int vIndex, tIndex, nIndex;
                ss >> vIndex; ss.ignore(1);  // read and ignore the slash
                ss >> tIndex; ss.ignore(1);  // question says this can be ignored
                ss >> nIndex; ss.ignore(1);  // read and ignore the space
                face.push_back(vIndex - 1);
                face.push_back(nIndex - 1);
            }
            vecf.push_back(face);
        }
    }
}



// This function is responsible for displaying the object.
void drawScene(void)
{
    int i;

    // Clear the rendering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 // Position the camera at [0,0,5], looking at [0,0,0],
    // with [0,1,0] as the up direction.
    gluLookAt(0.0, 0.0, 5.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);


    // Rotate the image 
    // ORDER MATTERS HERE; this piece has to be below gluLookAt
    glMatrixMode(GL_MODELVIEW);  // Current matrix affects objects positions
    glLoadIdentity();
    glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);  // Rotate the model around the y-axis
   

	// Here we use the first color entry as the diffuse color
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffColors[colorIndex]);

	// Define specular color and shininess
    GLfloat specColor[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat shininess[] = {100.0};

	// Note that the specular color and shininess can stay constant
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
  
    // Set light properties

    // Light color (RGBA)
    GLfloat Lt0diff[] = {1.0,1.0,1.0,1.0};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
    glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);

	// This GLUT method draws a teapot.  You should replace
	// it with code which draws the object you loaded.
	if (isTeapot) {
        glutSolidTeapot(1.0);
    } else {
        for (const auto& face : vecf) {
            glBegin(GL_TRIANGLES);
            for (int i = 0; i < face.size(); i += 2) {
                unsigned int vIndex = face[i];
                unsigned int nIndex = face[i + 1];
                glNormal3d(vecn[nIndex][0], vecn[nIndex][1], vecn[nIndex][2]);
                glVertex3d(vecv[vIndex][0], vecv[vIndex][1], vecv[vIndex][2]);
            }
            glEnd();
        }
    }


    std::cout << std::endl << std::endl << std::endl << std::endl << std::endl;
    std::cout << "Object loaded successfully. \n Press 'c' to change color \n Use arrow keys to shift lighting \n Press 'r' to rotate object over y axis \n Press 'b' to switch dark/light mode \n Hit 't' anytime to switch to default teapot object" << std::endl;
    // Dump the image to the screen.    
    // glFlush();
    glutSwapBuffers();


}

// Initialize OpenGL's rendering modes
void initRendering()
{
    glEnable(GL_DEPTH_TEST);   // Depth testing must be turned on
    glEnable(GL_LIGHTING);     // Enable lighting calculations
    glEnable(GL_LIGHT0);       // Turn on light #0.
}

// Called when the window is resized
// w, h - width and height of the window in pixels.
void reshapeFunc(int w, int h)
{
    // Always use the largest square viewport possible
    if (w > h) {
        glViewport((w - h) / 2, 0, h, h);
    } else {
        glViewport(0, (h - w) / 2, w, w);
    }

    // Set up a perspective view, with square aspect ratio
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // 50 degree fov, uniform aspect ratio, near = 1, far = 100
    gluPerspective(50.0, 1.0, 1.0, 100.0);
}


// Main routine.
// Set up OpenGL, define the callbacks and start the main loop
int main( int argc, char** argv )
{
    loadInput();
    glutInit(&argc,argv);

    // We're going to animate it, so double buffer 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

    // Initial parameters for window position and size
    glutInitWindowPosition( 60, 60 );
    glutInitWindowSize( 360, 360 );
    glutCreateWindow("Raja Ravindra Kathi: Assignment 0");

    // Initialize OpenGL parameters.
    initRendering();

    // Set up callback functions for key presses
    glutKeyboardFunc(keyboardFunc); // Handles "normal" ascii symbols
    glutSpecialFunc(specialFunc);   // Handles "special" keyboard keys

     // Set up the callback function for resizing windows
    glutReshapeFunc( reshapeFunc );

    // Call this whenever window needs redrawing
    glutDisplayFunc( drawScene );

    // Start the main loop.  glutMainLoop never returns.
    glutMainLoop( );

    return 0;	// This line is never reached.
}
