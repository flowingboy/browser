/*
 * Filename: BrowserHookerOverlay.js
 * Author:   mallory
 * Date:     2009-02-21
 * Version:  1.0
 **/

// global interfaces alias.
var Cc = Components.classes;
var Ci = Components.interfaces;
var Cr = Components.results;

// nsIDocumentLoaderFactory maybe the final rescure.
// http://www.archivum.info/mozilla.dev.tech.xpcom/2008-01/msg00027.html
// nsIStreamConverter.idl
// http://www.archivum.info/mozilla.dev.tech.xpcom/2008-01/msg00042.html manage loadGroup
// http://www.archivum.info/mozilla.dev.tech.xpcom/

function makeURI(aURL, aOriginCharset, aBaseURI){
    var ioService = Cc["@mozilla.org/network/io-service;1"].getService(Ci.nsIIOService);
    return ioService.newURI(aURL, aOriginCharset, aBaseURI);
}

// used to hold global contexts.
function BHContext(){
    this.listener = null;
    this.finished = false;
}

// http://www.softwareishard.com/blog/firebug/nsitraceablechannel-intercept-http-traffic/
// use nsILoadGroup to determine which tab is active.
// http://mxr.mozilla.org/mozilla-central/source/netwerk/base/public/nsILoadGroup.idl
var HttpRequestObserver = {
    QueryInterface : function(iid){
	if(iid.equals(Ci.nsIObserver)
	   || iid.equals(Ci.nsIRequestObserver)
                   || iid.equals(Ci.nsISupportsWeakReference)
	   || iid.equals(Ci.nsISupports)){
	    return this;
	}

	throw Components.results.NS_NOINTERFACE;
    },

    // http://mxr.mozilla.org/mozilla/source/netwerk/base/public/nsIRequest.idl
    // http://mxr.mozilla.org/mozilla/source/netwerk/base/public/nsILoadGroup.idl
    observe : function(aSubject, aTopic, aData){
	try{
	    // don't hook the firefox built-in safebrowsing mechanism.
	    if(aSubject.name.indexOf("http://safebrowsing.clients.google.com/") == 0
	       || aSubject.name.indexOf("http://static.cache.l.google.com/safebrowsing/") == 0){
		return false;
	    }

	    // bug fix for exception when use BrowserHookerUtils to get the main window variable.
	    var wm = Cc["@mozilla.org/appshell/window-mediator;1"].getService(Ci.nsIWindowMediator);
	    var mainWindow = wm.getMostRecentWindow(null);

	    var win = mainWindow.content;
	    if(!win) return false;

	    if(aSubject.name.indexOf("http://www.yahoo.com") === 0){
		return false;
	    }else
	    {
		if(aSubject.referer){
		    return false;
		}
	    }

	    var tabId = BrowserHookerUtils.getTabIdForWindow(win);
	    if(-1 == tabId) return false;

	    // https://developer.mozilla.org/En/Code_snippets/Tabbed_browser - Notification when a tab is added or removed
                    if (aTopic == "http-on-examine-response"){

		// don't deal with iframe window.
		if(!netContexts[tabId] && win == win.parent){
		    // do nothing.
		}else if(netContexts[tabId] && win == win.parent){
		    // judge http-referrer to determine whether the new url is from the page or 
		    // just input by user through the browser addressbar.
		    if(!aSubject.referrer)
		    {
			var newListener = new HttpTracingListener(tabId);
			netContexts[tabId].listener = newListener;
			netContexts[tabId].finished = true;
			aSubject.QueryInterface(Ci.nsITraceableChannel);
			netContexts[tabId].listener.originalListener = aSubject.setNewListener(newListener);
		    }

		}else{

		}
 	   // https://ladyr.es/svn/ladyrbrowser/LadyrBrowser/tags/JXPCOMBrowser-r0_1/JXPCOMBrowser/src/es/ladyr/jxpcombrowser/HTTPObserver.java
	   }else if(aTopic == "http-on-modify-request"){
	       // http://mxr.mozilla.org/mozilla/source/netwerk/protocol/http/public/nsIHttpChannel.idl
	    }else if(aTopic == "http-on-examine-cached-response"){
		// just for debug.
	    }else{

	    }

	}catch(e){
	    alert("HttpRequestObserver: "+e.message);
	}
    },

    onStartRequest : function(request, context){

    },

    onStopRequest : function(request, context, statusCode){

    },

};

