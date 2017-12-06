

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Wed Dec 06 12:29:25 2017
 */
/* Compiler settings for LinkParser.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __LinkParser_h__
#define __LinkParser_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IParser_FWD_DEFINED__
#define __IParser_FWD_DEFINED__
typedef interface IParser IParser;

#endif 	/* __IParser_FWD_DEFINED__ */


#ifndef __Parser_FWD_DEFINED__
#define __Parser_FWD_DEFINED__

#ifdef __cplusplus
typedef class Parser Parser;
#else
typedef struct Parser Parser;
#endif /* __cplusplus */

#endif 	/* __Parser_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IParser_INTERFACE_DEFINED__
#define __IParser_INTERFACE_DEFINED__

/* interface IParser */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IParser;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7863615D-6455-11D2-B8DB-00002441B3CB")
    IParser : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DictionaryCreate( 
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DictionaryDelete( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DictionaryGetMaxCost( 
            /* [retval][out] */ int *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ParseOptionsCreate( 
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ParseOptionsDelete( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Verbosity( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Verbosity( 
            /* [in] */ int newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LinkageLimit( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_LinkageLimit( 
            /* [in] */ int newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MinNullCount( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_MinNullCount( 
            /* [in] */ int newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MaxNullCount( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_MaxNullCount( 
            /* [in] */ int newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_NullBlock( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_NullBlock( 
            /* [in] */ int newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IslandsOK( 
            /* [retval][out] */ short *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_IslandsOK( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ShortLength( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ShortLength( 
            /* [in] */ int newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_AllShort( 
            /* [retval][out] */ short *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_AllShort( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DisplayWalls( 
            /* [retval][out] */ short *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_DisplayWalls( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DisplayUnion( 
            /* [retval][out] */ short *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_DisplayUnion( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_AllowNull( 
            /* [retval][out] */ short *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_AllowNull( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_EchoOn( 
            /* [retval][out] */ short *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_EchoOn( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_BatchMode( 
            /* [retval][out] */ short *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_BatchMode( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PanicMode( 
            /* [retval][out] */ short *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_PanicMode( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ScreenWidth( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ScreenWidth( 
            /* [in] */ int newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DisplayOn( 
            /* [retval][out] */ short *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_DisplayOn( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DisplayPostscript( 
            /* [retval][out] */ short *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_DisplayPostscript( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DisplayBad( 
            /* [retval][out] */ short *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_DisplayBad( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DisplayLinks( 
            /* [retval][out] */ short *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_DisplayLinks( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DisjunctCost( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_DisjunctCost( 
            /* [in] */ int newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MaxParseTime( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_MaxParseTime( 
            /* [in] */ int newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MaxMemory( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_MaxMemory( 
            /* [in] */ int newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TimerExpired( 
            /* [retval][out] */ short *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MemoryExhausted( 
            /* [retval][out] */ short *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ResourcesExhausted( 
            /* [retval][out] */ short *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SentenceCreate( 
            /* [in] */ BSTR Sentence,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SentenceDelete( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SentenceParse( 
            /* [retval][out] */ int *pRetVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Dictionary( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Sentence( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ParseOptions( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Linkage( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PostProcessor( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SentenceLength( 
            /* [retval][out] */ int *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SentenceGetWord( 
            /* [in] */ int Index,
            /* [retval][out] */ BSTR *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SentenceNullCount( 
            /* [retval][out] */ int *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SentenceNumLinkagesFound( 
            /* [retval][out] */ int *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SentenceNumValidLinkages( 
            /* [retval][out] */ int *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SentenceNumLinkagesPostProcessed( 
            /* [retval][out] */ int *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SentenceNumViolations( 
            /* [in] */ int Index,
            /* [retval][out] */ int *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SentenceDisjunctCost( 
            /* [in] */ int Index,
            /* [retval][out] */ int *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LinkageCreate( 
            /* [in] */ int Index,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LinkageGetNumSublinkages( 
            /* [retval][out] */ int *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LinkageSetCurrentSublinkage( 
            /* [in] */ int Index,
            /* [retval][out] */ short *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LinkageComputeUnion( 
            /* [retval][out] */ short *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LinkageGetNumWords( 
            /* [retval][out] */ int *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LinkageGetNumLinks( 
            /* [retval][out] */ int *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LinkageGetLinkLength( 
            /* [in] */ int Index,
            /* [retval][out] */ int *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LinkageGetLinkLWord( 
            /* [in] */ int Index,
            /* [retval][out] */ int *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LinkageGetLinkRWord( 
            /* [in] */ int Index,
            /* [retval][out] */ int *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LinkagePrintDiagram( 
            /* [retval][out] */ BSTR *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LinkagePrintPostscript( 
            /* [in] */ int Mode,
            /* [retval][out] */ BSTR *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LinkagePrintLinksAndDomains( 
            /* [retval][out] */ BSTR *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LinkageGetLinkLabel( 
            /* [in] */ int Index,
            /* [retval][out] */ BSTR *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LinkageGetLinkLLabel( 
            /* [in] */ int Index,
            /* [retval][out] */ BSTR *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LinkageGetLinkRLabel( 
            /* [in] */ int Index,
            /* [retval][out] */ BSTR *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LinkageGetLinkNumDomains( 
            /* [in] */ int Index,
            /* [retval][out] */ int *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LinkageGetLinkDomainNames( 
            /* [in] */ int Index,
            /* [in] */ BSTR Delimiter,
            /* [retval][out] */ BSTR *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LinkageGetViolationName( 
            /* [retval][out] */ BSTR *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LinkageGetWords( 
            /* [in] */ BSTR Delimiter,
            /* [retval][out] */ BSTR *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LinkageGetWord( 
            /* [in] */ int Index,
            /* [retval][out] */ BSTR *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LinkageUnusedWordCost( 
            /* [retval][out] */ int *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LinkageDisjunctCost( 
            /* [retval][out] */ int *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LinkageAndCost( 
            /* [retval][out] */ int *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LinkageLinkCost( 
            /* [retval][out] */ int *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LinkageDelete( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PostProcessOpen( 
            /* [in] */ BSTR PostProcessor,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PostProcessClose( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LinkagePostProcess( 
            /* [retval][out] */ short *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ResetResources( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CostModelType( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CostModelType( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LinkageIsCanonical( 
            /* [retval][out] */ short *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LinkageIsImproper( 
            /* [retval][out] */ short *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LinkageHasInconsistentDomains( 
            /* [retval][out] */ short *pVal) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IParserVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IParser * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IParser * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IParser * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IParser * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IParser * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IParser * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IParser * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DictionaryCreate )( 
            IParser * This,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DictionaryDelete )( 
            IParser * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DictionaryGetMaxCost )( 
            IParser * This,
            /* [retval][out] */ int *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ParseOptionsCreate )( 
            IParser * This,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ParseOptionsDelete )( 
            IParser * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Verbosity )( 
            IParser * This,
            /* [retval][out] */ int *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Verbosity )( 
            IParser * This,
            /* [in] */ int newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LinkageLimit )( 
            IParser * This,
            /* [retval][out] */ int *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_LinkageLimit )( 
            IParser * This,
            /* [in] */ int newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MinNullCount )( 
            IParser * This,
            /* [retval][out] */ int *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_MinNullCount )( 
            IParser * This,
            /* [in] */ int newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MaxNullCount )( 
            IParser * This,
            /* [retval][out] */ int *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_MaxNullCount )( 
            IParser * This,
            /* [in] */ int newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_NullBlock )( 
            IParser * This,
            /* [retval][out] */ int *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_NullBlock )( 
            IParser * This,
            /* [in] */ int newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IslandsOK )( 
            IParser * This,
            /* [retval][out] */ short *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_IslandsOK )( 
            IParser * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ShortLength )( 
            IParser * This,
            /* [retval][out] */ int *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ShortLength )( 
            IParser * This,
            /* [in] */ int newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AllShort )( 
            IParser * This,
            /* [retval][out] */ short *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_AllShort )( 
            IParser * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DisplayWalls )( 
            IParser * This,
            /* [retval][out] */ short *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DisplayWalls )( 
            IParser * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DisplayUnion )( 
            IParser * This,
            /* [retval][out] */ short *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DisplayUnion )( 
            IParser * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AllowNull )( 
            IParser * This,
            /* [retval][out] */ short *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_AllowNull )( 
            IParser * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_EchoOn )( 
            IParser * This,
            /* [retval][out] */ short *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_EchoOn )( 
            IParser * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BatchMode )( 
            IParser * This,
            /* [retval][out] */ short *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_BatchMode )( 
            IParser * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PanicMode )( 
            IParser * This,
            /* [retval][out] */ short *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_PanicMode )( 
            IParser * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ScreenWidth )( 
            IParser * This,
            /* [retval][out] */ int *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ScreenWidth )( 
            IParser * This,
            /* [in] */ int newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DisplayOn )( 
            IParser * This,
            /* [retval][out] */ short *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DisplayOn )( 
            IParser * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DisplayPostscript )( 
            IParser * This,
            /* [retval][out] */ short *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DisplayPostscript )( 
            IParser * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DisplayBad )( 
            IParser * This,
            /* [retval][out] */ short *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DisplayBad )( 
            IParser * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DisplayLinks )( 
            IParser * This,
            /* [retval][out] */ short *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DisplayLinks )( 
            IParser * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DisjunctCost )( 
            IParser * This,
            /* [retval][out] */ int *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DisjunctCost )( 
            IParser * This,
            /* [in] */ int newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MaxParseTime )( 
            IParser * This,
            /* [retval][out] */ int *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_MaxParseTime )( 
            IParser * This,
            /* [in] */ int newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MaxMemory )( 
            IParser * This,
            /* [retval][out] */ int *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_MaxMemory )( 
            IParser * This,
            /* [in] */ int newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TimerExpired )( 
            IParser * This,
            /* [retval][out] */ short *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MemoryExhausted )( 
            IParser * This,
            /* [retval][out] */ short *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ResourcesExhausted )( 
            IParser * This,
            /* [retval][out] */ short *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SentenceCreate )( 
            IParser * This,
            /* [in] */ BSTR Sentence,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SentenceDelete )( 
            IParser * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SentenceParse )( 
            IParser * This,
            /* [retval][out] */ int *pRetVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Dictionary )( 
            IParser * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Sentence )( 
            IParser * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ParseOptions )( 
            IParser * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Linkage )( 
            IParser * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PostProcessor )( 
            IParser * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SentenceLength )( 
            IParser * This,
            /* [retval][out] */ int *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SentenceGetWord )( 
            IParser * This,
            /* [in] */ int Index,
            /* [retval][out] */ BSTR *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SentenceNullCount )( 
            IParser * This,
            /* [retval][out] */ int *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SentenceNumLinkagesFound )( 
            IParser * This,
            /* [retval][out] */ int *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SentenceNumValidLinkages )( 
            IParser * This,
            /* [retval][out] */ int *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SentenceNumLinkagesPostProcessed )( 
            IParser * This,
            /* [retval][out] */ int *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SentenceNumViolations )( 
            IParser * This,
            /* [in] */ int Index,
            /* [retval][out] */ int *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SentenceDisjunctCost )( 
            IParser * This,
            /* [in] */ int Index,
            /* [retval][out] */ int *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LinkageCreate )( 
            IParser * This,
            /* [in] */ int Index,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LinkageGetNumSublinkages )( 
            IParser * This,
            /* [retval][out] */ int *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LinkageSetCurrentSublinkage )( 
            IParser * This,
            /* [in] */ int Index,
            /* [retval][out] */ short *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LinkageComputeUnion )( 
            IParser * This,
            /* [retval][out] */ short *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LinkageGetNumWords )( 
            IParser * This,
            /* [retval][out] */ int *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LinkageGetNumLinks )( 
            IParser * This,
            /* [retval][out] */ int *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LinkageGetLinkLength )( 
            IParser * This,
            /* [in] */ int Index,
            /* [retval][out] */ int *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LinkageGetLinkLWord )( 
            IParser * This,
            /* [in] */ int Index,
            /* [retval][out] */ int *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LinkageGetLinkRWord )( 
            IParser * This,
            /* [in] */ int Index,
            /* [retval][out] */ int *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LinkagePrintDiagram )( 
            IParser * This,
            /* [retval][out] */ BSTR *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LinkagePrintPostscript )( 
            IParser * This,
            /* [in] */ int Mode,
            /* [retval][out] */ BSTR *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LinkagePrintLinksAndDomains )( 
            IParser * This,
            /* [retval][out] */ BSTR *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LinkageGetLinkLabel )( 
            IParser * This,
            /* [in] */ int Index,
            /* [retval][out] */ BSTR *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LinkageGetLinkLLabel )( 
            IParser * This,
            /* [in] */ int Index,
            /* [retval][out] */ BSTR *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LinkageGetLinkRLabel )( 
            IParser * This,
            /* [in] */ int Index,
            /* [retval][out] */ BSTR *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LinkageGetLinkNumDomains )( 
            IParser * This,
            /* [in] */ int Index,
            /* [retval][out] */ int *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LinkageGetLinkDomainNames )( 
            IParser * This,
            /* [in] */ int Index,
            /* [in] */ BSTR Delimiter,
            /* [retval][out] */ BSTR *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LinkageGetViolationName )( 
            IParser * This,
            /* [retval][out] */ BSTR *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LinkageGetWords )( 
            IParser * This,
            /* [in] */ BSTR Delimiter,
            /* [retval][out] */ BSTR *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LinkageGetWord )( 
            IParser * This,
            /* [in] */ int Index,
            /* [retval][out] */ BSTR *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LinkageUnusedWordCost )( 
            IParser * This,
            /* [retval][out] */ int *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LinkageDisjunctCost )( 
            IParser * This,
            /* [retval][out] */ int *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LinkageAndCost )( 
            IParser * This,
            /* [retval][out] */ int *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LinkageLinkCost )( 
            IParser * This,
            /* [retval][out] */ int *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LinkageDelete )( 
            IParser * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PostProcessOpen )( 
            IParser * This,
            /* [in] */ BSTR PostProcessor,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PostProcessClose )( 
            IParser * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LinkagePostProcess )( 
            IParser * This,
            /* [retval][out] */ short *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ResetResources )( 
            IParser * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CostModelType )( 
            IParser * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CostModelType )( 
            IParser * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LinkageIsCanonical )( 
            IParser * This,
            /* [retval][out] */ short *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LinkageIsImproper )( 
            IParser * This,
            /* [retval][out] */ short *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LinkageHasInconsistentDomains )( 
            IParser * This,
            /* [retval][out] */ short *pVal);
        
        END_INTERFACE
    } IParserVtbl;

    interface IParser
    {
        CONST_VTBL struct IParserVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IParser_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IParser_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IParser_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IParser_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IParser_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IParser_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IParser_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IParser_DictionaryCreate(This,pRetVal)	\
    ( (This)->lpVtbl -> DictionaryCreate(This,pRetVal) ) 

#define IParser_DictionaryDelete(This)	\
    ( (This)->lpVtbl -> DictionaryDelete(This) ) 

#define IParser_DictionaryGetMaxCost(This,pRetVal)	\
    ( (This)->lpVtbl -> DictionaryGetMaxCost(This,pRetVal) ) 

#define IParser_ParseOptionsCreate(This,pRetVal)	\
    ( (This)->lpVtbl -> ParseOptionsCreate(This,pRetVal) ) 

#define IParser_ParseOptionsDelete(This)	\
    ( (This)->lpVtbl -> ParseOptionsDelete(This) ) 

#define IParser_get_Verbosity(This,pVal)	\
    ( (This)->lpVtbl -> get_Verbosity(This,pVal) ) 

#define IParser_put_Verbosity(This,newVal)	\
    ( (This)->lpVtbl -> put_Verbosity(This,newVal) ) 

#define IParser_get_LinkageLimit(This,pVal)	\
    ( (This)->lpVtbl -> get_LinkageLimit(This,pVal) ) 

#define IParser_put_LinkageLimit(This,newVal)	\
    ( (This)->lpVtbl -> put_LinkageLimit(This,newVal) ) 

#define IParser_get_MinNullCount(This,pVal)	\
    ( (This)->lpVtbl -> get_MinNullCount(This,pVal) ) 

#define IParser_put_MinNullCount(This,newVal)	\
    ( (This)->lpVtbl -> put_MinNullCount(This,newVal) ) 

#define IParser_get_MaxNullCount(This,pVal)	\
    ( (This)->lpVtbl -> get_MaxNullCount(This,pVal) ) 

#define IParser_put_MaxNullCount(This,newVal)	\
    ( (This)->lpVtbl -> put_MaxNullCount(This,newVal) ) 

#define IParser_get_NullBlock(This,pVal)	\
    ( (This)->lpVtbl -> get_NullBlock(This,pVal) ) 

#define IParser_put_NullBlock(This,newVal)	\
    ( (This)->lpVtbl -> put_NullBlock(This,newVal) ) 

#define IParser_get_IslandsOK(This,pVal)	\
    ( (This)->lpVtbl -> get_IslandsOK(This,pVal) ) 

#define IParser_put_IslandsOK(This,newVal)	\
    ( (This)->lpVtbl -> put_IslandsOK(This,newVal) ) 

#define IParser_get_ShortLength(This,pVal)	\
    ( (This)->lpVtbl -> get_ShortLength(This,pVal) ) 

#define IParser_put_ShortLength(This,newVal)	\
    ( (This)->lpVtbl -> put_ShortLength(This,newVal) ) 

#define IParser_get_AllShort(This,pVal)	\
    ( (This)->lpVtbl -> get_AllShort(This,pVal) ) 

#define IParser_put_AllShort(This,newVal)	\
    ( (This)->lpVtbl -> put_AllShort(This,newVal) ) 

#define IParser_get_DisplayWalls(This,pVal)	\
    ( (This)->lpVtbl -> get_DisplayWalls(This,pVal) ) 

#define IParser_put_DisplayWalls(This,newVal)	\
    ( (This)->lpVtbl -> put_DisplayWalls(This,newVal) ) 

#define IParser_get_DisplayUnion(This,pVal)	\
    ( (This)->lpVtbl -> get_DisplayUnion(This,pVal) ) 

#define IParser_put_DisplayUnion(This,newVal)	\
    ( (This)->lpVtbl -> put_DisplayUnion(This,newVal) ) 

#define IParser_get_AllowNull(This,pVal)	\
    ( (This)->lpVtbl -> get_AllowNull(This,pVal) ) 

#define IParser_put_AllowNull(This,newVal)	\
    ( (This)->lpVtbl -> put_AllowNull(This,newVal) ) 

#define IParser_get_EchoOn(This,pVal)	\
    ( (This)->lpVtbl -> get_EchoOn(This,pVal) ) 

#define IParser_put_EchoOn(This,newVal)	\
    ( (This)->lpVtbl -> put_EchoOn(This,newVal) ) 

#define IParser_get_BatchMode(This,pVal)	\
    ( (This)->lpVtbl -> get_BatchMode(This,pVal) ) 

#define IParser_put_BatchMode(This,newVal)	\
    ( (This)->lpVtbl -> put_BatchMode(This,newVal) ) 

#define IParser_get_PanicMode(This,pVal)	\
    ( (This)->lpVtbl -> get_PanicMode(This,pVal) ) 

#define IParser_put_PanicMode(This,newVal)	\
    ( (This)->lpVtbl -> put_PanicMode(This,newVal) ) 

#define IParser_get_ScreenWidth(This,pVal)	\
    ( (This)->lpVtbl -> get_ScreenWidth(This,pVal) ) 

#define IParser_put_ScreenWidth(This,newVal)	\
    ( (This)->lpVtbl -> put_ScreenWidth(This,newVal) ) 

#define IParser_get_DisplayOn(This,pVal)	\
    ( (This)->lpVtbl -> get_DisplayOn(This,pVal) ) 

#define IParser_put_DisplayOn(This,newVal)	\
    ( (This)->lpVtbl -> put_DisplayOn(This,newVal) ) 

#define IParser_get_DisplayPostscript(This,pVal)	\
    ( (This)->lpVtbl -> get_DisplayPostscript(This,pVal) ) 

#define IParser_put_DisplayPostscript(This,newVal)	\
    ( (This)->lpVtbl -> put_DisplayPostscript(This,newVal) ) 

#define IParser_get_DisplayBad(This,pVal)	\
    ( (This)->lpVtbl -> get_DisplayBad(This,pVal) ) 

#define IParser_put_DisplayBad(This,newVal)	\
    ( (This)->lpVtbl -> put_DisplayBad(This,newVal) ) 

#define IParser_get_DisplayLinks(This,pVal)	\
    ( (This)->lpVtbl -> get_DisplayLinks(This,pVal) ) 

#define IParser_put_DisplayLinks(This,newVal)	\
    ( (This)->lpVtbl -> put_DisplayLinks(This,newVal) ) 

#define IParser_get_DisjunctCost(This,pVal)	\
    ( (This)->lpVtbl -> get_DisjunctCost(This,pVal) ) 

#define IParser_put_DisjunctCost(This,newVal)	\
    ( (This)->lpVtbl -> put_DisjunctCost(This,newVal) ) 

#define IParser_get_MaxParseTime(This,pVal)	\
    ( (This)->lpVtbl -> get_MaxParseTime(This,pVal) ) 

#define IParser_put_MaxParseTime(This,newVal)	\
    ( (This)->lpVtbl -> put_MaxParseTime(This,newVal) ) 

#define IParser_get_MaxMemory(This,pVal)	\
    ( (This)->lpVtbl -> get_MaxMemory(This,pVal) ) 

#define IParser_put_MaxMemory(This,newVal)	\
    ( (This)->lpVtbl -> put_MaxMemory(This,newVal) ) 

#define IParser_get_TimerExpired(This,pVal)	\
    ( (This)->lpVtbl -> get_TimerExpired(This,pVal) ) 

#define IParser_get_MemoryExhausted(This,pVal)	\
    ( (This)->lpVtbl -> get_MemoryExhausted(This,pVal) ) 

#define IParser_get_ResourcesExhausted(This,pVal)	\
    ( (This)->lpVtbl -> get_ResourcesExhausted(This,pVal) ) 

#define IParser_SentenceCreate(This,Sentence,pRetVal)	\
    ( (This)->lpVtbl -> SentenceCreate(This,Sentence,pRetVal) ) 

#define IParser_SentenceDelete(This)	\
    ( (This)->lpVtbl -> SentenceDelete(This) ) 

#define IParser_SentenceParse(This,pRetVal)	\
    ( (This)->lpVtbl -> SentenceParse(This,pRetVal) ) 

#define IParser_get_Dictionary(This,pVal)	\
    ( (This)->lpVtbl -> get_Dictionary(This,pVal) ) 

#define IParser_get_Sentence(This,pVal)	\
    ( (This)->lpVtbl -> get_Sentence(This,pVal) ) 

#define IParser_get_ParseOptions(This,pVal)	\
    ( (This)->lpVtbl -> get_ParseOptions(This,pVal) ) 

#define IParser_get_Linkage(This,pVal)	\
    ( (This)->lpVtbl -> get_Linkage(This,pVal) ) 

#define IParser_get_PostProcessor(This,pVal)	\
    ( (This)->lpVtbl -> get_PostProcessor(This,pVal) ) 

#define IParser_SentenceLength(This,pRetVal)	\
    ( (This)->lpVtbl -> SentenceLength(This,pRetVal) ) 

#define IParser_SentenceGetWord(This,Index,pRetVal)	\
    ( (This)->lpVtbl -> SentenceGetWord(This,Index,pRetVal) ) 

#define IParser_SentenceNullCount(This,pRetVal)	\
    ( (This)->lpVtbl -> SentenceNullCount(This,pRetVal) ) 

#define IParser_SentenceNumLinkagesFound(This,pRetVal)	\
    ( (This)->lpVtbl -> SentenceNumLinkagesFound(This,pRetVal) ) 

#define IParser_SentenceNumValidLinkages(This,pRetVal)	\
    ( (This)->lpVtbl -> SentenceNumValidLinkages(This,pRetVal) ) 

#define IParser_SentenceNumLinkagesPostProcessed(This,pRetVal)	\
    ( (This)->lpVtbl -> SentenceNumLinkagesPostProcessed(This,pRetVal) ) 

#define IParser_SentenceNumViolations(This,Index,pRetVal)	\
    ( (This)->lpVtbl -> SentenceNumViolations(This,Index,pRetVal) ) 

#define IParser_SentenceDisjunctCost(This,Index,pRetVal)	\
    ( (This)->lpVtbl -> SentenceDisjunctCost(This,Index,pRetVal) ) 

#define IParser_LinkageCreate(This,Index,pRetVal)	\
    ( (This)->lpVtbl -> LinkageCreate(This,Index,pRetVal) ) 

#define IParser_LinkageGetNumSublinkages(This,pRetVal)	\
    ( (This)->lpVtbl -> LinkageGetNumSublinkages(This,pRetVal) ) 

#define IParser_LinkageSetCurrentSublinkage(This,Index,pRetVal)	\
    ( (This)->lpVtbl -> LinkageSetCurrentSublinkage(This,Index,pRetVal) ) 

#define IParser_LinkageComputeUnion(This,pRetVal)	\
    ( (This)->lpVtbl -> LinkageComputeUnion(This,pRetVal) ) 

#define IParser_LinkageGetNumWords(This,pRetVal)	\
    ( (This)->lpVtbl -> LinkageGetNumWords(This,pRetVal) ) 

#define IParser_LinkageGetNumLinks(This,pRetVal)	\
    ( (This)->lpVtbl -> LinkageGetNumLinks(This,pRetVal) ) 

#define IParser_LinkageGetLinkLength(This,Index,pRetVal)	\
    ( (This)->lpVtbl -> LinkageGetLinkLength(This,Index,pRetVal) ) 

#define IParser_LinkageGetLinkLWord(This,Index,pRetVal)	\
    ( (This)->lpVtbl -> LinkageGetLinkLWord(This,Index,pRetVal) ) 

#define IParser_LinkageGetLinkRWord(This,Index,pRetVal)	\
    ( (This)->lpVtbl -> LinkageGetLinkRWord(This,Index,pRetVal) ) 

#define IParser_LinkagePrintDiagram(This,pRetVal)	\
    ( (This)->lpVtbl -> LinkagePrintDiagram(This,pRetVal) ) 

#define IParser_LinkagePrintPostscript(This,Mode,pRetVal)	\
    ( (This)->lpVtbl -> LinkagePrintPostscript(This,Mode,pRetVal) ) 

#define IParser_LinkagePrintLinksAndDomains(This,pRetVal)	\
    ( (This)->lpVtbl -> LinkagePrintLinksAndDomains(This,pRetVal) ) 

#define IParser_LinkageGetLinkLabel(This,Index,pRetVal)	\
    ( (This)->lpVtbl -> LinkageGetLinkLabel(This,Index,pRetVal) ) 

#define IParser_LinkageGetLinkLLabel(This,Index,pRetVal)	\
    ( (This)->lpVtbl -> LinkageGetLinkLLabel(This,Index,pRetVal) ) 

#define IParser_LinkageGetLinkRLabel(This,Index,pRetVal)	\
    ( (This)->lpVtbl -> LinkageGetLinkRLabel(This,Index,pRetVal) ) 

#define IParser_LinkageGetLinkNumDomains(This,Index,pRetVal)	\
    ( (This)->lpVtbl -> LinkageGetLinkNumDomains(This,Index,pRetVal) ) 

#define IParser_LinkageGetLinkDomainNames(This,Index,Delimiter,pRetVal)	\
    ( (This)->lpVtbl -> LinkageGetLinkDomainNames(This,Index,Delimiter,pRetVal) ) 

#define IParser_LinkageGetViolationName(This,pRetVal)	\
    ( (This)->lpVtbl -> LinkageGetViolationName(This,pRetVal) ) 

#define IParser_LinkageGetWords(This,Delimiter,pRetVal)	\
    ( (This)->lpVtbl -> LinkageGetWords(This,Delimiter,pRetVal) ) 

#define IParser_LinkageGetWord(This,Index,pRetVal)	\
    ( (This)->lpVtbl -> LinkageGetWord(This,Index,pRetVal) ) 

#define IParser_LinkageUnusedWordCost(This,pRetVal)	\
    ( (This)->lpVtbl -> LinkageUnusedWordCost(This,pRetVal) ) 

#define IParser_LinkageDisjunctCost(This,pRetVal)	\
    ( (This)->lpVtbl -> LinkageDisjunctCost(This,pRetVal) ) 

#define IParser_LinkageAndCost(This,pRetVal)	\
    ( (This)->lpVtbl -> LinkageAndCost(This,pRetVal) ) 

#define IParser_LinkageLinkCost(This,pRetVal)	\
    ( (This)->lpVtbl -> LinkageLinkCost(This,pRetVal) ) 

#define IParser_LinkageDelete(This)	\
    ( (This)->lpVtbl -> LinkageDelete(This) ) 

#define IParser_PostProcessOpen(This,PostProcessor,pRetVal)	\
    ( (This)->lpVtbl -> PostProcessOpen(This,PostProcessor,pRetVal) ) 

#define IParser_PostProcessClose(This)	\
    ( (This)->lpVtbl -> PostProcessClose(This) ) 

#define IParser_LinkagePostProcess(This,pRetVal)	\
    ( (This)->lpVtbl -> LinkagePostProcess(This,pRetVal) ) 

#define IParser_ResetResources(This)	\
    ( (This)->lpVtbl -> ResetResources(This) ) 

#define IParser_get_CostModelType(This,pVal)	\
    ( (This)->lpVtbl -> get_CostModelType(This,pVal) ) 

#define IParser_put_CostModelType(This,newVal)	\
    ( (This)->lpVtbl -> put_CostModelType(This,newVal) ) 

#define IParser_get_LinkageIsCanonical(This,pVal)	\
    ( (This)->lpVtbl -> get_LinkageIsCanonical(This,pVal) ) 

#define IParser_get_LinkageIsImproper(This,pVal)	\
    ( (This)->lpVtbl -> get_LinkageIsImproper(This,pVal) ) 

#define IParser_get_LinkageHasInconsistentDomains(This,pVal)	\
    ( (This)->lpVtbl -> get_LinkageHasInconsistentDomains(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IParser_INTERFACE_DEFINED__ */



#ifndef __LINKPARSERLib_LIBRARY_DEFINED__
#define __LINKPARSERLib_LIBRARY_DEFINED__

/* library LINKPARSERLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_LINKPARSERLib;

EXTERN_C const CLSID CLSID_Parser;

#ifdef __cplusplus

class DECLSPEC_UUID("78636160-6455-11D2-B8DB-00002441B3CB")
Parser;
#endif
#endif /* __LINKPARSERLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


