 /***************************************************************************/
 /*                                                                         */
 /*       Copyright (C) 1991-1998  Daniel Sleator and Davy Temperley        */
 /*  See file "README" for information about commercial use of this system  */
 /*                                                                         */
 /***************************************************************************/

#include <stdarg.h>
#include "link-includes.h"

static int center[MAX_SENTENCE];
typedef struct String_s xString;
struct String_s {
    unsigned int allocated;  /* Unsigned so VC++ doesn't complain about comparisons */
    char * p;
    char * eos;
};

static int N_words_to_print;  /* version of N_words in this file for printing links */

char * trailer(int mode);
char * header(int mode);

/* This is a "safe" append function, used here to build up a link diagram
   incrementally.  Because the diagram is built up a few characters at
   a time, we keep around a pointer to the end of string to prevent
   the algorithm from being quadratic. */

xString * String_create() {
    xString * string;
    string = (xString *) exalloc(sizeof(xString));
    string->allocated = 1;
    string->p = (char *) exalloc(sizeof(char));
    string->p[0] = '\0';
    string->eos = string->p;
    return string;
}

int append_string(xString * string, char *fmt, ...) {
    char temp_string[1024];
    char * p;
    int new_size;
    va_list args;

    va_start(args, fmt);
    vsprintf(temp_string, fmt, args);
    va_end(args);

    if (string->allocated <= strlen(string->p)+strlen(temp_string)) {
	new_size = 2*string->allocated+strlen(temp_string)+1;
	p = exalloc(sizeof(char)*new_size);
	strcpy(p, string->p);
	strcat(p, temp_string);
	exfree(string->p, sizeof(char)*string->allocated);
	string->p = p;
	string->eos = strchr(p,'\0');
	string->allocated = new_size;
    }
    else {
	strcat(string->eos, temp_string);
	string->eos += strlen(temp_string);
    }

    return 0;
}

void set_centers(Linkage linkage, int print_word_0) {
    int i, len, tot;
    tot = 0;
    if (print_word_0) i=0; else i=1;
    for (; i<N_words_to_print; i++) {
	len = strlen(linkage->word[i]);
	center[i] = tot + (len/2);
	tot += len+1;
    }
}

/* the following are all for generating postscript */
static int link_heights[MAX_LINKS];
  /* tells the height of the links above the sentence */
static int row_starts[MAX_SENTENCE];
  /* the word beginning each row of the display */
static int N_rows;
  /* the number of rows */

void left_append_string(xString * string, char * s, char * t) {
/* prints s then prints the last |t|-|s| characters of t.
   if s is longer than t, it truncates s.
*/
    int i, j, k;
    j = strlen(t);
    k = strlen(s);
    for (i=0; i<j; i++) {
	if (i<k) {
	    append_string(string, "%c", s[i]);
	} else {
	    append_string(string, "%c", t[i]);
	}
    }
}



void print_a_link(xString * s, Linkage linkage, int link) {
    Sentence sent = linkage_get_sentence(linkage);
    Dictionary dict = sent->dict;
    int l, r;
    char *label, *llabel, *rlabel;

    l      = linkage_get_link_lword(linkage, link);
    r      = linkage_get_link_rword(linkage, link);
    label  = linkage_get_link_label(linkage, link);
    llabel = linkage_get_link_llabel(linkage, link);
    rlabel = linkage_get_link_rlabel(linkage, link);

    if ((l == 0) && dict->left_wall_defined) {
	left_append_string(s, LEFT_WALL_DISPLAY,"               ");
    } else if ((l == (linkage_get_num_words(linkage)-1)) && dict->right_wall_defined) {
	left_append_string(s, RIGHT_WALL_DISPLAY,"               ");
    } else {
	left_append_string(s, linkage_get_word(linkage, l), "               ");
    }
    left_append_string(s, llabel, "     ");
    append_string(s, "   <---");
    left_append_string(s, label, "-----");
    append_string(s, "->  ");
    left_append_string(s, rlabel, "     ");
    append_string(s, "     %s\r\n", linkage_get_word(linkage, r));
}


