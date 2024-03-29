 /***************************************************************************/
 /*                                                                         */
 /*       Copyright (C) 1991-1998  Daniel Sleator and Davy Temperley        */
 /*  See file "README" for information about commercial use of this system  */
 /*                                                                         */
 /***************************************************************************/

#include <stdarg.h>
#include "link-includes.h"

/* The functions in this file do several things: () take a linkage
 involving fat links and expand it into a sequence of linkages
 (involving a subset of the given words), one for each way of
 eliminating the conjunctions.  () determine if a linkage involving
 fat links has a structural violation.  () make sure each of the expanded
 linkages has a consistent post-processing behavior.  () compute the
 cost of the linkage. */


static List_o_links *word_links[MAX_SENTENCE]; /* ptr to l.o.l. out of word */
static int structure_violation;
static int dfs_root_word[MAX_SENTENCE]; /* for the depth-first search */
static int dfs_height[MAX_SENTENCE];    /* to determine the order to do the root word dfs */
static int height_perm[MAX_SENTENCE];   /* permute the vertices from highest to lowest */

/* The following three functions are all for computing the cost of and lists */
static int visited[MAX_SENTENCE];
static int and_element_sizes[MAX_SENTENCE];
static int N_and_elements;

typedef struct patch_element_struct Patch_element;
struct patch_element_struct
{
    char used;   /* TRUE if this link is used, else FALSE  */
    char changed;/* TRUE if this link changed, else FALSE  */
    int newl;    /* the new value of the left end          */
    int newr;    /* the new value of the right end         */
};

static Patch_element patch_array[MAX_LINKS];

typedef struct DIS_node_struct DIS_node;
typedef struct CON_node_struct CON_node;
typedef struct CON_list_struct CON_list;
typedef struct DIS_list_struct DIS_list;
typedef struct Links_to_patch_struct Links_to_patch;
typedef void (*void_returning_function)();

struct DIS_node_struct {
    CON_list * cl;     /* the list of children */
    List_o_links * lol;/* the links that comprise this region of the graph */
    int word;          /* the word defining this node */
};

struct CON_node_struct {
    DIS_list * dl;     /* the list of children */
    DIS_list * current;/* defines the current child */
    int word;          /* the word defining this node */
};

struct DIS_list_struct {
    DIS_list * next;
    DIS_node * dn;
};

struct CON_list_struct {
    CON_list * next;
    CON_node * cn;
};

struct Links_to_patch_struct {
    Links_to_patch * next;
    int link;
    char dir;   /* this is 'r' or 'l' depending on which end of the link
		   is to be patched. */
    int newvalue;    /* the new value of the end to be patched */
};


static Sublinkage * x_create_sublinkage(Parse_info * pi) {
    int i;
    Sublinkage *s = (Sublinkage *) xalloc (sizeof(Sublinkage));
    s->link = (Link *) xalloc(MAX_LINKS*sizeof(Link));
    s->pp_info = NULL;
    s->violation = NULL;
    for (i=0; i<MAX_LINKS; i++)	s->link[i] = NULL;
    s->num_links = pi->N_links;
    assert(pi->N_links < MAX_LINKS, "Too many links");
    return s;
}


static Sublinkage * ex_create_sublinkage(Parse_info * pi) {
    int i;
    Sublinkage *s = (Sublinkage *) exalloc (sizeof(Sublinkage));
    s->link = (Link *) exalloc(pi->N_links*sizeof(Link));
    s->pp_info = NULL;
    s->violation = NULL;
    for (i=0; i<pi->N_links; i++) s->link[i] = NULL;
    s->num_links = pi->N_links;
    assert(pi->N_links < MAX_LINKS, "Too many links");
    return s;
}


static void free_sublinkage(Sublinkage *s) {
    int i;
    for (i=0; i<MAX_LINKS; i++) {
	if (s->link[i]!=NULL) exfree_link(s->link[i]);
    }
    xfree(s->link, MAX_LINKS*sizeof(Link));
    xfree(s, sizeof(Sublinkage));
}

static void replace_link_name(Link l, char *s) {
    exfree(l->name, sizeof(char)*(strlen(l->name)+1));
    l->name = (char *) exalloc(sizeof(char)*(strlen(s)+1));
    strcpy(l->name, s);
}

