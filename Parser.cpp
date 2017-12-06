// Parser.cpp : Implementation of CParser

#include "stdafx.h"
#include "LinkParser.h"
#include "Parser.h"

#define DB_SQL 0
#define DB_FILE 1
#define NUMTABLES 52

// the ORDER BY is important to ensure the dictionary files are properly constructed
char * DictionaryNames[NUMTABLES][2] =
    {   { "SELECT SeqNo, StringData FROM 3_0_dict ORDER BY SeqNo","3.0.dict" },
        { "SELECT SeqNo, StringData FROM 3_0_knowledge ORDER BY SeqNo","3.0.knowledge" },
        { "SELECT SeqNo, StringData FROM words_adj_1 ORDER BY SeqNo","Words\\words.adj.1" },
        { "SELECT SeqNo, StringData FROM words_adj_2 ORDER BY SeqNo","Words\\words.adj.2" },
        { "SELECT SeqNo, StringData FROM words_adj_3 ORDER BY SeqNo","Words\\words.adj.3" },
        { "SELECT SeqNo, StringData FROM words_adv_1 ORDER BY SeqNo","Words\\words.adv.1" },
        { "SELECT SeqNo, StringData FROM words_adv_2 ORDER BY SeqNo","Words\\words.adv.2" },
        { "SELECT SeqNo, StringData FROM words_adv_3 ORDER BY SeqNo","Words\\words.adv.3" },
        { "SELECT SeqNo, StringData FROM words_n_1 ORDER BY SeqNo","Words\\words.n.1" },
        { "SELECT SeqNo, StringData FROM words_n_2_s ORDER BY SeqNo","Words\\words.n.2.s" },
        { "SELECT SeqNo, StringData FROM words_n_2_x ORDER BY SeqNo","Words\\words.n.2.x" },
        { "SELECT SeqNo, StringData FROM words_n_3 ORDER BY SeqNo","Words\\words.n.3" },
        { "SELECT SeqNo, StringData FROM words_n_4 ORDER BY SeqNo","Words\\words.n.4" },
        { "SELECT SeqNo, StringData FROM words_n_c_1 ORDER BY SeqNo","Words\\words.n.c.1" },
        { "SELECT SeqNo, StringData FROM words_n_c_2 ORDER BY SeqNo","Words\\words.n.c.2" },
        { "SELECT SeqNo, StringData FROM words_n_p ORDER BY SeqNo","Words\\words.n.p" },
        { "SELECT SeqNo, StringData FROM words_n_t ORDER BY SeqNo","Words\\words.n.t" },
        { "SELECT SeqNo, StringData FROM words_s ORDER BY SeqNo","Words\\words.s" },
        { "SELECT SeqNo, StringData FROM words_v_1_1 ORDER BY SeqNo","Words\\words.v.1.1" },
        { "SELECT SeqNo, StringData FROM words_v_1_2 ORDER BY SeqNo","Words\\words.v.1.2" },
        { "SELECT SeqNo, StringData FROM words_v_1_3 ORDER BY SeqNo","Words\\words.v.1.3" },
        { "SELECT SeqNo, StringData FROM words_v_1_4 ORDER BY SeqNo","Words\\words.v.1.4" },
        { "SELECT SeqNo, StringData FROM words_v_1_p ORDER BY SeqNo","Words\\words.v.1.p" },
        { "SELECT SeqNo, StringData FROM words_v_10_1 ORDER BY SeqNo","Words\\words.v.10.1" },
        { "SELECT SeqNo, StringData FROM words_v_10_2 ORDER BY SeqNo","Words\\words.v.10.2" },
        { "SELECT SeqNo, StringData FROM words_v_10_3 ORDER BY SeqNo","Words\\words.v.10.3" },
        { "SELECT SeqNo, StringData FROM words_v_10_4 ORDER BY SeqNo","Words\\words.v.10.4" },
        { "SELECT SeqNo, StringData FROM words_v_2_1 ORDER BY SeqNo","Words\\words.v.2.1" },
        { "SELECT SeqNo, StringData FROM words_v_2_2 ORDER BY SeqNo","Words\\words.v.2.2" },
        { "SELECT SeqNo, StringData FROM words_v_2_3 ORDER BY SeqNo","Words\\words.v.2.3" },
        { "SELECT SeqNo, StringData FROM words_v_2_4 ORDER BY SeqNo","Words\\words.v.2.4" },
        { "SELECT SeqNo, StringData FROM words_v_2_5 ORDER BY SeqNo","Words\\words.v.2.5" },
        { "SELECT SeqNo, StringData FROM words_v_4_1 ORDER BY SeqNo","Words\\words.v.4.1" },
        { "SELECT SeqNo, StringData FROM words_v_4_2 ORDER BY SeqNo","Words\\words.v.4.2" },
        { "SELECT SeqNo, StringData FROM words_v_4_3 ORDER BY SeqNo","Words\\words.v.4.3" },
        { "SELECT SeqNo, StringData FROM words_v_4_4 ORDER BY SeqNo","Words\\words.v.4.4" },
        { "SELECT SeqNo, StringData FROM words_v_4_5 ORDER BY SeqNo","Words\\words.v.4.5" },
        { "SELECT SeqNo, StringData FROM words_v_5_1 ORDER BY SeqNo","Words\\words.v.5.1" },
        { "SELECT SeqNo, StringData FROM words_v_5_2 ORDER BY SeqNo","Words\\words.v.5.2" },
        { "SELECT SeqNo, StringData FROM words_v_5_3 ORDER BY SeqNo","Words\\words.v.5.3" },
        { "SELECT SeqNo, StringData FROM words_v_5_4 ORDER BY SeqNo","Words\\words.v.5.4" },
        { "SELECT SeqNo, StringData FROM words_v_6_1 ORDER BY SeqNo","Words\\words.v.6.1" },
        { "SELECT SeqNo, StringData FROM words_v_6_2 ORDER BY SeqNo","Words\\words.v.6.2" },
        { "SELECT SeqNo, StringData FROM words_v_6_3 ORDER BY SeqNo","Words\\words.v.6.3" },
        { "SELECT SeqNo, StringData FROM words_v_6_4 ORDER BY SeqNo","Words\\words.v.6.4" },
        { "SELECT SeqNo, StringData FROM words_v_6_5 ORDER BY SeqNo","Words\\words.v.6.5" },
        { "SELECT SeqNo, StringData FROM words_v_8_1 ORDER BY SeqNo","Words\\words.v.8.1" },
        { "SELECT SeqNo, StringData FROM words_v_8_2 ORDER BY SeqNo","Words\\words.v.8.2" },
        { "SELECT SeqNo, StringData FROM words_v_8_3 ORDER BY SeqNo","Words\\words.v.8.3" },
        { "SELECT SeqNo, StringData FROM words_v_8_4 ORDER BY SeqNo","Words\\words.v.8.4" },
        { "SELECT SeqNo, StringData FROM words_v_8_5 ORDER BY SeqNo","Words\\words.v.8.5" },
        { "SELECT SeqNo, StringData FROM words_y ORDER BY SeqNo","Words\\words.y" }  };

