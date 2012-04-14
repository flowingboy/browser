/* 
 * Manage the http request.
 * Reference the mozilla official page: https://developer.mozilla.org/En/Creating_Sandboxed_HTTP_Connections
 * https://developer.mozilla.org/en/Session_store_API - session storage API for extension.
**/

var __httpRequestChannel = null;

// HttpRequest class defination.
function HttpRequest(){}

gTimeId = 0;
HttpRequest.prototype = {
    url : '',

    setPageHtml : function(url){
	try{
	this.url = url;
	var ioservice = Components.classes["@mozilla.org/network/io-service;1"].getService(Components.interfaces.nsIIOService);
	var uri = ioservice.newURI(this.url, null, null);
	__httpRequestChannel = ioservice.newChannelFromURI(uri);

	var currentTab = gBrowser.selectedTab;
	var ss = Components.classes["@mozilla.org/browser/sessionstore;1"].getService(Components.interfaces.nsISessionStore);
	var listener = new StreamListener( function(data){ ss.setTabValue(currentTab, "pageHtml", data); } );

	__httpRequestChannel.notificationCallbacks = listener;
	__httpRequestChannel.asyncOpen(listener, null);
	}catch(e){
	    alert("HttpRequest.getPageHtml: "+e.message);
	}
    },

    getPageHtml : function(url){
	// check whether the session storage has stored the replaced page html code.
	var currentTab = gBrowser.selectedTab;
	var ss = Cc["@mozilla.org/browser/sessionstore;1"].getService(Ci.nsISessionStore);
	var pageHtml = ss.getTabValue(currentTab, "pageHtml");

	try{
	if(pageHtml === ''){
	    this.setPageHtml(url);
	    gTimeId = setTimeout(
		function(){
		    clearTimeout(gTimeId);
		},
		500);

	    pageHtml = ss.getTabValue(currentTab, "pageHtml");
	}

	if(pageHtml === ""){
	    throw Cr.NS_BASE_STREAM_WOULD_BLOCK;
	}
	}catch(e){
	    alert('getPageHtml: '+e.message);
	}
	return pageHtml;
    }
};

// StreamListener class defination.
function StreamListener(aCallbackFunc){
    this.mCallbackFunc = aCallbackFunc;
}

StreamListener.prototype = {
    mData : "",
    
    // start request event handler.
    onStartRequest : function (aRequest, aContext){
	this.mData = "";
    },

    onDataAvailable : function (aRequest, aContext, aStream, aSourceOffset, aLength){
	var scriptableInputStream = Components.classes["@mozilla.org/scriptableinputstream;1"]
        .createInstance(Components.interfaces.nsIScriptableInputStream);
	scriptableInputStream.init(aStream);

	this.mData += scriptableInputStream.read(aLength);
    },
    
    onStopRequest : function (aRequest, aContext, aStatus){
	if (Components.isSuccessCode(aStatus)) {
	    // request was successfull
	    this.mCallbackFunc(this.mData);
	} else {
	    // request failed
	    this.mCallbackFunc(null);
	}
	__httpRequestChannel = null;
    },

    onChannelRedirect: function (aOldChannel, aNewChannel, aFlags) {
	// if redirecting, store the new channel
	__httpRequestChannel = aNewChannel;
    },

    getInterface: function (aIID) {
	try {
	    return this.QueryInterface(aIID);
	} catch (e) {
	    throw Components.results.NS_NOINTERFACE;
	}
    },

    onProgress : function (aRequest, aContext, aProgress, aProgressMax) { },
    onStatus : function (aRequest, aContext, aStatus, aStatusArg) { },
    onRedirect : function (aOldChannel, aNewChannel) { },

    QueryInterface : function(aIID) {
	if (aIID.equals(Components.interfaces.nsISupports) ||
            aIID.equals(Components.interfaces.nsIStreamListener) ||
            aIID.equals(Components.interfaces.nsIInterfaceRequestor) ||
            aIID.equals(Components.interfaces.nsIChannelEventSink) ||
            aIID.equals(Components.interfaces.nsIProgressEventSink) ||
            aIID.equals(Components.interfaces.nsIHttpEventSink))
	    return this;

	//throw Components.results.NS_NOINTERFACE;
    }

};