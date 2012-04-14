/*
 * The contents of this file are subject to the Mozilla Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 * 
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 * 
 * The Original Code is mozilla.org code.
 * 
 * The Initial Developer of the Original Code is Christopher Blizzard.
 * Portions created by Christopher Blizzard are Copyright (C)
 * Christopher Blizzard.  All Rights Reserved.
 * 
 * Contributor(s):
 *   Christopher Blizzard <blizzard@mozilla.org>
 */

#include <nsIDocShell.h>
#include <nsIWebProgress.h>
#include <nsIWebBrowserFocus.h>
#include <nsCRT.h>
#if 0
#include <nsNetUtil.h>
#include <nsIWebBrowserStream.h>
#endif

// for NS_APPSHELL_CID
#include <nsWidgetsCID.h>

// for do_GetInterface
#include <nsIInterfaceRequestor.h>
// for do_CreateInstance
#include <nsIComponentManager.h>

// for initializing our window watcher service
#include <nsIWindowWatcher.h>

//#include <nsILocalFile.h>
#include <nsEmbedAPI.h>

// all of the crap that we need for event listeners
// and when chrome windows finish loading
#include <nsIDOMWindow.h>
#include <nsIDOMWindowInternal.h>

// For seting scrollbar visibilty
#include <nsIDOMBarProp.h>

// app component registration
#include <nsIGenericFactory.h>
#include <nsIComponentRegistrar.h>

#include <nsISupportsUtils.h>

// all of our local includes
#include "EmbedPrivate.h"
#include "EmbedWindow.h"
#include "EmbedProgress.h"
#include "EmbedContentListener.h"
#include "EmbedEventListener.h"
#include "EmbedWindowCreator.h"
#include "EmbedStream.h"
#include "GtkPromptService.h"
#include "MozillaPrivate.h"

#ifdef MOZ_ACCESSIBILITY_ATK
#include "nsIAccessibilityService.h"
#include "nsIAccessible.h"
#include "nsIDOMDocument.h"
#endif

#ifdef _BUILD_STATIC_BIN
#include "nsStaticComponent.h"
nsresult PR_CALLBACK
gtk_getModuleInfo(nsStaticModuleInfo **info, PRUint32 *count);
#endif

static NS_DEFINE_CID(kAppShellCID, NS_APPSHELL_CID);

PRUint32     EmbedPrivate::sWidgetCount = 0;
char        *EmbedPrivate::sCompPath    = nsnull;
nsIAppShell *EmbedPrivate::sAppShell    = nsnull;
GList       *EmbedPrivate::sWindowList  = NULL;
char        *EmbedPrivate::sProfileDir  = nsnull;
char        *EmbedPrivate::sProfileName = nsnull;
GtkWidget   *EmbedPrivate::sOffscreenWindow = 0;
GtkWidget   *EmbedPrivate::sOffscreenFixed  = 0;
nsIDirectoryServiceProvider *EmbedPrivate::sAppFileLocProvider = nsnull;
MozillaEmbedPrivate         *EmbedPrivate::sMozillaEmbedPrivate = nsnull;

#if 0
#define NS_PROMPTSERVICE_CID \
 {0x95611356, 0xf583, 0x46f5, {0x81, 0xff, 0x4b, 0x3e, 0x01, 0x62, 0xc6, 0x19}}

NS_GENERIC_FACTORY_CONSTRUCTOR(GtkPromptService)

static const nsModuleComponentInfo defaultAppComps[] = {
  {
    "Prompt Service",
    NS_PROMPTSERVICE_CID,
    "@mozilla.org/embedcomp/prompt-service;1",
    GtkPromptServiceConstructor
  }
};

const nsModuleComponentInfo *EmbedPrivate::sAppComps = defaultAppComps;
int   EmbedPrivate::sNumAppComps = sizeof(defaultAppComps) / sizeof(nsModuleComponentInfo);
#endif

