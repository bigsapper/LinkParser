 /***************************************************************************/
 /*                                                                         */
 /*       Copyright (C) 1991-1998  Daniel Sleator and Davy Temperley        */
 /*  See file "README" for information about commercial use of this system  */
 /*                                                                         */
 /***************************************************************************/

#include "link-includes.h"

#define MAX_STRIP 10

static char * strip_left[] = {"(", "$", "``", NULL};
static char * strip_right[] = {")", "%", ",", ".", ":", ";", "?", "!", "''", "'", "'s", NULL};

static int is_initials_word(char * word) {
    /* This is rather esoteric and not terribly important.
       It returns TRUE if the word matches the pattern /[A-Z]\.]+/
       */
    int i;
    for (i=0; word[i] != '\0'; i += 2) {
	if (!isupper(word[i])) return FALSE;
	if (word[i+1] != '.') return FALSE;
    }
    return TRUE;
}

static int is_number(char * s) {
    if (!isdigit(*s)) return FALSE;
    for (; *s != '\0'; s++) {
	if (isdigit(*s) || (*s == '.') || (*s == ',') || (*s == ':')) continue;
	/* The ":" is included here so we allow "10:30" to be a number. */
	return FALSE;
    }
    return TRUE;
}

static int ishyphenated(char * s) {
/* returns TRUE iff it's an an appropritenly formed hyphenated word
   This means all letters, numbers, or hyphens, not beginning and
   ending with a hyphen.
*/
    int hyp, nonalpha;
    hyp = nonalpha = 0;
    if (*s == '-') return FALSE;
    while (*s != '\0') {
	if (!isalpha(*s) && !isdigit(*s) && (*s!='.') && (*s!=',')
	    && (*s!='-')) return FALSE;
	if (*s == '-') hyp++;
	s++;
    }
    return ((*(s-1)!='-') && (hyp>0));
}


static int issue_sentence_word(Sentence sent, char * s) {
    /* the string s is the next word of the sentence do not issue the empty
     * string.  return false if too many words or the word is too long.  */
    if (*s == '\0') return TRUE;
    if (strlen(s) > MAX_WORD) {
	lperror(SEPARATE, 
		". The word \"%s\" is too long.\n"  
		"A word can have a maximum of %d characters.\n", s, MAX_WORD);
	return FALSE;
    }

    if (sent->length == MAX_SENTENCE) {
	lperror(SEPARATE, ". The sentence has too many words.\n");
	return FALSE;
    }
    strcpy(sent->word[sent->length].string, s);
    sent->length++;
    return TRUE;
}

/*
    Here's a summary of how subscripts are handled:

    Reading the dictionary:

      If the last "." in a string is followed by a non-digit character,
      then the "." and everything after it is considered to be the subscript
      of the word.

      The dictionary reader does not allow you to have two words that
      match according to the criterion below.  (so you can't have
      "dog.n" and "dog")

      Quote marks are used to allow you to define words in the dictionary
      which would otherwise be considered part of the dictionary, as in

       ";": {@Xca-} & Xx- & (W+ or Qd+) & {Xx+};
       "%" : (ND- & {DD-} & <noun-sub-x> & 
	   (<noun-main-x> or B*x+)) or (ND- & (OD- or AN+));

    Rules for chopping words from the input sentence:

       First the prefix chars are stripped off of the word.  These
       characters are "(" and "$" (and now "``")

       Now, repeat the following as long as necessary:

	   Look up the word in the dictionary.
	   If it's there, the process terminates.

	   If it's not there and it ends in one of the right strippable
	   strings (see "right_strip") then remove the strippable string
	   and make it into a separate word.

	   If there is no strippable string, then the process terminates. 

    Rule for defining subscripts in input words:

       The subscript rule is followed just as when reading the dictionary.

    When does a word in the sentence match a word in the dictionary?

       Matching is done as follows: Two words with subscripts must match
       exactly.  If neither has a subscript they must match exactly.  If one
       does and one doesn't then they must match when the subscript is
       removed.  Notice that this is symmetric.

    So, under this system, the dictonary could have the words "Ill" and
    also the word "Ill."  It could also have the word "i.e.", which could be
    used in a sentence.
*/

