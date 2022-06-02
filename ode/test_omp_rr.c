/*

To compile, try variations on these::

gcc -I<path-to-headers) -fopenmp  test_omp_rr.c -L<path-to-lib> -lroadrunner_c_api -o test_omp_rr

gcc-11 -I<path-to-headers) -fopenmp  test_omp_rr.c -L<path-to-lib> -lroadrunner_c_api -o test_omp_rr

clang -Xpreprocessor -fopenmp -m64 -I<path-to-headers> -L<path-to-lib> -lroadrunner_c_api -L/usr/local/opt/libomp/lib -lomp  test_omp_rr.c -o test_omp_rr

export DYLD_LIBRARY_PATH=/Users/heiland/dev/roadrunner-osx-10.9-cp36m/lib

then try to run (assumes feedback.xml exists):
$ test_omp_rr 1 1


Additional docs:
http://sys-bio.github.io/roadrunner/c_api_docs/html/group__helper_routines.html
http://sys-bio.github.io/roadrunner/c_api_docs/html/rrc__api_8h.html
http://sys-bio.github.io/roadrunner/c_api_docs/html/group__initial_conditions.html

*/

#undef __cplusplus
#define STATIC_RRC
#include <stdio.h>
#include <stdlib.h>
#include "rrc_api.h"
#include "rrc_types.h"
#include "rrc_utilities.h"

#include <omp.h>

int main (int argc, char *argv[]) 
{
  if(argc < 3) {
//    printf("Provide args: <name of sbml file> <num cells> <num threads>\n");
    printf("Provide args:  <num cells> <num threads>\n");
    exit(1);
  }
  int ncells = atoi(argv[1]);
  printf("------  # of cells = %d\n\n", ncells);
  int nthreads = atoi(argv[2]);
  printf("------  # of threads = %d\n\n", nthreads);

  omp_set_num_threads(nthreads);


//   #pragma omp parallel for
  #pragma omp parallel 
  {
     RRHandle rrHandle;
//     RRHandle rrHandleArray[42];
     RRVectorPtr vptr;
     RRCDataPtr result;  // start time, end time, and number of points
     static int idx_oxygen = 3;
     
//  #pragma omp parallel for private(rrHandle, vptr, result, idx_oxygen)
  #pragma omp parallel for 
  for (int icell=0; icell<ncells; icell++)
  {
   printf ("------------  cell %d  ---------------\n", icell);

   // printf ("Starting Test Program %s\n", argv[0]);
   rrHandle = createRRInstance();
   printf("-- past createRRInstance()");
   if (!loadSBML (rrHandle, "feedback.xml")) {
      printf ("Error while loading SBML file\n");
      printf ("Error message: %s\n", getLastError());
      getchar ();
      exit (0);
   }
   printf("-- past loadSBML()");

   int r = getNumberOfReactions(rrHandle);
   printf("-- past getNumberOfReactions");
   int m = getNumberOfFloatingSpecies(rrHandle);
   int b = getNumberOfBoundarySpecies(rrHandle);
   int p = getNumberOfGlobalParameters(rrHandle);
   int c = getNumberOfCompartments(rrHandle);

   printf ("Number of reactions = %d\n", r);
   printf ("Number of floating species = %d\n", m);  // 4
   printf ("Number of boundary species = %d\n", b);  // 0
   printf ("Number of compartments = %d\n", c);  // 1

   printf ("Floating species names:\n");
   printf ("-----------------------\n");
   printf("%s\n\n",stringArrayToString(getFloatingSpeciesIds(rrHandle)));

/*   rrc_api.h:C_DECL_SPEC RRVectorPtr rrcCallConv getFloatingSpeciesConcentrations(RRHandle handle); 
rrc_api.h:C_DECL_SPEC bool rrcCallConv setFloatingSpeciesInitialConcentrations (RRHandle handle, const RRVectorPtr vec);
*/
   printf ("Floating species conc:\n");
   printf ("-------------------------------\n");
   // RRVectorPtr vptr = getFloatingSpeciesConcentrations(rrHandle);
   vptr = getFloatingSpeciesConcentrations(rrHandle);

/*
rrc_types.h:
typedef struct RRVector
{
    int             Count;
    double*         Data; 
} *RRVectorPtr; 
*/
   printf("%d\n",vptr->Count);
   for (int idx=0; idx<vptr->Count; idx++)
      printf("%d %f\n",idx, vptr->Data[idx]);

   // idx_oxygen = 3;
   vptr->Data[idx_oxygen] += 0.1;
   setFloatingSpeciesConcentrations(rrHandle, vptr);

   vptr = getFloatingSpeciesConcentrations(rrHandle);
   printf("%d\n",vptr->Count);
   for (int idx=0; idx<vptr->Count; idx++)
      printf("%d %f\n",idx, vptr->Data[idx]);

   result = simulateEx (rrHandle, 0, 10, 10);  // start time, end time, and number of points
   int index = 0;
   // Print out column headers... typically time and species.
   for (int col = 0; col < result->CSize; col++)
   {
      printf ("%10s", result->ColumnHeaders[index++]);
      if (col < result->CSize - 1)
      {
         printf ("\t");
      }
   }
   printf ("\n");

   index = 0;
   // Print out the data
   for (int row = 0; row < result->RSize; row++)
   {
      for (int col = 0; col < result->CSize; col++)
      {
         printf ("%10f", result->Data[index++]);
         if (col < result->CSize -1)
         {
            printf ("\t");
         }
      }
   printf ("\n");
   }

   //Cleanup
   freeRRCData (result);
   freeRRInstance (rrHandle);
   // getchar ();

  }  // end for
  }  // end pragma

   exit (0);
}