EmbedPrivate::EmbedPrivate(void)
{
  mOwningWidget     = nsnull;
  mWindow           = nsnull;
  mProgress         = nsnull;
  mContentListener  = nsnull;
  mEventListener    = nsnull;
  mStream	    = nsnull;
  mChromeMask       = nsIWebBrowserChrome::CHROME_ALL;
  mIsChrome         = PR_FALSE;
  mChromeLoaded     = PR_FALSE;
  mListenersAttached = PR_FALSE;
  mMozWindowWidget  = 0;
  mIsDestroyed      = PR_FALSE;

  PushStartup();

  sWindowList = g_list_append(sWindowList, this);
}

EmbedPrivate::~EmbedPrivate()
{
  sWindowList = g_list_remove(sWindowList, this);
  PopStartup();
}

nsresult
EmbedPrivate::Init(GtkMozEmbed *aOwningWidget)
{
  // are we being re-initialized?
  if (mOwningWidget)
    return NS_OK;

  // hang on with a reference to the owning widget
  mOwningWidget = aOwningWidget;

  // Create our embed window, and create an owning reference to it and
  // initialize it.  It is assumed that this window will be destroyed
  // when we go out of scope.
  mWindow = new EmbedWindow();
  mWindowGuard = NS_STATIC_CAST(nsIWebBrowserChrome *, mWindow);
  mWindow->Init(this);

  // Create our progress listener object, make an owning reference,
  // and initialize it.  It is assumed that this progress listener
  // will be destroyed when we go out of scope.
  mProgress = new EmbedProgress();
  mProgressGuard = NS_STATIC_CAST(nsIWebProgressListener *,
				       mProgress);
  mProgress->Init(this);

  // Create our content listener object, initialize it and attach it.
  // It is assumed that this will be destroyed when we go out of
  // scope.
  mContentListener = new EmbedContentListener();
  mContentListenerGuard = NS_STATIC_CAST(nsISupports*, NS_STATIC_CAST(nsIURIContentListener*, mContentListener));
  mContentListener->Init(this);

  // Create our key listener object and initialize it.  It is assumed
  // that this will be destroyed before we go out of scope.
  mEventListener = new EmbedEventListener();
  mEventListenerGuard =
    NS_STATIC_CAST(nsISupports *, NS_STATIC_CAST(nsIDOMKeyListener *,
						 mEventListener));
  mEventListener->Init(this);

  // has the window creator service been set up?
  static int initialized = PR_FALSE;
  // Set up our window creator ( only once )
  if (!initialized) {
    // We set this flag here instead of on success.  If it failed we
    // don't want to keep trying and leaking window creator objects.
    initialized = PR_TRUE;

    // create our local object
    EmbedWindowCreator *creator = new EmbedWindowCreator();
    nsCOMPtr<nsIWindowCreator> windowCreator;
    windowCreator = NS_STATIC_CAST(nsIWindowCreator *, creator);

    // Attach it via the watcher service
    nsCOMPtr<nsIWindowWatcher> watcher = do_GetService(NS_WINDOWWATCHER_CONTRACTID);
    if (watcher)
      watcher->SetWindowCreator(windowCreator);
  }
  return NS_OK;
}

nsresult
EmbedPrivate::Realize(PRBool *aAlreadyRealized)
{

  *aAlreadyRealized = PR_FALSE;

  // create the offscreen window if we have to
  EnsureOffscreenWindow();

  // Have we ever been initialized before?  If so then just reparetn
  // from the offscreen window.
  if (mMozWindowWidget) {
    gtk_widget_reparent(mMozWindowWidget, GTK_WIDGET(mOwningWidget));
    *aAlreadyRealized = PR_TRUE;
    return NS_OK;
  }

  // Get the nsIWebBrowser object for our embedded window.
  nsCOMPtr<nsIWebBrowser> webBrowser;
  mWindow->GetWebBrowser(getter_AddRefs(webBrowser));

  // get a handle on the navigation object
  mNavigation = do_QueryInterface(webBrowser);

  // Create our session history object and tell the navigation object
  // to use it.  We need to do this before we create the web browser
  // window.
  mSessionHistory = do_CreateInstance(NS_SHISTORY_CONTRACTID);
  mNavigation->SetSessionHistory(mSessionHistory);

  // create the window
  mWindow->CreateWindow();

  // bind the progress listener to the browser object
  nsCOMPtr<nsISupportsWeakReference> supportsWeak;
  supportsWeak = do_QueryInterface(mProgressGuard);
  nsCOMPtr<nsIWeakReference> weakRef;
  supportsWeak->GetWeakReference(getter_AddRefs(weakRef));
  webBrowser->AddWebBrowserListener(weakRef,
				    nsIWebProgressListener::GetIID());

  // set ourselves as the parent uri content listener
  nsCOMPtr<nsIURIContentListener> uriListener;
  uriListener = do_QueryInterface(mContentListenerGuard);
  webBrowser->SetParentURIContentListener(uriListener);

  // save the window id of the newly created window
  // get the native drawing area
  GdkWindow *tmp_window = MozillaPrivate::GetGdkWindow(mWindow->mBaseWindow);
  
  // and, thanks to superwin we actually need the parent of that.
  tmp_window = gdk_window_get_parent(tmp_window);
  // save the widget ID - it should be the mozarea of the window.
  gpointer data = nsnull;
  gdk_window_get_user_data(tmp_window, &data);
  mMozWindowWidget = NS_STATIC_CAST(GtkWidget *, data);

  // Apply the current chrome mask
  ApplyChromeMask();

  return NS_OK;
}