static int separate_word(Sentence sent, char *w, char *wend, int is_first_word) {
    /* w points to a string, wend points to the char one after the end.  The
     * "word" w contains no blanks.  This function splits up the word if
     * necessary, and calls "issue_sentence_word()" on each of the resulting
     * parts.  The process is described above.  returns TRUE of OK, FALSE if
     * too many punctuation marks */
    int i, len, n_stripped;
    int stripped[MAX_STRIP];  /* these were stripped from the right */
    char word[MAX_WORD+1];
    
    for (;;) {
	for (i=0; strip_left[i] != NULL; i++) {
	    if (strncmp(w, strip_left[i], strlen(strip_left[i])) == 0) {
		if (!issue_sentence_word(sent, strip_left[i])) return FALSE;
		w += strlen(strip_left[i]);
		break;
	    }
	}
	if (strip_left[i] == NULL) break;
    }

    for (n_stripped = 0; n_stripped < MAX_STRIP; n_stripped++) {
	strncpy(word, w, wend-w);
	word[wend-w] = '\0';
	if (wend == w) break;  /* it will work without this */
	
	if (boolean_dictionary_lookup(sent->dict, word) || is_initials_word(word)) break;
	if (is_first_word && isupper(word[0])) {
	    word[0] = tolower(word[0]);
	    if (boolean_dictionary_lookup(sent->dict, word)) {
		word[0] = toupper(word[0]);  /* restore word to what it was */
		break;
	    }
	    word[0] = toupper(word[0]);
	}
	for (i=0; strip_right[i] != NULL; i++) {
	    len = strlen(strip_right[i]);
	    if ((wend-w) < len) continue;  /* the remaining w is too short for a possible match */
	    if (strncmp(wend-len, strip_right[i], len) == 0) {
		stripped[n_stripped] = i;
		wend -= len;
		break;
	    }
	}
	if (strip_right[i] == NULL) break;
    }
    
    /* word is now what remains after all the stripping has been done */
    
    if (n_stripped == MAX_STRIP) {
	lperror(SEPARATE, 
		".\n\"%s\" is followed by too many punctuation marks.\n", word);
	return FALSE;
    }
    
    if (!issue_sentence_word(sent, word)) return FALSE;
    for (i=n_stripped-1; i>=0; i--) {
	if (!issue_sentence_word(sent, strip_right[stripped[i]])) return FALSE;
    }
    return TRUE;
}

int separate_sentence(char * s, Sentence sent) {
    /* The string s has just been read in from standard input.
       This function breaks it up into words and stores these words in
       the sent->word[] array.  Returns TRUE if all is well, FALSE otherwise.
       Quote marks are treated just like blanks.
       */
    char *t;
    int is_first;
    Dictionary dict = sent->dict;
    
    sent->length = 0;
    
    if (dict->left_wall_defined) 
	if (!issue_sentence_word(sent, LEFT_WALL_WORD)) return FALSE;
    
    is_first = TRUE;
    for(;;) {
	while(isspace(*s) || (*s == '\"')) s++;
	if (*s == '\0') break;
	for (t=s; !((isspace(*t) || (*t == '\"')) || *t=='\0'); t++);
	if (!separate_word(sent, s, t, is_first)) return FALSE;
	is_first = FALSE;
	s = t;
	if (*s == '\0') break;
    }
    
    if (dict->right_wall_defined) 
	if (!issue_sentence_word(sent, RIGHT_WALL_WORD)) return FALSE;

    return (sent->length > dict->left_wall_defined + dict->right_wall_defined);
}

static int special_string(Sentence sent, int i, char * s) {
    X_node * e;
    if (boolean_dictionary_lookup(sent->dict, s)) {
	sent->word[i].x = build_word_expressions(sent, s);
	for (e = sent->word[i].x; e != NULL; e = e->next) {
	    e->string = sent->word[i].string;
	}
	return TRUE;
    } else {
	lperror(BUILDEXPR, ".\n To process this sentence your dictionary "
		"needs the word \"%s\".\n", s);
	return FALSE;
    }
}

static void handle_unknown_word(Sentence sent, int i, char * s) {
  /* puts into word[i].x the expression for the unknown word */
  /* the parameter s is the word that was not in the dictionary */    
  /* it massages the names to have the corresponding subscripts */
  /* to those of the unknown words */
  /* so "grok" becomes "grok[?].v"  */
    char *t,*u;
    X_node *d;
    char str[MAX_WORD+1];
    
    sent->word[i].x = build_word_expressions(sent, UNKNOWN_WORD);
    if (sent->word[i].x == NULL) 
	assert(FALSE, "UNKNOWN_WORD should have been there");
    
    for (d = sent->word[i].x; d != NULL; d = d->next) {
	t = strchr(d->string, '.');
	if (t != NULL) {
	    sprintf(str, "%.50s[?].%.5s", s, t+1);
	} else {
	    sprintf(str, "%.50s[?]", s);
	}
	t = (char *) xalloc(strlen(str)+1);
	strcpy(t,str);
	u = string_set_add(t, sent->string_set);
	xfree(t, strlen(str)+1);
	d->string = u;
    }
}

