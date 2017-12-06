 /***************************************************************************/
 /*                                                                         */
 /*       Copyright (C) 1991-1998  Daniel Sleator and Davy Temperley        */
 /*  See file "README" for information about commercial use of this system  */
 /*                                                                         */
 /***************************************************************************/

#include "link-includes.h"

/* This file contains certain general utilities. */

int   verbosity;

void safe_strcpy(char *u, char * v, int usize) {
/* Copies as much of v into u as it can assuming u is of size usize */
/* guaranteed to terminate u with a '\0'.                           */    
    strncpy(u, v, usize-1);
    u[usize-1] = '\0';
}

void safe_strcat(char *u, char *v, int usize) {
/* catenates as much of v onto u as it can assuming u is of size usize           */
/* guaranteed to terminate u with a '\0'.  Assumes u and v are null terminated.  */
    strncat(u, v, usize-strlen(u)-1);
    u[usize-1] = '\0';
}    

void free_connectors(Connector *e) {
/* free the list of connectors pointed to by e
   (does not free any strings) 
*/
    Connector * n;
    for(;e != NULL; e = n) {
	n = e->next;
	xfree((char *)e, sizeof(Connector));
    }
}

void free_disjuncts(Disjunct *c) {
/* free the list of disjuncts pointed to by c
   (does not free any strings) 
*/
    Disjunct *c1;
    for (;c != NULL; c = c1) {
	c1 = c->next;
	free_connectors(c->left);
	free_connectors(c->right);
	xfree((char *)c, sizeof(Disjunct));
    }
}

Connector * init_connector(Connector *c) {
    c->length_limit = UNLIMITED_LEN;
    /*    c->my_word = NO_WORD;  */       /* mark it unset, to make sure it gets set later */
    return c;
}

void free_X_nodes(X_node * x) {
/* frees the list of X_nodes pointed to by x, and all of the expressions */
    X_node * y;
    for (; x!= NULL; x = y) {
	y = x->next;
	free_Exp(x->exp);
	xfree((char *)x, sizeof(X_node));
    }
}

void free_E_list(E_list *);
void free_Exp(Exp * e) {
    if (e->type != CONNECTOR_type) {
	free_E_list(e->u.l);
    }
    xfree((char *)e, sizeof(Exp));
}

void free_E_list(E_list * l) {
    if (l == NULL) return;
    free_E_list(l->next);
    free_Exp(l->e);
    xfree((char *)l, sizeof(E_list));
}

int size_of_expression(Exp * e) {
/* Returns the number of connectors in the expression e */
    int size;
    E_list * l;
    if (e->type == CONNECTOR_type) return 1;
    size = 0;
    for (l=e->u.l; l!=NULL; l=l->next) {
	size += size_of_expression(l->e);
    }
    return size;
}

unsigned int randtable[RTSIZE];

/* There is a legitimate question of whether having the hash function    */
/* depend on a large array is a good idea.  It might not be fastest on   */
/* a machine that depends on caching for its efficiency.  On the other   */
/* hand, Phong Vo's hash (and probably other linear-congruential) is     */
/* pretty bad.  So, mine is a "competitive" hash function -- you can't   */
/* make it perform horribly.                                             */

void init_randtable(void) {
    int i;
    srand(10);
    for (i=0; i<RTSIZE; i++) {
	randtable[i] = rand();
    }
}

/* Build a copy of the given expression (don't copy strings, of course) */
E_list * copy_E_list(E_list * l);
Exp * copy_Exp(Exp * e) {
    Exp * n;
    if (e == NULL) return NULL;
    n = (Exp *) xalloc(sizeof(Exp));
    *n = *e;
    if (e->type != CONNECTOR_type) {
	n->u.l = copy_E_list(e->u.l);
    }
    return n;
}

E_list * copy_E_list(E_list * l) {
    E_list * nl;
    if (l == NULL) return NULL;
    nl = (E_list *) xalloc(sizeof(E_list));
    *nl = *l;    /* not necessary -- both fields will be built below */
    nl->next = copy_E_list(l->next);
    nl->e = copy_Exp(l->e);
    return nl;
}

