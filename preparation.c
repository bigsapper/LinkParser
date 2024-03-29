 /***************************************************************************/
 /*                                                                         */
 /*       Copyright (C) 1991-1998  Daniel Sleator and Davy Temperley        */
 /*  See file "README" for information about commercial use of this system  */
 /*                                                                         */
 /***************************************************************************/

#include "link-includes.h"

static int null_links;

void print_statistics() {
}

void free_deletable(Sentence sent) {
    int w;
    if (sent->deletable != NULL) {
	for (w = -1; w<sent->length; w++) {
	    xfree((char *)sent->deletable[w],sizeof(char)*(sent->length+1));
	}
	sent->deletable--;
	xfree((char *) sent->deletable, (sent->length+1)*sizeof(char *));
	sent->deletable = NULL;
    }
}

void build_deletable(Sentence sent, int has_conjunction) {
/* Initialize the array deletable[i][j] to indicate if the words           */
/* i+1...j-1 could be non existant in one of the multiple linkages.  This  */
/* array is used in conjunction_prune and power_prune.  Regions of length  */
/* 0 are always deletable.  A region of length two with a conjunction at   */
/* one end is always deletable.  Another observation is that for the       */
/* comma to form the right end of a deletable region, it must be the case  */
/* that there is a conjunction to the right of the comma.  Also, when      */
/* considering deletable regions with a comma on their left sides, there   */
/* must be a conjunction inside the region to be deleted. Finally, the     */
/* words "either", "neither", "both", "not" and "not only" are all         */
/* deletable.                                                              */
   
    int i,j,k;

    free_deletable(sent);
    assert(sent->length < MAX_SENTENCE, "sent->length too big");

    sent->deletable = (char **) xalloc((sent->length+1)*sizeof(char *));
    sent->deletable++;  /* we need to be able to access the [-1] position in this array */

    for (i = -1; i<sent->length; i++) {
	sent->deletable[i] = (char *) xalloc(sent->length+1);  
	/* the +1 is to allow us to have the info for the last word
	   read the comment above */
	for (j=0; j<= sent->length; j++) {
	    if (j == i+1) {
		sent->deletable[i][j] = TRUE;
	    } else if (null_links) {
		sent->deletable[i][j] = TRUE;
	    } else if (!has_conjunction) {
		sent->deletable[i][j] = FALSE;
	    } else if ((j>i+2)&&(sent->is_conjunction[i+1] || 
				 sent->is_conjunction[j-1] ||
                                (strcmp(",",sent->word[i+1].string)==0 && 
				 conj_in_range(sent, i+2,j-1)) ||
 	                        (strcmp(",",sent->word[j-1].string)==0 && 
				 conj_in_range(sent, j,sent->length-1)))){
		sent->deletable[i][j] = TRUE;
	    } else if (j > i) {
		for (k=i+1; k<j; k++) {
		    if ((strcmp("either", sent->word[k].string) == 0) ||
			(strcmp("neither", sent->word[k].string) == 0) ||
			(strcmp("both", sent->word[k].string) == 0) ||
			(strcmp("not", sent->word[k].string) == 0)) continue;
		    if ((strcmp("only", sent->word[k].string)==0) && (k > i+1) &&
                                   (strcmp("not", sent->word[k-1].string)==0)) continue;
		    break;
		}
		sent->deletable[i][j] = (k==j);
	    } else {
		sent->deletable[i][j] = FALSE;
	    }
	}
    }
}

void free_effective_dist(Sentence sent) {
    int w;
    if (sent->effective_dist != NULL) {
	for (w=0; w<sent->length; w++) {
	    xfree((char *)sent->effective_dist[w],sizeof(char)*(sent->length+1));
	}
	xfree((char *) sent->effective_dist, sizeof(char *)*(sent->length));
	sent->effective_dist = NULL;
    }
}

