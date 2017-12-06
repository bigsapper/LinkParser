// Parser.h : Declaration of the CParser

#ifndef __PARSER_H_
#define __PARSER_H_

#include "resource.h"       // main symbols

#include "countmacros.h"	// for object life tracking
#include "link-includes.h"	// includes for Link Parser API
#include "DictionaryDB.h"	// dictionary database
#include <direct.h>			// directory control routines
#include <atldbcli.h>		// ATL database

/////////////////////////////////////////////////////////////////////////////
// CParser
class ATL_NO_VTABLE CParser :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CParser, &CLSID_Parser>,
	public IDispatchImpl<IParser, &IID_IParser, &LIBID_LINKPARSERLib>
{
public:
	CParser()
	{
		// macro to log object construction in _DEBUG mode
		CTOR_COUNT_MACRO

        m_boolPostProcessor = VARIANT_FALSE;
        m_boolLinkage = VARIANT_FALSE;
        m_boolSentence = VARIANT_FALSE;
        m_boolDictionary = VARIANT_FALSE;
        m_boolParseOptions = VARIANT_FALSE;
	}

	~CParser()
	{
		// macro to log object destruction in _DEBUG mode
		DTOR_COUNT_MACRO
        if ( m_boolPostProcessor )
            post_process_close(m_PostProcessor);
        if ( m_boolLinkage )
            linkage_delete(m_Linkage);
        if ( m_boolSentence )
            sentence_delete(m_Sentence);
        if ( m_boolDictionary )
            dictionary_delete(m_Dictionary);
        if ( m_boolParseOptions )
            parse_options_delete(m_ParseOptions);
}
DECLARE_REGISTRY_RESOURCEID(IDR_PARSER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CParser)
	COM_INTERFACE_ENTRY(IParser)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IParser
public:
	STDMETHOD(get_LinkageHasInconsistentDomains)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_LinkageIsImproper)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_LinkageIsCanonical)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_CostModelType)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_CostModelType)(/*[in]*/ long newVal);
	STDMETHOD(ResetResources)();
    STDMETHOD(LinkagePostProcess)(/*[out, retval]*/ short *pRetVal);
	STDMETHOD(PostProcessClose)();
	STDMETHOD(PostProcessOpen)(/*[in]*/ BSTR PostProcessor, /*[out, retval]*/ VARIANT_BOOL *pRetVal);
	STDMETHOD(LinkageDelete)();
	STDMETHOD(LinkageLinkCost)(/*[out, retval]*/ int *pRetVal);
	STDMETHOD(LinkageAndCost)(/*[out, retval]*/ int *pRetVal);
	STDMETHOD(LinkageDisjunctCost)(/*[out, retval]*/ int *pRetVal);
	STDMETHOD(LinkageUnusedWordCost)(/*[out, retval]*/ int *pRetVal);
	STDMETHOD(LinkageGetWord)(/*[in]*/ int Index, /*[out, retval]*/ BSTR *pRetVal);
    STDMETHOD(LinkageGetWords)(/*in*/ BSTR Delimiter, /*[out, retval]*/ BSTR *pRetVal);
	STDMETHOD(LinkageGetViolationName)(/*[out, retval]*/ BSTR *pRetVal);
    STDMETHOD(LinkageGetLinkDomainNames)(/*[in]*/ int Index, /*in*/ BSTR Delimiter, /*[out, retval]*/ BSTR *pRetVal);
	STDMETHOD(LinkageGetLinkNumDomains)(/*[in]*/ int Index, /*[out, retval]*/ int *pRetVal);
	STDMETHOD(LinkageGetLinkRLabel)(/*[in]*/ int Index, /*[out, retval]*/ BSTR *pRetVal);
	STDMETHOD(LinkageGetLinkLLabel)(/*[in]*/ int Index, /*[out, retval]*/ BSTR *pRetVal);
	STDMETHOD(LinkageGetLinkLabel)(/*[in]*/ int Index, /*[out, retval]*/ BSTR *pRetVal);
	STDMETHOD(LinkagePrintLinksAndDomains)(/*[out, retval]*/ BSTR *pRetVal);
	STDMETHOD(LinkagePrintPostscript)(/*[in]*/ int Mode, /*[out, retval]*/ BSTR *pRetVal);
	STDMETHOD(LinkagePrintDiagram)(/*[out, retval]*/ BSTR *pRetVal);
	STDMETHOD(LinkageGetLinkRWord)(/*[in]*/ int Index, /*[out, retval]*/ int *pRetVal);
	STDMETHOD(LinkageGetLinkLWord)(/*[in]*/ int Index, /*[out, retval]*/ int *pRetVal);
	STDMETHOD(LinkageGetLinkLength)(/*[in]*/ int Index, /*[out, retval]*/ int *pRetVal);
	STDMETHOD(LinkageGetNumLinks)(/*[out, retval]*/ int *pRetVal);
	STDMETHOD(LinkageGetNumWords)(/*[out, retval]*/ int *pRetVal);
	STDMETHOD(LinkageComputeUnion)(/*[out, retval]*/ short *pRetVal);
	STDMETHOD(LinkageSetCurrentSublinkage)(/*[in]*/ int Index, /*[out, retval]*/ short *pRetVal);
	STDMETHOD(LinkageGetNumSublinkages)(/*[out, retval]*/ int *pRetVal);
	STDMETHOD(LinkageCreate)(/*[in]*/ int Index, /*[out, retval]*/ VARIANT_BOOL *pRetVal);
	STDMETHOD(SentenceDisjunctCost)(/*[in]*/ int Index, /*[out, retval]*/ int *pRetVal);
	STDMETHOD(SentenceNumViolations)(/*[in]*/ int Index, /*[out, retval]*/ int *pRetVal);
	STDMETHOD(SentenceNumLinkagesPostProcessed)(/*[out, retval]*/ int *pRetVal);
	STDMETHOD(SentenceNumValidLinkages)(/*[out, retval]*/ int *pRetVal);
	STDMETHOD(SentenceNumLinkagesFound)(/*[out, retval]*/ int *pRetVal);
	STDMETHOD(SentenceNullCount)(/*[out, retval]*/ int *pRetVal);
    STDMETHOD(SentenceGetWord)(/*[in]*/ int Index, /*[out, retval]*/ BSTR *pRetVal);
    STDMETHOD(SentenceLength)(/*[out, retval]*/ int *pRetVal);
    STDMETHOD(get_PostProcessor)(/*[out, retval]*/ VARIANT_BOOL *pVal);
    STDMETHOD(get_Linkage)(/*[out, retval]*/ VARIANT_BOOL *pVal);
    STDMETHOD(get_ParseOptions)(/*[out, retval]*/ VARIANT_BOOL *pVal);
    STDMETHOD(get_Sentence)(/*[out, retval]*/ VARIANT_BOOL *pVal);
    STDMETHOD(get_Dictionary)(/*[out, retval]*/ VARIANT_BOOL *pVal);
    STDMETHOD(SentenceParse)(/*[out, retval]*/ int *pRetVal);
    STDMETHOD(SentenceDelete)();
    STDMETHOD(SentenceCreate)(/*[in]*/ BSTR Sentence, /*[out, retval]*/ VARIANT_BOOL *pRetVal);
	STDMETHOD(get_ResourcesExhausted)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_MemoryExhausted)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_TimerExpired)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_MaxMemory)(/*[out, retval]*/ int *pVal);
	STDMETHOD(put_MaxMemory)(/*[in]*/ int newVal);
	STDMETHOD(get_MaxParseTime)(/*[out, retval]*/ int *pVal);
	STDMETHOD(put_MaxParseTime)(/*[in]*/ int newVal);
	STDMETHOD(get_DisjunctCost)(/*[out, retval]*/ int *pVal);
	STDMETHOD(put_DisjunctCost)(/*[in]*/ int newVal);
	STDMETHOD(get_DisplayLinks)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_DisplayLinks)(/*[in]*/ short newVal);
	STDMETHOD(get_DisplayBad)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_DisplayBad)(/*[in]*/ short newVal);
	STDMETHOD(get_DisplayPostscript)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_DisplayPostscript)(/*[in]*/ short newVal);
	STDMETHOD(get_DisplayOn)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_DisplayOn)(/*[in]*/ short newVal);
	STDMETHOD(get_ScreenWidth)(/*[out, retval]*/ int *pVal);
	STDMETHOD(put_ScreenWidth)(/*[in]*/ int newVal);
	STDMETHOD(get_PanicMode)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_PanicMode)(/*[in]*/ short newVal);
	STDMETHOD(get_BatchMode)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_BatchMode)(/*[in]*/ short newVal);
	STDMETHOD(get_EchoOn)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_EchoOn)(/*[in]*/ short newVal);
	STDMETHOD(get_AllowNull)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_AllowNull)(/*[in]*/ short newVal);
	STDMETHOD(get_DisplayUnion)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_DisplayUnion)(/*[in]*/ short newVal);
	STDMETHOD(get_DisplayWalls)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_DisplayWalls)(/*[in]*/ short newVal);
	STDMETHOD(get_AllShort)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_AllShort)(/*[in]*/ short newVal);
    STDMETHOD(get_ShortLength)(/*[out, retval]*/ int *pVal);
    STDMETHOD(put_ShortLength)(/*[in]*/ int newVal);
	STDMETHOD(get_IslandsOK)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_IslandsOK)(/*[in]*/ short newVal);
    STDMETHOD(get_NullBlock)(/*[out, retval]*/ int *pVal);
    STDMETHOD(put_NullBlock)(/*[in]*/ int newVal);
	STDMETHOD(get_MaxNullCount)(/*[out, retval]*/ int *pVal);
    STDMETHOD(put_MaxNullCount)(/*[in]*/ int newVal);
    STDMETHOD(get_MinNullCount)(/*[out, retval]*/ int *pVal);
    STDMETHOD(put_MinNullCount)(/*[in]*/ int newVal);
	STDMETHOD(get_LinkageLimit)(/*[out, retval]*/ int *pVal);
	STDMETHOD(put_LinkageLimit)(/*[in]*/ int newVal);
	STDMETHOD(get_Verbosity)(/*[out, retval]*/ int *pVal);
	STDMETHOD(put_Verbosity)(/*[in]*/ int newVal);
	STDMETHOD(ParseOptionsDelete)();
	STDMETHOD(ParseOptionsCreate)(/*[out, retval]*/ short *pRetVal);
	STDMETHOD(DictionaryGetMaxCost)(/*[out, retval]*/ int *pRetVal);
	STDMETHOD(DictionaryDelete)();
	STDMETHOD(DictionaryCreate)(/*[out, retval]*/ VARIANT_BOOL *pRetVal);
private:
	VARIANT_BOOL DeflateDictionary();
	VARIANT_BOOL ExpandDictionary();
    char * PadChar(LPCSTR lpszInputStirng, char pad_char);
    PostProcessor m_PostProcessor;
    Linkage m_Linkage;
    Parse_Options m_ParseOptions;
    Sentence m_Sentence;
    Dictionary m_Dictionary;
    VARIANT_BOOL m_boolPostProcessor;
    VARIANT_BOOL m_boolLinkage;
    VARIANT_BOOL m_boolParseOptions;
    VARIANT_BOOL m_boolSentence;
    VARIANT_BOOL m_boolDictionary;
};

#endif //__PARSER_H_