Connector * copy_connectors(Connector * c) {
/* This builds a new copy of the connector list pointed to by c.
   Strings, as usual, are not copied.
*/
    Connector *c1;
    if (c == NULL) return NULL;
    c1 = init_connector((Connector *) xalloc(sizeof(Connector)));
    *c1 = *c;
    c1->next = copy_connectors(c->next);
    return c1;
}

Disjunct * copy_disjunct(Disjunct * d) {
/* This builds a new copy of the disjunct pointed to by d (except for the
   next field which is set to NULL).  Strings, as usual,
   are not copied.
*/
    Disjunct * d1;
    if (d == NULL) return NULL;
    d1 = (Disjunct *) xalloc(sizeof(Disjunct));
    *d1 = *d;
    d1->next = NULL;
    d1->left = copy_connectors(d->left);
    d1->right = copy_connectors(d->right);
    return d1;
}

int max_space_in_use;
int space_in_use;
int max_external_space_in_use;
int external_space_in_use;

void * xalloc(int size) {
/* To allow printing of a nice error message, and keep track of the
   space allocated.
*/   
    char * p = (char *) malloc(size);
    space_in_use += size;
    if (space_in_use > max_space_in_use) max_space_in_use = space_in_use;
    if ((p == NULL) && (size != 0)){
        printf("Ran out of space.\n");
	abort();
        exit(1);
    }
    return (void *) p;
}

void xfree(void * p, int size) {
    space_in_use -= size;
    free(p);
}

void * exalloc(int size) {

    char * p = (char *) malloc(size);
    external_space_in_use += size;
    if (external_space_in_use > max_external_space_in_use) {
	max_external_space_in_use = external_space_in_use;
    }
    if ((p == NULL) && (size != 0)){
        printf("Ran out of space.\n");
	abort();
        exit(1);
    }
    return (void *) p;
}

void exfree(void * p, int size) {
    external_space_in_use -= size;
    free(p);
}

/* This is provided as part of the API */
void string_delete(char * p) {
    exfree(p, strlen(p)+1);
}

void exfree_connectors(Connector *e) {
    Connector * n;
    for(;e != NULL; e = n) {
	n = e->next;
	exfree(e->string, sizeof(char)*(strlen(e->string)+1));
	exfree(e, sizeof(Connector));
    }
}

Connector * excopy_connectors(Connector * c) {
    Connector *c1;

    if (c == NULL) return NULL;

    c1 = init_connector((Connector *) exalloc(sizeof(Connector)));
    *c1 = *c;
    c1->string = (char *) exalloc(sizeof(char)*(strlen(c->string)+1));
    strcpy(c1->string, c->string);
    c1->next = excopy_connectors(c->next);

    return c1;
}


Link excopy_link(Link l) {
     Link newl;

     if (l == NULL) return NULL;

     newl = (Link) exalloc(sizeof(struct Link_s));
     newl->name = (char *) exalloc(sizeof(char)*(strlen(l->name)+1));
     strcpy(newl->name, l->name);
     newl->l = l->l;
     newl->r = l->r;
     newl->lc = excopy_connectors(l->lc);
     newl->rc = excopy_connectors(l->rc);

     return newl;
}

void exfree_link(Link l) {
     exfree_connectors(l->rc);
     exfree_connectors(l->lc);
     exfree(l->name, sizeof(char)*(strlen(l->name)+1));
     exfree(l, sizeof(struct Link_s));
}


Disjunct * catenate_disjuncts(Disjunct *d1, Disjunct *d2) {
/* Destructively catenates the two disjunct lists d1 followed by d2. */
/* Doesn't change the contents of the disjuncts */
/* Traverses the first list, but not the second */    
    Disjunct * dis = d1;

    if (d1 == NULL) return d2;
    if (d2 == NULL) return d1;
    while (dis->next != NULL) dis = dis->next;
    dis->next = d2;
    return d1;
}