static void copy_full_link(Link *dest, Link src) {
    if (*dest != NULL) exfree_link(*dest);
    *dest = excopy_link(src);
}

/* end new code 9/97 ALB */


static void build_digraph(Parse_info * pi) {
/* Constructs a graph in the word_links array based on the contents of    */
/* the global link_array.  Makes the word_links array point to a list of  */
/* words neighboring each word (actually a list of links).  This is a     */
/* directed graph, constructed for dealing with "and".  For a link in     */
/* which the priorities are UP or DOWN_priority, the edge goes from the   */
/* one labeled DOWN to the one labeled UP.                                */
/* Don't generate links edges for the bogus comma connectors              */
    int i, link, N_fat;
    Link lp;
    List_o_links * lol;
    N_fat = 0;
    for (i=0; i<pi->N_words; i++) {
	word_links[i] = NULL;
    }
    for (link=0; link<pi->N_links; link++) {
	lp = &(pi->link_array[link]);
	i = lp->lc->label;
	if (i < NORMAL_LABEL) {   /* one of those special links for either-or, etc */
	    continue;
	}
	lol = (List_o_links *) xalloc(sizeof(List_o_links));
	lol->next = word_links[lp->l];
	word_links[lp->l] = lol;
	lol->link = link;
	lol->word = lp->r;
	i = lp->lc->priority;
	if (i == THIN_priority) {
	    lol->dir = 0;
	} else if (i == DOWN_priority) {
	    lol->dir = 1;
	} else {
	    lol->dir = -1;
	}
	lol = (List_o_links *) xalloc(sizeof(List_o_links));
	lol->next = word_links[lp->r];
	word_links[lp->r] = lol;
	lol->link = link;
	lol->word = lp->l;
	i = lp->rc->priority;
	if (i == THIN_priority) {
	    lol->dir = 0;
	} else if (i == DOWN_priority) {
	    lol->dir = 1;
	} else {
	    lol->dir = -1;
	}
    }
}

static int is_CON_word(int w) {
/* Returns TRUE if there is at least one fat link pointing out of this word. */
    List_o_links * lol;
    for (lol = word_links[w]; lol!=NULL; lol = lol->next) {
	if (lol->dir == 1) {
	    return TRUE;
	}
    }
    return FALSE;
}

static DIS_node * build_DIS_node(int);
static CON_list * c_dfs(int, DIS_node *, CON_list *);

static CON_node * build_CON_node(int w) {
/* This word is a CON word (has fat links down).  Build the tree for it.  */
    List_o_links * lol;
    CON_node * a;
    DIS_list * d, *dx;
    d = NULL;
    for (lol = word_links[w]; lol!=NULL; lol = lol->next) {
	if (lol->dir == 1) {
	    dx = (DIS_list *) xalloc (sizeof (DIS_list));
	    dx->next = d;
	    d = dx;
	    d->dn = build_DIS_node(lol->word);
	}
    }
    a = (CON_node *) xalloc(sizeof (CON_node));
    a->dl = a->current = d;
    a->word = w;
    return a;
}

static CON_list * c_dfs(int w, DIS_node * start_dn, CON_list * c) {
/* Does a depth-first-search starting from w.  Puts on the front of the
   list pointed to by c all of the CON nodes it finds, and returns the
   result.  Also construct the list of all edges reached as part of this
   DIS_node search and append it to the lol list of start_dn.

   Both of the structure violations actually occur, and represent
   linkages that have improper structure.  Fortunately, they
   seem to be rather rare.
*/
    CON_list *cx;
    List_o_links * lol, *lolx;
    if (dfs_root_word[w] != -1) {
	if (dfs_root_word[w] != start_dn->word) {
	    structure_violation = TRUE;
	}
	return c;
    }
    dfs_root_word[w] = start_dn->word;
    for (lol=word_links[w]; lol != NULL; lol = lol->next) {
	if (lol->dir < 0) {  /* a backwards link */
	    if (dfs_root_word[lol->word] == -1) {
		structure_violation = TRUE;
	    }
	} else if (lol->dir == 0) {
	    lolx = (List_o_links *) xalloc(sizeof(List_o_links));
	    lolx->next = start_dn->lol;
	    lolx->link = lol->link;
	    start_dn->lol = lolx;
	    c = c_dfs(lol->word, start_dn, c);
	}
    }
    if (is_CON_word(w)) {  /* if the current node is CON, put it first */
	cx = (CON_list *) xalloc(sizeof(CON_list));
	cx->next = c;
	c = cx;
	c->cn = build_CON_node(w);
    }
    return c;
}

