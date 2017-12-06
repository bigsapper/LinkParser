#include <stdio.h>

/*
For a debug version, compile the macros to log construction
and destruction to a file called "objlog.txt". 
*/

#ifdef _DEBUG
	
	#define CTOR_COUNT_MACRO { \
		FILE *fp = fopen ("objlog.txt", "a+") ;\
		fprintf (fp, "C %x %s\n", this, __FILE__) ;\
		fclose (fp) ;} 

	#define DTOR_COUNT_MACRO {\
		FILE *fp = fopen ("objlog.txt", "a+") ;\
		fprintf (fp, "D %x %s\n", this, __FILE__) ;\
		fclose (fp) ;} 

/*
For a release version, compile the macros away
to nothing.
*/

#else

	#define CTOR_COUNT_MACRO 
	#define DTOR_COUNT_MACRO  

#endif