char * linkage_print_links_and_domains(Linkage linkage) {
    /* To the left of each link, print the sequence of domains it is in. */
    /* Printing a domain means printing its type                         */
    /* Takes info from pp_link_array and pp and chosen_words.            */
    int link, longest, j;
    int N_links = linkage_get_num_links(linkage);
    xString * s = String_create();
    char * links_string;
    char ** dname;

    longest = 0;
    for (link=0; link<N_links; link++) {
	if (linkage_get_link_lword(linkage, link) == -1) continue;
	if (linkage_get_link_num_domains(linkage, link) > longest)
	    longest = linkage_get_link_num_domains(linkage, link);
    }
    for (link=0; link<N_links; link++) {
	if (linkage_get_link_lword(linkage, link) == -1) continue;
	dname = linkage_get_link_domain_names(linkage, link);
	for (j=0; j<linkage_get_link_num_domains(linkage, link); ++j) {
	    append_string(s, " (%s)", dname[j]);
	}
	for (;j<longest; j++) {
	    append_string(s, "    ");
	}
	append_string(s, "   ");
	print_a_link(s, linkage, link);
    }
    append_string(s, "\r\n");
    if (linkage_get_violation_name(linkage) != NULL) {
	append_string(s, "P.P. violations:\r\n");
	append_string(s, "        %s\r\n\r\n", linkage_get_violation_name(linkage));
    }

    links_string = exalloc(strlen(s->p)+1);
    strcpy(links_string, s->p);
    exfree(s->p, sizeof(char)*s->allocated);
    exfree(s, sizeof(xString));

    return links_string;
}

char * build_linkage_postscript_string(Linkage linkage) {
    int link, i,j;
    int d;
    int print_word_0 = 0, print_word_N = 0, N_wall_connectors, suppressor_used;
    Sublinkage *sublinkage=&(linkage->sublinkage[linkage->current]);
    int N_links = sublinkage->num_links;
    Link *ppla = sublinkage->link;
    xString  * string;
    char * ps_string;
    Dictionary dict = linkage->sent->dict;
    Parse_Options opts = linkage->opts;

    string = String_create();

    N_wall_connectors = 0;
    if (dict->left_wall_defined) {
	suppressor_used = FALSE;
	if (!opts->display_walls)
	    for (j=0; j<N_links; j++) {
		if (ppla[j]->l == 0) {
		    if (ppla[j]->r == linkage->num_words-1) continue;
		    N_wall_connectors ++;
		    if (strcmp(ppla[j]->lc->string, LEFT_WALL_SUPPRESS)==0) {
			suppressor_used = TRUE;
		    }
		}
	    }
	print_word_0 = (((!suppressor_used) && (N_wall_connectors != 0))
			|| (N_wall_connectors > 1) || opts->display_walls);
    } else {
	print_word_0 = TRUE;
    }

    N_wall_connectors = 0;
    if (dict->right_wall_defined) {
	suppressor_used = FALSE;
	for (j=0; j<N_links; j++) {
	    if (ppla[j]->r == linkage->num_words-1) {
		N_wall_connectors ++;
		if (strcmp(ppla[j]->lc->string, RIGHT_WALL_SUPPRESS)==0){
		    suppressor_used = TRUE;
		}
	    }
	}
	print_word_N = (((!suppressor_used) && (N_wall_connectors != 0))
			|| (N_wall_connectors > 1) || opts->display_walls);
    }
    else {
	print_word_N = TRUE;
    }


    if (print_word_0) d=0; else d=1;

    i = 0;
    N_words_to_print = linkage->num_words;
    if (!print_word_N) N_words_to_print--;

    append_string(string, "[");
    for (j=d; j<N_words_to_print; j++) {
	if ((i%10 == 0) && (i>0)) append_string(string, "\r\n");
	i++;
	append_string(string, "(%s)", linkage->word[j]);
    }
    append_string(string,"]");
    append_string(string,"\r\n");

    append_string(string,"[");
    j = 0;
    for (link=0; link<N_links; link++) {
	if (!print_word_0 && (ppla[link]->l == 0)) continue;
	if (!print_word_N && (ppla[link]->r == linkage->num_words-1)) continue;
	if (ppla[link]->l == -1) continue;
	if ((j%7 == 0) && (j>0)) append_string(string,"\r\n");
	j++;
	append_string(string,"[%d %d %d",
		ppla[link]->l-d, ppla[link]->r-d,
		link_heights[link]);
	if (ppla[link]->lc->label < 0) {
	    append_string(string," (%s)]", ppla[link]->name);
	} else {
	    append_string(string," ()]");
	}
    }
    append_string(string,"]");
    append_string(string,"\r\n");
    append_string(string,"[");
    for (j=0; j<N_rows; j++ ){
	if (j>0) append_string(string, " %d", row_starts[j]);
	else append_string(string,"%d", row_starts[j]);
    }
    append_string(string,"]\r\n");

    ps_string = exalloc(strlen(string->p)+1);
    strcpy(ps_string, string->p);
    exfree(string->p, sizeof(char)*string->allocated);
    exfree(string, sizeof(xString));

    return ps_string;
}