static DIS_node * build_DIS_node(int w) {
/* This node is connected to its parent via a fat link.  Search the
   region reachable via thin links, and put all reachable nodes with fat
   links out of them in its list of children.
*/
    DIS_node * dn;
    dn = (DIS_node *) xalloc(sizeof (DIS_node));
    dn->word = w;   /* must do this before dfs so it knows the start word */
    dn->lol = NULL;
    dn->cl = c_dfs(w, dn, NULL);
    return dn;
}

void height_dfs(int w, int height) {
    List_o_links * lol;
    if (dfs_height[w] != 0) return;
    dfs_height[w] = height;
    for (lol=word_links[w]; lol != NULL; lol = lol->next) {
	/* The dir is 1 for a down link. */
	height_dfs(lol->word, height - lol->dir);
    }
}

int comp_height(int *a, int *b) {
    return dfs_height[*b] - dfs_height[*a];
}

#define COMPARE_TYPE int (*)(const void *, const void *)

static DIS_node * build_DIS_CON_tree(Parse_info * pi) {
    int xw, w;
    DIS_node * dnroot, * dn;
    CON_list * child, * xchild;
    List_o_links * lol, * xlol;

    /* The algorithm used here to build the DIS_CON tree depends on
       the search percolating down from the "top" of the tree.  The
       original version of this started its search at the wall.  This
       was fine because doing a DFS from the wall explore the tree in
       the right order.

       However, in order to handle null links correctly, a more careful
       ordering process must be used to explore the tree.  We use
       dfs_height[] for this.
     */

    for (w=0; w < pi->N_words; w++) dfs_height[w] = 0;
    for (w=0; w < pi->N_words; w++) height_dfs(w, MAX_SENTENCE);

    for (w=0; w < pi->N_words; w++) height_perm[w] = w;
    qsort(height_perm, pi->N_words, sizeof(height_perm[0]), (COMPARE_TYPE) comp_height);

    for (w=0; w<pi->N_words; w++) dfs_root_word[w] = -1;

    dnroot = NULL;
    for (xw=0; xw < pi->N_words; xw++) {
	w = height_perm[xw];
	if (dfs_root_word[w] == -1) {
	    dn = build_DIS_node(w);
	    if (dnroot == NULL) {
		dnroot = dn;
	    } else {
		for (child = dn->cl; child != NULL; child = xchild) {
		    xchild = child->next;
		    child->next = dnroot->cl;
		    dnroot->cl = child;
		}
		for (lol = dn->lol; lol != NULL; lol = xlol) {
		    xlol = lol->next;
		    lol->next = dnroot->lol;
		    dnroot->lol = lol;
		}
		xfree((void *) dn, sizeof(DIS_node));
	    }
	}
    }
    return dnroot;
}

static int advance_CON(CON_node *);
static int advance_DIS(DIS_node * dn) {
/* Cycically advance the current state of this DIS node.
   If it's now at the beginning of its cycle" return FALSE;
   Otherwise return TRUE;
*/
    CON_list * cl;
    for (cl = dn->cl; cl!=NULL; cl=cl->next) {
	if (advance_CON(cl->cn)) {
	    return TRUE;
	}
    }
    return FALSE;
}

static int advance_CON(CON_node * cn) {
/* Cycically advance the current state of this CON node.
   If it's now at the beginning of its cycle return FALSE;
   Otherwise return TRUE;
*/
    if (advance_DIS(cn->current->dn)) {
	return TRUE;
    } else {
	if (cn->current->next == NULL) {
	    cn->current = cn->dl;
	    return FALSE;
	} else {
	    cn->current = cn->current->next;
	    return TRUE;
	}
    }
}

static void fill_patch_array_CON(CON_node *, Links_to_patch *);

