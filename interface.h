/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Fri Oct 09 13:46:14 1998
 */
/* Compiler settings for C:\Dev\Gooch\Parser\Parser.odl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __interface_h__
#define __interface_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __ILinkParser_FWD_DEFINED__
#define __ILinkParser_FWD_DEFINED__
typedef interface ILinkParser ILinkParser;
#endif 	/* __ILinkParser_FWD_DEFINED__ */


#ifndef __LinkParser_FWD_DEFINED__
#define __LinkParser_FWD_DEFINED__

#ifdef __cplusplus
typedef class LinkParser LinkParser;
#else
typedef struct LinkParser LinkParser;
#endif /* __cplusplus */

#endif 	/* __LinkParser_FWD_DEFINED__ */


void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 


#ifndef __Parser_LIBRARY_DEFINED__
#define __Parser_LIBRARY_DEFINED__

/* library Parser */
/* [helpstring][version][uuid] */ 


DEFINE_GUID(LIBID_Parser,0x17844BD4,0x4298,0x11D2,0xB8,0x94,0x00,0x00,0x24,0x41,0xB3,0xCB);

#ifndef __ILinkParser_DISPINTERFACE_DEFINED__
#define __ILinkParser_DISPINTERFACE_DEFINED__

/* dispinterface ILinkParser */
/* [helpstring][uuid] */ 


DEFINE_GUID(DIID_ILinkParser,0x17844BE4,0x4298,0x11D2,0xB8,0x94,0x00,0x00,0x24,0x41,0xB3,0xCB);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("17844BE4-4298-11D2-B894-00002441B3CB")
    ILinkParser : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct ILinkParserVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ILinkParser __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ILinkParser __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ILinkParser __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ILinkParser __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ILinkParser __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ILinkParser __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ILinkParser __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } ILinkParserVtbl;

    interface ILinkParser
    {
        CONST_VTBL struct ILinkParserVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILinkParser_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ILinkParser_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ILinkParser_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ILinkParser_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ILinkParser_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ILinkParser_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ILinkParser_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __ILinkParser_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_LinkParser,0x17844BE5,0x4298,0x11D2,0xB8,0x94,0x00,0x00,0x24,0x41,0xB3,0xCB);

#ifdef __cplusplus

class DECLSPEC_UUID("17844BE5-4298-11D2-B894-00002441B3CB")
LinkParser;
#endif
#endif /* __Parser_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
