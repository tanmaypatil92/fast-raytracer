#ifndef _PIXEL_H_
#define _PIXEL_H_

#include "Color.h"

class Pixel {
	public:
	Color	  pixelColor;
	double    alpha;
	Pixel();
	Pixel(Color);
  void setColor(Color newColor);
};

#endif