static void fill_patch_array_DIS(DIS_node * dn, Links_to_patch * ltp) {
/* Patches up appropriate links in the patch_array for this DIS_node     */
/* and this patch list.                                                  */
    CON_list * cl;
    List_o_links * lol;
    Links_to_patch * ltpx;
    for (lol=dn->lol; lol != NULL; lol=lol->next) {
	patch_array[lol->link].used = TRUE;
    }
    if ((dn->cl == NULL) || (dn->cl->cn->word != dn->word)) {
	for (; ltp != NULL; ltp = ltpx) {
	    ltpx = ltp->next;
	    patch_array[ltp->link].changed = TRUE;
	    if (ltp->dir == 'l') {
		patch_array[ltp->link].newl = dn->word;
	    } else {
		patch_array[ltp->link].newr = dn->word;
	    }
	    xfree((void *) ltp, sizeof(Links_to_patch));
	}
    }
    /* ltp != NULL at this point means that dn has child which is a cn
       which is the same word */
    for (cl=dn->cl; cl!=NULL; cl=cl->next) {
	fill_patch_array_CON(cl->cn, ltp);
	ltp = NULL;
    }
}

static void fill_patch_array_CON(CON_node * cn, Links_to_patch * ltp) {
    List_o_links * lol;
    Links_to_patch *ltpx;
    for (lol=word_links[cn->word]; lol != NULL; lol = lol->next) {
	if (lol->dir == 0) {
	    ltpx = (Links_to_patch *) xalloc(sizeof(Links_to_patch));
	    ltpx->next = ltp;
	    ltp = ltpx;
        ltp->newvalue = cn->word;
	    ltp->link = lol->link;
	    if (lol->word > cn->word) {
		ltp->dir = 'l';
	    } else {
		ltp->dir = 'r';
	    }
	}
    }
    fill_patch_array_DIS(cn->current->dn, ltp);
}

static void free_digraph(Parse_info * pi)
{
  List_o_links * lol, *lolx;
  int i;
  for (i=0; i<pi->N_words; i++)
    {
      for (lol=word_links[i]; lol!=NULL; lol=lolx)
	{
	  lolx = lol->next;
	  xfree((void *) lol, sizeof(List_o_links));
	}
    }
}

static void free_CON_tree(CON_node *);
static void free_DIS_tree(DIS_node * dn) {
    List_o_links * lol, *lolx;
    CON_list *cl, *clx;
    for (lol=dn->lol; lol!=NULL; lol=lolx) {
	lolx = lol->next;
	xfree((void *) lol, sizeof(List_o_links));
    }
    for (cl = dn->cl; cl!=NULL; cl=clx) {
	clx = cl->next;
	free_CON_tree(cl->cn);
	xfree((void *) cl, sizeof(CON_list));
    }
    xfree((void *) dn, sizeof(DIS_node));
}

static void free_CON_tree(CON_node * cn) {
    DIS_list *dl, *dlx;
    for (dl = cn->dl; dl!=NULL; dl=dlx) {
	dlx = dl->next;
	free_DIS_tree(dl->dn);
	xfree((void *) dl, sizeof(DIS_list));
    }
    xfree((void *) cn, sizeof(CON_node));
}

static void and_dfs_full(int w) {
/* scope out this and element */
    List_o_links *lol;
    if (visited[w]) return;
    visited[w] = TRUE;
    and_element_sizes[N_and_elements]++;
    for (lol = word_links[w]; lol != NULL; lol = lol->next) {
	if (lol->dir >= 0) {
	    and_dfs_full(lol->word);
	}
    }
}

static void and_dfs_commas(Sentence sent, int w) {
/* get down the tree past all the commas */
    List_o_links *lol;
    if (visited[w]) return;
    visited[w] = TRUE;
    for (lol = word_links[w]; lol != NULL; lol = lol->next) {
	if (lol->dir == 1) {
             /* we only consider UP or DOWN priority links here */

	    if (strcmp(sent->word[lol->word].string, ",")==0) {
		    /* pointing to a comma */
		and_dfs_commas(sent, lol->word);
	    } else {
		and_dfs_full(lol->word);
		N_and_elements++;
	    }
	}
    }
}

