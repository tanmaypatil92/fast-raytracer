#include "Gz.h"
#include "Disp.h"
#include <iostream>
#include <stdio.h>

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
   putc(0,fptr);
   putc(0,fptr); putc(0,fptr); 
   putc(0,fptr); putc(0,fptr); 
   putc(0,fptr); putc(0,fptr);          /* X Origin */
   putc(0,fptr); putc(0,fptr);          /* Y Origin */
   putc((width & 0xff),fptr);
   putc((width & 0xff00) >> 8,fptr);
   putc((height & 0xff),fptr);
   putc((height & 0xff00) >> 8,fptr);
   putc(24,fptr);                        /* 24 bit bitmap */
   putc(0,fptr);
   /* 
   for(int x = 0; x < width*height; x++) // not sure why it has to be widthXheight but iterating through
   {                                     // width and height seperately does not work!! 

			putc(pixels[x].pixelColor.blue  * 255, fptr);
			putc(pixels[x].pixelColor.green * 255, fptr);
			putc(pixels[x].pixelColor.red   * 255, fptr);

   } */

   for (int j = 0; j < height; j++)
   {
	   for (int i = 0; i < width; i++)
	   {
		   putc(pixels[i + j*width].pixelColor.blue * 255, fptr);
		   putc(pixels[i + j*width].pixelColor.green * 255, fptr);
		   putc(pixels[i + j*width].pixelColor.red * 255, fptr);
	   }
   }
   fclose(fptr);
   return SUCCESS;
}

