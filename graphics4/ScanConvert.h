#ifndef SCAN_CONVERT_H
#define SCAN_CONVERT_H

#define ImageW 500
#define ImageH 500

struct color {
	float r, g, b;		// Color (R,G,B values)
};

// Clears framebuffer to black
void clearFramebuffer();

// Sets pixel x,y to the color RGB
void setFramebuffer(int x, int y, float R, float G, float B);

#endif