void compute_chosen_words(Sentence sent, Linkage linkage) {
    /*
       This takes the current chosen_disjuncts array and uses it to
       compute the chosen_words array.  "I.xx" suffixes are eliminated.
       */
    int i, l;
    char * s, *t, *u;
    Parse_info * pi = sent->parse_info;
    char * chosen_words[MAX_SENTENCE];
    Parse_Options opts = linkage->opts;

    for (i=0; i<sent->length; i++) {   /* get rid of those ugly ".Ixx" */
	chosen_words[i] = sent->word[i].string;
	if (pi->chosen_disjuncts[i] == NULL) {
	    /* no disjunct is used on this word because of null-links */
	    t = chosen_words[i];
	    l = strlen(t) + 2;
	    s = (char *) xalloc(l+1);
	    sprintf(s, "[%s]", t);
	    t = string_set_add(s, sent->string_set);
	    xfree(s, l+1);
	    chosen_words[i] = t;
	} else if (opts->display_word_subscripts) {
	    t = pi->chosen_disjuncts[i]->string;
	    if (is_idiom_word(t)) {
		l = strlen(t);
		s = (char *) xalloc(l+1);
		strcpy(s,t);
		for (u=s; *u != '.'; u++)
		  ;
		*u = '\0';
		t = string_set_add(s, sent->string_set);
		xfree(s, l+1);
		chosen_words[i] = t;
	    } else {
		chosen_words[i] = t;
	    }
	}
    }
    if (sent->dict->left_wall_defined) {
	chosen_words[0] = LEFT_WALL_DISPLAY;
    }
    if (sent->dict->right_wall_defined) {
	chosen_words[sent->length-1] = RIGHT_WALL_DISPLAY;
    }
    for (i=0; i<linkage->num_words; ++i) {
	linkage->word[i] = (char *) exalloc(strlen(chosen_words[i])+1);
	strcpy(linkage->word[i], chosen_words[i]);
    }
}


#define MAX_HEIGHT 30

static char picture[MAX_HEIGHT][MAX_LINE];
static char xpicture[MAX_HEIGHT][MAX_LINE];