/////////////////////////////////////////////////////////////////////////////
// CParser
STDMETHODIMP CParser::DictionaryCreate(VARIANT_BOOL *pRetVal)
{
	m_Dictionary = NULL;
	HRESULT hRetVal;

	m_boolDictionary = ExpandDictionary();

	if ( m_boolDictionary == VARIANT_TRUE ) {
		m_Dictionary = dictionary_create(DictionaryNames[0][DB_FILE], DictionaryNames[1][DB_FILE]);

		if ( !m_Dictionary == NULL ) {
			m_boolDictionary = VARIANT_TRUE;
			hRetVal = S_OK;
		} else {
			m_boolDictionary = VARIANT_FALSE;
			hRetVal = S_FALSE;
		}
	} else {
		hRetVal = S_FALSE;
	}

	*pRetVal = m_boolDictionary;

	return hRetVal;
}

STDMETHODIMP CParser::DictionaryDelete()
{
	if ( m_boolDictionary == VARIANT_TRUE ) {
		dictionary_delete(m_Dictionary);
		m_boolDictionary = VARIANT_FALSE;
	}

	DeflateDictionary();

	return S_OK;
}

STDMETHODIMP CParser::DictionaryGetMaxCost(int *pRetVal)
{
	int iRetVal;

	if ( m_boolDictionary )
		iRetVal = dictionary_get_max_cost(m_Dictionary);
	else
		iRetVal = -1;

	*pRetVal = iRetVal;

	return S_OK;
}

STDMETHODIMP CParser::ParseOptionsCreate(VARIANT_BOOL *pRetVal)
{
	m_ParseOptions = parse_options_create();

	if ( !m_ParseOptions == NULL ) {
		m_boolParseOptions = VARIANT_TRUE;
	} else {
		m_boolParseOptions = VARIANT_FALSE;
	}

	*pRetVal = m_boolParseOptions;

	return S_OK;
}

STDMETHODIMP CParser::ParseOptionsDelete()
{
	if ( m_boolParseOptions == VARIANT_TRUE ) {
		parse_options_delete(m_ParseOptions);
		m_boolParseOptions = VARIANT_FALSE;
	}

	return S_OK;
}