void
EmbedPrivate::Unrealize(void)
{
  // reparent to our offscreen window
  gtk_widget_reparent(mMozWindowWidget, sOffscreenFixed);
}

void
EmbedPrivate::Show(void)
{
  // Get the nsIWebBrowser object for our embedded window.
  nsCOMPtr<nsIWebBrowser> webBrowser;
  mWindow->GetWebBrowser(getter_AddRefs(webBrowser));

  // and set the visibility on the thing
  nsCOMPtr<nsIBaseWindow> baseWindow = do_QueryInterface(webBrowser);
  baseWindow->SetVisibility(PR_TRUE);
}

void
EmbedPrivate::Hide(void)
{
  // Get the nsIWebBrowser object for our embedded window.
  nsCOMPtr<nsIWebBrowser> webBrowser;
  mWindow->GetWebBrowser(getter_AddRefs(webBrowser));

  // and set the visibility on the thing
  nsCOMPtr<nsIBaseWindow> baseWindow = do_QueryInterface(webBrowser);
  baseWindow->SetVisibility(PR_FALSE);
}

void
EmbedPrivate::Resize(PRUint32 aWidth, PRUint32 aHeight)
{
  mWindow->SetDimensions(nsIEmbeddingSiteWindow::DIM_FLAGS_POSITION |
			 nsIEmbeddingSiteWindow::DIM_FLAGS_SIZE_INNER,
			 0, 0, aWidth, aHeight);
}

void
EmbedPrivate::GetSize(PRInt32 *aWidth, PRInt32 *aHeight)
{
  mWindow->GetDimensions(nsIEmbeddingSiteWindow::DIM_FLAGS_SIZE_INNER,
			 nsnull, nsnull, aWidth, aHeight);
}

void
EmbedPrivate::Destroy(void)
{
  // This flag might have been set from
  // EmbedWindow::DestroyBrowserWindow() as well if someone used a
  // window.close() or something or some other script action to close
  // the window.  No harm setting it again.
  mIsDestroyed = PR_TRUE;

  // Get the nsIWebBrowser object for our embedded window.
  nsCOMPtr<nsIWebBrowser> webBrowser;
  mWindow->GetWebBrowser(getter_AddRefs(webBrowser));

  // Release our progress listener
  nsCOMPtr<nsISupportsWeakReference> supportsWeak;
  supportsWeak = do_QueryInterface(mProgressGuard);
  nsCOMPtr<nsIWeakReference> weakRef;
  supportsWeak->GetWeakReference(getter_AddRefs(weakRef));
  webBrowser->RemoveWebBrowserListener(weakRef,
				       nsIWebProgressListener::GetIID());
  weakRef = nsnull;
  supportsWeak = nsnull;

  // Release our content listener
  webBrowser->SetParentURIContentListener(nsnull);
  mContentListenerGuard = nsnull;
  mContentListener = nsnull;

  // Now that we have removed the listener, release our progress
  // object
  mProgressGuard = nsnull;
  mProgress = nsnull;

  // detach our event listeners and release the event receiver
  DetachListeners();
  if (mEventReceiver)
    mEventReceiver = nsnull;
  
  // destroy our child window
  mWindow->ReleaseChildren();

  // release navigation
  mNavigation = nsnull;

  // release session history
  mSessionHistory = nsnull;

  mOwningWidget = nsnull;

  mMozWindowWidget = 0;
}

