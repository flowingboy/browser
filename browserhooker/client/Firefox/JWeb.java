/*
Copyright or © or Copr. Sylvain Ferriol

sferriol@imag.fr

This software is a computer program whose purpose is to provide Mozilla 
Web components as Swing components

This software is governed by the CeCILL-C license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL-C
license as circulated by CEA, CNRS and INRIA at the following URL 
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL-C license and that you accept its terms.
*/

package fr.imag.jweb;

import java.awt.Canvas;
import java.awt.Graphics;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.logging.Logger;

import org.mozilla.interfaces.*;
import org.mozilla.xpcom.*;

/**
 * JWeb is a library for providing Mozilla Web components as Swing components.
 * 
 * @author sferriol
 */

public final class JWeb extends Canvas implements nsISupports, nsIInterfaceRequestor {

    public static Logger logger = Logger.getLogger("fr.imag.jweb.JWeb");

    private static nsIServiceManager serviceManager = null;

    private static nsIComponentManager componentManager = null;

    private static nsIEventQueue eventQueue = null;

    private static nsIProxyObjectManager proxy = null;

    private nsIWebBrowser webBrowser = null;

    private static Boolean mozilla = false;

    public JWeb() throws JWebException {
        jwebImpl();
    }

    /**
     * Interface nsISupports 
     * @param uuid The IID of the interface being requested.
     */
    public nsISupports queryInterface(String uuid) {
        return Mozilla.queryInterface(this, uuid);
    }

    /**
     * Interface nsIInterfaceRequestor.
     * 
     * The nsIInterfaceRequestor interface defines a generic interface for
     * requesting interfaces that a given object might provide access to.
     * 
     * @param uuid The IID of the interface being requested.
     */
    public nsISupports getInterface(String uuid) {

        if (uuid.equals(nsIMarkupDocumentViewer.NS_IMARKUPDOCUMENTVIEWER_IID)) {
            return this.getMarkupDocumentViewer();
        }
        if (uuid.equals(nsIIOService.NS_IIOSERVICE_IID)) {
            return this.getIOService();
        }
        if (uuid.equals(nsIWebBrowserStream.NS_IWEBBROWSERSTREAM_IID)) {
            return this.getWebBrowserStream();
        }
        if (uuid.equals(nsIWebNavigation.NS_IWEBNAVIGATION_IID)) {
            return this.getWebNavigation();
        }
        if (uuid.equals(nsIWebProgress.NS_IWEBPROGRESS_IID)) {
            return this.getWebProgress();
        }

        return null;
    }
    
    private nsISupports getWebBrowserInterface(String aInterface) {
        return JWeb.proxy.getProxyForObject(JWeb.eventQueue, aInterface,
                this.webBrowser, nsIProxyObjectManager.INVOKE_SYNC);
    }

    private nsIDocShell getDocShell() {
        nsIInterfaceRequestor requestor = (nsIInterfaceRequestor) getWebBrowserInterface(nsIInterfaceRequestor.NS_IINTERFACEREQUESTOR_IID);
        return (nsIDocShell) requestor.getInterface(nsIDocShell.NS_IDOCSHELL_IID);
    }

    private nsIMarkupDocumentViewer getMarkupDocumentViewer() {
        nsIDocShell docshell = getDocShell();
        nsIContentViewer cviewer = docshell.getContentViewer();
        return (nsIMarkupDocumentViewer) cviewer.queryInterface(nsIMarkupDocumentViewer.NS_IMARKUPDOCUMENTVIEWER_IID);
    }

    private nsIWebNavigation getWebNavigation() {
        return (nsIWebNavigation) getWebBrowserInterface(nsIWebNavigation.NS_IWEBNAVIGATION_IID);
    }

    private nsIWebProgress getWebProgress() {
        nsIDocShell docshell = getDocShell();
        return (nsIWebProgress) docshell.queryInterface(nsIWebProgress.NS_IWEBPROGRESS_IID);
    }


    private nsIWebBrowserStream getWebBrowserStream() {
        return (nsIWebBrowserStream) getWebBrowserInterface(nsIWebBrowserStream.NS_IWEBBROWSERSTREAM_IID);
    }