STDMETHODIMP CParser::get_Verbosity(int *pVal)
{
	*pVal = parse_options_get_verbosity(m_ParseOptions);

	return S_OK;
}

STDMETHODIMP CParser::put_Verbosity(int newVal)
{
	parse_options_set_verbosity(m_ParseOptions, newVal);

	return S_OK;
}

STDMETHODIMP CParser::get_LinkageLimit(int *pVal)
{
	*pVal = parse_options_get_linkage_limit(m_ParseOptions);

	return S_OK;
}

STDMETHODIMP CParser::put_LinkageLimit(int newVal)
{
	parse_options_set_linkage_limit(m_ParseOptions, newVal);

	return S_OK;
}

STDMETHODIMP CParser::get_MinNullCount(int *pVal)
{
	*pVal = parse_options_get_min_null_count(m_ParseOptions);

	return S_OK;
}

STDMETHODIMP CParser::put_MinNullCount(int newVal)
{
	parse_options_set_min_null_count(m_ParseOptions, newVal);

	return S_OK;
}

STDMETHODIMP CParser::get_MaxNullCount(int *pVal)
{
	*pVal = parse_options_get_max_null_count(m_ParseOptions);

	return S_OK;
}

STDMETHODIMP CParser::put_MaxNullCount(int newVal)
{
	parse_options_set_max_null_count(m_ParseOptions, newVal);

	return S_OK;
}

STDMETHODIMP CParser::get_NullBlock(int *pVal)
{
	*pVal = parse_options_get_null_block(m_ParseOptions);

	return S_OK;
}

STDMETHODIMP CParser::put_NullBlock(int newVal)
{
	parse_options_set_null_block(m_ParseOptions, newVal);

	return S_OK;
}

STDMETHODIMP CParser::get_IslandsOK(short *pVal)
{
	*pVal = parse_options_get_islands_ok(m_ParseOptions);

	return S_OK;
}

STDMETHODIMP CParser::put_IslandsOK(short newVal)
{
	parse_options_set_islands_ok(m_ParseOptions, newVal);

	return S_OK;
}

STDMETHODIMP CParser::get_ShortLength(int *pVal)
{
	*pVal = parse_options_get_short_length(m_ParseOptions);

	return S_OK;
}

STDMETHODIMP CParser::put_ShortLength(int newVal)
{
	parse_options_set_short_length(m_ParseOptions, newVal);

	return S_OK;
}

STDMETHODIMP CParser::get_AllShort(short *pVal)
{
	*pVal = parse_options_get_all_short_connectors(m_ParseOptions);

	return S_OK;
}

STDMETHODIMP CParser::put_AllShort(short newVal)
{
	parse_options_set_all_short_connectors(m_ParseOptions, newVal);

	return S_OK;
}

STDMETHODIMP CParser::get_DisplayWalls(short *pVal)
{
	*pVal = parse_options_get_display_walls(m_ParseOptions);

	return S_OK;
}

STDMETHODIMP CParser::put_DisplayWalls(short newVal)
{
	parse_options_set_display_walls(m_ParseOptions, newVal);

	return S_OK;
}

STDMETHODIMP CParser::get_DisplayUnion(short *pVal)
{
	*pVal = parse_options_get_display_union(m_ParseOptions);

	return S_OK;
}

STDMETHODIMP CParser::put_DisplayUnion(short newVal)
{
	parse_options_set_display_union(m_ParseOptions, newVal);

	return S_OK;
}

STDMETHODIMP CParser::get_AllowNull(short *pVal)
{
	*pVal = parse_options_get_allow_null(m_ParseOptions);

	return S_OK;
}

STDMETHODIMP CParser::put_AllowNull(short newVal)
{
	parse_options_set_allow_null(m_ParseOptions, newVal);

	return S_OK;
}

STDMETHODIMP CParser::get_EchoOn(short *pVal)
{
	*pVal = parse_options_get_echo_on(m_ParseOptions);

	return S_OK;
}

STDMETHODIMP CParser::put_EchoOn(short newVal)
{
	parse_options_set_echo_on(m_ParseOptions, newVal);

	return S_OK;
}

STDMETHODIMP CParser::get_BatchMode(short *pVal)
{
	*pVal = parse_options_get_batch_mode(m_ParseOptions);

	return S_OK;
}

STDMETHODIMP CParser::put_BatchMode(short newVal)
{
	parse_options_set_batch_mode(m_ParseOptions, newVal);

	return S_OK;
}

STDMETHODIMP CParser::get_PanicMode(short *pVal)
{
	*pVal = parse_options_get_panic_mode(m_ParseOptions);

	return S_OK;
}