void
EmbedPrivate::SetURI(const char *aURI)
{
  NS_CStringToUTF16(nsEmbedCString(aURI),
	  	    NS_CSTRING_ENCODING_UTF8, mURI);
}

void
EmbedPrivate::LoadCurrentURI(void)
{
  if (mURI.Length()) {
#if 0 
    nsCOMPtr<nsPIDOMWindow> piWin;
    GetPIDOMWindow(getter_AddRefs(piWin));

    nsAutoPopupStatePusher popupStatePusher(piWin, openAllowed);
#endif
    mNavigation->LoadURI(mURI.get(),                        // URI string
                         nsIWebNavigation::LOAD_FLAGS_NONE, // Load flags
                         nsnull,                            // Referring URI
                         nsnull,                            // Post data
                         nsnull);                           // extra headers
  }
}

void
EmbedPrivate::Reload(PRUint32 reloadFlags)
{
  /* Use the session history if it is available, this
   * allows framesets to reload correctly */
  nsCOMPtr<nsIWebNavigation> wn;

  if (mSessionHistory) {
    wn = do_QueryInterface(mSessionHistory);
  }
  if (!wn)
    wn = mNavigation;

  if (wn)
    wn->Reload(reloadFlags);
}

void
EmbedPrivate::ApplyChromeMask()
{
   if (mWindow) {
      nsCOMPtr<nsIWebBrowser> webBrowser;
      mWindow->GetWebBrowser(getter_AddRefs(webBrowser));

      nsCOMPtr<nsIDOMWindow> domWindow;
      webBrowser->GetContentDOMWindow(getter_AddRefs(domWindow));
      if (domWindow) {

         nsCOMPtr<nsIDOMBarProp> scrollbars;
         domWindow->GetScrollbars(getter_AddRefs(scrollbars));
         if (scrollbars) {

            scrollbars->SetVisible
               (mChromeMask & nsIWebBrowserChrome::CHROME_SCROLLBARS ?
                PR_TRUE : PR_FALSE);
         }
      }
   }
}


void
EmbedPrivate::SetChromeMask(PRUint32 aChromeMask)
{
   mChromeMask = aChromeMask;

   ApplyChromeMask();
}

/* static */
void
EmbedPrivate::PushStartup(void)
{
  // increment the number of widgets
  sWidgetCount++;
  
  // if this is the first widget, fire up xpcom
  if (sWidgetCount == 1) {
    nsresult rv;
    nsCOMPtr<nsILocalFile> binDir;
    
    if (sCompPath) {
      rv = NS_NewNativeLocalFile(nsEmbedCString(sCompPath), 1, getter_AddRefs(binDir));
      if (NS_FAILED(rv))
	return;
    }

#ifdef _BUILD_STATIC_BIN
    // Initialize XPCOM's module info table
    NSGetStaticModuleInfo = gtk_getModuleInfo;
#endif

    rv = NS_InitEmbedding(binDir, sAppFileLocProvider);
    if (NS_FAILED(rv))
      return;

    // we no longer need a reference to the DirectoryServiceProvider
    if (sAppFileLocProvider) {
      NS_RELEASE(sAppFileLocProvider);
      sAppFileLocProvider = nsnull;
    }

    sMozillaEmbedPrivate = new MozillaEmbedPrivate;
    rv = sMozillaEmbedPrivate->StartupProfile(sProfileDir, sProfileName);
    NS_WARN_IF_FALSE(NS_SUCCEEDED(rv), "Warning: Failed to start up profiles.\n");

    rv = RegisterAppComponents();
    NS_WARN_IF_FALSE(NS_SUCCEEDED(rv), "Warning: Failed to register app components.\n");

    // XXX startup appshell service?
    // XXX create offscreen window for appshell service?
    // XXX remove X prop from offscreen window?
    
    nsCOMPtr<nsIAppShell> appShell;
    appShell = do_CreateInstance(kAppShellCID);
    if (!appShell) {
      NS_WARNING("Failed to create appshell in EmbedPrivate::PushStartup!\n");
      return;
    }
    sAppShell = appShell.get();
    NS_ADDREF(sAppShell);
    sAppShell->Create(0, nsnull);
    sAppShell->Spinup();
  }
}

