/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Jan 28 23:39:28 2010
 */
/* Compiler settings for E:\projects\Scriptlance\BrowserHooker\client\IE\BrowserHooker_new\BrowserHooker.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_IRedirector = {0x4236760C,0x9DDF,0x4B64,{0x90,0xD7,0x17,0x29,0xE3,0xAD,0x56,0xDC}};


const IID LIBID_BROWSERHOOKERLib = {0x68905ABF,0xDC52,0x4C6B,{0x92,0x16,0xB4,0xD5,0x82,0x55,0x28,0x37}};


const CLSID CLSID_Redirector = {0x894FA370,0xFD14,0x4C2E,{0xB2,0x8B,0xE3,0xE1,0x19,0xCA,0x20,0x6B}};


#ifdef __cplusplus
}
#endif

