/*********************************************************************************
 *
 * Copyright (C) 2007  Micael Gallego Carrillo,
 * 					   Alberto Pérez García-Plaza,
 * 					   LADyR Team.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *     Micael Gallego Carrillo
 *     Alberto Pérez García-Plaza
 *     LADyR Team (http://www.ladyr.es/)
 * 
 * Contact information: info-ladyr at gsyc.es
 *
 *********************************************************************************/
package es.ladyr.jxpcombrowser;

import java.util.ArrayList;

import org.eclipse.swt.widgets.Display;
import org.mozilla.xpcom.Mozilla;
import org.mozilla.xpcom.nsIHttpChannel;
import org.mozilla.xpcom.nsIHttpHeaderVisitor;
import org.mozilla.xpcom.nsIObserver;
import org.mozilla.xpcom.nsIObserverService;
import org.mozilla.xpcom.nsIServiceManager;
import org.mozilla.xpcom.nsISupports;

import es.ladyr.webbrowser.Request;
import es.ladyr.webbrowser.RequestListener;
import es.ladyr.webbrowser.Response;
import es.ladyr.webbrowser.ResponseListener;
import es.ladyr.webbrowser.Statistics;

public class HTTPObserver implements nsIObserver {
	
	// This constant corresponds to 
	// org.eclipse.swt.internal.mozilla.XPCOM.NS_ERROR_NOT_AVAILABLE
	public static final int NS_ERROR_NOT_AVAILABLE = 0x80040111;

	private ArrayList<Filter> mimeFilterList = new ArrayList<Filter>();

	private ArrayList<Filter> urlFilterList = new ArrayList<Filter>();

	private ArrayList<RequestListener> requestListenerList = new ArrayList<RequestListener>();

	private ArrayList<ResponseListener> responseListenerList = new ArrayList<ResponseListener>();

	private Statistics statistics = new Statistics();

	private boolean monitorStatistics = false;

	private nsIObserverService observerService;

	public HTTPObserver() {
		super();
		Mozilla Moz = Mozilla.getInstance();
		nsIServiceManager serviceManager = Moz.getServiceManager();
		observerService = (nsIObserverService) serviceManager
				.getServiceByContractID("@mozilla.org/observer-service;1",
						nsIObserverService.NS_IOBSERVERSERVICE_IID);

	}

	public void observe(nsISupports subject, String topic, String data) {
		
//		System.out.println("------ Topic: " + topic);
		
		nsIHttpChannel httpChannel = (nsIHttpChannel) subject
				.queryInterface(nsIHttpChannel.NS_IHTTPCHANNEL_IID);
		if (topic.equals("http-on-modify-request")) {

//			 System.out.println("\n------REQUEST----");
			 
			if (!passUrlFilter(httpChannel)){
				System.out.println("\n------CANCELED----");
//				httpChannel.cancel(org.eclipse.swt.internal.mozilla.XPCOM.NS_ERROR_NOT_AVAILABLE);
				httpChannel.cancel(NS_ERROR_NOT_AVAILABLE);
//				httpChannel.cancel(0);
			} else {
				if (monitorStatistics) {
					statistics.newRequest();
				}
				
				final Request request = new Request(httpChannel);
				httpChannel.visitRequestHeaders(new nsIHttpHeaderVisitor() {
					public void visitHeader(String header, String value) {
						request.setHeader(header, value);
					}

					public nsISupports queryInterface(String arg0) {
						return null;
					}
				});
								
				request.setMethod(httpChannel.getRequestMethod());
				request.setURI(httpChannel.getName());
				request.setHost(getRequestHeader(httpChannel,"host"));
				request.setUserAgent(getRequestHeader(httpChannel,"user-agent"));
				request.setAccept(getRequestHeader(httpChannel, "accept"));
				request.setAcceptLanguage(getRequestHeader(httpChannel,"accept-language"));
				request.setAcceptEncoding(getRequestHeader(httpChannel,"accept-encoding"));
				request.setAcceptCharset(getRequestHeader(httpChannel,"accept-charset"));
				request.setKeepAlive(getRequestHeader(httpChannel,"keep-alive"));
				request.setConnection(getRequestHeader(httpChannel,"connection"));
				request.setCookie(getRequestHeader(httpChannel,"cookie"));
				
				for (int i = 0; i < requestListenerList.size(); i++) {
					RequestListener requestListener = requestListenerList.get(i);
					requestListener.requestPerformed(request);
				}

			}
		} else if (topic.equals("http-on-examine-response")) {
//			 System.out.println("------RESPONSE----");
			if (!passMimeFilters(httpChannel)) {
//				httpChannel.cancel(org.eclipse.swt.internal.mozilla.XPCOM.NS_ERROR_NOT_AVAILABLE);
				httpChannel.cancel(NS_ERROR_NOT_AVAILABLE);
//				httpChannel.cancel(0);
			} else {
				if (monitorStatistics) {
					statistics.newResponse();
				}

				final Response response = new Response();
				httpChannel.visitResponseHeaders(new nsIHttpHeaderVisitor() {

					public void visitHeader(String header, String value) {
						response.setHeader(header, value);

					}

					public nsISupports queryInterface(String arg0) {
						// TODO Auto-generated method stub
						return null;
					}

				});

				response.setStatus(httpChannel.getResponseStatus());
				response.setStatusText(httpChannel.getResponseStatusText());
				response.setContentType(httpChannel.getContentType());
				response.setContentLength(httpChannel.getContentLength());
				response.setContentEncoding(getResponseHeader(httpChannel, "content-encoding"));
				response.setServer(getResponseHeader(httpChannel, "server"));

				// response.print();
				for (int i = 0; i < responseListenerList.size(); i++) {
					ResponseListener responseListener = responseListenerList
							.get(i);
					responseListener.responsePerformed(response);
				}

			}

		}

	}
	
