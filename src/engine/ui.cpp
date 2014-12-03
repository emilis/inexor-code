#include "engine.h"

namespace ui
{
    vec4 strokeColor( 0,112,25, 255);     ///< Line drawing color
    vec4 fillColor( 255,255,255, 255);   ///< Area drawing color
    int uiw = uih = 0;


//// Canvas Settings ////

    ///creates a Canvas of dimensions w and h
    //it still will spawn over the whole screen, w and h are for your orientation

    void createCanvas(int w, int h, bool depth)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, w, h, 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    /// Clear the window with a background color
    void background(const vec4 c) {
        glClearColor (c.r * (1.0/255), c.g * (1.0/255), c.b * (1.0/255), c.a * (1.0/255));
        glClear (GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    }


//// Primitives ////

    /// Draws a point.
    void point (float x, float y, float z)
    {
        if(strokeColor.a <= 0) return; //fully transparent

        // Draw point using the stroke color
        glColor4fv (strokeColor.v);
        glBegin (GL_POINTS);
        glVertex3f (x,y,z);
        glEnd();
    }

    /// Draws a line segment in 2D
    // given the coordinates of two vertices
    void line (float x0, float y0, float x1, float y1)
    {
        if(strokeColor.a <= 0) return; //fully transparent

        glColor4fv(strokeColor.v);
        glBegin(GL_LINES);
            glVertex2f (x0, y0);
            glVertex2f (x1, y1);
        glEnd();
    }

    /// Draws a triangle in 2D given the coordinates
    /// of its vertices.
    void triangle (float x0, float y0,
                   float x1, float y1,
                   float x2, float y2)
    {

        GLfloat vertices[] = {    x0, y0,    x1, y1,    x2, y2 };
        // activate and specify pointer to vertex array
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, vertices);

        if (fillColor.a > 0) {
            // See if filled triangle is required
            glColor4fv (fillColor.v);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawArrays(GL_TRIANGLES,0,3);
        }
        if (strokeColor.a > 0) {
            // See if outline triangle is required
            glColor4fv (strokeColor.v);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawArrays(GL_TRIANGLES,0,3);
        }
        // deactivate vertex arrays after drawing
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    /// Draws a filled 2D quadrilateral given the coordinates
    // of its vertices.
    void quad (float x0, float y0,
                float x1, float y1,
               float x2, float y2,
               float x3, float y3)
    {
        GLfloat vertices[] = { x0, y0, x1, y1, x2, y2, x3, y3 };

        // activate and specify pointer to vertex array
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, vertices);

        if (fillColor.a > 0) // Filling not transparent
        {
            glColor4fv (fillColor.v);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawArrays(GL_QUADS, 0, 4);
        }
        if (strokeColor.a > 0) //outline isnt transparent
        {
            glColor4fv (strokeColor.v);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawArrays(GL_QUADS, 0, 4);
        }
        // deactivate vertex arrays after drawing
        glDisableClientState(GL_VERTEX_ARRAY);
    }

//// main drawing ////
    void rest()
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        defaultshader->set();
        glEnable(GL_TEXTURE_2D);

        //g3d_render();

        notextureshader->set();
        glDisable(GL_TEXTURE_2D);

        gl_drawhud();
    }
    FVARP(posz, -200, -1, 200);
    void render()
    {
        createCanvas(screenw, screenh, true);
        background(vec4(255,20,120, 255));
        strokeColor.a = 255;
        line(screenw / 2, screenh/ 2, 40, 900);
        triangle(posz, 500, posz, 1000, screenw, 0);
        rest();
    }

    ///////// 3D Primitives /////////

    /// Draws a line segment in 3D
    // @param x0, y0 : first vertex coordinates
    // @param x1, y1: second vertex coordinates
    void line3D (float x0, float y0, float z0, float x1, float y1, float z1)
    {
        if(strokeColor.a <= 0) return; //fully transparent

        glColor4fv(strokeColor.v);
        glBegin(GL_LINES);
            glVertex3f (x0, y0, z0);
            glVertex3f (x1, y1, z1);
        glEnd();
    }

        /// Draws a filled triangle given the coordinates
    /// of its vertices.
    void triangle3D (float x0, float y0, float z0,
                   float x1, float y1, float z1,
                   float x2, float y2, float z2)
    {

        GLfloat vertices[] = {    x0, y0, z0, x1, y1, z1, x2, y2, z2};
        // activate and specify pointer to vertex array
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, vertices);

        if (fillColor.a > 0) {
            // See if filled triangle is required
            glColor4fv (fillColor.v);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawArrays(GL_TRIANGLES,0,3);
        }
        if (strokeColor.a > 0) {
            // See if outline triangle is required
            glColor4fv (strokeColor.v);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawArrays(GL_TRIANGLES,0,3);
        }
        // deactivate vertex arrays after drawing
        glDisableClientState(GL_VERTEX_ARRAY);
    }
}