#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#define ImageW 500
#define ImageH 500
#include "vertex.h"
#include "scene.h"

struct color {
	float r, g, b;		// Color (R,G,B values)
};
const Vertex EYE_POINT = Vertex(ImageW/2,ImageW/2,-100); //eye pos is center of screen


//const Vertex VIEWPORT_BL = (0,0,-10);//bottom left
//const Vertex VIEWPORT_TR = (ImageW,ImageH,-10);// top right, orthogonal to z-dir

const int VIEWPORT_Z = -10;

Vertex eye_vector_to_pixel(int x, int y);
void ray_trace(Scene scene);

// Clears framebuffer to black
void clearFramebuffer();

// Sets pixel x,y to the color RGB
void setFramebuffer(int x, int y, float R, float G, float B);

#endif