static int and_cost(Sentence sent) {
/* Computes the imbalance in the and lists.  Assumes the graph has already */
/* been constructed.                                                       */
    int w, i, min, max, j, cost;
    char * s;
    Parse_info * pi = sent->parse_info;

    cost = 0;
    for(w = 0; w<pi->N_words; w++) {
	s = sent->word[w].string;
	if (sent->is_conjunction[w]) {
	    N_and_elements = 0;
	    for(i=0; i<pi->N_words; i++) {
		visited[i] = FALSE;
		and_element_sizes[i] = 0;
	    }
	    if (sent->dict->left_wall_defined)
		visited[0] = TRUE;  /* this prevents the wall  */
                       /* from being erroniously put in an and list count*/
                       /* it also introduces and violation in "walk and run"*/
	    and_dfs_commas(sent, w);
	    if (N_and_elements > 0) {
		min=MAX_SENTENCE;
		max=0;
		for (i=0; i<N_and_elements; i++) {
		    j = and_element_sizes[i];
		    if (j < min) min=j;
		    if (j > max) max=j;
		}
		cost += max-min;   /* is this the right function? */
	    }
	}
    }
    return cost;
}

static int cost_for_length(int length) {
/* this function defines the cost of a link as a function of its length */
      return length-1;
}

static int link_cost(Parse_info * pi) {
/* computes the cost of the current parse of the current sentence */
/* due to the length of the links                                 */
    int lcost, i;
    lcost =  0;
    for (i=0; i<pi->N_links; i++) {
	lcost += cost_for_length(pi->link_array[i].r - pi->link_array[i].l);
    }
    return lcost;
}

static int null_cost(Parse_info * pi) {
  /* computes the number of null links in the linkage */
  /* No one seems to care about this -- ALB */
  return 0;
}

static int unused_word_cost(Parse_info * pi) {
    int lcost, i;
    lcost =  0;
    for (i=0; i<pi->N_words; i++)
	lcost += (pi->chosen_disjuncts[i] == NULL);
    return lcost;
}


static int disjunct_cost(Parse_info * pi) {
/* computes the cost of the current parse of the current sentence     */
/* due to the cost of the chosen disjuncts                            */
    int lcost, i;
    lcost =  0;
    for (i=0; i<pi->N_words; i++) {
      if (pi->chosen_disjuncts[i] != NULL)
	lcost += pi->chosen_disjuncts[i]->cost;
    }
    return lcost;
}

static int strictly_smaller_name(char * s, char * t) {
/*
   Returns TRUE if string s represents a strictly smaller match set
   than does t.  An almost identical function appears in and.c.
   The difference is that here we don't require s and t to be the
   same length.
*/
    int strictness, ss, tt;
    strictness = 0;
    while ((*s!='\0') || (*t!='\0')) {
	if (*s == '\0') {
	    ss = '*';
	} else {
	    ss = *s;
	    s++;
	}
	if (*t == '\0') {
	    tt = '*';
	} else {
	    tt = *t;
	    t++;
	}
	if (ss == tt) continue;
	if ((tt == '*') || (ss == '^')) {
	    strictness++;
	} else {
	    return FALSE;
	}
    }
    return (strictness > 0);
}

static void compute_link_names(Sentence sent) {
/*
   The name of the link is set to be the GCD of the names of
   its two endpoints.
*/
    int i;
    Parse_info * pi = sent->parse_info;

    for (i=0; i<pi->N_links; i++) {
	pi->link_array[i].name = intersect_strings(sent,
	   pi->link_array[i].lc->string, pi->link_array[i].rc->string);
    }
}

static void compute_pp_link_names(Sentence sent, Sublinkage *sublinkage)
{
/*
   This fills in the sublinkage->link[].name field.  We assume that
   link_array[].name have already been filled in.  As above, in the
   standard case, the name is just the GCD of the two end points.
   If pluralization has occurred, then we want to use the name
   already in link_array[].name.  We detect this in two ways.
   If the endpoints don't match, then we know pluralization
   has occured.  If they do, but the name in link_array[].name
   is *less* restrictive, then pluralization must have occured.
*/
    int i;
    char * s;
    Parse_info * pi = sent->parse_info;

    for (i=0; i<pi->N_links; i++)
      {
	if (sublinkage->link[i]->l == -1) continue;
	if (!x_match(sublinkage->link[i]->lc, sublinkage->link[i]->rc))
	  replace_link_name(sublinkage->link[i], pi->link_array[i].name);
	else
	  {
	    s = intersect_strings(sent, sublinkage->link[i]->lc->string,
				  sublinkage->link[i]->rc->string);
	    if (strictly_smaller_name(s, pi->link_array[i].name))
	      replace_link_name(sublinkage->link[i], pi->link_array[i].name);
	    else replace_link_name(sublinkage->link[i], s);
	  }
    }
}