/* static */
void
EmbedPrivate::PopStartup(void)
{
  sWidgetCount--;
  if (sWidgetCount == 0) {

    // destroy the offscreen window
    DestroyOffscreenWindow();
    
    // shut down the profiles
    sMozillaEmbedPrivate->ShutdownProfile();

    if (sAppShell) {
      // Shutdown the appshell service.
      sAppShell->Spindown();
      NS_RELEASE(sAppShell);
      sAppShell = 0;
    }

    delete sMozillaEmbedPrivate;
    // shut down XPCOM/Embedding
    NS_TermEmbedding();
  }
}

/* static */
void
EmbedPrivate::SetCompPath(const char *aPath)
{
  if (sCompPath)
    free(sCompPath);
  if (aPath)
    sCompPath = strdup(aPath);
  else
    sCompPath = nsnull;
}

/* static */
void
EmbedPrivate::SetAppComponents(const nsModuleComponentInfo* aComps,
                               int aNumComponents)
{
//  sAppComps = aComps;
//  sNumAppComps = aNumComponents;
}

/* static */
void
EmbedPrivate::SetProfilePath(const char *aDir, const char *aName)
{
  if (sProfileDir) {
    nsMemory::Free(sProfileDir);
    sProfileDir = nsnull;
  }

  if (sProfileName) {
    nsMemory::Free(sProfileName);
    sProfileName = nsnull;
  }

  if (aDir)
    sProfileDir = (char *)nsMemory::Clone(aDir, strlen(aDir) + 1);
  
  if (aName)
    sProfileName = (char *)nsMemory::Clone(aName, strlen(aName) + 1);
}

void 
EmbedPrivate::SetDirectoryServiceProvider(nsIDirectoryServiceProvider * appFileLocProvider) 
{
  if (sAppFileLocProvider)
    NS_RELEASE(sAppFileLocProvider);
  
  if (appFileLocProvider) {
    sAppFileLocProvider = appFileLocProvider;
    NS_ADDREF(sAppFileLocProvider);
  }
}

#if 1
nsresult
EmbedPrivate::OpenStream(const char *aBaseURI, const char *aContentType)
{
  nsresult rv;

  if (!mStream) {
    mStream = new EmbedStream();
    mStreamGuard = do_QueryInterface(mStream);
    mStream->InitOwner(this);
    rv = mStream->Init();
    if (NS_FAILED(rv))
      return rv;
  }

  rv = mStream->OpenStream(aBaseURI, aContentType);
}

nsresult
EmbedPrivate::AppendToStream(const char *aData, PRInt32 aLen)
{
  if (!mStream)
    return NS_ERROR_FAILURE;

  // Attach listeners to this document since in some cases we don't
  // get updates for content added this way.
  ContentStateChange();

  return mStream->AppendToStream(aData, aLen);
}

nsresult
EmbedPrivate::CloseStream(void)
{
  nsresult rv;

  if (!mStream)
    return NS_ERROR_FAILURE;
  rv = mStream->CloseStream();

  // release
  mStream = 0;
  mStreamGuard = 0;

  return rv;
}
#endif

#if 0
nsresult
EmbedPrivate::OpenStream(const char *aBaseURI, const char *aContentType)
{
  nsCOMPtr<nsIWebBrowser> webBrowser;
  mWindow->GetWebBrowser(getter_AddRefs(webBrowser));

  nsCOMPtr<nsIWebBrowserStream> wbStream = do_QueryInterface(webBrowser);
  if (!wbStream) return NS_ERROR_FAILURE;

  nsCOMPtr<nsIURI> uri;
  nsresult rv = NS_NewURI(getter_AddRefs(uri), aBaseURI);
  if (NS_FAILED(rv))
    return rv;

  rv = wbStream->OpenStream(uri, nsDependentCString(aContentType));
  return rv;
}

