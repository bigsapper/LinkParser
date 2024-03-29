#include "link-includes.h"

static struct {
    int verbosity;
    int timeout;
    int memory;
    int linkage_limit;
    int null_block;
    int islands_ok;
    int short_length;
    int batch_mode;  
    int panic_mode;  
    int allow_null;
    int echo_on; 
    int screen_width;
    int display_on;
    int display_postscript;
    int display_bad;
    int display_links;
    int display_walls;
    int display_union;
} local;

typedef struct {
    char * string;
    int    isboolean;
    char * description;
    int  * p;
} Switch;

Switch default_switches[] = {
    {"verbosity",    0, "Level of detail in output",        &local.verbosity},
    {"timeout",      0, "Abort parsing after this long",    &local.timeout},
    {"memory",       0, "Max memory allowed",               &local.memory},
    {"limit",        0, "The maximum linkages processed",   &local.linkage_limit},
    {"null-block",   0, "Size of blocks with null cost 1",  &local.null_block},
    {"islands-ok",   1, "Use of null-linked islands",       &local.islands_ok},
    {"short",        0, "Max length of short links",        &local.short_length},    
    {"batch",        1, "Batch mode",                       &local.batch_mode},
    {"panic",        1, "Use of \"panic mode\"",            &local.panic_mode},
    {"null",         1, "Null links",                       &local.allow_null},
    {"width",        0, "The width of the display",         &local.screen_width},
    {"echo",         1, "Echoing of input sentence",        &local.echo_on},
    {"graphics",     1, "Graphical display of linkage",     &local.display_on},
    {"postscript",   1, "Generate postscript output",       &local.display_postscript},
    {"bad",          1, "Display of bad linkages",          &local.display_bad},
    {"links",        1, "Showing of complete link data",    &local.display_links},
    {"walls",        1, "Showing of wall words",            &local.display_walls},
    {"union",        1, "Showing of 'union' linkage",       &local.display_union},
    {NULL,           1,  NULL,                              NULL}
};

struct {char * s; char * str;} user_command[] = {
    {"variables",    "List user-settable variables and their functions"},
    {"help",         "List the commands and what they do"},
    {NULL,           NULL}
};

void clean_up_string(char * s) {
    /* gets rid of all the white space in the string s.  Changes s */
    char * x, * y;
    y = x = s;
    while(*x != '\0') {
	if (!isspace(*x)) {
	    *y = *x; x++; y++;
	} else {
	    x++;
	}
    }
    *y = '\0';
}

int is_numerical_rhs(char *s) {
    /* return TRUE if s points to a number:
     optional + or - followed by 1 or more
     digits.
     */
    if (*s=='+' || *s == '-') s++;
    if (*s == '\0') return FALSE;
    for (; *s != '\0'; s++) if (!isdigit(*s)) return FALSE;
    return TRUE;
}

