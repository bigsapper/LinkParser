 /***************************************************************************/
 /*                                                                         */
 /*       Copyright (C) 1991-1998  Daniel Sleator and Davy Temperley        */
 /*  See file "README" for information about commercial use of this system  */
 /*                                                                         */
 /***************************************************************************/

#include "link-includes.h"
#include <time.h>

#if !defined(WIN32)
   #include <sys/time.h>
   #include <sys/resource.h>
#endif

#if defined(__linux__)
/* based on reading the man page for getrusage on linux, I inferred that
   I needed to include this.  However it doesn't seem to be necessary */
   #include <unistd.h>
#endif

#if defined(__hpux__)
  #include <sys/syscall.h>
  int syscall(int, int, struct rusage *rusage);  /* can't find
						    the prototype for this */
  #define getrusage(a, b)  syscall(SYS_GETRUSAGE, (a), (b))
#endif /* __hpux__ */

#if defined(__sun__)
int getrusage(int who, struct rusage *rusage);
/* Declaration missing from sys/resource.h in sun operating systems (?) */
#endif /* __sun__ */

static double current_usage_time(void) {
/* returns the current usage time clock in seconds */
#if !defined(WIN32)
    struct rusage u;
    getrusage (RUSAGE_SELF, &u);
    return (u.ru_utime.tv_sec + ((double) u.ru_utime.tv_usec) / 1000000.0);
#else
    return ((double) clock())/CLOCKS_PER_SEC;
#endif
}

void print_time(Parse_Options opts, char * s) {
    resources_print_time(opts->verbosity, opts->resources, s);
}

void print_total_time(Parse_Options opts) {
    resources_print_total_time(opts->verbosity, opts->resources);
}

void print_total_space(Parse_Options opts) {
    resources_print_total_space(opts->verbosity, opts->resources);
}

Resources resources_create() {
    Resources r;

    r = (Resources) xalloc(sizeof(struct Resources_s));
    r->max_parse_time = MAX_PARSE_TIME_DEFAULT;
    r->when_created = current_usage_time();
    r->when_last_called = current_usage_time();
    r->time_when_parse_started = current_usage_time();
    r->space_when_parse_started = space_in_use;
    r->max_memory = MAX_MEMORY_DEFAULT;
    r->cumulative_time = 0;
    r->memory_exhausted = FALSE;
    r->timer_expired = FALSE;

    return r;
}

void resources_delete(Resources r) {
    xfree(r, sizeof(struct Resources_s));
}

void resources_reset(Resources r) {
    r->when_last_called = r->time_when_parse_started = current_usage_time();
    r->space_when_parse_started = space_in_use;
    r->timer_expired = FALSE;
    r->memory_exhausted = FALSE;
}

void resources_reset_time(Resources r) {
    r->when_last_called = r->time_when_parse_started = current_usage_time();
}

void resources_reset_space(Resources r) {
    r->space_when_parse_started = space_in_use;
}

int resources_exhausted(Resources r) {
    if (resources_timer_expired(r)) {
	r->timer_expired = TRUE;
    }
    if (resources_memory_exhausted(r)) {
	r->memory_exhausted = TRUE;
    }
    return (r->timer_expired || r->memory_exhausted);
}

int resources_timer_expired(Resources r) {
    if (r->max_parse_time == MAX_PARSE_TIME_DEFAULT) return 0;
    else return (r->timer_expired || (current_usage_time() - r->time_when_parse_started > r->max_parse_time));
}

int resources_memory_exhausted(Resources r) {
    if (r->max_memory == MAX_MEMORY_DEFAULT) return 0;
    else return (r->memory_exhausted || (space_in_use > r->max_memory));
}

void resources_print_time(int verbosity, Resources r, char * s) {
/* print out the cpu ticks since this was last called */
    double new_t;
    new_t = current_usage_time();
    if (verbosity > 1) {
	printf("++++");
	left_print_string(stdout, s, "                                           ");
	printf("%7.2f seconds\n", new_t - r->when_last_called);
    }
    r->when_last_called = new_t;
}

void resources_print_total_time(int verbosity, Resources r) {
/* print out the cpu ticks since this was last called */
    double new_t;
    new_t = current_usage_time();
    r->cumulative_time += (new_t - r->time_when_parse_started) ;
    if (verbosity > 0) {
        printf("++++");
	left_print_string(stdout, "Time",
			  "                                           ");
	printf("%7.2f seconds (%.2f total)\n", 
	       new_t - r->time_when_parse_started, r->cumulative_time);
    }
    r->time_when_parse_started = new_t;
}

void resources_print_total_space(int verbosity, Resources r) {
    if (verbosity > 1) {
       printf("++++");
      left_print_string(stdout, "Total space",
			"                                            ");
      printf("%d bytes (%d max)\n", space_in_use, max_space_in_use);
    }
}