STDMETHODIMP CParser::put_PanicMode(short newVal)
{
	parse_options_set_panic_mode(m_ParseOptions, newVal);

	return S_OK;
}

STDMETHODIMP CParser::get_ScreenWidth(int *pVal)
{
	*pVal = parse_options_get_screen_width(m_ParseOptions);

	return S_OK;
}

STDMETHODIMP CParser::put_ScreenWidth(int newVal)
{
	parse_options_set_screen_width(m_ParseOptions, newVal);

	return S_OK;
}

STDMETHODIMP CParser::get_DisplayOn(short *pVal)
{
	*pVal = parse_options_get_display_on(m_ParseOptions);

	return S_OK;
}

STDMETHODIMP CParser::put_DisplayOn(short newVal)
{
	parse_options_set_display_on(m_ParseOptions, newVal);

	return S_OK;
}

STDMETHODIMP CParser::get_DisplayPostscript(short *pVal)
{
	*pVal = parse_options_get_display_postscript(m_ParseOptions);

	return S_OK;
}

STDMETHODIMP CParser::put_DisplayPostscript(short newVal)
{
	parse_options_set_display_postscript(m_ParseOptions, newVal);

	return S_OK;
}

STDMETHODIMP CParser::get_DisplayBad(short *pVal)
{
	*pVal = parse_options_get_display_bad(m_ParseOptions);

	return S_OK;
}

STDMETHODIMP CParser::put_DisplayBad(short newVal)
{
	parse_options_set_display_bad(m_ParseOptions, newVal);

	return S_OK;
}

STDMETHODIMP CParser::get_DisplayLinks(short *pVal)
{
	*pVal = parse_options_get_display_links(m_ParseOptions);

	return S_OK;
}

STDMETHODIMP CParser::put_DisplayLinks(short newVal)
{
	parse_options_set_display_links(m_ParseOptions, newVal);

	return S_OK;
}

STDMETHODIMP CParser::get_DisjunctCost(int *pVal)
{
	*pVal = parse_options_get_disjunct_cost(m_ParseOptions);

	return S_OK;
}

STDMETHODIMP CParser::put_DisjunctCost(int newVal)
{
	parse_options_set_disjunct_cost(m_ParseOptions, newVal);

	return S_OK;
}

STDMETHODIMP CParser::get_MaxParseTime(int *pVal)
{
	*pVal = parse_options_get_max_parse_time(m_ParseOptions);

	return S_OK;
}

STDMETHODIMP CParser::put_MaxParseTime(int newVal)
{
	parse_options_set_max_parse_time(m_ParseOptions, newVal);

	return S_OK;
}

STDMETHODIMP CParser::get_MaxMemory(int *pVal)
{
	*pVal = parse_options_get_max_memory(m_ParseOptions);

	return S_OK;
}

STDMETHODIMP CParser::put_MaxMemory(int newVal)
{
	parse_options_set_max_memory(m_ParseOptions, newVal);

	return S_OK;
}

STDMETHODIMP CParser::get_TimerExpired(short *pVal)
{
	short RetVal = parse_options_timer_expired(m_ParseOptions);

	*pVal = RetVal;

	return S_OK;
}

STDMETHODIMP CParser::get_MemoryExhausted(short *pVal)
{
	short RetVal = parse_options_memory_exhausted(m_ParseOptions);

	*pVal = RetVal;

	return S_OK;
}

STDMETHODIMP CParser::get_ResourcesExhausted(short *pVal)
{
	short RetVal = parse_options_resources_exhausted(m_ParseOptions);

	*pVal = RetVal;

	return S_OK;
}

STDMETHODIMP CParser::SentenceCreate(BSTR Sentence, VARIANT_BOOL *pRetVal)
{
	USES_CONVERSION;
	char * lpszSentence = OLE2A(Sentence);

	if ( (m_boolDictionary == VARIANT_TRUE) && (lpszSentence != NULL) ) {
		m_Sentence = sentence_create(lpszSentence, m_Dictionary);

		if ( !m_Sentence == NULL ) {
			m_boolSentence = VARIANT_TRUE;
		} else {
			m_boolSentence = VARIANT_FALSE;
		}
	} else {
		m_boolSentence = VARIANT_FALSE;
	}

	*pRetVal = m_boolSentence;

	return S_OK;
}

STDMETHODIMP CParser::SentenceDelete()
{
	if ( m_boolSentence == VARIANT_TRUE ) {
		sentence_delete(m_Sentence);
		m_boolSentence = VARIANT_FALSE;
	}

	return S_OK;
}

