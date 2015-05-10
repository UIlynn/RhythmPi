//
// Book:      OpenGL(R) ES 2.0 Programming Guide
// Authors:   Aaftab Munshi, Dan Ginsburg, Dave Shreiner
// ISBN-10:   0321502795
// ISBN-13:   9780321502797
// Publisher: Addison-Wesley Professional
// URLs:      http://safari.informit.com/9780321563835
//            http://www.opengles-book.com
//

// Hello_Triangle.c
//
//    This is a simple example that draws a single triangle with
//    a minimal vertex/fragment shader.  The purpose of this 
//    example is to demonstrate the basic concepts of 
//    OpenGL ES 2.0 rendering.

#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <esUtil.h>
#include <rpUtil.h>
#include <rpNote.h>

typedef struct
{
   // Handle to a program object
   GLuint programObject;
   float temp;
} UserData;

GLfloat vVertices[] = { -0.1f, -0.1f, 0.0f, 
                        -0.1f,  0.1f, 0.0f,
                        -0.3f, -0.1f, 0.0f,
			-0.3f,  0.1f, 0.0f };

GLfloat tVertices[] = {  0.2f, -0.1f, 0.0f, 
                         0.2f,  0.1f, 0.0f,
                        -0.2f, -0.1f, 0.0f,
			-0.2f,  0.1f, 0.0f };

///
// Create a shader object, load the shader source, and
// compile the shader.
//
GLuint LoadShader ( GLenum type, const char *shaderSrc )
{
   GLuint shader;
   GLint compiled;
   
   // Create the shader object
   shader = glCreateShader ( type );

   if ( shader == 0 )
   	return 0;

   // Load the shader source
   glShaderSource ( shader, 1, &shaderSrc, NULL );
   
   // Compile the shader
   glCompileShader ( shader );

   // Check the compile status
   glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

   if ( !compiled ) 
   {
      GLint infoLen = 0;

      glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );
      
      if ( infoLen > 1 )
      {
         char* infoLog = malloc (sizeof(char) * infoLen );

         glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
         esLogMessage ( "Error compiling shader:\n%s\n", infoLog );            
         
         free ( infoLog );
      }

      glDeleteShader ( shader );
      return 0;
   }

   return shader;

}

///
// Initialize the shader and program object
//
int Init ( ESContext *esContext )
{
   esContext->userData = malloc(sizeof(UserData));

   UserData *userData = esContext->userData;
   GLbyte vShaderStr[] =
      "attribute vec4 vPosition;    \n"
      "attribute vec4 vColour;      \n"
      "varying vec4 vFragmentColour; \n"
      "void main()                  \n"
      "{                            \n"
      "   gl_Position = vPosition;  \n"
      "   vFragmentColour = vColour; \n"
      "}                            \n";
   
   GLbyte fShaderStr[] =  
      "precision mediump float;                     \n"
      "void main()                                  \n"
      "{                                            \n"
      "  gl_FragColor = vec4 ( 1.0, 1.0, 1.0, 1.0); \n"
      "}                                            \n";

   GLuint vertexShader;
   GLuint fragmentShader;
   GLuint programObject;
   GLint linked;

   // Load the vertex/fragment shaders
   vertexShader = LoadShader ( GL_VERTEX_SHADER, vShaderStr );
   fragmentShader = LoadShader ( GL_FRAGMENT_SHADER, fShaderStr );

   // Create the program object
   programObject = glCreateProgram ( );
   
   if ( programObject == 0 )
      return 0;

   glAttachShader ( programObject, vertexShader );
   glAttachShader ( programObject, fragmentShader );

   // Bind vPosition to attribute 0   
   glBindAttribLocation ( programObject, 0, "vPosition" );

   // Link the program
   glLinkProgram ( programObject );

   // Check the link status
   glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

   if ( !linked ) 
   {
      GLint infoLen = 0;

      glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );
      
      if ( infoLen > 1 )
      {
         char* infoLog = malloc (sizeof(char) * infoLen );

         glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
         esLogMessage ( "Error linking program:\n%s\n", infoLog );            
         
         free ( infoLog );
      }

      glDeleteProgram ( programObject );
      return GL_FALSE;
   }

   // Store the program object
   userData->programObject = programObject;

   glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
   return GL_TRUE;
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( ESContext *esContext )
{
   UserData *userData = esContext->userData;
   /*GLfloat vVertices[] = {  0.0f,  0.5f, 0.0f, 
                           -0.4f, -0.4f, 0.0f,
                            0.4f, -0.4f, 0.0f };*/
      
   // Set the viewport
   glViewport ( 0, 0, esContext->width, esContext->height );
   
   // Clear the color buffer
   glClear ( GL_COLOR_BUFFER_BIT );

   // Use the program object
   glUseProgram ( userData->programObject );

   // Load the vertex data
   glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
   glEnableVertexAttribArray ( 0 );

   glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 );
}

void Update ( ESContext *esContext, float deltaTime ) {
   int i;
   UserData *userData = ( UserData* ) esContext->userData;
   
   if(!rp_kbhit()) {
      //rp_fpurge_stdin();
      userData->temp += ( deltaTime * 60.0f );
      if( userData->temp >= 60.0f)
         userData->temp -= 60.0f;
   /*
      for(i = 0; i < 12; i++) {
         vVertices[i] = tVertices[i] * ( userData->temp / 360.0f );
      }
   */
      vVertices[1] = 1.0f - 2.0f * ( userData->temp / 60.0f );
      vVertices[4] = 0.95f - 2.0f * ( userData->temp / 60.0f ); 
      vVertices[7] = 1.0f - 2.0f * ( userData->temp / 60.0f );
      vVertices[10] = 0.95f - 2.0f * ( userData->temp / 60.0f );
   }
   /*else {
      printf("%c", rp_getch());
   }*/
}

int main ( int argc, char *argv[] )
{
   ESContext esContext;
   UserData  userData;

   esInitContext ( &esContext );
   esContext.userData = &userData;

   esCreateWindow ( &esContext, "Hello Triangle", esContext.width, esContext.height, ES_WINDOW_RGB );

   if ( !Init ( &esContext ) )
      return 0;

   esRegisterDrawFunc ( &esContext, Draw );
   esRegisterUpdateFunc ( &esContext, Update );

   esMainLoop ( &esContext );

   return 0;
}
