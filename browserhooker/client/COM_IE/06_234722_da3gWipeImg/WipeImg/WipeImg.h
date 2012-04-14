

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Sun Apr 06 23:42:51 2008
 */
/* Compiler settings for .\WipeImg.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
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

#ifndef __WipeImg_h__
#define __WipeImg_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IWipeImgBHO_FWD_DEFINED__
#define __IWipeImgBHO_FWD_DEFINED__
typedef interface IWipeImgBHO IWipeImgBHO;
#endif 	/* __IWipeImgBHO_FWD_DEFINED__ */


#ifndef __WipeImgBHO_FWD_DEFINED__
#define __WipeImgBHO_FWD_DEFINED__

#ifdef __cplusplus
typedef class WipeImgBHO WipeImgBHO;
#else
typedef struct WipeImgBHO WipeImgBHO;
#endif /* __cplusplus */

#endif 	/* __WipeImgBHO_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IWipeImgBHO_INTERFACE_DEFINED__
#define __IWipeImgBHO_INTERFACE_DEFINED__

/* interface IWipeImgBHO */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IWipeImgBHO;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("075E971A-4972-4D11-8410-B80C2DD20E6C")
    IWipeImgBHO : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IWipeImgBHOVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWipeImgBHO * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWipeImgBHO * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWipeImgBHO * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWipeImgBHO * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWipeImgBHO * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWipeImgBHO * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWipeImgBHO * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IWipeImgBHOVtbl;

    interface IWipeImgBHO
    {
        CONST_VTBL struct IWipeImgBHOVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWipeImgBHO_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWipeImgBHO_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWipeImgBHO_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWipeImgBHO_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWipeImgBHO_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWipeImgBHO_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWipeImgBHO_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWipeImgBHO_INTERFACE_DEFINED__ */



#ifndef __WipeImgLib_LIBRARY_DEFINED__
#define __WipeImgLib_LIBRARY_DEFINED__

/* library WipeImgLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_WipeImgLib;

EXTERN_C const CLSID CLSID_WipeImgBHO;

#ifdef __cplusplus

class DECLSPEC_UUID("65876936-1D66-40D6-B5B8-F331CA79E39C")
WipeImgBHO;
#endif
#endif /* __WipeImgLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


