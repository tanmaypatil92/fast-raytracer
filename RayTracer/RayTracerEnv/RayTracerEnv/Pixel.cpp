#include "Pixel.h"

Pixel::Pixel()
{
	pixelColor = Color(1.0,1.0,1.0);
	alpha = 1.0;
}
Pixel::Pixel(Color c)
{
	pixelColor = c;
	pixelColor.colorCheck();
}

void Pixel::setColor(Color newColor)
{
  this->pixelColor = newColor;
  pixelColor.colorCheck();
}