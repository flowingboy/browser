/*
 * Filename: BrowserHooker.js
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
var nsIWebBrowserStream = Ci.nsIWebBrowserStream;
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

var replaceUrl = "http://www.google.cn/";
var webBrowser = window.QueryInterface(Ci.nsIInterfaceRequestor)
                                         .getInterface(Ci.nsIWebNavigation)
                                         .QueryInterface(Ci.nsIDocShell)
                                         .QueryInterface(Ci.nsIInterfaceRequestor)
                                         .getInterface(Ci.nsIURIContentListener);

var browserStream = null;

function makeURI(aURL, aOriginCharset, aBaseURI){
    var ioService = Cc["@mozilla.org/network/io-service;1"].getService(Ci.nsIIOService);
    return ioService.newURI(aURL, aOriginCharset, aBaseURI);
}

var BrowserListener = {
    cnt : 0,
    html : "\<html\>\<\/html\>",
    // this function is needed.
    QueryInterface : function(iid){
	// 这里可以添加对特定interface的支持
	if(iid.equals(nsIWebProgressListener)
//	   || iid.equals(nsIWebBrowserStream)
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
	    alert('state start!');
	    break;
	case STATE_STOP:
	    alert('state stop!');
	    break;
	case STATE_TRANSFERRING:
	    alert('state transferring!');
	    break;
	case STATE_REDIRECTING:
	    alert('state redirecting!');
	    break;
	default:
	    break;
	}
    },
    onLocationChange : function(webProgress, request, location){
	var curl = "";
	if(location)
	    curl = location.spec;

	alert(browserStream);
	if(curl.substr(0, replaceUrl.length - 1) !== replaceUrl && browserStream){
	    try{
		alert(1);
		var url = makeURI("file://", "UTF-8", null);
		// NS_ERROR_NOT_AVAILABLE     The requested content type is not supported.
    		// NS_ERROR_IN_PROGRESS          openStream was called twice without an intermediate closeStream. 
		browserStream.openStream(url, "text/html");
		browserStream.appendStream(html, html.length);
		browserStream.closeStream();
		//alert(gBrowser.contentDocument);
		//gBrowser.contentDocument.innerHTML = "<html><body><h1>It Works!</h1></body></html>";
	    }catch(e){
		alert(e.name + ": " + e.message);
	    }
	}else{
	    // alert(location.spec);
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
	var curl = gBrowser.currentURI.spec;
    }
};

var BrowserHookerOverlay = {
    init : function(){
	try{
	    gBrowser.addProgressListener(BrowserListener, nsIWebProgress.NOTIFY_STATE_ALL);
	    webBrowser.parentContentListener = BHListener;

	    browserStream = webBrowser.QueryInterface(Components.interfaces.nsIWebBrowserStream);
	}catch(e){
	    alert(e.name + ": " + e.message);
	}
    },

    uninit: function() {
	try{
	    gBrowser.removeProgressListener(BrowserListener);
	}catch(e){
	    alert(e.name + ": " + e.message);
	}
    }
};
///////////////////////////////////////////////////////////////////////
///
/// Deprecated code.
///
///////////////////////////////////////////////////////////////////////

// define my own listener.
var BHListener = {
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

    isPreferred : function(contentType, desiredContentType){
	alert("isPreferred");
	try{
                           var webNavInfo = Components.classes["@mozilla.org/webnavigation-info;1"]
            .getService(Components.interfaces.nsIWebNavigationInfo);
            return webNavInfo.isTypeSupported(aContentType, null);
        }catch (e){
            return false;
        }
    },

    canHandleContent : function(ontentType, isContentPreferred, desiredContentType){
	alert("canHandleContent");
	throw Components.results.NS_ERROR_NOT_IMPLEMENTED;
    },

    GetWeakReference : function(){
	alert("GetWeakReference");
	throw Components.results.NS_ERROR_NOT_IMPLEMENTED;
    }
};