STDMETHODIMP CParser::SentenceParse(int *pRetVal)
{
	int iRetVal;

	if ( m_boolSentence && m_boolParseOptions )
		iRetVal = sentence_parse(m_Sentence, m_ParseOptions);
	else
		iRetVal = -1;

	*pRetVal = iRetVal;

	return S_OK;
}

STDMETHODIMP CParser::get_Dictionary(VARIANT_BOOL *pVal)
{
	*pVal = m_boolDictionary;

	return S_OK;
}

STDMETHODIMP CParser::get_Sentence(VARIANT_BOOL *pVal)
{
	*pVal = m_boolSentence;

	return S_OK;
}

STDMETHODIMP CParser::get_ParseOptions(VARIANT_BOOL *pVal)
{
	*pVal = m_boolParseOptions;

	return S_OK;
}

STDMETHODIMP CParser::get_Linkage(VARIANT_BOOL *pVal)
{
	*pVal = m_boolLinkage;

	return S_OK;
}

STDMETHODIMP CParser::get_PostProcessor(VARIANT_BOOL *pVal)
{
	*pVal = m_boolPostProcessor;

	return S_OK;
}

STDMETHODIMP CParser::SentenceLength(int *pRetVal)
{
	int iRetVal;

	if ( m_boolSentence )
		iRetVal = sentence_length(m_Sentence);
	else
		iRetVal = -1;

	*pRetVal = iRetVal;

	return S_OK;
}

STDMETHODIMP CParser::SentenceGetWord(int Index, BSTR *pRetVal)
{
	char * lpszRetVal = NULL;

	if ( m_boolSentence )
		lpszRetVal = sentence_get_word(m_Sentence, Index);

	*pRetVal = A2BSTR(lpszRetVal);

	return *pRetVal ? S_OK : E_OUTOFMEMORY;
}

STDMETHODIMP CParser::SentenceNullCount(int *pRetVal)
{
	int iRetVal;

	if ( m_boolSentence )
		iRetVal = sentence_null_count(m_Sentence);
	else
		iRetVal = -1;

	*pRetVal = iRetVal;

	return S_OK;
}

STDMETHODIMP CParser::SentenceNumLinkagesFound(int *pRetVal)
{
	int iRetVal;

	if ( m_boolSentence )
		iRetVal = sentence_num_linkages_found(m_Sentence);
	else
		iRetVal = -1;

	*pRetVal = iRetVal;

	return S_OK;
}

STDMETHODIMP CParser::SentenceNumValidLinkages(int *pRetVal)
{
	int iRetVal;

	if ( m_boolSentence )
		iRetVal = sentence_num_valid_linkages(m_Sentence);
	else
		iRetVal = -1;

	*pRetVal = iRetVal;

	return S_OK;
}

STDMETHODIMP CParser::SentenceNumLinkagesPostProcessed(int *pRetVal)
{
	int iRetVal;

	if ( m_boolSentence )
		iRetVal = sentence_num_linkages_post_processed(m_Sentence);
	else
		iRetVal = -1;

	*pRetVal = iRetVal;

	return S_OK;
}

STDMETHODIMP CParser::SentenceNumViolations(int Index, int *pRetVal)
{
	int iRetVal;

	if ( m_boolSentence )
		iRetVal = sentence_num_violations(m_Sentence, Index);
	else
		iRetVal = -1;

	*pRetVal = iRetVal;

	return S_OK;
}

STDMETHODIMP CParser::SentenceDisjunctCost(int Index, int *pRetVal)
{
	int iRetVal;

	if ( m_boolSentence )
		iRetVal = sentence_disjunct_cost(m_Sentence, Index);
	else
		iRetVal = -1;

	*pRetVal = iRetVal;

	return S_OK;
}

STDMETHODIMP CParser::LinkageCreate(int Index, VARIANT_BOOL *pRetVal)
{
	if ( (m_boolSentence == VARIANT_TRUE) && (m_boolParseOptions == VARIANT_TRUE) ) {
		m_Linkage = linkage_create(Index, m_Sentence, m_ParseOptions);

		if ( !m_Linkage == NULL ) {
			m_boolLinkage = VARIANT_TRUE;
		} else {
			m_boolLinkage = VARIANT_FALSE;
		}
	} else {
		m_boolLinkage = VARIANT_FALSE;
	}

	*pRetVal = m_boolLinkage;

	return S_OK;
}

STDMETHODIMP CParser::LinkageGetNumSublinkages(int *pRetVal)
{
	int iRetVal;

	if ( m_boolLinkage )
		iRetVal = linkage_get_num_sublinkages(m_Linkage);
	else
		iRetVal = -1;

	*pRetVal = iRetVal;

	return S_OK;
}