/* String allocated with exalloc.  Needs to be freed with exfree */
char * linkage_print_diagram(Linkage linkage) {
    int i, j, k, cl, cr, row, top_row, width, flag;
    char *t, *s;
    int print_word_0 = 0, print_word_N = 0, N_wall_connectors, suppressor_used;
    char connector[MAX_TOKEN_LENGTH];
    int line_len, link_length;
    Sublinkage *sublinkage=&(linkage->sublinkage[linkage->current]);
    int N_links = sublinkage->num_links;
    Link *ppla = sublinkage->link;
    xString * string;
    char * gr_string;
    Dictionary dict = linkage->sent->dict;
    Parse_Options opts = linkage->opts;
    int x_screen_width = parse_options_get_screen_width(opts);

    string = String_create();

    N_wall_connectors = 0;
    if (dict->left_wall_defined) {
	suppressor_used = FALSE;
	if (!opts->display_walls)
	    for (j=0; j<N_links; j++) {
		if ((ppla[j]->l == 0)) {
		    if (ppla[j]->r == linkage->num_words-1) continue;
		    N_wall_connectors ++;
		    if (strcmp(ppla[j]->lc->string, LEFT_WALL_SUPPRESS)==0){
			suppressor_used = TRUE;
		    }
		}
	    }
	print_word_0 = (((!suppressor_used) && (N_wall_connectors != 0))
			|| (N_wall_connectors > 1) || opts->display_walls);
    }
    else {
	print_word_0 = TRUE;
    }

    N_wall_connectors = 0;
    if (dict->right_wall_defined) {
	suppressor_used = FALSE;
	for (j=0; j<N_links; j++) {
	    if (ppla[j]->r == linkage->num_words-1) {
		N_wall_connectors ++;
		if (strcmp(ppla[j]->lc->string, RIGHT_WALL_SUPPRESS)==0){
		    suppressor_used = TRUE;
		}
	    }
	}
	print_word_N = (((!suppressor_used) && (N_wall_connectors != 0))
			|| (N_wall_connectors > 1) || opts->display_walls);
    }
    else {
	print_word_N = TRUE;
    }

    N_words_to_print = linkage->num_words;
    if (!print_word_N) N_words_to_print--;

    set_centers(linkage, print_word_0);
    line_len = center[N_words_to_print-1]+1;

    for (k=0; k<MAX_HEIGHT; k++) {
	for (j=0; j<line_len; j++) picture[k][j] = ' ';
	picture[k][line_len] = '\0';
    }
    top_row = 0;

    for (link_length = 1; link_length < N_words_to_print; link_length++) {
	for (j=0; j<N_links; j++) {
	    if (ppla[j]->l == -1) continue;
	    if ((ppla[j]->r - ppla[j]->l) != link_length)
	      continue;
	    if (!print_word_0 && (ppla[j]->l == 0)) continue;
	    /* gets rid of the irrelevant link to the left wall */
	    if (!print_word_N && (ppla[j]->r == linkage->num_words-1)) continue;
	    /* gets rid of the irrelevant link to the right wall */

	    /* put it into the lowest position */
	    cl = center[ppla[j]->l];
	    cr = center[ppla[j]->r];
	    for (row=0; row < MAX_HEIGHT; row++) {
		for (k=cl+1; k<cr; k++) {
		    if (picture[row][k] != ' ') break;
		}
		if (k == cr) break;
	    }
	    /* we know it fits, so put it in this row */

	    link_heights[j] = row;

	    if (2*row+2 > MAX_HEIGHT-1) {
		append_string(string, "The diagram is too high.\r\n");
		gr_string = exalloc(strlen(string->p)+1);
		strcpy(gr_string, string->p);
		exfree(string->p, sizeof(char)*string->allocated);
        exfree(string, sizeof(xString));
		return gr_string;
	    }
	    if (row > top_row) top_row = row;

	    picture[row][cl] = '+';
	    picture[row][cr] = '+';
	    for (k=cl+1; k<cr; k++) {
		picture[row][k] = '-';
	    }
	    s = ppla[j]->name;

	    if (opts->display_link_subscripts) {
	      if (!isalpha(*s))
		s = "";
	    } else {
	      if (!isupper(*s)) {
		s = "";   /* Don't print fat link connector name */
	      }
	    }
	    strncpy(connector, s, MAX_TOKEN_LENGTH-1);
	    connector[MAX_TOKEN_LENGTH-1] = '\0';
	    k=0;
	    if (opts->display_link_subscripts)
	      k = strlen(connector);
	    else
	      for (t=connector; isupper(*t); t++) k++; /* uppercase len of conn*/
	    if ((cl+cr-k)/2 + 1 <= cl) {
		t = picture[row] + cl + 1;
	    } else {
		t = picture[row] + (cl+cr-k)/2 + 1;
	    }
	    s = connector;
	    if (opts->display_link_subscripts)
	      while((*s != '\0') && (*t == '-')) *t++ = *s++;
	    else
	      while(isupper(*s) && (*t == '-')) *t++ = *s++;

	    /* now put in the | below this one, where needed */
	    for (k=0; k<row; k++) {
		if (picture[k][cl] == ' ') {
		    picture[k][cl] = '|';
		}
		if (picture[k][cr] == ' ') {
		    picture[k][cr] = '|';
		}
	    }
	}
    }

    /* we have the link picture, now put in the words and extra "|"s */

    s = xpicture[0];
    if (print_word_0) k = 0; else k = 1;
    for (; k<N_words_to_print; k++) {
	t = linkage->word[k];
	i=0;
	while(*t != '\0') {
	    *s++ = *t++;
	    i++;
	}
	*s++ = ' ';
    }
    *s = '\0';

    if (opts->display_short) {
	for (k=0; picture[0][k] != '\0'; k++) {
	    if ((picture[0][k] == '+') || (picture[0][k] == '|')) {
		xpicture[1][k] = '|';
	    } else {
		xpicture[1][k] = ' ';
	    }
	}
	xpicture[1][k] = '\0';
	for (row=0; row <= top_row; row++) {
	    strcpy(xpicture[row+2],picture[row]);
	}
	top_row = top_row+2;
    } else {
	for (row=0; row <= top_row; row++) {
	    strcpy(xpicture[2*row+2],picture[row]);
	    for (k=0; picture[row][k] != '\0'; k++) {
		if ((picture[row][k] == '+') || (picture[row][k] == '|')) {
		    xpicture[2*row+1][k] = '|';
		} else {
		    xpicture[2*row+1][k] = ' ';
		}
	    }
	    xpicture[2*row+1][k] = '\0';
	}
	top_row = 2*top_row + 2;
    }

    /* we've built the picture, now print it out */

    if (print_word_0) i = 0; else i = 1;
    k = 0;
    N_rows = 0;
    row_starts[N_rows] = 0;
    N_rows++;
    while(i < N_words_to_print) {
	append_string(string, "\r\n");
	width = 0;
	do {
	    width += strlen(linkage->word[i])+1;
	    i++;
	} while((i<N_words_to_print) &&
	      (width + ((int)strlen(linkage->word[i]))+1 < x_screen_width));
	row_starts[N_rows] = i - (!print_word_0);    /* PS junk */
	if (i<N_words_to_print) N_rows++;     /* same */
	for (row = top_row; row >= 0; row--) {
	    flag = TRUE;
	    for (j=k;flag&&(j<k+width)&&(xpicture[row][j]!='\0'); j++){
		flag = flag && (xpicture[row][j] == ' ');
	    }
	    if (!flag) {
		for (j=k;(j<k+width)&&(xpicture[row][j]!='\0'); j++){
		    append_string(string, "%c", xpicture[row][j]);
		}
		append_string(string, "\r\n");
	    }
	}
	append_string(string, "\r\n");
	k += width;
    }
    gr_string = exalloc(strlen(string->p)+1);
    strcpy(gr_string, string->p);
    exfree(string->p, sizeof(char)*string->allocated);
    exfree(string, sizeof(xString));
    return gr_string;
}