nsresult
EmbedPrivate::AppendToStream(const PRUint8 *aData, PRUint32 aLen)
{
  // Attach listeners to this document since in some cases we don't
  // get updates for content added this way.
  ContentStateChange();

  nsCOMPtr<nsIWebBrowser> webBrowser;
  mWindow->GetWebBrowser(getter_AddRefs(webBrowser));

  nsCOMPtr<nsIWebBrowserStream> wbStream = do_QueryInterface(webBrowser);
  if (!wbStream) return NS_ERROR_FAILURE;

  return wbStream->AppendToStream(aData, aLen);
}

nsresult
EmbedPrivate::CloseStream(void)
{
  nsCOMPtr<nsIWebBrowser> webBrowser;
  mWindow->GetWebBrowser(getter_AddRefs(webBrowser));

  nsCOMPtr<nsIWebBrowserStream> wbStream = do_QueryInterface(webBrowser);
  if (!wbStream) return NS_ERROR_FAILURE;

  return wbStream->CloseStream();
}
#endif

/* static */
EmbedPrivate *
EmbedPrivate::FindPrivateForBrowser(nsIWebBrowserChrome *aBrowser)
{
  if (!sWindowList)
    return nsnull;

  // Get the number of browser windows.
  PRInt32 count = (PRInt32)g_list_length(sWindowList);
  // This function doesn't get called very often at all ( only when
  // creating a new window ) so it's OK to walk the list of open
  // windows.
  for (int i = 0; i < count; i++) {
    EmbedPrivate *tmpPrivate = NS_STATIC_CAST(EmbedPrivate *,
					      g_list_nth_data(sWindowList, i));
    // get the browser object for that window
    nsIWebBrowserChrome *chrome = NS_STATIC_CAST(nsIWebBrowserChrome *,
						 tmpPrivate->mWindow);
    if (chrome == aBrowser)
      return tmpPrivate;
  }

  return nsnull;
}

void
EmbedPrivate::ContentStateChange(void)
{

  // we don't attach listeners to chrome
  if (mListenersAttached && !mIsChrome)
    return;

  GetListener();

  if (!mEventReceiver)
    return;
  
  AttachListeners();

}

void
EmbedPrivate::ContentFinishedLoading(void)
{
  if (mIsChrome) {
    // We're done loading.
    mChromeLoaded = PR_TRUE;

    // get the web browser
    nsCOMPtr<nsIWebBrowser> webBrowser;
    mWindow->GetWebBrowser(getter_AddRefs(webBrowser));
    
    // get the content DOM window for that web browser
    nsCOMPtr<nsIDOMWindow> domWindow;
    webBrowser->GetContentDOMWindow(getter_AddRefs(domWindow));
    if (!domWindow) {
      NS_WARNING("no dom window in content finished loading\n");
      return;
    }
    
    // resize the content
    domWindow->SizeToContent();

    // and since we're done loading show the window, assuming that the
    // visibility flag has been set.
    PRBool visibility;
    mWindow->GetVisibility(&visibility);
    if (visibility)
      mWindow->SetVisibility(PR_TRUE);
  }
}

// handle focus in and focus out events

void
EmbedPrivate::ChildFocusIn(void)
{
  nsresult result;

  if (mIsDestroyed)
    return;

  nsCOMPtr<nsIWebBrowser> webBrowser;
  result = mWindow->GetWebBrowser(getter_AddRefs(webBrowser));
  if (NS_FAILED (result)) return;

  nsCOMPtr<nsIWebBrowserFocus> webBrowserFocus;
  webBrowserFocus = do_QueryInterface (webBrowser);
  if (!webBrowserFocus) return;

  webBrowserFocus->Activate();
}

void
EmbedPrivate::ChildFocusOut(void)
{
  nsresult result;

  if (mIsDestroyed)
    return;

  nsCOMPtr<nsIWebBrowser> webBrowser;
  result = mWindow->GetWebBrowser(getter_AddRefs(webBrowser));
  if (NS_FAILED (result)) return;

  nsCOMPtr<nsIWebBrowserFocus> webBrowserFocus;
  webBrowserFocus = do_QueryInterface (webBrowser);
  if (!webBrowserFocus) return;

  webBrowserFocus->Deactivate();
}

