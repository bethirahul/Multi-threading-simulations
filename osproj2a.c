#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/* Static variables */
#define MAX_THREADS 512


void *calc_pi( void * ); // Threads call this function

/* Global variables */
int pts; 			// Sample Points
int t_hits; 		// Total Hits
int hit[ MAX_THREADS ];
int pts_per_thread; // Sample points per Thread
int num_threads;	// Number of Threads

/* MAIN function */
int main( int argc, char *argv[] )
{
	struct timespec t1, t2; 
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1); // Record Start Time
	
    /* Local variables */
    int            i;
    int            ret_val;					// return value of the main function
    pthread_t      p_thread[ MAX_THREADS ]; // Thread IDs
    pthread_attr_t attr;					// Thread attributes
    double         final_pi;				// To store final PI value
	double		   t_taken;					// Time taken to execute the program

    /* Initialize local variables */
    ret_val = 0;

    pthread_attr_init    ( &attr					   ); // Initialize attributes of Threads
    pthread_attr_setscope( &attr, PTHREAD_SCOPE_SYSTEM ); // Set scope for them

    pts		    = atoi( argv[1] ); // Input Sample points
    num_threads = atoi( argv[2] ); // Input Number of Threads

    t_hits         = 0;
    pts_per_thread = pts / num_threads; // Calculated Sample points per Thread

    for( i=0; i<num_threads; i++ )
    {
        hit[i] = i;
        pthread_create( &p_thread[i], &attr, calc_pi, (void *) &hit[i] ); // Creating Threads
    }

    for( i=0; i<num_threads; i++ )
    {
        pthread_join( p_thread[i], NULL ); // Waiting for each Thread to exit
        t_hits += hit[i]; // Calculating the total number of hits in the which fall in the circle
    }

    final_pi = 4.0 * (double) t_hits / ((double) (pts)); // PI formula to calculate its value based on the points hitting the circle

    printf( "Calculated PI = %lf\n", final_pi ); // Final PI value
	
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2); // Record final time taken
    t_taken = ((1000.0*t2.tv_sec) + (1e-6*t2.tv_nsec)) - ((1000.0*t1.tv_sec) + (1e-6*t1.tv_nsec)); // Calculate time taken in nanoseconds and correcting the 0.0000001 error
    printf("Time taken: %.2f ms\n", t_taken); // Display total time taken to execute

    /* return to calling environment */
    return( ret_val );
}

void *calc_pi( void *s ) // Threads call this function
{
    unsigned int	 seed;
			 int	 i;
			 int	*hit_ptr;
			 int	 l_hits;
			 double  x;
			 double  y;
			 double  r;
	
    hit_ptr =  (int *)s;
    seed    =        *hit_ptr;
    l_hits  =         0;
	r       = (double)RAND_MAX;

    for( i=0; i<pts_per_thread; i++ )
    {
        x  = ( (double)(rand_r( &seed )) / r ); // Take a random point (which actually is a sequence of numbers)
        y  = ( (double)(rand_r( &seed )) / r );
        if( ( ((x - 0.5)*(x - 0.5)) + ((y - 0.5)*(y - 0.5)) ) < 0.25 ) // Calculate if the point obtained is inside the circle.
        //if(  ( pow(x, 2) + pow(y, 2) ) < 0.25  )
        {
	        l_hits++; // increment hit if the point is inside the circle
        }
        seed *= i;
    }

   *hit_ptr = l_hits;
    pthread_exit(0);
}