STDMETHODIMP CParser::LinkageSetCurrentSublinkage(int Index, short *pRetVal)
{
	bool boolRetVal;

	if ( m_boolLinkage ) {
		if ( linkage_set_current_sublinkage(m_Linkage, Index) == 1 ) {
			boolRetVal = true;
		} else {
			boolRetVal = false;
		}
	} else {
		boolRetVal = true;
	}

	*pRetVal = boolRetVal;

	return S_OK;
}

STDMETHODIMP CParser::LinkageComputeUnion(short *pRetVal)
{
	bool boolRetVal;

	if ( m_boolLinkage ) {
		if ( linkage_compute_union(m_Linkage) == 1 ) {
			boolRetVal = true;
		} else {
			boolRetVal = false;
		}
	} else {
		boolRetVal = false;
	}

	*pRetVal = boolRetVal;

	return S_OK;
}

STDMETHODIMP CParser::LinkageGetNumWords(int *pRetVal)
{
	int iRetVal;

	if ( m_boolLinkage )
		iRetVal = linkage_get_num_words(m_Linkage);
	else
		iRetVal = -1;

	*pRetVal = iRetVal;

	return S_OK;
}

STDMETHODIMP CParser::LinkageGetNumLinks(int *pRetVal)
{
	int iRetVal;

	if ( m_boolLinkage )
		iRetVal = linkage_get_num_links(m_Linkage);
	else
		iRetVal = -1;

	*pRetVal = iRetVal;

	return S_OK;
}

STDMETHODIMP CParser::LinkageGetLinkLength(int Index, int *pRetVal)
{
	int iRetVal;

	if ( m_boolLinkage )
		iRetVal = linkage_get_link_length(m_Linkage, Index);
	else
		iRetVal = -1;

	*pRetVal = iRetVal;

	return S_OK;
}

STDMETHODIMP CParser::LinkageGetLinkLWord(int Index, int *pRetVal)
{
	int iRetVal;

	if ( m_boolLinkage )
		iRetVal = linkage_get_link_lword(m_Linkage, Index);
	else
		iRetVal = -1;

	*pRetVal = iRetVal;

	return S_OK;
}

STDMETHODIMP CParser::LinkageGetLinkRWord(int Index, int *pRetVal)
{
	int iRetVal;

	if ( m_boolLinkage )
		iRetVal = linkage_get_link_rword(m_Linkage, Index);
	else
		iRetVal = -1;

	*pRetVal = iRetVal;

	return S_OK;
}

STDMETHODIMP CParser::LinkagePrintDiagram(BSTR *pRetVal)
{
	char * lpszRetVal = NULL;

	if ( m_boolLinkage )
		lpszRetVal = linkage_print_diagram(m_Linkage);

	*pRetVal = A2BSTR(lpszRetVal);

	return *pRetVal ? S_OK : E_OUTOFMEMORY;
}

STDMETHODIMP CParser::LinkagePrintPostscript(int Mode, BSTR *pRetVal)
{
	char * lpszRetVal = NULL;

	if ( m_boolLinkage )
		lpszRetVal = linkage_print_postscript(m_Linkage, Mode);

	*pRetVal = A2BSTR(lpszRetVal);

	return *pRetVal ? S_OK : E_OUTOFMEMORY;
}

STDMETHODIMP CParser::LinkagePrintLinksAndDomains(BSTR *pRetVal)
{
	char * lpszRetVal = NULL;

	if ( m_boolLinkage )
		lpszRetVal = linkage_print_links_and_domains(m_Linkage);

	*pRetVal = A2BSTR(lpszRetVal);

	return *pRetVal ? S_OK : E_OUTOFMEMORY;
}

STDMETHODIMP CParser::LinkageGetLinkLabel(int Index, BSTR *pRetVal)
{
	char * lpszRetVal = NULL;

	if ( m_boolLinkage )
		lpszRetVal = linkage_get_link_label(m_Linkage, Index);

	*pRetVal = A2BSTR(lpszRetVal);

	return *pRetVal ? S_OK : E_OUTOFMEMORY;
}

STDMETHODIMP CParser::LinkageGetLinkLLabel(int Index, BSTR *pRetVal)
{
	char * lpszRetVal = NULL;

	if ( m_boolLinkage )
		lpszRetVal = linkage_get_link_llabel(m_Linkage, Index);

	*pRetVal = A2BSTR(lpszRetVal);

	return *pRetVal ? S_OK : E_OUTOFMEMORY;
}

