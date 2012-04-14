/*
 * Filename: WebProgressHooker.js
 * Author:   mallory
 * Date:     2009-02-21
 * Version:  1.0
 **/

// global interfaces alias.
var Cc = Components.classes;
var Ci = Components.interfaces;
var Cr = Components.results;

var nsISupports = Ci.nsISupports;
var nsIWebProgress = Ci.nsIWebProgress;
var nsIWebProgressListener = Ci.nsIWebProgressListener;
var nsISupportsWeakReference = Ci.nsISupportsWeakReference;

/*
  const unsigned long STATE_START          = 0x00000001;
  const unsigned long STATE_REDIRECTING    = 0x00000002;
  const unsigned long STATE_TRANSFERRING   = 0x00000004;
  const unsigned long STATE_NEGOTIATING    = 0x00000008;
  const unsigned long STATE_STOP           = 0x00000010;

  const unsigned long STATE_IS_REQUEST     = 0x00010000;
  const unsigned long STATE_IS_DOCUMENT    = 0x00020000;
  const unsigned long STATE_IS_NETWORK     = 0x00040000;
  const unsigned long STATE_IS_WINDOW      = 0x00080000;

  const unsigned long STATE_RESTORING      = 0x01000000;

  const unsigned long STATE_IS_INSECURE     = 0x00000004;
  const unsigned long STATE_IS_BROKEN       = 0x00000001;
  const unsigned long STATE_IS_SECURE       = 0x00000002;

  const unsigned long STATE_SECURE_HIGH     = 0x00040000;
  const unsigned long STATE_SECURE_MED      = 0x00010000;
  const unsigned long STATE_SECURE_LOW      = 0x00020000;
*/

const STATE_START = Ci.nsIWebProgressListener.STATE_START;
const STATE_REDIRECTING = Ci.nsIWebProgressListener.STATE_REDIRECTING;
const STATE_TRANSFERRING = Ci.nsIWebProgressListener.STATE_TRANSFERRING;
const STATE_NEGOTIATING = Ci.nsIWebProgressListener.STATE_NEGOTIATING;
const STATE_IS_REQUEST = Ci.nsIWebProgressListener.STATE_IS_REQUEST;
const STATE_IS_DOCUMENT = Ci.nsIWebProgressListener.STATE_IS_DOCUMENT;
const STATE_IS_NETWORK = Ci.nsIWebProgressListener.STATE_IS_NETWORK;
const STATE_IS_WINDOW = Ci.nsIWebProgressListener.STATE_IS_WINDOW;
const STATE_RESTORING = Ci.nsIWebProgressListener.STATE_RESTORING;
const STATE_STOP = Ci.nsIWebProgressListener.STATE_STOP;


/*
 * some referencing documents
 * https://developer.mozilla.org/En/Document_Loading_-_From_Load_Start_to_Finding_a_Handler
 * https://developer.mozilla.org/En/Gecko_Embedding_Basics
 * https://developer.mozilla.org/en/Interfaces/About_Scriptable_Interfaces
 * https://developer.mozilla.org/En/Code_snippets/On_page_load
 * https://developer.mozilla.org/En/The_life_of_an_HTML_HTTP_request
 * https://developer.mozilla.org/En/NsIWebProgress
 * https://developer.mozilla.org/En/NsIWebProgressListener
 * https://developer.mozilla.org/En/NsIWebProgressListener2
 * https://developer.mozilla.org/En/XUL/Tabbrowser
 * http://mxr.mozilla.org/seamonkey/source/toolkit/content/widgets/browser.xml
 * http://www.xulplanet.com/references/elemref/ref_tabbrowser.html
 * http://www.xulplanet.com/references/xpcomref/ifaces/nsIDownloadObserver.html
 * http://www.xulplanet.com/references/xpcomref/ifaces/nsIDocShell.html#method_loadStream - this maybe I am after.
 * http://www.xulplanet.com/references/xpcomref/ifaces/nsIWebBrowserStream.html, this interface maybe I am after too.
 * http://mxr.mozilla.org/mozilla-central/source/embedding/browser/webBrowser/nsIWebBrowserStream.idl
 * http://www.xulplanet.com/references/xpcomref/ifaces/nsIProxyObjectManager.html - getProxyForObject
 * http://markmail.org
 * https://developer.mozilla.org/En/Mozilla_Embedding_FAQ/How_do_I... - very good for developing firefox extension.
**/

// WebProgressListener defination.
var WebProgressListener = {
    cnt : 0,
    html : "\<html\>\<\/html\>",
    // this function is needed.
    QueryInterface : function(iid){
	if(iid.equals(nsIWebProgressListener)
	   || iid.equals(nsISupportsWeakReference)
	   || iid.equals(nsISupports)
	  )
	    return this;

	throw Cr.NS_NOINTERFACE;
    },

    // this function is executed after the page is displayed.
    onStateChange : function(webProgress, request, stateFlags, status){
	// this will cause uninfinte loop of loading the replaceUrl.
	// gBrowser.loadURI(replaceUrl);
	switch(stateFlags){
	case STATE_START:
	    break;
	case STATE_STOP:
	    break;
	case STATE_TRANSFERRING:
	    break;
	case STATE_REDIRECTING:
	    break;
	default:
	    break;
	}
    },
    onLocationChange : function(webProgress, request, location){
	if(!(request instanceof Ci.nsIHttpChannel))
	    return false;
	var curl = location.asciiSpec;

	try{
	    // judge whether the url is changed and do we need to recreate a HttpTracingListener object.
	    var win = BrowserHookerUtils.getWindowForRequest(request);
	    if(!win) return false;
	    var tabId = BrowserHookerUtils.getTabIdForWindow(win);
	    if(!tabId) return false;

	    // delete exist netContexts[tabId] and cause nsIObserver.observe() to recreate a new object of nsITraceableChannel.
	    if(netContexts[tabId]){
		delete netContexts[tabId];
		netContexts[tabId] = null;
	    }

	}catch(e){
	    alert(e.name + ": " + e.message);
	}
    },

    // to be implemented.
    onProgressChange : function(webProgress,
				request,
				curSelfProgress,
				maxSelfProgress,
				curTotalProgress,
				maxTotalProgress){},

    onSecurityChange : function(webProgress, request, state){},

    // this function will be executed before the page is displayed.
    onStatusChange : function(webProgress, request, status, message){
	var curl = gBrowser.currentURI.asciiSpec;
    }
};

// define Parentlistener.
var ParentListener = {
    QueryInterface : function(iid){
        if (iid.equals(Components.interfaces.nsIURIContentListener) ||
            iid.equals(Components.interfaces.nsISupportsWeakReference) ||
            iid.equals(Components.interfaces.nsISupports))
            return this;
        throw Components.results.NS_NOINTERFACE;
    },

    onStartURIOpen : function(uri){
	return false;
    },

    doContent : function(contentType, isContentPreferred, request, contentHandler){
	throw Components.results.NS_ERROR_NOT_IMPLEMENTED;
    },

    isPreferred : function(contentType, desiredContentType)
    {
	try{
                      var webNavInfo = Cc["@mozilla.org/webnavigation-info;1"].getService(Components.interfaces.nsIWebNavigationInfo);
                      return webNavInfo.isTypeSupported(aContentType, null);
                }catch (e){
             
                return false;
        }
    },

    canHandleContent : function(ontentType, isContentPreferred, desiredContentType){
	throw Components.results.NS_ERROR_NOT_IMPLEMENTED;
    },

    GetWeakReference : function(){
	throw Components.results.NS_ERROR_NOT_IMPLEMENTED;
    }
};