X_node * catenate_X_nodes(X_node *d1, X_node *d2) {
/* Destructively catenates the two disjunct lists d1 followed by d2. */
/* Doesn't change the contents of the disjuncts */
/* Traverses the first list, but not the second */    
    X_node * dis = d1;

    if (d1 == NULL) return d2;
    if (d2 == NULL) return d1;
    while (dis->next != NULL) dis = dis->next;
    dis->next = d2;
    return d1;
}

int next_power_of_two_up(int i) {
/* Returns the smallest power of two that is at least i and at least 1 */
    int j=1;
    while(j<i) j = j<<1;
    return j;
}

int upper_case_match(char *s, char *t) {
/* returns TRUE if the initial upper case letters of s and t match */
    while(isupper(*s) || isupper(*t)) {
	if (*s != *t) return FALSE;
	s++;
	t++;
    }
    return (!isupper(*s) && !isupper(*t));
}

void left_print_string(FILE * fp, char * s, char * t) {
/* prints s then prints the last |t|-|s| characters of t.
   if s is longer than t, it truncates s.
*/
    int i, j, k;
    j = strlen(t);
    k = strlen(s);
    for (i=0; i<j; i++) {
	if (i<k) {
	    fprintf(fp, "%c", s[i]);
	} else {
	    fprintf(fp, "%c", t[i]);
	}
    }
}

int sentence_contains(Sentence sent, char * s) {
/* Returns TRUE if one of the words in the sentence is s */
    int w;
    for (w=0; w<sent->length; w++) {
	if (strcmp(sent->word[w].string, s) == 0) return TRUE;
    }
    return FALSE;
}

void set_is_conjunction(Sentence sent) {
    int w;
    char * s;
    for (w=0; w<sent->length; w++) {
	s = sent->word[w].string;
	sent->is_conjunction[w] = ((strcmp(s, "and")==0) || (strcmp(s, "or" )==0) ||
				   (strcmp(s, "but")==0) || (strcmp(s, "nor")==0));
    }
}

int sentence_contains_conjunction(Sentence sent) {
/* Return true if the sentence contains a conjunction.  Assumes
   is_conjunction[] has been initialized.
*/
    int w;
    for (w=0; w<sent->length; w++) {
	if (sent->is_conjunction[w]) return TRUE;
    }
    return FALSE;
}

int conj_in_range(Sentence sent, int lw, int rw) {
/* Returns true if the range lw...rw inclusive contains a conjunction     */
    for (;lw <= rw; lw++) {
	if (sent->is_conjunction[lw]) return TRUE;
    }
    return FALSE;
}

FILE *dictopen(char *dictname, char *filename, char *how) {
    /* This function is used to open a dictionary file or a word file.
       It tries the following sequence of places to look for the file.
       The first one that is successful is used.  (1) Open the given
       file.  (2) if DICTPATH is defined, try the paths defined by that.
       (3) Use the path name of the given dictionary.  */

    char *dictpath, completename[MAX_PATH_NAME+1];
    char dummy[MAX_PATH_NAME+1];
    char *pos, *oldpos;
    int filenamelen, len;
    FILE *fp;

    dictpath = getenv(DICTPATH);
    if (dictpath == NULL && dictname != NULL) {
	/* look in the dictname part */
	safe_strcpy(dummy, dictname, sizeof(dummy));
	pos = strrchr(dummy, '/');
	if (pos != NULL) {
	    *pos = '\0';
	    dictpath = dummy;
	}
    }

    if (dictpath == NULL) {
	printf("   Opening %s\n", filename); 
	return (fopen(filename, how));
    } else if ((fp = fopen(filename, how)) != NULL) {
	printf("   Opening %s\n", filename);
	return fp;
    }
    
    filenamelen = strlen(filename);
    len = strlen(dictpath)+ filenamelen + 1 + 1;
    oldpos = dictpath;
    fp = NULL;
    while ((pos = strchr(oldpos, ':')) != NULL) {
	strncpy(completename, oldpos, (pos-oldpos));
	*(completename+(pos-oldpos)) = '/';
	strcpy(completename+(pos-oldpos)+1,filename);
	if ((fp = fopen(completename, how)) != NULL) {
	    printf("   Opening %s\n", completename); 
	    return fp;
	}
	oldpos = pos+1;
    }
    pos = oldpos+strlen(oldpos);
    strcpy(completename, oldpos);
    *(completename+(pos-oldpos)) = '/';
    strcpy(completename+(pos-oldpos)+1,filename);
    fp = fopen(completename,how);
    printf("   Opening %s\n", completename); 
    return fp;
}