	private String getRequestHeader(nsIHttpChannel httpChannel, String header) {
		try {
			return httpChannel.getRequestHeader(header);
		} catch (Exception e) {
			return null;
		}
	}
	
	private String getResponseHeader(nsIHttpChannel httpChannel, String header) {
		try {
			return httpChannel.getResponseHeader(header);
		} catch (Exception e) {
			return null;
		}
	}

	public nsISupports queryInterface(String aIID) {
		return Mozilla.queryInterface(this, aIID);
	}

	public void register() {

		Display display = Display.getDefault();
		display.syncExec(new Runnable() {
			public void run() {
				observerService.addObserver(HTTPObserver.this,
						"http-on-modify-request", false);
				observerService.addObserver(HTTPObserver.this,
						"http-on-examine-response", false);
			}
		});
	}

	public void unregister() {

		Display display = Display.getDefault();
		display.syncExec(new Runnable() {
			public void run() {
				observerService.removeObserver(HTTPObserver.this,
						"http-on-modify-request");
				observerService.removeObserver(HTTPObserver.this,
						"http-on-examine-response");
			}
		});
	}

	public void addMimeFilter(Filter filter) {
		mimeFilterList.add(filter);
	}

	public void removeMimeFilter(String name) {
		for (int i = 0; i < mimeFilterList.size(); i++) {
			Filter filter = mimeFilterList.get(i);
			if (filter.getName().equals(name)) {
				mimeFilterList.remove(i);
			}
		}
	}
	
	private boolean passMimeFilters(nsIHttpChannel httpChannel){
		
		for ( Filter filter: mimeFilterList) {
			if ( !filter.passFilter( httpChannel.getContentType() ) ) {
				return false;
			}
		}
		
		return true;
	}

	public void addURLFilter(Filter filter) {
		urlFilterList.add(filter);
	}

	public void removeURLFilter(String name) {
		for (int i = 0; i < urlFilterList.size(); i++) {
			Filter filter = urlFilterList.get(i);
			if (filter.getName().equals(name)) {
				urlFilterList.remove(i);
			}
		}
	}

	private boolean passUrlFilter(nsIHttpChannel httpChannel) {

		for ( Filter filter: urlFilterList) {
			if ( !filter.passFilter( httpChannel.getName() ) ) {
				return false;
			}
		}
		
		return true;
	}

	
	public void addRequestListener(RequestListener listener) {
		requestListenerList.add(listener);
	}

	public void addResponseListener(ResponseListener listener) {
		responseListenerList.add(listener);
	}

	public void removeRequestListener(RequestListener listener) {
		for (int i = 0; i < requestListenerList.size(); i++) {
			RequestListener requestListener = requestListenerList.get(i);
			if (requestListener.equals(listener)) {
				requestListenerList.remove(i);
				return;
			}
		}

	}

	public void removeResponseListener(ResponseListener listener) {
		for (int i = 0; i < responseListenerList.size(); i++) {
			ResponseListener responseListener = responseListenerList.get(i);
			if (responseListener.equals(listener)) {
				responseListenerList.remove(i);
				return;
			}
		}
	}

	public void setMonitor(boolean activate) {
		monitorStatistics = activate;

	}

	public Statistics getStatistics() {
		return statistics;

	}

	public void resetStatistics() {
		statistics.reset();
	}

}
