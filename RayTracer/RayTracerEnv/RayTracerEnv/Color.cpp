#include "Color.h"

Color::Color()
{
	red   = 0.0;
	blue  = 0.0;
	green = 0.0;
	alpha = 1.0;
}
Color::Color(double r,double g,double b)
{
	red   = r;
	green = g;
	blue  = b;
	alpha = 1.0;
	colorCheck();
}
Color::Color(double r,double g,double b,double a)
{
	red   = r;
	green = g;
	blue  = b;
	alpha = a;
	colorCheck();
}
void Color::colorCheck()
{
	if(red   < 0.0) red   = 0.0;if(red   > 1.0) red   = 1.0;
	if(green < 0.0) green = 0.0;if(green > 1.0) green = 1.0;
	if(blue  < 0.0) blue  = 0.0;if(blue  > 1.0) blue  = 1.0;
}

void Color::addColor(Color c){
	red += c.red;
	blue += c.blue;
	green += c.green;
	alpha += c.alpha;
}

void Color::avgColor(int div){
	red /= div;
	green /=div;
	blue /=div;
	alpha /= div;
}