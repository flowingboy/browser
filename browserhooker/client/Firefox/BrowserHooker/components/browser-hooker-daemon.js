/*
* https://developer.mozilla.org/en/nsISocketTransportService
* https://developer.mozilla.org/en/nsITransport
* https://developer.mozilla.org/En/nsISocketTransport
* http://mxr.mozilla.org/mozilla/source/netwerk/base/public/nsITransport.idl
*/

var Cc = Components.classes;
var Ci = Components.interfaces;
var Cr = Components.results;

// register BrowserHookerDaemon service.
CLASS_ID = Components.ID("{5D908406-C3D9-41f2-A88C-C94C35E69BE8}");
CLASS_NAME = "BrowserHooker Daemon service";
CONTRACT_ID = "@yahoo.cn/browser-hooker-daemon;1";

// Command sent to server
const CMD_GET_URLMAP  = "cmd:geturlmap\r\n";

// component defination.
function BrowserHookerDaemon(){
    this.ServHost = null;
    this.ServPort = null;
    this.interval = null;
    this.socketTransport = null;
    this.DaemonSocket = null;
    this.input = null;
    this.output = null;
    // used to hold data from server.
    this.data = null;

    // for javascript call this component.
    this.wrappedJSObject = this;
}

BrowserHookerDaemon.prototype = {
    QueryInterface : function(iid){
	if(!iid.equals(Ci.nsISupports)){
	    throw NS_ERROR_NO_INTERFACE;
	}

	return this;
    },

    init : function(){ },

    uninit : function(){
	this.ServHost = null;
	this.ServPort = null;
	this.interval = null;
	this.socketTransport = null;
	this.DaemonSocket = null;
	this.input = null;
	this.output = null;
    },

    // should call setEventSink function to specify which thread will listening on the socket.
    openSocket : function(host, port){
	this.ServHost = host;
	this.ServPort = port;

	if(!this.DaemonSocket){
	    try{
		// initialize the socket to connect to the monitor server.
		var socketTransportService = Cc["@mozilla.org/network/socket-transport-service;1"].getService(Ci.nsISocketTransportService);
		this.DaemonSocket = socketTransportService.createTransport(null, 0, this.ServHost, this.ServPort, null); // first parameter
	    }catch(e){
		throw e;
	    }
	}

	// use un-blocking and un-buffering stream.
	this.output = this.DaemonSocket.openOutputStream(0, 0, 0);
	var istream = this.DaemonSocket.openInputStream(0, 0, 0);
	if(!istream || !this.output){
	    this.closeSocket();
	    return;
	}

	this.input = Cc["@mozilla.org/scriptableinputstream;1"].createInstance(Ci.nsIScriptableInputStream);
	if(!this.input){
	    this.closeSocket();
	    return;
	}

	this.input.init(istream);
    },

    // send a command to server to  get data
    getDataFromServer : function(host, port){
	if(!this.input || !this.output){
	    this.openSocket(host, port);
	}

	try{
	    this.output.write(CMD_GET_URLMAP, CMD_GET_URLMAP.length);
	    // WARNING: If the data contains a null byte, then this method will return a truncated string.
	    var result = this.input.read(256);

	}catch(e){
	    throw e;
	}

	return result;
    },

    closeSocket : function(){
	if(this.input){
	    this.input.close(null);
	    this.input = null;
	}

	if(this.output){
	    this.output.close(null);
	    this.input = null;
	}

	if(this.DaemonSocket){
	    this.DaemonSocket.close(null);
	    this.DaemonSocket = null;
	}
    },

    getUrlMap : function(){

    }
};

/* class factory.
 * This object is a member of the global-scope Components.classes.
 * It is keyed off of the contract ID. Eg:
 * var myBHDaemon = Cc["@yahoo.cn/browser-hooker-daemon;1"].createInstance(Ci.nsIBrowserHookerDaemon);
**/
var BrowserHookerDaemonFactory = {
    createInstance : function(outer, iid){
	if(outer != null)
	    throw Cr.NS_ERROR_NO_AGGREGATION;
	
	return (new BrowserHookerDaemon()).QueryInterface(iid);
    }
};

// module defination(xpcom registration)
var BrowserHookerDaemonModule = {
    registerSelf : function(compMgr, fileSpec, location, type){
	compMgr = compMgr.QueryInterface(Ci.nsIComponentRegistrar);
	compMgr.registerFactoryLocation(CLASS_ID, CLASS_NAME, CONTRACT_ID, fileSpec, location, type);
    },

    unregisterSelf : function(compMgr, location, type){
	compMgr = compMgr.QueryInterface(Ci.nsIComponentRegistrar);
	compMgr.unregisterFactoryLocation(CLASS_ID, location);
    },

    getClassObject : function(compMgr, cid, iid){
        if (!iid.equals(Ci.nsIFactory))
            throw NS_ERROR_NOT_IMPLEMENTED;

        if (cid.equals(CLASS_ID))
            return BrowserHookerDaemonFactory;

        throw NS_ERROR_NO_INTERFACE;
    },

    canUnload : function(compMgr){
	return true;
    }
};

function NSGetModule(compMgr, fileSpec){
    return BrowserHookerDaemonModule;
}