/********************** exported functions *****************************/

Linkage_info analyze_fat_linkage(Sentence sent, Parse_Options opts, int analyze_pass) {
  /* This uses link_array.  It enumerates and post-processes
     all the linkages represented by this one.  We know this contains
     at least one fat link. */
    int i;
    Linkage_info li;
    DIS_node *d_root;
    PP_node *pp;
    Postprocessor *postprocessor;
    Sublinkage *sublinkage;
    Parse_info * pi = sent->parse_info;
    PP_node accum;               /* for domain ancestry check */
    D_type_list * dtl0, * dtl1;  /* for domain ancestry check */

    sublinkage = x_create_sublinkage(pi);
    postprocessor = sent->dict->postprocessor;
    build_digraph(pi);
    structure_violation = FALSE;
    d_root = build_DIS_CON_tree(pi); /* may set structure_violation to TRUE */

    li.N_violations = 0;
    li.improper_fat_linkage = structure_violation;
    li.inconsistent_domains = FALSE;
    li.unused_word_cost = unused_word_cost(sent->parse_info);
    li.disjunct_cost = disjunct_cost(pi);
    li.null_cost = null_cost(pi);
    li.link_cost = link_cost(pi);

    if (structure_violation) {
	li.N_violations++;
	li.and_cost = 0;
	free_sublinkage(sublinkage);
	free_digraph(pi);
	free_DIS_tree(d_root);
	return li;
    }

    li.and_cost = and_cost(sent);
    compute_link_names(sent);

    for (i=0; i<pi->N_links; i++) accum.d_type_array[i] = NULL;

    for (;;) {	/* loop through all the sub linkages */
	for (i=0; i<pi->N_links; i++) {
	    patch_array[i].used = patch_array[i].changed = FALSE;
	    patch_array[i].newl = pi->link_array[i].l;
	    patch_array[i].newr = pi->link_array[i].r;
	    copy_full_link(&sublinkage->link[i], &(pi->link_array[i]));
	}
	fill_patch_array_DIS(d_root, NULL);

	for (i=0; i<pi->N_links; i++) {
	    if (patch_array[i].changed || patch_array[i].used) {
		sublinkage->link[i]->l = patch_array[i].newl;
		sublinkage->link[i]->r = patch_array[i].newr;
	    }
	    else if ((dfs_root_word[pi->link_array[i].l] != -1) &&
		     (dfs_root_word[pi->link_array[i].r] != -1)) {
		sublinkage->link[i]->l = -1;
	    }
	}

	compute_pp_link_array_connectors(sent, sublinkage);
	compute_pp_link_names(sent, sublinkage);

	/* 'analyze_pass' logic added ALB 1/97 */
	if (analyze_pass==PP_FIRST_PASS) {
	    post_process_scan_linkage(postprocessor,opts,sent,sublinkage);
	    if (!advance_DIS(d_root)) break;
	    else continue;
	}

	pp = post_process(postprocessor, opts, sent, sublinkage);

	if (pp==NULL) {
	    if (postprocessor != NULL) li.N_violations = 1;
	}
	else if (pp->violation == NULL)  {
	    /* the purpose of this stuff is to make sure the domain
	       ancestry for a link in each of its sentences is consistent.

	       Here's an example where this eliminates a linksge....

	       I wonder who John and Fred said Joe saw

                                 +-------------B*w-------------+
                    +-Sp*i-+-QI*d+    +-----------Ss-----------+
                    |      |     |    |                        |
                   I.p wonder.v who John and Fred said.v Joe saw.v

                              /////          RW      <---RW---->  RW        /////
                    (m)       /////          Wd      <---Wd---->  Wd        I.p
                    (m)       I.p            Sp*i    <---Sp*i-->  Sp        wonder.v
                    (m)       wonder.v       QI      <---QI*d-->  QI*d      who
                    (m) (s)   who            B*w     <---B*w--->  B         saw.v
                    (m) (s)   John           Ss      <---Ss---->  S         saw.v


                                 +-------------B*w-------------+
                    +-Sp*i-+-QI*d+             +--Ss-+-Ce-+-Ss-+
                    |      |     |             |     |    |    |
                   I.p wonder.v who John and Fred said.v Joe saw.v

                                  /////          RW      <---RW---->  RW        /////
                    (m)           /////          Wd      <---Wd---->  Wd        I.p
                    (m)           I.p            Sp*i    <---Sp*i-->  Sp        wonder.v
                    (m)           wonder.v       QI      <---QI*d-->  QI*d      who
                    (m) (s) (e)   who            B*w     <---B*w--->  B         saw.v
                    (m) (s) (e)   Joe            Ss      <---Ss---->  S         saw.v
                    (m) (s)       said.v         Ce      <---Ce---->  C         Joe
                    (m) (s)       Fred           Ss      <---Ss---->  S         said.v

	    */

	    for (i=0; i<pi->N_links; i++) {
		if (sublinkage->link[i]->l == -1) continue;
		if (accum.d_type_array[i] == NULL) {
		    accum.d_type_array[i] = copy_d_type(pp->d_type_array[i]);
		} else {
		    dtl0 = pp->d_type_array[i];
		    dtl1 = accum.d_type_array[i];
		    while((dtl0 != NULL) && (dtl1 != NULL) && (dtl0->type == dtl1->type)) {
			dtl0 = dtl0->next;
			dtl1 = dtl1->next;
		    }
		    if ((dtl0 != NULL) || (dtl1 != NULL)) break;
		}
	    }
	    if (i != pi->N_links) {
		li.N_violations++;
		li.inconsistent_domains = TRUE;
	    }
	}
	else if (pp->violation!=NULL) {
	    li.N_violations++;
	}

	if (!advance_DIS(d_root)) break;
    }

    for (i=0; i<pi->N_links; ++i) {
	free_d_type(accum.d_type_array[i]);
    }

    /* if (display_on && (li.N_violations != 0) &&
       (verbosity > 3) && should_print_messages)
       printf("P.P. violation in one part of conjunction.\n"); */
    free_sublinkage(sublinkage);
    free_digraph(pi);
    free_DIS_tree(d_root);
    return li;
}

