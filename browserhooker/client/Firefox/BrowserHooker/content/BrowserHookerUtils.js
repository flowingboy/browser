var BrowserHookerUtils = {
    getWindowForRequest : function(request){
	var webProgress = this.getRequestWebProgress(request);
	try {
	    if (webProgress){
		return webProgress.DOMWindow;
	    }
	}
	catch (e) {
	    alert("getWindowForRequest: "+e.message);
	}
	
	return null;
    },

    getRequestWebProgress : function(request){

	try{
	    if (request.loadGroup && request.loadGroup.groupObserver){
		return request.loadGroup.groupObserver.QueryInterface(Ci.nsIWebProgress);
	    }
	} catch (e) {
	    alert("getRequestWebProgress: "+e.message);
	}
	
	try{
	    if (request.notificationCallbacks){
		return request.notificationCallbacks.getInterface(Ci.nsIWebProgress);
	    }
	} catch (e) {
	    alert('getRequestWebProgress: '+e.message);
	}

	return null;
    },

    getRootWindow : function(win)
    {
	for (; win; win = win.parent)
	{
                        if (!win.parent || win == win.parent || !(win.parent instanceof Window) )
		return win;
	}

	return null;
    },

    getTabIdForWindow : function(aWindow)
    {
	aWindow = this.getRootWindow(aWindow);

	if (!aWindow || !gBrowser.getBrowserIndexForDocument)
                         return null;

	try {
                    var targetDoc = aWindow.document;
                    var tab = null;
                    var targetBrowserIndex = gBrowser.getBrowserIndexForDocument(targetDoc);
/*                    if (targetBrowserIndex > -1)
                    {
		tab = gBrowser.tabContainer.childNodes[targetBrowserIndex];
		return tab.linkedPanel;
                    }
*/

	    return targetBrowserIndex;
	} catch (e) {
	    alert("getTabIdForWindow: "+e.message);
	}

	return -1;
    }
};