int build_sentence_expressions(Sentence sent) {
    /* Corrects case of first word, fills in other proper nouns, and
       builds the expression lists for the resulting words.
       
       Algorithm:  
       Apply the following step to all words w:
       if w is in the dictionary, use it.
       else if w is upper case use PROPER_WORD disjuncts for w.
       else if it's hyphenated, use HYPHENATED_WORD
       else if it's a number, use NUMBER_WORD.
       
       Now, we correct the first word, w.
       if w is upper case, let w' be the lower case version of w.
       if both w and w' are in the dict, concatenate these disjncts.
       else if w' is in dict, use disjuncts of w'
       else leave the disjuncts alone
       */
    int i, first_word;  /* the index of the first word after the wall */
    char *s, *u, temp_word[MAX_WORD+1];
    X_node * e;
    Dictionary dict = sent->dict;


    if (dict->left_wall_defined) {
	first_word = 1;
    } else {
	first_word = 0;
    }
    
    /* the following loop treats all words the same 
       (nothing special for 1st word) */
    for (i=0; i<sent->length; i++) {
	s = sent->word[i].string;
	if (boolean_dictionary_lookup(sent->dict, s)) {
	    sent->word[i].x = build_word_expressions(sent, s);
	} else if (isupper(s[0]) && dict->capitalized_word_defined) {
	    if (!special_string(sent, i, PROPER_WORD)) return FALSE;
	} else if (is_number(s) && dict->number_word_defined) {
	    /* we know it's a plural number, or 1 */
	    /* we'll only be here if 1's not in the dictionary */
	    if (!special_string(sent, i, NUMBER_WORD)) return FALSE;
	} else if (ishyphenated(s) && dict->hyphenated_word_defined) {
	    /* singular hyphenated */
	    if (!special_string(sent, i, HYPHENATED_WORD)) return FALSE;
	} else if (dict->unknown_word_defined && dict->use_unknown_word) {
	    /* Insert the unknown word here */
	    handle_unknown_word(sent, i, s);
	} else {
	    /* the reason I can assert this is that the word
	       should have been looked up alread if we get here. */
	    assert(FALSE, "I should have found that word.");
	}
    }

    /* now we make corrections for the 1st word */
    /* well, we now include words after a ":" too */

    for (i=0; i<sent->length; i++) {
	if (! (i==first_word || (i>0 && strcmp(":", sent->word[i-1].string)==0)) ) continue;
	s = sent->word[i].string;
	if (isupper(s[0])) {
	    safe_strcpy(temp_word, s, sizeof(temp_word));
	    temp_word[0] = tolower(temp_word[0]);
	    u = string_set_add(temp_word, sent->string_set);
	    if (boolean_dictionary_lookup(sent->dict, u)) {
		if (boolean_dictionary_lookup(sent->dict, s)) {
		    e = build_word_expressions(sent, u);
		    sent->word[i].x = 
			catenate_X_nodes(sent->word[i].x, e);
		} else {
		    s[0] = tolower(s[0]);
		    e = build_word_expressions(sent, s);
		    free_X_nodes(sent->word[i].x);
		    sent->word[i].x = e;
		}
	    }
	}
    }
    
    return TRUE;
}


/* This just looks up all the words in the sentence, and builds
   up an appropriate error message in case some are not there.
   It has no side effect on the sentence.  Returns TRUE if all
   went well.
 */
int sentence_in_dictionary(Sentence sent){
    int w, ok_so_far;
    char * s;
    Dictionary dict = sent->dict;
    char temp[1024];

    ok_so_far = TRUE;
    for (w=0; w<sent->length; w++) {
	s = sent->word[w].string;
	if (!boolean_dictionary_lookup(dict, s) &&
	    !(isupper(s[0])   && dict->capitalized_word_defined) &&
	    !(ishyphenated(s) && dict->hyphenated_word_defined)  &&
	    !(is_number(s)     && dict->number_word_defined)) {
	    if (ok_so_far) {
		safe_strcpy(temp, "The following words are not in the dictionary:", sizeof(temp));
		ok_so_far = FALSE;
	    }
	    safe_strcat(temp, " \"", sizeof(temp));
	    safe_strcat(temp, sent->word[w].string, sizeof(temp));
	    safe_strcat(temp, "\"", sizeof(temp));
	}
    }
    if (!ok_so_far) {
	lperror(NOTINDICT, "\n%s\n", temp);
    }
    return ok_so_far;
}