// nsIStreamListener implementation.
function HttpTracingListener(tabId){
    this.tabId = tabId;
    this.originalListener = null;
    this.interceptData = '';
    this.cnt = 0;

}

HttpTracingListener.prototype = {
    QueryInterface : function(iid){
	if(iid.equals(Ci.nsIStremListener)
	   || iid.equals(Ci.nsIRequestObserver)
                   || iid.equals(Ci.nsISupportsWeakReference)
  	   || iid.equals(Ci.nsISupports)){
	    return this;
	}

               throw Components.results.NS_NOINTERFACE;
    },

    getInterface : function(iid){
	try{
	    this.QueryInterface(iid);
	}catch(e){
	    throw Cr.NS_NOINTERFACE;
	}
    },

    // check the request name to determine whether need to hook http response stream.
    // need to filter out all http requests from others extensions.
    onDataAvailable : function(
	request,		                // nsIRequest
	context,		                // nsISupports
	inputStream,		// nsIInputStream
	offset,			// int
	count){			// int
	try{
	    //  || request.name == 'about:document-onload-blocker')
	    if(!(request instanceof Ci.nsIHttpChannel) || request.name.indexOf("http") != 0){
		//this.originalListener.onDataAvailable(request, context, inputStream, offset, count);
	    }else{

		var requestStr = '';
		var i = 0;

		/*var enumRequests = request.loadGroup.requests;
		while(enumRequests.hasMoreElements()){
		    var req = enumRequests.getNext().QueryInterface(Ci.nsIRequest);
		    requestStr += req.name + '\n';
		    i++;
		}
              
		alert(requestStr);
		*/

		if(0 == this.cnt){
		    // check whether the session storage has stored the replaced page html code.
		    var rurl = "http://www.yahoo.com";
		    var hr = new HttpRequest();
		    var pageHtml = hr.getPageHtml(rurl);

		    // set base uri for current loaded html page.
		    this.interceptData = pageHtml.replace(/<head>/i, "<head><base href=\""+rurl+"\" />");

		    var storageStream = Cc['@mozilla.org/storagestream;1'].createInstance(Ci.nsIStorageStream);
		    var binaryOutputStream = Cc['@mozilla.org/binaryoutputstream;1'].createInstance(Ci.nsIBinaryOutputStream);
		    storageStream.init(8192, 1024000, null);
		    binaryOutputStream.setOutputStream(storageStream.getOutputStream(0));
		    binaryOutputStream.writeBytes(this.interceptData, this.interceptData.length);
		    
		    this.cnt++;
		    this.originalListener.onDataAvailable(request, context, storageStream.newInputStream(0), 0, this.interceptData.length);
		}else if(this.cnt > 0){
		    // here need to be commented:D
		    //this.originalListener.onDataAvailable(request, context, inputStream, offset, count);
                               }
	    }
	}catch(e){
	    alert("onDataAvailable: "+e.message);
	}
    },

    onStartRequest : function(request, context){
	this.originalListener.onStartRequest(request, context);
	// Make sure listener can't be replaced after OnStartRequest was called.
	request.QueryInterface(Ci.nsITraceableChannel);
	try {
	    var newListener = new HttpTracingListener();
	    newListener.originalListener = request.setNewListener(newListener);
	} catch(e) {
	    return; // OK
	}
    },

    onStopRequest : function(request, context, statusCode){
	this.originalListener.onStopRequest(request, context, statusCode);
    }
};