Linkage_info analyze_thin_linkage(Sentence sent, Parse_Options opts, int analyze_pass)
{
    /* This uses link_array.  It post-processes
       this linkage, and prints the appropriate thing.  There are no fat
       links in it. */
    int i;
    Linkage_info li;
    PP_node * pp;
    Postprocessor * postprocessor;
    Sublinkage *sublinkage;
    Parse_info * pi = sent->parse_info;

	memset(&li, 0, sizeof(li));
	//ZeroMemory(&li, sizeof(li));
    sublinkage = x_create_sublinkage(pi);
    postprocessor = sent->dict->postprocessor;
    compute_link_names(sent);
    for (i=0; i<pi->N_links; i++) {
      copy_full_link(&(sublinkage->link[i]), &(pi->link_array[i]));
    }

    if (analyze_pass==PP_FIRST_PASS) {
        post_process_scan_linkage(postprocessor, opts, sent, sublinkage);
	free_sublinkage(sublinkage);
        return li;
    }

    pp = post_process(postprocessor, opts, sent, sublinkage);

    li.N_violations = 0;
    li.and_cost = 0;
    li.unused_word_cost = unused_word_cost(sent->parse_info);
    li.improper_fat_linkage = FALSE;
    li.inconsistent_domains = FALSE;
    li.disjunct_cost = disjunct_cost(pi);
    li.null_cost = null_cost(pi);
    li.link_cost = link_cost(pi);

    if (pp==NULL) {
	if (postprocessor != NULL) li.N_violations = 1;
    } else if (pp->violation!=NULL) {
	li.N_violations++;
    }

    free_sublinkage(sublinkage);
    return li;
}

void extract_thin_linkage(Sentence sent, Parse_Options opts, Linkage linkage)
{
    int i;
    Sublinkage *sublinkage;
    Parse_info * pi = sent->parse_info;

    sublinkage = x_create_sublinkage(pi);
    compute_link_names(sent);
    for (i=0; i<pi->N_links; i++) {
	copy_full_link(&sublinkage->link[i],&(pi->link_array[i]));
    }

    linkage->num_sublinkages = 1;
    linkage->sublinkage = ex_create_sublinkage(pi);

    for (i=0; i<pi->N_links; ++i) {
	linkage->sublinkage->link[i] = excopy_link(sublinkage->link[i]);
    }

    free_sublinkage(sublinkage);
}


