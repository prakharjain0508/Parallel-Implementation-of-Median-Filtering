/*******************************************************************************
* Program: median.c
* Purpose: This program will apply a median filter to an image with a user
* specified window size.
*
* Compile & Run : ./median_parallel.sh
* Name: Prakhar Jain, MATH 424
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>
#include "imageio.h"

void insertion_sort(unsigned char *item, int count);

int main(int argc, char *argv[])
{
   char *inputfilename1=NULL, *inputfilename2=NULL, *inputfilename3=NULL, *inputfilename4=NULL, *inputfilename5=NULL;
   char *outputfilename = NULL;
   int n=0, rows, cols, i, c, flag=0;
   unsigned char **image1=NULL, **image2=NULL, **image3=NULL, **image4=NULL, **image5=NULL, **filteredimage1 = NULL, **filteredimage2 = NULL, **filteredimage3 = NULL, **filteredimage4 = NULL, **filteredimage5 = NULL;
   int thread_count,t;
   double t1,t2;
   void median_filter(unsigned char **image, int rows, int cols, int n, unsigned char ***filteredimage, int thread_count);

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
         else thread_count = strtol(argv[i], NULL, 10);
      }
   }

   if((n <= 0) || (inputfilename1==NULL) ||  (inputfilename2==NULL) ||  (inputfilename3==NULL) ||  (inputfilename4==NULL) || (inputfilename5==NULL) ||(outputfilename==NULL) || (thread_count <= 0))
   {

      printf("\n********************************************************************************\n");
      printf("This program will apply a median filter to an image. You need to specify the\n");
      printf("size of the window to use in median filtering the image (i.e. -n 5), \n");
      printf("the 5-input images to proces, the name of an output file\n");
      printf("in which the median filtered image will be written and the number of threads.\n");
      printf("Thus, you could run the program as follows:\n");
      printf("\n");
      printf("Ex: median -n 5 Input_Image1.pgm Input_Image2.pgm Input_Image3.pgm Input_Image4.pgm Input_Image5.pgm medianfiltered.pgm 8\n");
      printf("\n");
      printf("********************************************************************************\n");

      fprintf(stderr, "\n<USAGE> median -n # inputPGMfile outputPGMfile #of_threads\n\n");
      exit(1);
   }

   /****************************************************************************
   * printf("Inputfilename = %s\n", inputfilename);
   * printf("Outputfilename = %s\n", outputfilename);
   * printf("n = %d\n", n);
   ****************************************************************************/


 /****************************************************************************
   * Read in the PGM image from the file.
   ****************************************************************************/

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
    t1= omp_get_wtime();
      median_filter(image1, rows, cols, n, &filteredimage1, thread_count);
    t2= omp_get_wtime();
  }
  if(flag==2)
  {
    t1= omp_get_wtime();
      median_filter(image2, rows, cols, n, &filteredimage2, thread_count);
    t2= omp_get_wtime();
  }
  if(flag==3)
  {
    t1= omp_get_wtime();
      median_filter(image3, rows, cols, n, &filteredimage3, thread_count);
    t2= omp_get_wtime();
  }
  if(flag==4)
  {
    t1= omp_get_wtime();
      median_filter(image4, rows, cols, n, &filteredimage4, thread_count);
    t2= omp_get_wtime();
  }
  if(flag==5)
  {
    t1= omp_get_wtime();
      median_filter(image5, rows, cols, n, &filteredimage5, thread_count);
    t2= omp_get_wtime();
  }

   /****************************************************************************
   * Print out the size of window used to compute the median
   * and the time taken to do the median filtering.
   ****************************************************************************/
   printf("Window size = %d (You can increase the window size (only odd numbers) for more smoothing of the image) \nTime taken by entire parallel region for median filtering = %lf \n", n, (t2-t1));

   printf("Check the local folder for the MedianFiltered_Image_Parallel.pgm \n");


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
void median_filter(unsigned char **image, int rows, int cols, int n, unsigned char ***filteredimage, int thread_count)
{
   unsigned char *pixel_values=NULL;
   int r,t, c, rr, cc, p,my_rank;
   double t3,t4;
   double *time = (double*) malloc((thread_count)*sizeof(double));
   for(t=0;t<thread_count;t++)
     time[t] = 0.0;

   /****************************************************************************
   * These are the function prototypes for the three functions students must
   * write in a file mysort.c.
   ****************************************************************************/

   /****************************************************************************
   * Allocate an array to store pixel values. There will be up to n x n pixel
   * values to sort at each pixel location in the image.
   ****************************************************************************/
   if((pixel_values = (unsigned char *) malloc((n*n) * sizeof(unsigned char))) == NULL){
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
    # pragma omp parallel for collapse(2) ordered num_threads(thread_count) default(none) shared(image, filteredimage, rows,cols,n,time) firstprivate(pixel_values) private(my_rank,t3,t4,p,rr,cc) schedule(static,1)
    for(r=0;r<rows;r++)
    {
      for(c=0;c<cols;c++)
      {
         my_rank = omp_get_thread_num();
         t3 = omp_get_wtime();
         p=0;
         for(rr=(r-(n/2));rr<(r-(n/2)+n);rr++)
         {
           for(cc=(c-(n/2));cc<(c-(n/2)+n);cc++)
           {
              if((rr>=0)&&(rr<rows)&&(cc>=0)&&(cc<cols))
              {
                 pixel_values[p] = image[rr][cc];
                 p++;
	      }
           }
         }

         /*******************************************************************
         * Sort the array of pixels. Although there can be up
         * to n x n pixels in the array, there are actually only p values.
         *******************************************************************/
        #pragma omp critical
        {
  	 insertion_sort(pixel_values, p);
        }
         /**********************************************************************
         * Assign the median pixel value to the filtered image.
         **********************************************************************/
         #pragma omp ordered
         {
           (*filteredimage)[r][c] = pixel_values[p/2];
         }
         t4= omp_get_wtime();
         time[my_rank]+=(t4-t3);
      }

   }
   for(t=0;t<thread_count;t++)
     printf("Time taken by thread %d to compute its chunk = %lf \n",t,time[t]);
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