void build_effective_dist(Sentence sent, int has_conjunction) {
  /*
    The "effective distance" between two words is the actual distance minus
    the largest deletable region strictly between the two words.  If the
    effective distance between two words is greater than a connector's max
    link length, then that connector cannot be satisfied by linking these
    two words.

    [Note: The effective distance is not monotically increasing as you move
    away from a word.]

    This function creates effective_dist[][].  It assumes that deleteble[][]
    has already been computed.

    Dynamic programming is used to compute this.  The order used is smallest
    region to largest.

    Just as deletable[i][j] is constructed for j=N_words (which is one
    off the end of the sentence) we do that for effective_dist[][].
  */

    int i, j, diff;

    free_effective_dist(sent);
    sent->effective_dist = (char **) xalloc((sent->length)*sizeof(char *));

    for (i=0; i<sent->length; i++) {
	sent->effective_dist[i] = (char *) xalloc(sent->length+1);
    }
    for (i=0; i<sent->length; i++) {  
	/* Fill in the silly part */
	for (j=0; j<=i; j++) {
	    sent->effective_dist[i][j] = j-i;
	}
    }

    /* what is the rationale for ignoring the effective_dist 
       if null links are allowed? */
    if (null_links) {  
	for (i=0; i<sent->length; i++) {  
	    for (j=0; j<=sent->length; j++) {
		sent->effective_dist[i][j] = j-i;
	    }
	}
    } 
    else {
	for (diff = 1; diff < sent->length; diff++) {
	    for (i=0; i+diff <= sent->length; i++) {
		j = i+diff;
		if (sent->deletable[i][j]) {  /* note that deletable[x][x+1] is TRUE */
		    sent->effective_dist[i][j] = 1;		    
		} else {
		    sent->effective_dist[i][j] = 1 + MIN(sent->effective_dist[i][j-1],sent->effective_dist[i+1][j]);
		}
	    }
	}

	/* now when you link to a conjunction, your effective length is 1 */

	for (i=0; i<sent->length; i++) {  
	    for (j=i+1; j<sent->length; j++) {
		if (sent->is_conjunction[i] || 
		    sent->is_conjunction[j]) sent->effective_dist[i][j] = 1;
	    }
	}
    }

    /* sent->effective_dist[i][i] should be 0 */

    /*      
    for (j=0; j<=sent->length; j++) {
      printf("%4d", j);
    }
    printf("\n");
    for (i=0; i<sent->length; i++) {  
      for (j=0; j<=sent->length; j++) {
	printf("%4d", sent->effective_dist[i][j]);
      }
      printf("\n");
    }
    */
}

void install_fat_connectors(Sentence sent) {
/* Installs all the special fat disjuncts on all of the words of the   */
/* sentence */  
    int i;
    for (i=0; i<sent->length; i++) {
	if (sent->is_conjunction[i]) {
	    sent->word[i].d = catenate_disjuncts(sent->word[i].d,
			       build_AND_disjunct_list(sent, sent->word[i].string));
	} else {
	    sent->word[i].d = catenate_disjuncts(sent->word[i].d,
			       explode_disjunct_list(sent, sent->word[i].d));
	    if (strcmp(sent->word[i].string, ",") == 0) {
		sent->word[i].d = catenate_disjuncts(sent->word[i].d,
					       build_COMMA_disjunct_list(sent));
	    }
	}
    }
}

static void set_connector_list_length_limit(Connector *c, Connector_set *conset, int short_len, Parse_Options opts) {
    for (; c!=NULL; c=c->next) {
        if (parse_options_get_all_short_connectors(opts)) {
	    c->length_limit = short_len;
	}
	else if (conset == NULL || match_in_connector_set(conset, c, '+')) {
	    c->length_limit = UNLIMITED_LEN;	    
	} else {
	    c->length_limit = short_len;
	}
    }
}