char * linkage_print_postscript(Linkage linkage, int mode) {
    char * ps, * qs;
    int size;

    ps = build_linkage_postscript_string(linkage);
    size = strlen(header(mode)) + strlen(ps) + strlen(trailer(mode)) + 1;

    qs = exalloc(sizeof(char)*size);
    sprintf(qs, "%s%s%s", header(mode), ps, trailer(mode));
    exfree(ps, strlen(ps)+1);

    return qs;
}

void print_disjunct_counts(Sentence sent) {
    int i;
    int c;
    Disjunct *d;
    for (i=0; i<sent->length; i++) {
	c = 0;
	for (d=sent->word[i].d; d != NULL; d = d->next) {
	    c++;
	}
	printf("%s(%d) ",sent->word[i].string, c);
    }
    printf("\r\n\r\n");
}

void print_expression_sizes(Sentence sent) {
    X_node * x;
    int w, size;
    for (w=0; w<sent->length; w++) {
	size = 0;
	for (x=sent->word[w].x; x!=NULL; x = x->next) {
	    size += size_of_expression(x->exp);
	}
	printf("%s[%d] ",sent->word[w].string, size);
    }
    printf("\r\n\r\n");
}

void print_sentence(FILE *fp, Sentence sent, int w) {
/* this version just prints it on one line.  */
    int i;
    if (sent->dict->left_wall_defined) i=1; else i=0;
    for (; i<sent->length - sent->dict->right_wall_defined; i++) {
	fprintf(fp, "%s ", sent->word[i].string);
    }
    fprintf(fp, "\r\n");
}

char * trailer(int mode) {
    static char * trailer_string=
        "diagram\r\n"
	"\r\n"
	"%%EndDocument\r\n"
	;

    if (mode==1) return trailer_string;
    else return "";
}