    private nsIIOService getIOService() {
        return (nsIIOService) JWeb.serviceManager.getServiceByContractID(
                "@mozilla.org/network/io-service;1",
                nsIIOService.NS_IIOSERVICE_IID);
    }

    //
    // Component
    //

    /**
     * Notifies this component that it now has a parent component.
     */
    @SuppressWarnings("deprecation")
    public void addNotify() {
        logger.finest("JWeb::addNotify");

        super.addNotify();
        nsIBaseWindow baseWindow = (nsIBaseWindow) getWebBrowserInterface(nsIBaseWindow.NS_IBASEWINDOW_IID);

        long windowHandle = 0;
        try {
            // grab the class that does all the action
            Class cl = Class.forName("sun.awt.windows.WComponentPeer");
            // find the hidden field
            java.lang.reflect.Field f = cl.getDeclaredField("hwnd");
            // expose the field
            f.setAccessible(true);
            // here is what we want
            windowHandle = f.getLong(this.getPeer());
            logger.finest("windowHandle = "+windowHandle);
        } catch (Exception ex) {
            // something went wrong trying to expose the field we want
            // System.err.println("Fatal Error:");
            logger.severe(ex.getMessage());
        }

        // System.out.println("baseWindow.initWindow");
        baseWindow.initWindow(windowHandle, 0, 0, 0, 0, 0);
        // System.out.println("baseWindow.create");
        baseWindow.create();
        // System.out.println("baseWindow.setVisibility");
        baseWindow.setVisibility(true);

    }

    public void paint(Graphics g) {

        logger.finest("paint x=" + this.getLocationOnScreen().x + " y="
                + this.getLocationOnScreen().y + " w=" + this.getWidth()
                + " h=" + this.getHeight());

        nsIBaseWindow baseWindow = (nsIBaseWindow) getWebBrowserInterface(nsIBaseWindow.NS_IBASEWINDOW_IID);

        baseWindow.setPositionAndSize(0, 0, this.getWidth(), this.getHeight(),
                true);

    }

    public void setEnabled(boolean b) {
        logger.finest("JWeb::setEnabled");

        nsIBaseWindow baseWindow = (nsIBaseWindow) getWebBrowserInterface(nsIBaseWindow.NS_IBASEWINDOW_IID);
        baseWindow.setEnabled(b);
        super.setEnabled(b);
    }

    //
    // Gecko Engine
    //

    private static class AppStartup extends Thread {
        class LocationProvider implements IAppFileLocProvider {
            private final File libXULPath;

            int counter = 0;

            public LocationProvider(File grePath) {
                this.libXULPath = grePath;
            }

            public File getFile(String aProp, boolean[] aPersistent) {
                File file = null;
                if (aProp.equals("GreD") || aProp.equals("GreComsD")) {
                    file = libXULPath;
                    if (aProp.equals("GreComsD")) {
                        file = new File(file, "components");
                    }
                } else if (aProp.equals("MozBinD") || aProp.equals("CurProcD")
                        || aProp.equals("ComsD") || aProp.equals("ProfD")) {
                    file = libXULPath;
                    if (aProp.equals("ComsD")) {
                        file = new File(file, "components");
                    }
                }
                return file;
            }

            public File[] getFiles(String aProp) {
                File[] files = null;
                if (aProp.equals("APluginsDL")) {
                    files = new File[1];
                    files[0] = new File(libXULPath, "plugins");
                }
                return files;
            }
        }

        File grePath = null;

        nsIAppStartup appStartup = null;

        public AppStartup(File aGrePath) throws JWebException {
            this.grePath = aGrePath;

            if (this.grePath == null) {
                GREVersionRange[] range = new GREVersionRange[1];
                range[0] = new GREVersionRange("1.8.0.1", true, "1.9", false);
                // work with trunk nightly version 1.9a1 ^^

                try {
                    this.grePath = Mozilla
                    .getGREPathWithProperties(range, null);
                } catch (FileNotFoundException e) {
                    throw new JWebException(e);
                }
            }
        }