void extract_fat_linkage(Sentence sent, Parse_Options opts, Linkage linkage) {
  /* This procedure mimics analyze_fat_linkage in order to
     extract the sublinkages and copy them to the Linkage
     data structure passed in.
     */
    int i, j, N_thin_links;
    DIS_node *d_root;
    int num_sublinkages;
    Sublinkage * sublinkage;
    Parse_info * pi = sent->parse_info;

    sublinkage = x_create_sublinkage(pi);
    build_digraph(pi);
    structure_violation = FALSE;
    d_root = build_DIS_CON_tree(pi);

    if (structure_violation) {
	compute_link_names(sent);
	for (i=0; i<pi->N_links; i++) {
	    copy_full_link(&sublinkage->link[i],&(pi->link_array[i]));
	}

	linkage->num_sublinkages=1;
	linkage->sublinkage = ex_create_sublinkage(pi);

	/* This will have fat links! */
	for (i=0; i<pi->N_links; ++i) {
	    linkage->sublinkage->link[i] = excopy_link(sublinkage->link[i]);
	}

	free_sublinkage(sublinkage);
	free_digraph(pi);
	free_DIS_tree(d_root);
	return;
    }
    
    /* first get number of sublinkages and allocate space */
    num_sublinkages = 0;
    for (;;) {
	num_sublinkages++;
	if (!advance_DIS(d_root)) break; 
    }
    
    linkage->num_sublinkages = num_sublinkages;
    linkage->sublinkage = 
	(Sublinkage *) exalloc(sizeof(Sublinkage)*num_sublinkages);
    for (i=0; i<num_sublinkages; ++i) {
	linkage->sublinkage[i].pp_info = NULL;
	linkage->sublinkage[i].violation = NULL;
    }
    
    /* now fill out the sublinkage arrays */
    compute_link_names(sent);
    
    num_sublinkages = 0;
    for (;;) {
	for (i=0; i<pi->N_links; i++) {
	    patch_array[i].used = patch_array[i].changed = FALSE;
	    patch_array[i].newl = pi->link_array[i].l;
	    patch_array[i].newr = pi->link_array[i].r;
	    copy_full_link(&sublinkage->link[i], &(pi->link_array[i]));
	}
	fill_patch_array_DIS(d_root, NULL);
	
	for (i=0; i<pi->N_links; i++) {
	    if (patch_array[i].changed || patch_array[i].used) {
		sublinkage->link[i]->l = patch_array[i].newl;
		sublinkage->link[i]->r = patch_array[i].newr;
	    } else if ((dfs_root_word[pi->link_array[i].l] != -1) &&
		       (dfs_root_word[pi->link_array[i].r] != -1)) {
		sublinkage->link[i]->l = -1;
	    }
	}
	
	compute_pp_link_array_connectors(sent, sublinkage);
	compute_pp_link_names(sent, sublinkage);
	
	/* Don't copy the fat links into the linkage */
	N_thin_links = 0;
	for (i= 0; i<pi->N_links; ++i) {
	    if (sublinkage->link[i]->l == -1) continue;
	    N_thin_links++;
	}
	
	linkage->sublinkage[num_sublinkages].num_links = N_thin_links;
	linkage->sublinkage[num_sublinkages].link = 
	    (Link *) exalloc(sizeof(Link)*N_thin_links);
	linkage->sublinkage[num_sublinkages].pp_info = NULL;
	linkage->sublinkage[num_sublinkages].violation = NULL;
	
	for (i=0, j=0; i<pi->N_links; ++i) {
	    if (sublinkage->link[i]->l == -1) continue;
	    linkage->sublinkage[num_sublinkages].link[j++] = 
		excopy_link(sublinkage->link[i]);
	}
	
	
	num_sublinkages++;
	if (!advance_DIS(d_root)) break; 
    }
    
    free_sublinkage(sublinkage);
    free_digraph(pi);
    free_DIS_tree(d_root);
}