STDMETHODIMP CParser::LinkageGetLinkRLabel(int Index, BSTR *pRetVal)
{
	char * lpszRetVal = NULL;

	if ( m_boolLinkage )
		lpszRetVal = linkage_get_link_rlabel(m_Linkage, Index);

	*pRetVal = A2BSTR(lpszRetVal);

	return *pRetVal ? S_OK : E_OUTOFMEMORY;
}

STDMETHODIMP CParser::LinkageGetLinkNumDomains(int Index, int *pRetVal)
{
	int iRetVal;

	if ( m_boolLinkage )
		iRetVal = linkage_get_link_num_domains(m_Linkage, Index);
	else
		iRetVal = -1;

	*pRetVal = iRetVal;

	return S_OK;
}

STDMETHODIMP CParser::LinkageGetLinkDomainNames(int Index, BSTR Delimiter, BSTR *pRetVal)
{
	CComBSTR szRetVal(NULL);
	char **lpszNames;
	int i, number_of_domains, length = 0;

	if ( m_boolLinkage ) {
		lpszNames = linkage_get_link_domain_names(m_Linkage, Index);
		if ( lpszNames != NULL ) {
			number_of_domains = m_Linkage->sublinkage[m_Linkage->current].pp_info[Index].num_domains;

			//concantenate the array of names into a single string
			for ( i = 0; i < number_of_domains; i++ ) {
				szRetVal.Append(lpszNames[i]);
				szRetVal.Append(Delimiter);
			}
		}
	}

	*pRetVal = szRetVal.Detach();

	return *pRetVal ? S_OK : E_OUTOFMEMORY;
}

STDMETHODIMP CParser::LinkageGetViolationName(BSTR *pRetVal)
{
	char * lpszRetVal = NULL;

	if ( m_boolLinkage )
		lpszRetVal = linkage_get_violation_name(m_Linkage);

	*pRetVal = A2BSTR(lpszRetVal);

	return *pRetVal ? S_OK : E_OUTOFMEMORY;
}

STDMETHODIMP CParser::LinkageGetWords(BSTR Delimiter, BSTR *pRetVal)
{
	CComBSTR szRetVal(NULL);
	char **lpszWords;
	int i, length = 0;

	if ( m_boolLinkage ) {
		lpszWords = linkage_get_words(m_Linkage);
		if ( lpszWords != NULL ) {
			//concantenate the array of words into a single string
			for ( i = 0; i < m_Linkage->num_words; i++ ) {
				szRetVal.Append(lpszWords[i]);
				szRetVal.Append(Delimiter);
			}
		}
	}

	*pRetVal = szRetVal.Detach();

	return *pRetVal ? S_OK : E_OUTOFMEMORY;
}

STDMETHODIMP CParser::LinkageGetWord(int Index, BSTR *pRetVal)
{
	char * lpszRetVal = NULL;

	if ( m_boolLinkage )
		lpszRetVal = linkage_get_word(m_Linkage, Index);

	*pRetVal = A2BSTR(lpszRetVal);

	return *pRetVal ? S_OK : E_OUTOFMEMORY;
}

STDMETHODIMP CParser::LinkageUnusedWordCost(int *pRetVal)
{
	int iRetVal;

	if ( m_boolLinkage )
		iRetVal = linkage_unused_word_cost(m_Linkage);
	else
		iRetVal = -1;

	*pRetVal = iRetVal;

	return S_OK;
}

STDMETHODIMP CParser::LinkageDisjunctCost(int *pRetVal)
{
	int iRetVal;

	if ( m_boolLinkage )
		iRetVal = linkage_disjunct_cost(m_Linkage);
	else
		iRetVal = -1;

	*pRetVal = iRetVal;

	return S_OK;
}

STDMETHODIMP CParser::LinkageAndCost(int *pRetVal)
{
	int iRetVal;

	if ( m_boolLinkage )
		iRetVal = linkage_and_cost(m_Linkage);
	else
		iRetVal = -1;

	*pRetVal = iRetVal;

	return S_OK;
}

STDMETHODIMP CParser::LinkageLinkCost(int *pRetVal)
{
	int iRetVal;

	if ( m_boolLinkage )
		iRetVal = linkage_link_cost(m_Linkage);
	else
		iRetVal = -1;

	*pRetVal = iRetVal;

	return S_OK;
}

STDMETHODIMP CParser::LinkageDelete()
{
	if ( m_boolLinkage == VARIANT_TRUE ) {
		linkage_delete(m_Linkage);
		m_boolLinkage = VARIANT_FALSE;
	}

	return S_OK;
}

