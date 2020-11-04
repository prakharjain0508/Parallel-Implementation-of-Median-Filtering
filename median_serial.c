/*******************************************************************************
* Program: median.c
* Purpose: This program will apply a median filter to an image with a user
* specified window size.
*
* gcc -o median median.c imageio.c
*
* Name: Prakhar Jain, MATH 424
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "imageio.h"

void insertion_sort(unsigned char *item, int count);

int main(int argc, char *argv[])
{
   char *inputfilename1=NULL, *inputfilename2=NULL, *inputfilename3=NULL, *inputfilename4=NULL, *inputfilename5=NULL;
   char *outputfilename = NULL;
   int n=0,c, rows, cols, i, flag=0;
   unsigned char **image1=NULL, **image2=NULL, **image3=NULL, **image4=NULL, **image5=NULL, **filteredimage1 = NULL, **filteredimage2 = NULL, **filteredimage3 = NULL, **filteredimage4 = NULL, **filteredimage5 = NULL;
   time_t t0, t1;
   clock_t starttime, endtime;

   void median_filter(unsigned char **image, int rows, int cols, int n, unsigned char ***filteredimage);

   /****************************************************************************
   * Get the command line parameters.
   ****************************************************************************/
   for(i=1;i<argc;i++)
   {
      if(strcmp(argv[i], "-n") == 0)
      {
        n = atoi(argv[i+1]);
        i++;
      }
      else
      {
         if(inputfilename1 == NULL) inputfilename1 = argv[i];
         else if(inputfilename2 == NULL) inputfilename2 = argv[i];
         else if(inputfilename3 == NULL) inputfilename3 = argv[i];
         else if(inputfilename4 == NULL) inputfilename4 = argv[i];
         else if(inputfilename5 == NULL) inputfilename5 = argv[i];
         else if(outputfilename == NULL) outputfilename = argv[i];
         else outputfilename = argv[i];
      }
   }
   if((n <= 0) || (inputfilename1==NULL) ||  (inputfilename2==NULL) ||  (inputfilename3==NULL) ||  (inputfilename4==NULL) || (inputfilename5==NULL) ||(outputfilename==NULL))
   {

      printf("\n********************************************************************************\n");
      printf("This program will apply a median filter to an image. You need to specify the\n");
      printf("size of the window to use in median filtering the image (i.e. -n 5), \n");
      printf("the input image to process (i.e. enbnoisy.pgm)\n");
      printf("and the name of an output file in which the median filtered image will be\n");
      printf("written. Thus, you could run the program as follows:\n");
      printf("\n");
      printf(" median -n 5 Input_Image1.pgm Input_Image2.pgm Input_Image3.pgm Input_Image4.pgm Input_Image5.pgm medianfiltered.pgm\n");
      printf("\n");
      printf("********************************************************************************\n");

      fprintf(stderr, "\n<USAGE> median -n # inputPGMfile outputPGMfile\n\n");
      exit(1);
   }

   printf("For Input_Image1.pgm - 1\n");
   printf("For Input_Image2.pgm - 2\n");
   printf("For Input_Image3.pgm - 3\n");
   printf("For Input_Image4.pgm - 4\n");
   printf("For Input_Image5.pgm - 5\n");
   printf("Choose which image you want to smooth using median filter : ");
   scanf("%d", &c);
   printf("\n");
   if(c<1 || c>5)
   {
     fprintf(stderr, "\n<USAGE> No such image available. Choose between 1-5\n\n");
     exit(1);
   }

   /****************************************************************************
   * Read in the PGM image from the file.
   ****************************************************************************/
   switch(c)
   {
     case 1:
      {
        if(read_pgm_image(inputfilename1, &image1, &rows, &cols) == 0) exit(1);
        flag=1;
      }
      break;
     case 2:
      {
        if(read_pgm_image(inputfilename2, &image2, &rows, &cols) == 0) exit(1);
        flag=2;
      }
      break;
     case 3:
      {
        if(read_pgm_image(inputfilename3, &image3, &rows, &cols) == 0) exit(1);
        flag=3;
      }
      break;
     case 4:
      {
        if(read_pgm_image(inputfilename4, &image4, &rows, &cols) == 0) exit(1);
        flag=4;
      }
      break;
     case 5:
      {
        if(read_pgm_image(inputfilename5, &image5, &rows, &cols) == 0) exit(1);
        flag=5;
      }
      break;
   }

   /****************************************************************************
   * Median filter the image.
   ****************************************************************************/
  if(flag==1)
  {
    starttime = clock();
      median_filter(image1, rows, cols, n, &filteredimage1);
    endtime = clock();  }
  if(flag==2)
  {
    starttime = clock();
      median_filter(image2, rows, cols, n, &filteredimage2);
    endtime = clock();  }
  if(flag==3)
  {
    starttime = clock();
      median_filter(image3, rows, cols, n, &filteredimage3);
    endtime = clock();
  }
  if(flag==4)
  {
    starttime = clock();
      median_filter(image4, rows, cols, n, &filteredimage4);
    endtime = clock();  }
  if(flag==5)
  {
    starttime = clock();
      median_filter(image5, rows, cols, n, &filteredimage5);
    endtime = clock();
  }


   /****************************************************************************
   * Print out the size of window used to compute the median
   * and the time taken to do the median filtering.
   ****************************************************************************/
   printf("Window size = %d (You can increase the window size (only odd numbers) for more smoothing of the image) \nTime for filtering = %f \nCheck the local folder for the MedianFiltered_Image.pgm \n", n, (endtime - starttime)/ (float)CLOCKS_PER_SEC);
   printf("Check the local folder for the MedianFiltered_Image_Serial.pgm \n");

   /****************************************************************************
   * Write the filtered image out to a file.
   ****************************************************************************/
   if(flag==1)
   {
      if((write_pgm_image(outputfilename, filteredimage1, rows, cols, (unsigned char *)NULL, 255)) == 0)
        exit(1);
      free_image(image1, rows);
      free_image(filteredimage1, rows);
   }
  if(flag==2)
   {
      if((write_pgm_image(outputfilename, filteredimage2, rows, cols, (unsigned char *)NULL, 255)) == 0)
        exit(1);
      free_image(image2, rows);
      free_image(filteredimage2, rows);
   }
 if(flag==3)
   {
      if((write_pgm_image(outputfilename, filteredimage3, rows, cols, (unsigned char *)NULL, 255)) == 0)
        exit(1);
      free_image(image3, rows);
      free_image(filteredimage3, rows);
   }
 if(flag==4)
   {
      if((write_pgm_image(outputfilename, filteredimage4, rows, cols, (unsigned char *)NULL, 255)) == 0)
        exit(1);
      free_image(image4, rows);
      free_image(filteredimage4, rows);
   }
 if(flag==5)
   {
      if((write_pgm_image(outputfilename, filteredimage5, rows, cols, (unsigned char *)NULL, 255)) == 0)
        exit(1);
      free_image(image5, rows);
      free_image(filteredimage5, rows);
   }

}