        public void run() {
            logger.finest("AppStartup run");

            //nsIAppStartup appStartup = null;

            Mozilla.getInstance().initialize(this.grePath);

            // set mozilla startup flag
            Mozilla aMozilla = Mozilla.getInstance();
            
            nsIAppShell appShell = null;

            LocationProvider locProvider = new LocationProvider(this.grePath);
            try {
                aMozilla.initEmbedding(this.grePath, this.grePath, locProvider);
                JWeb.serviceManager = aMozilla.getServiceManager();
                JWeb.componentManager = aMozilla.getComponentManager();

                String NS_APPSHELL_CID = "{2d96b3df-c051-11d1-a827-0040959a28c9}";
                appShell = (nsIAppShell) JWeb.componentManager
                .createInstance(NS_APPSHELL_CID, null,
                        nsIAppShell.NS_IAPPSHELL_IID);
                appShell.create(null, null);
                appShell.spinup();

                JWeb.mozilla = true;

            } catch (XPCOMException e) {
                // this exception is thrown if initEmbedding failed
                // TODO
                //throw new JWebException(e);
                logger.severe(e.getMessage());
                e.printStackTrace();
            }
            
            synchronized (this) {
                this.notify();
            }

            // infinite loop
            if (appShell != null) appShell.run();
        }
    }

    /**
     * Start JWeb engine. NOTE: This function may only be called once in a given
     * process.
     */
    public static void start() throws JWebException {
        startImpl(null);
    }

    /**
     * Start JWeb engine.
     * 
     * @param aGrePath
     *            Gecko Runtime Environment directory path
     */
    public static void start(String aGrePath) throws JWebException {
        File grePath = new File(aGrePath);
        if (grePath.isDirectory() == false) {
            throw new JWebException("Gre path must be a directory");
        }
        startImpl(grePath);
    }

    /**
     * Stop JWeb engine. Clean up and free resources allocated by start
     */
    /*
     * not useful because mozilla crashs if we try to restart the engine
     * synchronized public static void stop() throws JWebException { if
     * (isStarted() == false) return; try {
     * Mozilla.getInstance().termEmbedding(); //JWeb.hiddenWindow = null;
     * JWeb.mozilla = false; } catch (XPCOMException e) { // this exception is
     * thrown if termEmbedding failed throw new JWebException(e); } }
     */

    synchronized private static void startImpl(File aGrePath)
    throws JWebException {
        logger.finest("JWeb startImpl");

        if (JWeb.isStarted() == true)
            return;
        AppStartup appStartup = new AppStartup(aGrePath);
        appStartup.start();
        synchronized (appStartup) {
            try {
                appStartup.wait();
            } catch (Exception e) {
                throw new JWebException(e.getMessage());
            }
        }
        if (JWeb.isStarted() == false) {
            throw new JWebException("Unable to start mozilla engine");
        }

        // now Gecko run in another thread, so we have to use a proxy
        // for gecko instances access
        nsIEventQueueService eventQueueServive = (nsIEventQueueService) serviceManager
        .getServiceByContractID("@mozilla.org/event-queue-service;1",
                nsIEventQueueService.NS_IEVENTQUEUESERVICE_IID);
        JWeb.eventQueue = eventQueueServive
        .getSpecialEventQueue(nsIEventQueueService.UI_THREAD_EVENT_QUEUE);
        JWeb.proxy = (nsIProxyObjectManager) componentManager
        .createInstanceByContractID("@mozilla.org/xpcomproxy;1", null,
                nsIProxyObjectManager.NS_IPROXYOBJECTMANAGER_IID);

        logger.finest("JWeb mozilla=" + JWeb.mozilla);
        logger.finest("JWeb end startImpl");
    }

    synchronized private static boolean isStarted() {
        return (JWeb.mozilla == true);
    }

    private void jwebImpl() throws JWebException {
        logger.finest("JWeb jwebImpl begin");

        if (JWeb.isStarted() == false) {
            throw new JWebException(
            "You must call JWeb.start() before creating a JWeb component");
        }
        ;

        String NS_IWEBBROWSER_CID = "F1EAC761-87E9-11d3-AF80-00A024FFC08C";
        this.webBrowser = (nsIWebBrowser) JWeb.componentManager.createInstance(
                NS_IWEBBROWSER_CID, null, nsIWebBrowser.NS_IWEBBROWSER_IID);

    }

}