STDMETHODIMP CParser::PostProcessOpen(BSTR PostProcessor, VARIANT_BOOL *pRetVal)
{
	USES_CONVERSION;
	char * lpszPostProcessor = OLE2A(PostProcessor);

	if ( lpszPostProcessor != NULL ) {
		m_PostProcessor = post_process_open(lpszPostProcessor, NULL);

		if ( !m_PostProcessor == NULL ) {
			m_boolPostProcessor = VARIANT_TRUE;
		} else {
			m_boolPostProcessor = VARIANT_FALSE;
		}
	} else {
		m_boolPostProcessor = VARIANT_FALSE;
	}

	*pRetVal = m_boolPostProcessor;

	return S_OK;
}

STDMETHODIMP CParser::PostProcessClose()
{
	if ( m_boolPostProcessor == VARIANT_TRUE ) {
		post_process_close(m_PostProcessor);
		m_boolPostProcessor = VARIANT_FALSE;
	}

	return S_OK;
}

STDMETHODIMP CParser::LinkagePostProcess(short *pRetVal)
{
	if ( m_boolPostProcessor && m_boolLinkage )
		linkage_post_process(m_Linkage, m_PostProcessor);

	short sRetVal = true;

	*pRetVal = sRetVal;

	return S_OK;
}

STDMETHODIMP CParser::ResetResources()
{
	if ( m_boolParseOptions )
		parse_options_reset_resources(m_ParseOptions);

	return S_OK;
}

STDMETHODIMP CParser::get_CostModelType(long *pVal)
{
	*pVal = parse_options_get_cost_model_type(m_ParseOptions);

	return S_OK;
}

STDMETHODIMP CParser::put_CostModelType(long newVal)
{
	parse_options_set_cost_model_type(m_ParseOptions, newVal);

	return S_OK;
}

STDMETHODIMP CParser::get_LinkageIsCanonical(short *pVal)
{
	*pVal = linkage_is_canonical(m_Linkage);

	return S_OK;
}

STDMETHODIMP CParser::get_LinkageIsImproper(short *pVal)
{
	*pVal = linkage_is_improper(m_Linkage);

	return S_OK;
}

STDMETHODIMP CParser::get_LinkageHasInconsistentDomains(short *pVal)
{
	*pVal = linkage_has_inconsistent_domains(m_Linkage);

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Private Helper Functions

// double pads the pad_char in lpszInputString, i.e.,
//		PadChar("c:\app\3.0.dict", "\"); returns "c:\\app\\3.0.dict"
char * CParser::PadChar(LPCSTR lpszInputString, char pad_char)
{
	char *new_string = NULL;
	int string_length = strlen(lpszInputString);
	int i, j = 0, pad_char_count = 0;

	for ( i = 0; i < string_length; i++ ) {
		if ( lpszInputString[i] == pad_char ) {
			pad_char_count++;
		}
	}

	new_string = new CHAR[string_length + pad_char_count + 1];

	for ( i = 0; i < string_length; i++ ) {
		new_string[j++] = lpszInputString[i];
		if ( lpszInputString[i] == pad_char ) {
			new_string[j++] = pad_char;
		}
	}
	new_string[j] = '\0';

	return new_string;
}

// Creates the dictionary files from the Access database
VARIANT_BOOL CParser::ExpandDictionary()
{
	FILE * fp;
	CDictionaryDB dictionaryDB;
	VARIANT_BOOL bRetVal = VARIANT_TRUE;

 	// open the connection
	if ( dictionaryDB.OpenDataSource() == S_OK ) {
		// create the dictionary directory
		if ( _mkdir("Words") == 0 ) {
			for ( int i = 0; i < NUMTABLES; i++ ) {
				// open session and table
				if ( dictionaryDB.OpenRowset(DictionaryNames[i][DB_SQL]) == S_OK ) {
					// open output file
					if ( (fp = fopen(DictionaryNames[i][DB_FILE], "w")) != NULL ) {
						// dump table to file
						while ( dictionaryDB.MoveNext() == S_OK ) {
							fprintf(fp, "%s\n", dictionaryDB.m_StringData);
						}
						// close file
						fclose(fp);
					} else {
						bRetVal = VARIANT_FALSE;
					}
					// close table
					dictionaryDB.Close();
				} else {
					bRetVal = VARIANT_FALSE;
				}
			}
		} else {
			bRetVal = VARIANT_FALSE;
		}
	} else {
		bRetVal = VARIANT_FALSE;
	}

	if ( bRetVal == VARIANT_FALSE )
		DeflateDictionary();

	return bRetVal;
}

VARIANT_BOOL CParser::DeflateDictionary()
{
	for ( int i = 0; i < NUMTABLES; i++ ) {
		// delete the file
		remove(DictionaryNames[i][DB_FILE]);
	}
	_rmdir("Words");

	VARIANT_BOOL bRetVal = VARIANT_TRUE;
	return bRetVal;
}