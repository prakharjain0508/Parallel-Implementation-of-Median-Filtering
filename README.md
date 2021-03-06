# PARALLEL IMPLEMENTATION OF MEDIAN FILTERING

## INTRODUCTION
Image filters can be classified as linear or nonlinear. Linear filters are also known as convolution filters as they can be represented using a matrix multiplication. Thresholding and image equalization are examples of nonlinear operations, as is the median filter. Median filtering is a nonlinear operation often used in image processing to reduce "salt and pepper" noise. A median filter is more effective than convolution when the goal is to simultaneously reduce noise and preserve the edges in an image.

In median filtering, the noisy pixel considered is replaced by the median value of the neighboring pixels. The number of neighboring pixels depend on the size of the mask. Typically, a (3×3) mask is the smallest mask that can be used. Other than this, mask sizes of (5×5), (7×7), (9×9) and so on can also be used. Thus, the median filter works by sliding through the image pixel by pixel and replacing them by the median value obtained among the neighboring pixels. The median is calculated by first sorting the values of all the pixels in the increasing order of magnitude and then picking up the middle value from this sorted array of pixel values. This is one of the reasons why the size of the mask is always an odd multiple.

An image consists of millions of pixels and better the quality of the image, more the number of pixels. Processing each and every pixel is a time-consuming task and if done sequentially, i.e. for every mask, sorting the pixel values and then replacing the considered pixel with the median value ultimately results in slow processing of the image. This time is inversely proportional to the size of the mask. This means that as the size of the mask decreases, the processing time increases.
Comparatively, parallelizing the processing task can help reduce the processing time of the image considerably. The serial program for the above implementation of the median filter has been constructed and is provided later in this report. Looking ahead, I will parallelize the deduced serial code in OpenMP. This will include dividing the iterations pertaining to the sorting the pixel values and finding the median among them for every sliding window.

## WORKED OUT EXAMPLE
Given below is an example of how Median Filtering is done on an image. Consider the matrix given below as the part of the image. Here a (3 x 3) window is used to filter the given image. For altering the boundary elements of the image, their duplicates are considered at the respective edge. The elements or the pixel values within the window size are sorted and the considered pixel is replaced with the median value.
![Median Filtering](http://what-when-how.com/wp-content/uploads/2011/09/tmp6198.png)

## THE SELECTED OPENMP CLAUSES

**# pragma omp parallel for collapse(2) ordered num_threads(thread_count) default(none) shared(image, filteredimage, rows, cols, n, time) firstprivate(pixel_values) private(my_rank, t3, t4, p, rr, cc) schedule(static,1)**

The above schedule clause which is ‘static’ with chunk size 1 is selected because it has the best performance among all the other scheduling techniques which included dynamic, guided, auto and runtime. Along with running the program with different schedules, I also tried different schedules with different chunk sizes, viz. 1, 4, 8, 16 and default chunk sizes.

For static schedule, the default value of the chunk size is the number of iterations divided by the number of threads i.e. 𝑁𝑢𝑚𝑏𝑒𝑟 𝑜𝑓 𝑖𝑡𝑒𝑟𝑎𝑡𝑖𝑜𝑛𝑠/𝑁𝑢𝑚𝑏𝑒𝑟 𝑜𝑓 𝑡ℎ𝑟𝑒𝑎𝑑𝑠. For dynamic and guided schedules, the default value for the chunk size is 1. As for auto and runtime schedules, there is no chunk size. The compiler allocates the threads at runtime.
As the section of the code that I have parallelized contains ‘for’ loops, I have used ‘parallel for’ construct for parallelization. I have used an additional collapse clause which I will mention later. Another construct I have used is the ‘ordered’ construct which is used to sequentialize and orders the execution of ordered regions. Unordered populating of the rows and columns with the filtered pixel values would have distorted the image. Thus, I have used the ‘ordered’ construct while populating the rows and columns of the output image.

The scope of the variables was also defined as for parallelization, some variables needed to be shared, private and firstprivate. The variables that were shared are {image, filteredimage, rows, cols, n, time}. The variable ‘image’ is a 2D vector which contains the input image. As each thread is only reading the image, there is no problem of race conditions between the threads. The ‘filteredimage’ variable is a pointer to a 2D vector and points to the output image. It is shared amonf the thread because it is the output image and every thread has to populate the rows and columns of it by the pixels they have computed individually. The variables, ‘rows’ and ‘columns’ contains the number of rows and columns of the input image passed and ‘n’ is the size of the window/mask that performs the median filtering over (n x n) area in the image. These variables are also shared among the threads because the threads only have to read them and so there is no problem of race conditions. The last shared variable is ‘time’ which is an array used to store the time required by each thread to compute their own chunk. As each thread stores their time in it based on their rank, this variable is also shared.

The variables that were private to each thread are {pixel_values, my_rank, t3, t4, p, rr, cc}. The variable ‘pixel_values’ is firstprivate because it needed to be initialized with the value that it encounters in the previous construct. If it would be private, every time the thread returned after an iteration, the value of pixel_values would be zero which we do not want. Other variables like ‘my_rank’, ‘t3’ and ‘t4’ relate to an individual thread which is reason for them to be private. Variables ‘p’, ‘rr’ and ‘cc’ are used by each thread to compute their own chunk and so are also private.

## RESULTS
The input and output images for both serial and parallel execution are of .pgm format which is a Portable Grayscale Map. A PGM image represents a grayscale graphic image. There are many pseudo-PGM formats in use where everything is as specified herein except for the meaning of individual pixel values. For most purposes, a PGM image can just be thought of an array of arbitrary integers, and all the programs in the world that think they're processing a grayscale image can easily be tricked into processing something else. 
### Data Set 1: Input_Image1.pgm (18,994 KB)
Speedup = Tserial / Tparallel
        = 160.820007 / 59.778781
        ~ 2.69

Efficiency = Speedup / No.of threads
           = 2.69 / 16
           = 16.8%
           
### Data Set 2: Input_Image2.pgm (23,438 KB)
Speedup = Tserial / Tparallel
        = 204.369995 / 74.463772
        ~ 2.74

Efficiency = Speedup / No.of threads
           = 2.74 / 16 
           = 17.12%

## REFERENCES
[1] https://www.mathworks.com/help/images/ref/medfilt2.html

[2] https://www.cs.auckland.ac.nz/courses/compsci373s1c/PatricesLectures/Image%20Filtering_2up.pdf

[3] Youlian Zhu, Cheng Huang, An Improved Median Filtering Algorithm for Image Noise Reduction, Physics Procedia, Volume 25, 2012, Pages 609-616, ISSN 1875-3892

[4] https://www.cs.cmu.edu/~eugene/teach/algs00a/progs/

[5] https://www.youtube.com/watch?v=APkdYObUJRU

[6] Data-set source: https://unsplash.com/search/photos/4k Converted .jpg to .pgm image from: https://convertio.co/jpg-pgm/

[7] https://www.openmp.org/wp-content/uploads/openmp-4.5.pdf

[8] https://www.openmp.org/wp-content/uploads/openmp-examples-4.5.0.pdf