// get data from the server side.
const TimerCallbackForServerData = {
    notify : function(timer){
	try{

	    // getService(Ci.nsISupports) or getService()?
	    var bhd = Cc["@yahoo.cn/browser-hooker-daemon;1"].getService(nsISupports).wrappedJSObject;
	    //var bhd = Cc["@yahoo.cn/browser-hooker-daemon;1"].createInstance(Ci.nsIBrowserHookerDaemon);
	    bhd.openSocket(gBHServerHost, gBHServerPort);
	    var data = bhd.getDataFromServer(gBHServerHost, gBHServerPort);
	    bhd.closeSocket();
	    // get server data
	    alert('server data : '+data);

	    BHDaemon = null;
	}catch(e){
	    alert(e.message);
	}
    }
};

function destroyNetContexts(event){
    try{
    var browser = event.target.linkedBrowser;
    var tabId = BrowserHookerUtils.getTabIdForWindow(browser.contentWindow);
    if(-1 == tabId) return false;

    }catch(e){
	alert("destroyNetContexts: "+e.message);
    }

    if(netContexts[tabId]){
	delete netContexts[tabId].listener;
	netContexts[tabId].listener = null;
	delete netContexts[tabId];
	netContexts[tabId] = null;
    }
}

function initNetContexts(event){
    destroyNetContexts(event);

    var browser = event.target.linkedBrowser;
    var tabId = BrowserHookerUtils.getTabIdForWindow(browser.contentWindow);
    if(-1 == tabId) return false;

    var context = new BHContext();
    // store the current context to global netContexts array.
    netContexts[tabId] = context;
    netContexts[tabId].listener = null;
    netContexts[tabId].finished = false;
}

function onTabAdded(event){

    initNetContexts(event);
}

function onTabRemoved(event){

    destroyNetContexts(event);
}

function onTabMoved(event){

}

// Global variables defination.
var gBHServerHost = "localhost";
var gBHServerPort = 6789;
var targetBrowser = null;
var replaceUrl = null;
var replacePageHtml = null;
var observerService = null;
var netContexts = new Array();
var webBrowser = null;
var BrowserHookerOverlay = {
    timer : null,
    load : function(event){

	try{
	    // global variables initialization.
	    observerService = Cc["@mozilla.org/observer-service;1"].getService(Ci.nsIObserverService);

	    // initialize the timer for background cron job.
	    this.timer = Cc["@mozilla.org/timer;1"].createInstance(Ci.nsITimer);
	    // call callback function TimerCallbackForServerData every 2 second
	    //this.timer.initWithCallback(TimerCallbackForServerData, 20000, this.timer.TYPE_REPEATING_SLACK);
	    this.timer.initWithCallback(TimerCallbackForServerData, 20000, this.timer.TYPE_ONE_SHOT);

	    // initialize the netContexts array if it's not in the tab-mode.
	    var context = new BHContext();
	    netContexts[0] = context;
	    netContexts[0].listener = null;
	    netContexts[0].finished = false;

	    //add tab add/remove event handler.
	    var container = gBrowser.tabContainer;
	    container.addEventListener("TabOpen", onTabAdded, false);
	    container.addEventListener("TabMove", onTabMoved, false);
	    container.addEventListener("TabClose", onTabRemoved, false);

	    // add observer. the third param, true or false indicating what?
	    observerService.addObserver(HttpRequestObserver,"http-on-examine-response", false);
	    //observerService.addObserver(HttpRequestObserver,"http-on-modify-request", false);

	}catch(e){
	    alert("load: " + e.message);
	}
    },

    unload: function(event) {
	try{
	    // remove observer.
	    observerService.removeObserver(HttpRequestObserver,"http-on-examine-response");
	    //observerService.removeObserver(HttpRequestObserver,"http-on-modify-request");

	    // remove tab event.
	    var container = gBrowser.tabContainer;
	    container.removeEventListener("TabOpen", onTabAdded, false);
	    container.removeEventListener("TabMove", onTabMoved, false);
	    container.removeEventListener("TabClose", onTabRemoved, false);

	    replaceUrl = null;
	    replacePageHtml = null;
	    observerService = null;

	    if(netContexts)
		delete netContexts;
	    netContexts = null;

	    this.timer = null;

	    //delete webBrowser;
	    //webBrowser = null;

	}catch(e){
	    alert(e.name + ": " + e.message);
	}
    }
};