char * header(int mode) {
    static char * header_string=
        "%!PS-Adobe-2.0 EPSF-1.2\r\n"
        "%%Pages: 1\r\n"
        "%%BoundingBox: 0 -20 500 200\r\n"
        "%%EndComments\r\n"
        "%%BeginDocument: \r\n"
        "\r\n"
        "% compute size of diagram by adding\r\n"
        "% #rows x 8.5\r\n"
        "% (#rows -1) x 10\r\n"
        "% \\sum maxheight x 10\r\n"
        "/nulllink () def                     % The symbol of a null link\r\n"
        "/wordfontsize 11 def      % the size of the word font\r\n"
        "/labelfontsize 9 def      % the size of the connector label font\r\n"
        "/ex 10 def  % the horizontal radius of all the links\r\n"
        "/ey 10 def  % the height of the level 0 links\r\n"
        "/ed 10 def  % amount to add to this height per level\r\n"
        "/radius 10 def % radius for rounded arcs\r\n"
        "/row-spacing 10 def % the space between successive rows of the diagram\r\n"
        "\r\n"
        "/gap wordfontsize .5 mul def  % the gap between words\r\n"
        "/top-of-words wordfontsize .85 mul def\r\n"
        "             % the delta y above where the text is written where\r\n"
        "             % the major axis of the ellipse is located\r\n"
        "/label-gap labelfontsize .1 mul def\r\n"
        "\r\n"
        "/xwordfontsize 10 def      % the size of the word font\r\n"
        "/xlabelfontsize 10 def      % the size of the connector label font\r\n"
        "/xex 10 def  % the horizontal radius of all the links\r\n"
        "/xey 10 def  % the height of the level 0 links\r\n"
        "/xed 10 def  % amount to add to this height per level\r\n"
        "/xradius 10 def % radius for rounded arcs\r\n"
        "/xrow-spacing 10 def % the space between successive rows of the diagram\r\n"
        "/xgap wordfontsize .5 mul def  % the gap between words\r\n"
        "\r\n"
        "/centerpage 6.5 72 mul 2 div def\r\n"
        "  % this number of points from the left margin is the center of page\r\n"
        "\r\n"
        "/rightpage 6.5 72 mul def\r\n"
        "  % number of points from the left margin is the the right margin\r\n"
        "\r\n"
        "/show-string-centered-dict 5 dict def\r\n"
        "\r\n"
        "/show-string-centered {\r\n"
        "  show-string-centered-dict begin\r\n"
        "  /string exch def\r\n"
        "  /ycenter exch def\r\n"
        "  /xcenter exch def\r\n"
        "  xcenter string stringwidth pop 2 div sub\r\n"
        "  ycenter labelfontsize .3 mul sub\r\n"
        "  moveto\r\n"
        "  string show\r\n"
        "  end\r\n"
        "} def\r\n"
        "\r\n"
        "/clear-word-box {\r\n"
        "  show-string-centered-dict begin\r\n"
        "  /string exch def\r\n"
        "  /ycenter exch def\r\n"
        "  /xcenter exch def\r\n"
        "  newpath\r\n"
        "  /urx string stringwidth pop 2 div def\r\n"
        "  /ury labelfontsize .3 mul def\r\n"
        "  xcenter urx sub ycenter ury sub moveto\r\n"
        "  xcenter urx add ycenter ury sub lineto\r\n"
        "  xcenter urx add ycenter ury add lineto\r\n"
        "  xcenter urx sub ycenter ury add lineto\r\n"
        "  closepath\r\n"
        "  1 setgray fill\r\n"
        "  0 setgray\r\n"
        "  end\r\n"
        "} def\r\n"
        "\r\n"
        "/diagram-sentence-dict 20 dict def\r\n"
        "\r\n"
        "/diagram-sentence-circle\r\n"
        "{diagram-sentence-dict begin  \r\n"
        "   /links exch def\r\n"
        "   /words exch def\r\n"
        "   /n words length def\r\n"
        "   /Times-Roman findfont wordfontsize scalefont setfont\r\n"
        "   /x 0 def\r\n"
        "   /y 0 def\r\n"
        "\r\n"
        "   /left-ends [x dup words {stringwidth pop add gap add dup}\r\n"
        "	                     forall pop pop] def\r\n"
        "   /right-ends [x words {stringwidth pop add dup gap add} forall pop] def\r\n"
        "   /centers [0 1 n 1 sub {/i exch def\r\n"
        "		      left-ends i get\r\n"
        "		      right-ends i get\r\n"
        "		      add 2 div\r\n"
        "		    } for ] def\r\n"
        "\r\n"
        "   x y moveto\r\n"
        "   words {show gap 0 rmoveto} forall\r\n"
        "\r\n"
        "   .5 setlinewidth \r\n"
        "\r\n"
        "   links {dup 0 get /leftword exch def\r\n"
        "          dup 1 get /rightword exch def\r\n"
        "          dup 2 get /level exch def\r\n"
        "          3 get /string exch def\r\n"
        "          newpath\r\n"
        "          string nulllink eq {[2] 1 setdash}{[] 0 setdash} ifelse\r\n"
        "%          string nulllink eq {.8 setgray}{0 setgray} ifelse\r\n"
        "          centers leftword get\r\n"
        "	  y top-of-words add\r\n"
        "          moveto\r\n"
        "      \r\n"
        "          centers rightword get\r\n"
        "          centers leftword get\r\n"
        "          sub 2  div dup\r\n"
        "          radius \r\n"
        "          lt {/radiusx exch def}{pop /radiusx radius def} ifelse\r\n"
        "  \r\n"
        "          \r\n"
        " \r\n"
        "          centers leftword get\r\n"
        "	  y top-of-words add ey ed level mul add add\r\n"
        "          centers rightword get\r\n"
        "	  y top-of-words add ey ed level mul add add\r\n"
        "	  radiusx\r\n"
        "          arcto\r\n"
        "          4 {pop} repeat\r\n"
        "	  centers rightword get\r\n"
        "          y top-of-words add ey ed level mul add add\r\n"
        "	  centers rightword get\r\n"
        "	  y top-of-words add\r\n"
        "	  radiusx\r\n"
        "	  arcto\r\n"
        "          4 {pop} repeat\r\n"
        "	  centers rightword get\r\n"
        "	  y top-of-words add\r\n"
        "	  lineto\r\n"
        "\r\n"
        "	  stroke\r\n"
        "\r\n"
        "          /radius-y    ey ed level mul add	  def\r\n"
        "\r\n"
        "	  /center-arc-x\r\n"
        "	     centers leftword get centers rightword get add 2 div\r\n"
        "	  def\r\n"
        "	  \r\n"
        "          /center-arc-y\r\n"
        "             y top-of-words radius-y add add\r\n"
        "	  def\r\n"
        "\r\n"
        "          /Courier-Bold findfont labelfontsize scalefont setfont \r\n"
        "	  center-arc-x center-arc-y string clear-word-box\r\n"
        "	  center-arc-x center-arc-y string show-string-centered\r\n"
        "          } forall\r\n"
        "	  end\r\n"
        "  } def\r\n"
        "\r\n"
        "/diagramdict 20 dict def\r\n"
        "\r\n"
        "/diagram\r\n"
        "{diagramdict begin\r\n"
        "   /break-words exch def\r\n"
        "   /links exch def\r\n"
        "   /words exch def\r\n"
        "   /n words length def\r\n"
        "   /n-rows break-words length def\r\n"
        "   /Times-Roman findfont wordfontsize scalefont setfont\r\n"
        "\r\n"
        "   /left-ends [0 dup words {stringwidth pop add gap add dup}\r\n"
        "	                     forall pop pop] def\r\n"
        "   /right-ends [0 words {stringwidth pop add dup gap add} forall pop] def\r\n"
        "\r\n"
        "   /lwindows [ break-words {left-ends exch get gap 2 div sub } forall ] def\r\n"
        "   /rwindows [1 1 n-rows 1 sub {/i exch def\r\n"
        "		      lwindows i get } for\r\n"
        "	              right-ends n 1 sub get gap 2 div add\r\n"
        "	      ] def\r\n"
        "\r\n"
        "\r\n"
        "    /max 0 def\r\n"
        "    0 1 links length 1 sub {\r\n"
        "	/i exch def\r\n"
        "	/t links i get 2 get def\r\n"
        "	t max gt {/max t def} if\r\n"
        "      } for\r\n"
        "\r\n"
        "    /max-height ed max mul ey add top-of-words add row-spacing add def\r\n"
        "    /total-height n-rows max-height mul row-spacing sub def\r\n"
        "\r\n"
        "    /max-width 0 def            % compute the widest window\r\n"
        "    0 1 n-rows 1 sub {\r\n"
        "        /i exch def\r\n"
        "        /t rwindows i get lwindows i get sub def\r\n"
        "        t max-width gt {/max-width t def} if\r\n"
        "      } for\r\n"
        "\r\n"
        "    centerpage max-width 2 div sub 0 translate  % centers it\r\n"
        "   % rightpage max-width sub 0 translate      % right justified\r\n"
        "                        % Delete both of these to make it left justified\r\n"
        "\r\n"
        "   n-rows 1 sub -1 0\r\n"
        "     {/i exch def\r\n"
        "	gsave\r\n"
        "	newpath\r\n"
        "        %/centering centerpage rwindows i get lwindows i get sub 2 div sub def\r\n"
        "               % this line causes each row to be centered\r\n"
        "        /centering 0 def\r\n"
        "               % set centering to 0 to prevent centering of each row \r\n"
        "\r\n"
        "	centering -100 moveto  % -100 because some letters go below zero\r\n"
        "        centering max-height n-rows mul lineto\r\n"
        "        rwindows i get lwindows i get sub centering add\r\n"
        "                       max-height n-rows mul lineto\r\n"
        "        rwindows i get lwindows i get sub centering add\r\n"
        "                       -100 lineto\r\n"
        "	closepath\r\n"
        "        clip\r\n"
        "	lwindows i get neg n-rows i sub 1 sub max-height mul translate\r\n"
        "        centerpage centering 0 translate\r\n"
        "        words links diagram-sentence-circle\r\n"
        "	grestore\r\n"
        "     } for\r\n"
        "     end\r\n"
        "} def \r\n"
        "\r\n"
        "/diagramx\r\n"
        "{diagramdict begin\r\n"
        "   /break-words exch def\r\n"
        "   /links exch def\r\n"
        "   /words exch def\r\n"
        "   /n words length def\r\n"
        "   /n-rows break-words length def\r\n"
        "   /Times-Roman findfont xwordfontsize scalefont setfont\r\n"
        "\r\n"
        "   /left-ends [0 dup words {stringwidth pop add gap add dup}\r\n"
        "	                     forall pop pop] def\r\n"
        "   /right-ends [0 words {stringwidth pop add dup gap add} forall pop] def\r\n"
        "\r\n"
        "   /lwindows [ break-words {left-ends exch get gap 2 div sub } forall ] def\r\n"
        "   /rwindows [1 1 n-rows 1 sub {/i exch def\r\n"
        "		      lwindows i get } for\r\n"
        "	              right-ends n 1 sub get xgap 2 div add\r\n"
        "	      ] def\r\n"
        "\r\n"
        "\r\n"
        "    /max 0 def\r\n"
        "    0 1 links length 1 sub {\r\n"
        "	/i exch def\r\n"
        "	/t links i get 2 get def\r\n"
        "	t max gt {/max t def} if\r\n"
        "      } for\r\n"
        "\r\n"
        "    /max-height xed max mul xey add top-of-words add xrow-spacing add def\r\n"
        "    /total-height n-rows max-height mul xrow-spacing sub def\r\n"
        "\r\n"
        "    /max-width 0 def            % compute the widest window\r\n"
        "    0 1 n-rows 1 sub {\r\n"
        "        /i exch def\r\n"
        "        /t rwindows i get lwindows i get sub def\r\n"
        "        t max-width gt {/max-width t def} if\r\n"
        "      } for\r\n"
        "\r\n"
        "    centerpage max-width 2 div sub 0 translate  % centers it\r\n"
        "   % rightpage max-width sub 0 translate      % right justified\r\n"
        "                        % Delete both of these to make it left justified\r\n"
        "\r\n"
        "   n-rows 1 sub -1 0\r\n"
        "     {/i exch def\r\n"
        "	gsave\r\n"
        "	newpath\r\n"
        "        %/centering centerpage rwindows i get lwindows i get sub 2 div sub def\r\n"
        "               % this line causes each row to be centered\r\n"
        "        /centering 0 def\r\n"
        "               % set centering to 0 to prevent centering of each row \r\n"
        "\r\n"
        "	centering -100 moveto  % -100 because some letters go below zero\r\n"
        "        centering max-height n-rows mul lineto\r\n"
        "        rwindows i get lwindows i get sub centering add\r\n"
        "                       max-height n-rows mul lineto\r\n"
        "        rwindows i get lwindows i get sub centering add\r\n"
        "                       -100 lineto\r\n"
        "	closepath\r\n"
        "        clip\r\n"
        "	lwindows i get neg n-rows i sub 1 sub max-height mul translate\r\n"
        "        centerpage centering 0 translate\r\n"
        "        words links diagram-sentence-circle\r\n"
        "	grestore\r\n"
        "     } for\r\n"
        "     end\r\n"
        "} def \r\n"
        "\r\n"
        "/ldiagram\r\n"
        "{diagramdict begin\r\n"
        "   /break-words exch def\r\n"
        "   /links exch def\r\n"
        "   /words exch def\r\n"
        "   /n words length def\r\n"
        "   /n-rows break-words length def\r\n"
        "   /Times-Roman findfont wordfontsize scalefont setfont\r\n"
        "\r\n"
        "   /left-ends [0 dup words {stringwidth pop add gap add dup}\r\n"
        "	                     forall pop pop] def\r\n"
        "   /right-ends [0 words {stringwidth pop add dup gap add} forall pop] def\r\n"
        "\r\n"
        "   /lwindows [ break-words {left-ends exch get gap 2 div sub } forall ] def\r\n"
        "   /rwindows [1 1 n-rows 1 sub {/i exch def\r\n"
        "		      lwindows i get } for\r\n"
        "	              right-ends n 1 sub get gap 2 div add\r\n"
        "	      ] def\r\n"
        "\r\n"
        "\r\n"
        "    /max 0 def\r\n"
        "    0 1 links length 1 sub {\r\n"
        "	/i exch def\r\n"
        "	/t links i get 2 get def\r\n"
        "	t max gt {/max t def} if\r\n"
        "      } for\r\n"
        "\r\n"
        "    /max-height ed max mul ey add top-of-words add row-spacing add def\r\n"
        "    /total-height n-rows max-height mul row-spacing sub def\r\n"
        "\r\n"
        "    /max-width 0 def            % compute the widest window\r\n"
        "    0 1 n-rows 1 sub {\r\n"
        "        /i exch def\r\n"
        "        /t rwindows i get lwindows i get sub def\r\n"
        "        t max-width gt {/max-width t def} if\r\n"
        "      } for\r\n"
        "\r\n"
        "   % centerpage max-width 2 div sub 0 translate  % centers it\r\n"
        "   % rightpage max-width sub 0 translate      % right justified\r\n"
        "                        % Delete both of these to make it left justified\r\n"
        "\r\n"
        "   n-rows 1 sub -1 0\r\n"
        "     {/i exch def\r\n"
        "	gsave\r\n"
        "	newpath\r\n"
        "        %/centering centerpage rwindows i get lwindows i get sub 2 div sub def\r\n"
        "               % this line causes each row to be centered\r\n"
        "        /centering 0 def\r\n"
        "               % set centering to 0 to prevent centering of each row \r\n"
        "\r\n"
        "	centering -100 moveto  % -100 because some letters go below zero\r\n"
        "        centering max-height n-rows mul lineto\r\n"
        "        rwindows i get lwindows i get sub centering add\r\n"
        "                       max-height n-rows mul lineto\r\n"
        "        rwindows i get lwindows i get sub centering add\r\n"
        "                       -100 lineto\r\n"
        "	closepath\r\n"
        "        clip\r\n"
        "	lwindows i get neg n-rows i sub 1 sub max-height mul translate\r\n"
        "        centerpage centering 0 translate\r\n"
        "        words links diagram-sentence-circle\r\n"
        "	grestore\r\n"
        "     } for\r\n"
        "     end\r\n"
        "} def \r\n"
	;
    if (mode==1) return header_string;
    else return "";
}