void x_issue_special_command(char * line, Parse_Options opts, Dictionary dict) {
    char *s, myline[1000], *x, *y;
    int i, count, j, k;
    Switch * as = default_switches;

    strncpy(myline, line, sizeof(myline));
    myline[sizeof(myline)-1] = '\0';
    clean_up_string(myline);

    s = myline;
    j = k = -1;
    count = 0;
    for (i=0; as[i].string != NULL; i++) {
	if (as[i].isboolean && strncasecmp(s, as[i].string, strlen(s)) == 0) {
	    count++;
	    j = i;
	}
    }
    for (i=0; user_command[i].s != NULL; i++) {
	if (strncasecmp(s, user_command[i].s, strlen(s)) == 0) {
	    count++;
	    k = i;
	}
    }

    if (count > 1) {
	printf("Ambiguous command.  Type \"!help\" or \"!variables\"\n");
	return;
    } 
    else if (count == 1) {
	if (j >= 0) {
	    *as[j].p = !(*as[j].p);
	    printf("%s turned %s.\n", as[j].description, (*as[j].p)? "on" : "off");
	    return;
	} 
	else {
	    /* replace the abbreviated command by the full one */
	    strcpy(s, user_command[k].s);  
	}
    }



    if (strcmp(s, "variables")==0) {
	printf(" Variable     Controls                                      Value\n");
	printf(" --------     --------                                      -----\n");
	for (i=0; as[i].string != NULL; i++) {
	    printf(" ");
	    left_print_string(stdout, as[i].string, "             ");
	    left_print_string(stdout, as[i].description, "                                              ");
	    printf("%5d", *as[i].p);
	    if (as[i].isboolean) {
		if (*as[i].p) printf(" (On)"); else printf(" (Off)");
	    }
	    printf("\n");
	}
	printf("\n");
	printf("Toggle a boolean variable as in \"!batch\"; ");
	printf("set a variable as in \"!width=100\".\n");
	return;
    }
    if (strcmp(s, "help")==0) {
	printf("Special commands always begin with \"!\".  Command and variable names\n");
	printf("can be abbreviated.  Here is a list of the commands:\n\n");
	for (i=0; user_command[i].s != NULL; i++) {
	    printf(" !");
	    left_print_string(stdout, user_command[i].s, "                  ");
	    left_print_string(stdout, user_command[i].str, "                                                    ");
	    printf("\n");
	}
	printf(" !!<string>         Print all the dictionary words matching <string>.\n");
	printf("                    Also print the number of disjuncts of each.\n");
	printf("\n");
	printf(" !<var>             Toggle the specified boolean variable.\n");
	printf(" !<var>=<val>       Assign that value to that variable.\n");
	return;
    }

    if(s[0] == '!') {
	dict_display_word_info(dict, s+1);
	return;
    }
    
    /* test here for an equation */
    for (x=s; (*x != '=') && (*x != '\0') ; x++)
      ;
    if (*x == '=') {
	*x = '\0';
	y = x+1;
	x = s;
	/* now x is the first word and y is the rest */

	if (is_numerical_rhs(y)) {
	    for (i=0; as[i].string != NULL; i++) {
		if (strcmp(x, as[i].string) == 0) break;
	    }
	    if (as[i].string  == NULL) {
		printf("There is no user variable called \"%s\".\n", x);
	    } else {
		*(as[i].p) = atoi(y);
		printf("%s set to %d\n", x, atoi(y));
	    }
	    return;
	}
    }


    printf("I can't interpret \"%s\" as a command.  Try \"!help\".\n", myline);
}

void put_opts_in_local_vars(Parse_Options opts) {
    local.verbosity = parse_options_get_verbosity(opts);
    local.timeout = parse_options_get_max_parse_time(opts);;
    local.memory = parse_options_get_max_memory(opts);;
    local.linkage_limit = parse_options_get_linkage_limit(opts);
    local.null_block = parse_options_get_null_block(opts);
    local.islands_ok = parse_options_get_islands_ok(opts);
    local.short_length = parse_options_get_short_length(opts);
    local.echo_on = parse_options_get_echo_on(opts);
    local.batch_mode = parse_options_get_batch_mode(opts);
    local.panic_mode = parse_options_get_panic_mode(opts);
    local.screen_width = parse_options_get_screen_width(opts);
    local.allow_null = parse_options_get_allow_null(opts);
    local.screen_width = parse_options_get_screen_width(opts);
    local.display_on = parse_options_get_display_on(opts);
    local.display_postscript = parse_options_get_display_postscript(opts);
    local.display_bad = parse_options_get_display_bad(opts);
    local.display_links = parse_options_get_display_links(opts);
    local.display_walls = parse_options_get_display_walls(opts);
    local.display_union = parse_options_get_display_union(opts);
}

void put_local_vars_in_opts(Parse_Options opts) {
    parse_options_set_verbosity(opts, local.verbosity);
    parse_options_set_max_parse_time(opts, local.timeout);
    parse_options_set_max_memory(opts, local.memory);
    parse_options_set_linkage_limit(opts, local.linkage_limit);
    parse_options_set_null_block(opts, local.null_block);
    parse_options_set_islands_ok(opts, local.islands_ok);
    parse_options_set_short_length(opts, local.short_length);
    parse_options_set_echo_on(opts, local.echo_on);
    parse_options_set_batch_mode(opts, local.batch_mode);
    parse_options_set_panic_mode(opts, local.panic_mode);
    parse_options_set_screen_width(opts, local.screen_width);
    parse_options_set_allow_null(opts, local.allow_null);
    parse_options_set_screen_width(opts, local.screen_width);
    parse_options_set_display_on(opts, local.display_on);
    parse_options_set_display_postscript(opts, local.display_postscript);
    parse_options_set_display_bad(opts, local.display_bad);
    parse_options_set_display_links(opts, local.display_links);
    parse_options_set_display_walls(opts, local.display_walls);
    parse_options_set_display_union(opts, local.display_union);
}

void issue_special_command(char * line, Parse_Options opts, Dictionary dict) {
    put_opts_in_local_vars(opts);
    x_issue_special_command(line, opts, dict);
    put_local_vars_in_opts(opts);
}