/*******************************************************************************
* Function: median_filter
* Purpose: This function will median filter an image using an n x n window.
*******************************************************************************/
void median_filter(unsigned char **image, int rows, int cols, int n, unsigned char ***filteredimage)
{
   unsigned char *pixel_values=NULL;
   int r, c, rr, cc, p;

   /****************************************************************************
   * These are the function prototypes for the three functions students must
   * write in a file mysort.c.
   ****************************************************************************/

   /****************************************************************************
   * Allocate an array to store pixel values. There will be up to n x n pixel
   * values to sort at each pixel location in the image.
   ****************************************************************************/
   if((pixel_values = (unsigned char *) calloc(n*n, sizeof(unsigned char))) == NULL){
      fprintf(stderr, "Error allocating an array in median_filter().\n");
      exit(1);
   }

   /****************************************************************************
   * Allocate the filtered image.
   ****************************************************************************/
   if(((*filteredimage) = allocate_image(rows, cols)) == NULL) exit(1);

   /****************************************************************************
   * Scan through the image and compute the median of the local pixel values
   * at each pixel position.
   ****************************************************************************/
   for(r=0;r<rows;r++){
      for(c=0;c<cols;c++){
         p = 0;
         for(rr=(r-(n/2));rr<(r-(n/2)+n);rr++){
            for(cc=(c-(n/2));cc<(c-(n/2)+n);cc++){
               if((rr>=0)&&(rr<rows)&&(cc>=0)&&(cc<cols)){
                  pixel_values[p] = image[rr][cc];
                  p++;
	       }
            }
         }

         /*******************************************************************
         * Sort the array of pixels. Although there can be up
         * to n x n pixels in the array, there are actually only p values.
         *******************************************************************/
	insertion_sort(pixel_values, p);

         /**********************************************************************
         * Assign the median pixel value to the filtered image.
         **********************************************************************/
         (*filteredimage)[r][c] = pixel_values[p/2];
      }
   }

   free(pixel_values);
}

 void insertion_sort(unsigned char *item, int count)
   {
    int c,d,t;
    for (c = 1 ; c <= count; c++)
    {
      d = c;
      while ( d > 0 && item[d-1] > item[d])
      {
        t = item[d];
        item[d] = item[d-1];
        item[d-1] = t;
        d--;
      }
    }
  }