// Get the event listener for the chrome event handler.

void
EmbedPrivate::GetListener(void)
{
  if (mEventReceiver)
    return;

  nsCOMPtr<nsIWebBrowser> webBrowser;
  mWindow->GetWebBrowser(getter_AddRefs(webBrowser));

  // get the content DOM window for that web browser
  nsCOMPtr<nsIDOMWindow> domWindow;
  webBrowser->GetContentDOMWindow(getter_AddRefs(domWindow));
  if (!domWindow)
    return; 

  MozillaPrivate::GetEventReceiver(domWindow, getter_AddRefs(mEventReceiver));
}

// attach key and mouse event listeners

void
EmbedPrivate::AttachListeners(void)
{
  if (!mEventReceiver || mListenersAttached)
    return;

  nsIDOMEventListener *eventListener =
    NS_STATIC_CAST(nsIDOMEventListener *,
		   NS_STATIC_CAST(nsIDOMKeyListener *, mEventListener));

  // add the key listener
  nsresult rv;
  rv = mEventReceiver->AddEventListenerByIID(eventListener,
					     NS_GET_IID(nsIDOMKeyListener));
  if (NS_FAILED(rv)) {
    NS_WARNING("Failed to add key listener\n");
    return;
  }

  rv = mEventReceiver->AddEventListenerByIID(eventListener,
					     NS_GET_IID(nsIDOMMouseListener));
  if (NS_FAILED(rv)) {
    NS_WARNING("Failed to add mouse listener\n");
    return;
  }
  rv = mEventReceiver->AddEventListenerByIID(eventListener,
					     NS_GET_IID(nsIDOMEventListener));
#if 0
  const PRUnichar domLiteral[] = { 'D', 'O', 'M', 'L', 'i', 'n', 'k',
	  			   'A', 'd', 'd', 'e', 'd', '\0' };

  nsCOMPtr<nsIDOMEventTarget> target;
  target = do_QueryInterface (mEventReceiver);
  target->AddEventListener(nsEmbedString(domLiteral),
			   eventListener, PR_FALSE);
#endif
#if 0
  rv = mEventReceiver->AddEventListenerByIID(eventListener,
                                             NS_GET_IID(nsIDOMUIListener));
  if (NS_FAILED(rv)) {
    NS_WARNING("Failed to add UI listener\n");
    return;
  }
#endif
  // ok, all set.
  mListenersAttached = PR_TRUE;
}

void
EmbedPrivate::DetachListeners(void)
{
  if (!mListenersAttached || !mEventReceiver)
    return;

  nsIDOMEventListener *eventListener =
    NS_STATIC_CAST(nsIDOMEventListener *,
		   NS_STATIC_CAST(nsIDOMKeyListener *, mEventListener));

  nsresult rv;
  rv = mEventReceiver->RemoveEventListenerByIID(eventListener,
						NS_GET_IID(nsIDOMKeyListener));
  if (NS_FAILED(rv)) {
    NS_WARNING("Failed to remove key listener\n");
    return;
  }

  rv =
    mEventReceiver->RemoveEventListenerByIID(eventListener,
					     NS_GET_IID(nsIDOMMouseListener));
  if (NS_FAILED(rv)) {
    NS_WARNING("Failed to remove mouse listener\n");
    return;
  }

#if 0
  rv = mEventReceiver->RemoveEventListenerByIID(eventListener,
						NS_GET_IID(nsIDOMUIListener));
  if (NS_FAILED(rv)) {
    NS_WARNING("Failed to remove UI listener\n");
    return;
  }
#endif
  mListenersAttached = PR_FALSE;
}

