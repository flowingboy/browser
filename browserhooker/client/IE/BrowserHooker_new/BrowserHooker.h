/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Jan 28 23:39:28 2010
 */
/* Compiler settings for E:\projects\Scriptlance\BrowserHooker\client\IE\BrowserHooker_new\BrowserHooker.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


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

#ifndef __BrowserHooker_h__
#define __BrowserHooker_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IRedirector_FWD_DEFINED__
#define __IRedirector_FWD_DEFINED__
typedef interface IRedirector IRedirector;
#endif 	/* __IRedirector_FWD_DEFINED__ */


#ifndef __Redirector_FWD_DEFINED__
#define __Redirector_FWD_DEFINED__

#ifdef __cplusplus
typedef class Redirector Redirector;
#else
typedef struct Redirector Redirector;
#endif /* __cplusplus */

#endif 	/* __Redirector_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IRedirector_INTERFACE_DEFINED__
#define __IRedirector_INTERFACE_DEFINED__

/* interface IRedirector */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IRedirector;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4236760C-9DDF-4B64-90D7-1729E3AD56DC")
    IRedirector : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IRedirectorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IRedirector __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IRedirector __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IRedirector __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IRedirector __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IRedirector __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IRedirector __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IRedirector __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } IRedirectorVtbl;

    interface IRedirector
    {
        CONST_VTBL struct IRedirectorVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRedirector_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IRedirector_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IRedirector_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IRedirector_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IRedirector_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IRedirector_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IRedirector_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IRedirector_INTERFACE_DEFINED__ */



#ifndef __BROWSERHOOKERLib_LIBRARY_DEFINED__
#define __BROWSERHOOKERLib_LIBRARY_DEFINED__

/* library BROWSERHOOKERLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_BROWSERHOOKERLib;

EXTERN_C const CLSID CLSID_Redirector;

#ifdef __cplusplus

class DECLSPEC_UUID("894FA370-FD14-4C2E-B28B-E3E119CA206B")
Redirector;
#endif
#endif /* __BROWSERHOOKERLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
