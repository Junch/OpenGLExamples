// Triangle.cpp
// Our first OpenGL program that will just draw a triangle on the screen.

#include <stdio.h>
#include <GL/glew.h>
#include <GL/glfw.h>

#include "glslprogram.h"

GLSLProgram prog;
GLuint vaoHandle;

static const char * szIdentityShaderVP = "#version 130 \n"
                                         "in vec3 vVertex;"
                                         "in vec3 vColor;"
                                         "out vec3 Color;"
                                         "void main(void)"
                                         "{ Color = vColor;"
                                         "  gl_Position = vec4(vVertex, 1.0);"
                                         "}";

static const char * szIdentityShaderFP = "#version 130 \n"
                                         "in vec3 Color;"
                                         "out vec4 FragColor;"
                                         "void main(void)"
                                         "{ FragColor = vec4(Color, 1.0);"
                                         "}";

//static const char * szIdentityShaderFP = "in vec3 Color;"
//                                         "void main(void)"
//                                         "{ gl_FragColor = vec4(Color, 1.0);"
//                                         "}";

///////////////////////////////////////////////////////////////////////////////
// Window has changed size, or has just been created. In either case, we need
// to use the window dimensions to set the viewport and the projection matrix.
void ChangeSize( int w , int h)
{
    glViewport(0 , 0 , w , h );
}

///////////////////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering context.
// This is the first opportunity to do any OpenGL related tasks.
void SetupRC()
{
    // Black background
    glClearColor(0.0f , 0.0f , 0.0f , 1.0f );

    prog.compileShaderFromString (szIdentityShaderVP, GLSLShader:: VERTEX);
    prog.compileShaderFromString (szIdentityShaderFP, GLSLShader:: FRAGMENT);
    // These three lines could not be removed
    prog.bindAttribLocation(0, "vVertex");
    prog.bindAttribLocation(1, "vColor");
    prog.bindFragDataLocation(0, "FragColor");
    prog.link ();
    prog.use ();

    /////////////////// Create the VBO ////////////////////
    GLfloat positionData [] = {
        -0.8f, -0.8f , 0.0f ,
         0.8f, -0.8f, 0.0f,
         0.0f,  0.8f, 0.0f };

    GLfloat colorData[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f };

    // Create and populate the buffer objects
    GLuint vboHandles[2];
    glGenBuffers(2, vboHandles);
    GLuint positionBufferHandle = vboHandles[0];
    GLuint colorBufferHandle = vboHandles[1];

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), positionData, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colorData, GL_STATIC_DRAW);

    // Create and set-up the vertex array object
    glGenVertexArrays( 1, &vaoHandle );
    glBindVertexArray(vaoHandle);

    glEnableVertexAttribArray(0);  // Vertex position
    glEnableVertexAttribArray(1);  // Vertex color

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );

    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );
}


///////////////////////////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene( void)
{
    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glBindVertexArray(vaoHandle );
    glDrawArrays(GL_TRIANGLES , 0 , 3 );
}


///////////////////////////////////////////////////////////////////////////////
// Main entry point for GLUT based programs
int main( int argc , char * argv [])
{
    int running = GL_TRUE;
    // Initialize GLFW
    if( !glfwInit() )
    {
        exit( EXIT_FAILURE );
    }

    // Open an OpenGL window
    if( !glfwOpenWindow( 800,600, 0,0,0,0,0,0, GLFW_WINDOW ) )
    {
        glfwTerminate();
        exit( EXIT_FAILURE );
    }

    GLenum err = glewInit();
    if ( GLEW_OK != err) {
        fprintf(stderr , "GLEW Error: %s\n" , glewGetErrorString (err));
        exit( EXIT_FAILURE );
    }

    SetupRC();

    // Main loop
    while( running )
    {
        //// OpenGL rendering goes here...
        RenderScene();
        glfwSwapBuffers();
        // Check if ESC key was pressed or window was closed

        running = !glfwGetKey( GLFW_KEY_ESC ) &&
            glfwGetWindowParam( GLFW_OPENED );
    }
    // Close window and terminate GLFW
    glfwTerminate();
    // Exit program
    exit( EXIT_SUCCESS );
}