#if 0
nsresult
EmbedPrivate::GetPIDOMWindow(nsPIDOMWindow **aPIWin)
{
  *aPIWin = nsnull;

  // get the web browser
  nsCOMPtr<nsIWebBrowser> webBrowser;
  mWindow->GetWebBrowser(getter_AddRefs(webBrowser));

  // get the content DOM window for that web browser
  nsCOMPtr<nsIDOMWindow> domWindow;
  webBrowser->GetContentDOMWindow(getter_AddRefs(domWindow));
  if (!domWindow)
    return NS_ERROR_FAILURE;

  // get the private DOM window
  nsCOMPtr<nsPIDOMWindow> domWindowPrivate = do_QueryInterface(domWindow);
  // and the root window for that DOM window
  nsCOMPtr<nsIDOMWindowInternal> rootWindow;
  domWindowPrivate->GetPrivateRoot(getter_AddRefs(rootWindow));
  
  nsCOMPtr<nsIChromeEventHandler> chromeHandler;
  nsCOMPtr<nsPIDOMWindow> piWin(do_QueryInterface(rootWindow));

  *aPIWin = piWin.get();

  if (*aPIWin) {
    NS_ADDREF(*aPIWin);
    return NS_OK;
  }

  return NS_ERROR_FAILURE;

}
#endif

#ifdef MOZ_ACCESSIBILITY_ATK
void *
EmbedPrivate::GetAtkObjectForCurrentDocument()
{
  if (!mNavigation)
    return nsnull;

  nsCOMPtr<nsIAccessibilityService> accService =
    do_GetService("@mozilla.org/accessibilityService;1");
  if (accService) {
    //get current document
    nsCOMPtr<nsIDOMDocument> domDoc;
    mNavigation->GetDocument(getter_AddRefs(domDoc));
    NS_ENSURE_TRUE(domDoc, nsnull);

    nsCOMPtr<nsIDOMNode> domNode(do_QueryInterface(domDoc));
    NS_ENSURE_TRUE(domNode, nsnull);

    nsCOMPtr<nsIAccessible> acc;
    accService->GetAccessibleFor(domNode, getter_AddRefs(acc));
    NS_ENSURE_TRUE(acc, nsnull);

    void *atkObj = nsnull;
    if (NS_SUCCEEDED(acc->GetNativeInterface(&atkObj)))
      return atkObj;
  }
  return nsnull;
}
#endif /* MOZ_ACCESSIBILITY_ATK */

/* static */
nsresult
EmbedPrivate::RegisterAppComponents(void)
{
  nsCOMPtr<nsIComponentRegistrar> cr;
  nsresult rv = NS_GetComponentRegistrar(getter_AddRefs(cr));
  NS_ENSURE_SUCCESS(rv, rv);

  nsCOMPtr<nsIComponentManager> cm;
  rv = NS_GetComponentManager (getter_AddRefs (cm));
  NS_ENSURE_SUCCESS (rv, rv);
#if 0
  for (int i = 0; i < sNumAppComps; ++i) {
    nsCOMPtr<nsIGenericFactory> componentFactory;
    rv = NS_NewGenericFactory(getter_AddRefs(componentFactory),
                              &(sAppComps[i]));
    if (NS_FAILED(rv)) {
      NS_WARNING("Unable to create factory for component");
      continue;  // don't abort registering other components
    }

    rv = cr->RegisterFactory(sAppComps[i].mCID, sAppComps[i].mDescription,
                             sAppComps[i].mContractID, componentFactory);
    NS_ASSERTION(NS_SUCCEEDED(rv), "Unable to register factory for component");
    // Call the registration hook of the component, if any
    if (sAppComps[i].mRegisterSelfProc) {
      rv = sAppComps[i].mRegisterSelfProc(cm, nsnull, nsnull, nsnull,
                                          &(sAppComps[i]));
      NS_ASSERTION(NS_SUCCEEDED(rv), "Unable to self-register component");
    }
  }
#endif
  return rv;
}
			     
/* static */
void
EmbedPrivate::EnsureOffscreenWindow(void)
{
  if (sOffscreenWindow)
    return;
  sOffscreenWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_widget_realize(sOffscreenWindow);
  sOffscreenFixed = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(sOffscreenWindow), sOffscreenFixed);
  gtk_widget_realize(sOffscreenFixed);
}

/* static */
void
EmbedPrivate::DestroyOffscreenWindow(void)
{
  if (!sOffscreenWindow)
    return;
  gtk_widget_destroy(sOffscreenWindow);
  sOffscreenWindow = 0;
}
