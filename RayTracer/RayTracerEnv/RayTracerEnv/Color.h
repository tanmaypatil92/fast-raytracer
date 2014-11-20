#ifndef _COLOR_H_
#define _COLOR_H_

class Color{
	public:
	double    red;	
	double    green;
	double    blue;
	double    alpha;
	Color();
	Color(double,double,double);
	Color(double,double,double,double);
	void addColor(Color);
	void avgColor(int);
	void colorCheck();
};

#endif