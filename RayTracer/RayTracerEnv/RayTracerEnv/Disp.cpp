#include "Gz.h"
#include "Disp.h"
#include <iostream>
#include <stdio.h>

#define DEBUG_J 367
#define DEBUG_I 244

#define DEBUG_THIS 0
int saveImage(Pixel *pixels)
{
	int width  = XRES;
	int height = YRES;
	// Writing to tga file, borrowed from PaulBrooke's website //
  
	FILE *fptr = fopen(outfile, "w");
	if (fptr == NULL) {
      fprintf(stderr,"Failed to open outputfile\n");
      return FAILURE;
   }
   putc(0,fptr);
   putc(0,fptr);
   putc(2,fptr);                         /* uncompressed RGB */
  
   /* Field : color map, 5 bytes */
   putc(0,fptr);
   putc(0,fptr); putc(0,fptr); 
   putc(0,fptr); putc(0,fptr);
   /* X origin */
   putc(0,fptr); putc(0,fptr);          /* X Origin */
   putc(0,fptr); putc(0,fptr);          /* Y Origin */
  
   putc((width & 0xff),fptr);
   putc((width & 0xff00) >> 8,fptr);
  
   putc((height & 0xff),fptr);
   putc((height & 0xff00) >> 8,fptr);
  
   putc(24,fptr);                        /* 24 bit bitmap */
   putc(0,fptr);
   // char r, g, b;
   for (int j = 0; j < height; j++)
   {
	   for (int i = 0; i < width; i++)
	   {
       char r = pixels[i + j*width].pixelColor.red * 255;
       char g = pixels[i + j*width].pixelColor.green * 255;
       char b = pixels[i + j*width].pixelColor.blue * 255;
     
       if(i == 90 && j == 260)
       {
         std::cout<<"BP"<<std::endl;
       }
       
       putc(b, fptr);
       putc(g, fptr);
		   putc(r, fptr);
	
	   }
   }
   fclose(fptr);
   return SUCCESS;
}