static void set_connector_length_limits(Sentence sent, Parse_Options opts) {
    int i;
    int len;
    Disjunct *d;

    len = opts->short_length;
    if (len > UNLIMITED_LEN) len = UNLIMITED_LEN;
    
    for (i=0; i<sent->length; i++) {
	for (d = sent->word[i].d; d != NULL; d = d->next) {
	    set_connector_list_length_limit(d->left, sent->dict->unlimited_connector_set, len, opts);
	    set_connector_list_length_limit(d->right, sent->dict->unlimited_connector_set, len, opts);
	}
    }
}

void free_sentence_expressions(Sentence sent) {
  int i;
  for (i=0; i<sent->length; i++) {
    free_X_nodes(sent->word[i].x);
  }
}

void free_sentence_disjuncts(Sentence sent) {
    int i;

    for (i=0; i<sent->length; ++i) {
	free_disjuncts(sent->word[i].d);
	sent->word[i].d = NULL;
    }
    if (sentence_contains_conjunction(sent)) free_AND_tables(sent);
}


void prepare_to_parse(Sentence sent, Parse_Options opts) {
/* assumes that the sentence expression lists have been generated     */
/* this does all the necessary pruning and building of and            */
/* structures.                                                        */
    int i, has_conjunction;

    build_sentence_disjuncts(sent, opts->disjunct_cost);
    if (verbosity > 2) {
	printf("After expanding expressions into disjuncts:") ;
	print_disjunct_counts(sent);
    }
    print_time(opts, "Built disjuncts");
    
    for (i=0; i<sent->length; i++) {
	sent->word[i].d = eliminate_duplicate_disjuncts(sent->word[i].d);
    }
    print_time(opts, "Eliminated duplicate disjuncts");

    if (verbosity > 2) {
	printf("\nAfter expression pruning and duplicate elimination:\n");
	print_disjunct_counts(sent);
    }

    null_links = (opts->min_null_count > 0);

    has_conjunction = sentence_contains_conjunction(sent);
    set_connector_length_limits(sent, opts);
    build_deletable(sent, has_conjunction);
    build_effective_dist(sent, has_conjunction);  
    /* why do we do these here instead of in
       first_prepare_to_parse() only?  The
       reason is that the deletable region
       depends on if null links are in use.
       with null_links everything is deletable */

    if (!has_conjunction) {
	pp_and_power_prune(sent, RUTHLESS, opts);
    } else {
	pp_and_power_prune(sent, GENTLE, opts);
	/*
	if (verbosity > 2) {
	    printf("\nAfter Gentle power pruning:\n");
	    print_disjunct_counts(sent);
	}
	*/
	/*print_time(opts, "Finished gentle power pruning"); */
	conjunction_prune(sent, opts);
	if (verbosity > 2) {
	    printf("\nAfter conjunction pruning:\n");
	    print_disjunct_counts(sent);
	    print_statistics();
	}
	print_time(opts, "Done conjunction pruning");
	build_conjunction_tables(sent);
	install_fat_connectors(sent);
	install_special_conjunctive_connectors(sent);
	if (verbosity > 2) {
	    printf("After conjunctions, disjuncts counts:\n");
	    print_disjunct_counts(sent);
	}
	set_connector_length_limits(sent, opts);
              /* have to do this again cause of the
	         new fat connectors and disjuncts */

	print_time(opts, "Constructed fat disjuncts");
	
	prune(sent);
	print_time(opts, "Pruned fat disjuncts");
	
	for (i=0; i<sent->length; i++) {
	    sent->word[i].d = eliminate_duplicate_disjuncts(sent->word[i].d);
	}
	if (verbosity > 2) {
	    printf("After pruning and duplicate elimination:\n");
	    print_disjunct_counts(sent);
	}
	print_time(opts, "Eliminated duplicate disjuncts (again)");
	
	if (verbosity > 2) print_AND_statistics(sent);

	power_prune(sent, RUTHLESS, opts);
    }

    /*
    if (verbosity > 2) {
	printf("\nAfter RUTHLESS power-pruning:\n");
	print_disjunct_counts(sent);
    }
    */
    /* print time for power pruning used to be here */
    /* now done in power_prune itself */
    print_time(opts, "Initialized fast matcher and hash table");
}