static int random_state[2] = {0,0};
static int random_count = 0;
static int random_inited = FALSE;

int step_generator(int d) {
    /* no overflow should occur, so this is machine independent */
    random_state[0] = ((random_state[0] * 3) + d + 104729) % 179424673;
    random_state[1] = ((random_state[1] * 7) + d + 48611) % 86028121;
    return random_state[0] + random_state[1];;
}

void my_random_initialize(int seed) {
    assert(!random_inited, "Random number generator not finalized.");
    
    seed = (seed < 0) ? -seed : seed;
    seed = seed % (1 << 30);

    random_state[0] = seed % 3;
    random_state[1] = seed % 5;
    random_count = seed;
    random_inited = TRUE;
}

void my_random_finalize() {
    assert(random_inited, "Random number generator not initialized.");
    random_inited = FALSE;
}

int my_random(void) {
    random_count++;
    return (step_generator(random_count));
}


/* Here's the connector set data structure */

#if 0
/* this is defined in parser_api.h */
typedef struct {
    Connector ** hash_table;
    int          table_size;
    int          is_defined;  /* if 0 then there is no such set */
} Connector_set;
#endif

int connector_set_hash(Connector_set *conset, char * s, int d) {
/* This hash function only looks at the leading upper case letters of
   the string, and the direction, '+' or '-'.
*/
    int i;
    for(i=d; isupper(*s); s++) i = i + (i<<1) + randtable[(*s + i) & (RTSIZE-1)];
    return (i & (conset->table_size-1));
}

void build_connector_set_from_expression(Connector_set * conset, Exp * e) {
    E_list * l;
    Connector * c;
    int h;
    if (e->type == CONNECTOR_type) {
	c = init_connector((Connector *) xalloc(sizeof(Connector)));
	c->string = e->u.string;
	c->label = NORMAL_LABEL;        /* so we can use match() */
	c->priority = THIN_priority;
	c->word = e->dir;     /* just use the word field to give the dir */
	h = connector_set_hash(conset, c->string, c->word);
	c->next = conset->hash_table[h];
	conset->hash_table[h] = c;
    } else {
	for (l=e->u.l; l!=NULL; l=l->next) {
	    build_connector_set_from_expression(conset, l->e);
	}
    }
}	

Connector_set * connector_set_create(Exp *e) {
    int i;
    Connector_set *conset;

    conset = (Connector_set *) xalloc(sizeof(Connector_set));
    conset->table_size = next_power_of_two_up(size_of_expression(e));
    conset->hash_table =
      (Connector **) xalloc(conset->table_size * sizeof(Connector *));
    for (i=0; i<conset->table_size; i++) conset->hash_table[i] = NULL;
    build_connector_set_from_expression(conset, e);
    return conset;
}

void connector_set_delete(Connector_set * conset) {
    int i;
    if (conset == NULL) return;
    for (i=0; i<conset->table_size; i++) free_connectors(conset->hash_table[i]);
    xfree(conset->hash_table, conset->table_size * sizeof(Connector *));
    xfree(conset, sizeof(Connector_set));
}

int match_in_connector_set(Connector_set *conset, Connector * c, int d) {
/* Returns TRUE the given connector is in this conset.  FALSE otherwise.
   d='+' means this connector is on the right side of the disjunct.
   d='-' means this connector is on the left side of the disjunct.
*/
    int h;
    Connector * c1;
    if (conset == NULL) return FALSE;
    h = connector_set_hash(conset, c->string, d);
    for (c1=conset->hash_table[h]; c1!=NULL; c1 = c1->next) {
	if (x_match(c1, c) && (d == c1->word)) return TRUE;
    }
    return FALSE;
}
