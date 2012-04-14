//  **** BEGIN LICENSE BLOCK ****
//  Copyright(c) 2008 ku ku0522a*gmail.com
//
//  This program is free software; you can redistribute it and/or modify it under
//  the terms of the GNU General Public License as published by the Free
//  Software Foundation; either version 2 of the License, or (at your option)
//  any later version.
//  
//  This program is distributed in the hope that it will be useful, but
//  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
//  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
//  more details.
//  
//  You should have received a copy of the GNU General Public License along with
//  this program; if not, write to the Free Software Foundation, Inc., 59 Temple
//  Place, Suite 330, Boston, MA 02111-1307 USA
//  **** END LICENSE BLOCK ****


( function () {

	// to avoid ISO8601DateUtils js error.
	function debug () {}

  function log() {
	if ( Firebug && Firebug.Console && Firebug.Console.log  ) {
		if ( arguments.length == 1 ) {
			Firebug.Console.log(arguments[0]);
		} else {
			var args = [];
			for (var i = 0; i < arguments.length; i++) {
				args.push(arguments[i]);
			}
			Firebug.Console.log(args);
		}
	}
	}


var ios = Components.classes["@mozilla.org/network/io-service;1"]
								.getService(Components.interfaces.nsIIOService);
var dm = Cc["@mozilla.org/download-manager;1"]
					.getService(Components.interfaces.nsIDownloadManager);

// JSA does not define window laoded from DOMContentLoaded.
// document.defaultView is about;blank idont know why.
var window = Application.activeWindow.activeTab.document.defaultView;

{
var MP3Recorder = function (hooks) {
	var VERSION = '0.0.1';
	this.downloaded = {}; 
	this.hooks = hooks;
	this.localfilenamePrefix = "air";
	this.init();

	this.songMeta = null;
}

MP3Recorder.fromQuerystring = function (q) {
	return q.split(/&/).reduce( function (params, pair) {
		var [name, value] = pair.split(/=/).map( decodeURIComponent );
		params[name] = value;
		return params;
	}, {} );
}
MP3Recorder.prototype.init = function () {
	var self = this;

	var obs = Components.classes["@mozilla.org/observer-service;1"].getService(Components.interfaces.nsIObserverService);
	obs.addObserver(self, "http-on-examine-response", false);

	for ( var i in this.hooks ) {
		if ( ! i.match(/^\w+Hook$/) )
			continue;

		var v = ( this.hooks[i].topic );
		if ( v ) {
			obs.addObserver(self, v, false);
		}
	}

	window.addEventListener( 'unload', function () {
		obs.removeObserver(self, "http-on-examine-response");
		for ( var i in self.hooks ) {
			var v = ( self.hooks[i].topic );
			if ( v ) {
				obs.removeObserver(self, v);
			}
		}
	}, true);
}

MP3Recorder.prototype.observe = function(aSubject, aTopic, aData) {
	if (aTopic == 'http-on-modify-request') {
		aSubject.QueryInterface(Components.interfaces.nsIHttpChannel);
		aSubject.QueryInterface(Components.interfaces.nsIRequest);

	} else if (aTopic == 'http-on-examine-response') {
		aSubject.QueryInterface(Components.interfaces.nsIHttpChannel);
		if ( !aSubject.contentType )
			return;
		if ( aSubject.contentType.match(/^image\//i) )
			return;
		if ( aSubject.contentType.match(/^text\/javascript/i) )
			return;
		
		if (
			aSubject.contentType.match( new RegExp(this.hooks.contentHook.contentType) )
		) {
			// aSubject.loadFlags &= ~aSubject.LOAD_REPLACE;
			// aSubject.loadFlags |= aSubject.LOAD_FROM_CACHE;

			var uri = aSubject.name;

			if ( this.downloaded[uri] ) {
				return;
			} else {
				this.downloaded[uri] = aSubject;
			}

			aSubject.QueryInterface(Components.interfaces.nsITraceableChannel);

			this.saveTraceableChannel(aSubject);


		}
	}

	if ( aTopic == this.hooks.metaDataHook.topic ) {
		var hook =  this.hooks.metaDataHook;
		for ( var k in hook.conditions ) {
			var v = hook.conditions[k];
			if ( k == 'url' ) {
				if ( ! aSubject.name.match(v) )
					return;
			} else if ( k == 'method' ) {
				if ( aSubject.requestMethod != v )
					return;
			}
		}
		var metadata = hook.handler.call(this, aSubject);
		this.setMetaData(metadata);
	}
}

MP3Recorder.prototype.setMetaData = function(metadata) {
	this.songMeta = metadata;
}

MP3Recorder.prototype.saveTraceableChannel = function(traceableChannel) {
	try {
	log("saving", arguments, this);

		var mp3uri = traceableChannel.name;

		var sourceUri = ios.newURI(mp3uri, null, null);
		var localfile = Components.classes["@mozilla.org/file/local;1"] .createInstance(Components.interfaces.nsILocalFile);

		//var filename = metadata.title.replace(/[\/\|\?\*\"\!-+:-<>-\@\\\~\^]/g, '_');
		var  filename = this.localfilenamePrefix;

		var t = ISO8601DateUtils.create( new Date() );
		t = t.replace(/\W/g, '');

		var basename = filename + t + ".mp3";
		localfile.initWithPath( dm.userDownloadsDirectory.path);
		localfile.append(basename);

		var hybridListener = new HybridListener(localfile);

		var download = dm.addDownload(
			dm.DOWNLOAD_TYPE_DOWNLOAD,
			sourceUri,
			ios.newFileURI(localfile),
			basename,
			null,
			null,
			localfile,
			hybridListener
		);

		var o_stream = Components.classes["@mozilla.org/network/file-output-stream;1"]
			.createInstance(Components.interfaces.nsIFileOutputStream);
		o_stream.init(localfile, 0x04 | 0x08 | 0x20, 0664, 0); // write, create, truncate

		var self = this;
			
		hybridListener.o_stream = o_stream;
		hybridListener.download = download;
		hybridListener.songMetaCallback = function () {
			return {
				TPE1: self.songMeta.artist,
				TIT2: self.songMeta.title,
				TALB: self.songMeta.album,
				COMM: 'eng\x00' + window.location.href
			}
		};

		hybridListener.listener = traceableChannel.setNewListener(hybridListener);

	} catch(e) {
		log(e);
	}
	
}


{
function HybridListener(localfile) {
	var VERSION = '0.0.5';


	this.canceling = false;
	this.success = false;
	this.total_downloaded = 0;
	this.download = null;

	var o_stream = Components.classes["@mozilla.org/network/file-output-stream;1"]
		.createInstance(Components.interfaces.nsIFileOutputStream);
	o_stream.init(localfile, 0x04 | 0x08 | 0x20, 0664, 0); // write, create, truncate
	this.outputStream = o_stream;
}

HybridListener.prototype.QueryInterface = function(aIID) {
			if (aIID.equals(Components.interfaces.nsISupports) ||
				aIID.equals(Components.interfaces.nsIStreamListener) ||
				aIID.equals(Components.interfaces.nsICancelable) ||
				aIID.equals(Components.interfaces.nsIWebBrowserPersist)) {
				return this;
			} else {
				throw Components.results.NS_NOINTERFACE;
			}
}

// nsICancelable
HybridListener.prototype.cancel = function ( ) {
	this.canceling = true;
	this.onStopRequest(null);
}

// nsIWebBrowserPersist
HybridListener.prototype.CancelSave = function ( ) { }
// nsIStreamListener
HybridListener.prototype.addCustomFrames = function () { }

HybridListener.prototype.onStartRequest = function ( request, context ) {
	this.listener.onStartRequest(request, context);

	this.bytes = "";

	MP3.ID3v2.frames = {};
	this.success = request.requestSucceeded;
	this.writemeta = false;
}
HybridListener.prototype.onDataAvailable = function (request, context, inputStream, offset, count) {
		try {
			if ( this.canceling || ! this.success ) {
				this.onEndTransfer(request);
				return;
			}

			// update download progress.

				this.total_downloaded += count;
			
				request.QueryInterface(Components.interfaces.nsIHttpChannel);
				this.download.onProgressChange64(null, request,
						this.total_downloaded, request.contentLength,
						this.total_downloaded, request.contentLength
				);
			
			// save bytes and copy to inject original listener.
			
			var binaryInputStream = Cc["@mozilla.org/binaryinputstream;1"].
					createInstance(Components.interfaces.nsIBinaryInputStream);
			binaryInputStream.setInputStream(inputStream);

			var data = binaryInputStream.readBytes(count);
			this.bytes += data;

			var storageStream = Cc["@mozilla.org/storagestream;1"].
				createInstance(Components.interfaces.nsIStorageStream);
			var binaryOutputStream = Cc["@mozilla.org/binaryoutputstream;1"].
				createInstance(Components.interfaces.nsIBinaryOutputStream);

			storageStream.init(8192, count, null);
			binaryOutputStream.setOutputStream(storageStream.getOutputStream(0));
			binaryOutputStream.writeBytes(data, count);

			this.listener.onDataAvailable( request, context ,
				storageStream.newInputStream(0), 0, count );
	}catch(e) {
		log("onDataAvailable", e);
	}
}
HybridListener.prototype.onEndTransfer = function (request) {
	if ( this.o_stream ) {
		this.o_stream.flush();
		this.o_stream.close();
		this.o_stream = null;
	}

	this.download.onStateChange(null, request, this.download.STATE_STOP |
		this.download.STATE_IS_NETWORK,
		Components.results.NS_OK);
}

HybridListener.prototype.onStopRequest = function ( request, context,  statusCode ) {
	//log(this, this.bytes.length);
	this.writeToFile();

	this.onEndTransfer(request, statusCode);
	this.listener.onStopRequest(request, context, statusCode);
}
HybridListener.prototype.writeToFile = function( ) {
	var os = this.outputStream;

	var songMeta = this.songMetaCallback();

	this.bytes;

	var extendedHeaderSize = MP3.ID3v2.parseHeader( this.bytes );

	var writemeta = false;

	if ( extendedHeaderSize <= 0 ) {
		writemeta = true;
		for ( var name in songMeta ) {
			if ( !( name in MP3.ID3v2.frames) ) {
				MP3.ID3v2.addFrame(name, songMeta[name]);
			}
		}
	}

	if ( writemeta ) {
		var id3 = MP3.ID3v2.dump();
		os.write(id3, id3.length);
	}

	os.write(this.bytes, this.bytes.length);
	os.close();

}



var Pack = {
	VERSION: "0.0.3",
	_range: function (n) {
		var a = [];
		while (n--) { a.push(n) }
		return a;
	},
	_bytes: function (value, bytes) {
		return this._range(bytes).map( function(n) {
					return String.fromCharCode( (value & (0xFF << ( 8 * n )) ) >> (8 * n) );
		} );
	},
	_16: function (n) { return this._bytes(n, 2) },
	_32: function (n) { return this._bytes(n, 4) },
	N: function (n) { return this._32(n).join(""); },
	n: function (n) { return this._16(n).join(""); },
	v: function (n) { return this._16(n).reverse().join(""); },
	C: function (n) { return String.fromCharCode( n ); }
};

var MP3 = {
	VERSION: "0.0.4",
	ID3v2: {
		frames: {},
		v2tov3: {
			TBP: 'TBPM',
			TOT: 'TOAL',
			WAR: 'WOAR',
			TP4: 'TPE4',
			GEO: 'GEOB',
			TRK: 'TRCK',
			EQU: 'EQUA',
			WXX: 'WXXX',
			TYE: 'TYER',
			TCO: 'TCON',
			MLL: 'MLLT',
			CNT: 'PCNT',
			TRD: 'TRDA',
			TDY: 'TDLY',
			TP2: 'TPE2',
			TP1: 'TPE1',
			TFT: 'TFLT',
			WCM: 'WCOM',
			TOL: 'TOLY',
			CRA: 'AENC',
			COM: 'COMM',
			TSI: 'TSIZ',
			POP: 'POPM',
			TLE: 'TLEN',
			IPL: 'IPLS',
			REV: 'RVRB',
			TDA: 'TDAT',
			STC: 'SYTC',
			TRC: 'TSRC',
			TAL: 'TALB',
			TSS: 'TSSE',
			TOR: 'TORY',
			TT2: 'TIT2',
			TXT: 'TEXT',
			TP3: 'TPE3',
			MCI: 'MDCI',
			TCR: 'TCOP',
			TIM: 'TIME',
			RVA: 'RVAD',
			TLA: 'TLAN',
			ULT: 'USLT',
			SLT: 'SYLT',
			TPB: 'TPUB',
			TCM: 'TCOM',
			WAF: 'WOAF',
			TXX: 'TXXX',
			TOA: 'TOPE',
			TKE: 'TKEY',
			UFI: 'UFID',
			BUF: 'RBUF',
			WPB: 'WPUB',
			TT3: 'TIT3',
			WAS: 'WOAS',
			ETC: 'ETCO',
			TT1: 'TIT1',
			TMT: 'TMED',
			TEN: 'TENC',
			TPA: 'TPOS',
			WCP: 'WCOP',
			TOF: 'TOFN',
		},
		extendedHeaderSize: null,
		toID3Size: function (n) {
			var size = 0;
			for ( var i = 0; i < 4; i++ ) {
				var v =  (n & 0x7f);
				n >>= 7;
				size += v << (8*i);
			}
			return size;
		},
		dumpFrame: function (id) {
			var frame = this.frames[id];
			var n = frame.length;
			return [
				id,
				Pack.N( this.toID3Size(n) ),
				Pack.C(0),
				Pack.C(0),
				frame.content
			].join('');
		},
		dump: function () {
			var ret = '';
			var content = '';
			for ( var id in this.frames ) {
				ret += this.dumpFrame( id );
			}

			var s = this.toID3Size( ret.length )
			
			return [
				'ID3',
				String.fromCharCode( 0x03 ),	// version.
				String.fromCharCode( 0 ),
				String.fromCharCode( 0 ),		// flags.
				Pack.N( s ),
				ret
			].join('');
		},
		parseHeader: function (buffer) {
			var m;
			if ( m = buffer.match(/^ID3(..)(.)(....)/) ) {
				this.version = (m[1].charCodeAt(0) * 100) + m[2].charCodeAt(0);
				this.flags = m[2];
				return this.extendedHeaderSize =
					(m[3].charCodeAt(0) << 21) +
					(m[3].charCodeAt(1) << 14) +
					(m[3].charCodeAt(2) << 7) +
					(m[3].charCodeAt(3) << 0) ;
			}
			return null;
		},
		parse: function (buffer) {
			var length = this.parseHeader(buffer);

			var offset = 10;
			// skip extended header.
			if ( this.version >= 300 && this.flags & 0x40 ) {
				offset += 6 + 
					(buffer.charCodeAt(offset + 0) << 21) +
					(buffer.charCodeAt(offset + 1) << 14) +
					(buffer.charCodeAt(offset + 2) << 7) +
					(buffer.charCodeAt(offset + 3) << 0) ;
			}

			while ( length > offset ) {
				var key;
				if ( this.version >= 300 ) {
					k = buffer.slice(offset, offset + 4);

					var frameLength =
						(buffer.charCodeAt((offset + 4) + 0) << 21) +
						(buffer.charCodeAt((offset + 4) + 1) << 14) +
						(buffer.charCodeAt((offset + 4) + 2) << 7) +
						(buffer.charCodeAt((offset + 4) + 3) << 0) ;
					this.frames[k] = {
						length: frameLength,
						encoding: null,
						content: buffer.slice(offset + 10, (offset + 10) + frameLength),
					};
					offset += frameLength + 10;

				} else if ( this.version >= 200 ) {
					k = buffer.slice(offset, offset + 3);

					if ( k in this.v2tov3 ) {
						k = this.v2tov3[k];
					//} else if ( k == 'PIC' ) {
					} else {
						return;
					}

					var frameLength =
						(buffer.charCodeAt((offset + 3) + 0) << 16) +
						(buffer.charCodeAt((offset + 3) + 1) << 8) +
						(buffer.charCodeAt((offset + 3) + 2) << 0) ;

					var textEncoding = buffer.charCodeAt(offset + 6);

					var content;
					var contentLength;

					if ( textEncoding ) {
						// -1 for textEncoding. it is included in frameLength. 
						content = '\x01\xff\xfe' + buffer.slice(offset + 7, (offset + 7) + frameLength - 1);
						contentLength = 3 + (frameLength - 1);
					} else {
						content = buffer.slice(offset + 6, (offset + 6) + frameLength);
						contentLength = frameLength ;
					}

					this.frames[k] = {
						length: contentLength,
						content: content
					};
					offset += frameLength + 6;
				}
			}
		},
		addFrame: function (id, content) {
			var c = this.encodeFrameText(content);
			this.frames[id] = {
				length: c.length,
				content: c
			}
		},
		encodeFrameText: function (content) {
			var unicode = false;
			for ( var i = 0; i < content.length; i++ ) {
				if ( unicode = ( content.charCodeAt(i) > 0x80 ) ) {
					break;
				}
			}
			if ( unicode ) {
				var s = [];
				for ( var i = 0; i < content.length; i++ ) {
					var c = content.charCodeAt(i);
					s.push( Pack.v(c) );
				}
				return '\x01\xff\xfe' + s.join('') ;
			} else {
				return  '\x00' + content;
			}
		},
	},
};

}

}

var rules = [
{
	urlRegex: /last.fm/,
	contentHook: {
		contentType: 'audio/mpeg',
		handler: function () {
			this.songMeta = null;
		}
	},
	metaDataHook: {
		topic: 'http-on-modify-request',
		conditions: {
			url: /^http:\/\/post.audioscrobbler.com\/np_/,
			method: "POST",
		}, 
		handler : function (subject) {
			var postdata = LiveHTTPHeaders.getPostData(subject);

			var body = postdata.getPostBody();
			var params = MP3Recorder.fromQuerystring(body);

			return {
				artist: params.a,
				album: params.t,
				title: params.b,
			};
		}
	}
}, 

].some( function (rule) {
	return ( document.location.href.match( rule.urlRegex ) ) && new MP3Recorder( rule );
} );




//  **** BEGIN LICENSE BLOCK ****
//  Copyright(c) 2002-2003 Daniel Savard.
//
//  LiveHTTPHeaders: this programs have two purpose
//  - Add a tab in PageInfo to show http headers sent and received
//  - Add a tool that display http headers in real time while loading pages
//
//  This program is free software; you can redistribute it and/or modify it under
//  the terms of the GNU General Public License as published by the Free
//  Software Foundation; either version 2 of the License, or (at your option)
//  any later version.
//
//  This program is distributed in the hope that it will be useful, but
//  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
//  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
//  more details.
//
//  You should have received a copy of the GNU General Public License along with
//  this program; if not, write to the Free Software Foundation, Inc., 59 Temple
//  Place, Suite 330, Boston, MA 02111-1307 USA
//  **** END LICENSE BLOCK ****
var LiveHTTPHeaders = {
  getPostData : function(oHttp) {
    function postData(stream) {
      // Scriptable Stream Constants
      const JS_SCRIPTABLEINPUTSTREAM_CID = "@mozilla.org/scriptableinputstream;1";
      const JS_SCRIPTABLEINPUTSTREAM     = "nsIScriptableInputStream";
      const JS_ScriptableInputStream = new Components.Constructor
           ( JS_SCRIPTABLEINPUTSTREAM_CID, JS_SCRIPTABLEINPUTSTREAM );
      // Create a scriptable stream
      this.seekablestream = stream;
      this.stream = new JS_ScriptableInputStream();
      this.stream.init(this.seekablestream);
      this.mode = this.FAST;

      // Check if the stream has headers
      try { 
        this.seekablestream.QueryInterface(Components.interfaces.nsIMIMEInputStream);
        this.hasheaders = true;
        this.body = -1; // Must read header to find body
      } catch (ex) {
        this.hasheaders = false;
        this.body = 0;  // Body at the start of the stream
      }
    }
    postData.prototype = {
      NONE: 0,
      FAST: 1,
      SLOW: 2,
      rewind: function() {
        this.seekablestream.seek(0,0);
      },
      tell: function() {
        return this.seekablestream.tell();
      },
      readLine: function() {
        var line = "";
        var size = 0;
        try { size = this.stream.available(); } catch (ex) { size = 0; }
        for (var i=0; i<size; i++) {
          var c = this.stream.read(1);
          if (c == '\r') {
          } else if (c == '\n') {
            break;
          } else {
            line += c;
          }
        }
        return line;
      },
      setMode: function(mode) {
        if (mode < this.NONE && mode > this.SLOW) {
          throw "postData: unsupported mode: " + this.mode;
        }
        this.mode = mode;
      },
      visitPostHeaders: function(visitor) {
        this.rewind();
        if (!this.hasheaders) { return; }
        var line = this.readLine();
        while(line) {
          if (visitor) {
            var tmp = line.split(/:\s?/);
            visitor.visitHeader(tmp[0],tmp[1]);
          }
          line = this.readLine();
        }
        body = this.tell();
      },
      getPostBody: function(max) {
        // Position the stream to the start of the body
        if (this.body < 0 || this.seekablestream.tell() != this.body) {
          this.visitPostHeaders(null);
        }

        var size = 0;
        try { size = this.stream.available(); } catch(ex) { size = 0; }
        if (max && max >= 0 && max<size) size = max;

        var postString = "";
        try {
          switch (this.mode) {
            case this.NONE:
              //Don't get any content
              break;
            case this.FAST:
              //Get the content in one shot
              if (size>0) {
                postString = this.stream.read(size);
              }
              break;
            case this.SLOW:
              //Must read octet by octet because of a bug in nsIMultiplexStream.cpp
              //This is to avoid 'NS_BASE_STREAM_CLOSED' exception that may occurs
              //See bug #188328.
              for (var i=0; i<size; i++) {
                var c=this.stream.read(1);
                c ? postString+=c : postString+='\0';
              }
              break;
          }
        } catch (ex) {
          //dump("Exception while getting POST CONTENT with mode "+this.mode+": "+ex+"\n");
          return ""+ex;
        } finally {
          // Need to close the stream after use.  
          this.seekablestream.close();
          this.stream.close();
        }
	return postString;
      }
    }
   
    // Get the postData stream from the Http Object 
    try {
      // Must change HttpChannel to UploadChannel to be able to access post data
      oHttp.QueryInterface(Components.interfaces.nsIUploadChannel);
      // Get the post data stream
      if (oHttp.uploadStream) {
        // Must change to SeekableStream to be able to rewind
        oHttp.uploadStream.QueryInterface(Components.interfaces.nsISeekableStream);
        // And return a postData object
        return new postData(oHttp.uploadStream);
      } 
    } catch (e) {
      //dump("POSTDATAEXCEPTION:"+e+"\n");
    }
  return null;
  }
}

/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is Robert Sayre.
 * Portions created by the Initial Developer are Copyright (C) 2006
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * Flock Inc. <erwan@flock.com>
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

const HOURS_TO_MINUTES = 60;
const MINUTES_TO_SECONDS = 60;
const SECONDS_TO_MILLISECONDS = 1000;
const MINUTES_TO_MILLISECONDS = MINUTES_TO_SECONDS * SECONDS_TO_MILLISECONDS;
const HOURS_TO_MILLISECONDS = HOURS_TO_MINUTES * MINUTES_TO_MILLISECONDS;

var EXPORTED_SYMBOLS = ["ISO8601DateUtils"];

debug("*** loading ISO8601DateUtils\n");

var ISO8601DateUtils = {

  /**
  * XXX Thunderbird's W3C-DTF function
  *
  * Converts a W3C-DTF (subset of ISO 8601) date string to a Javascript
  * date object. W3C-DTF is described in this note:
  * http://www.w3.org/TR/NOTE-datetime IETF is obtained via the Date
  * object's toUTCString() method.  The object's toString() method is
  * insufficient because it spells out timezones on Win32
  * (f.e. "Pacific Standard Time" instead of "PST"), which Mail doesn't
  * grok.  For info, see
  * http://lxr.mozilla.org/mozilla/source/js/src/jsdate.c#1526.
  */
  parse: function ISO8601_parse(aDateString) {
    var dateString = aDateString;
    if (!dateString.match('-')) {
      // Workaround for server sending
      // dates such as: 20030530T11:18:50-08:00
      // instead of: 2003-05-30T11:18:50-08:00
      var year = dateString.slice(0, 4);
      var month = dateString.slice(4, 6);
      var rest = dateString.slice(6, dateString.length);
      dateString = year + "-" + month + "-" + rest;
    }

    var parts = dateString.match(/(\d{4})(-(\d{2,3}))?(-(\d{2}))?(T(\d{2}):(\d{2})(:(\d{2})(\.(\d+))?)?(Z|([+-])(\d{2}):(\d{2}))?)?/);

    // Here's an example of a W3C-DTF date string and what .match returns for it.
    //
    // date: 2003-05-30T11:18:50.345-08:00
    // date.match returns array values:
    //
    //   0: 2003-05-30T11:18:50-08:00,
    //   1: 2003,
    //   2: -05,
    //   3: 05,
    //   4: -30,
    //   5: 30,
    //   6: T11:18:50-08:00,
    //   7: 11,
    //   8: 18,
    //   9: :50,
    //   10: 50,
    //   11: .345,
    //   12: 345,
    //   13: -08:00,
    //   14: -,
    //   15: 08,
    //   16: 00

    // Create a Date object from the date parts.  Note that the Date
    // object apparently can't deal with empty string parameters in lieu
    // of numbers, so optional values (like hours, minutes, seconds, and
    // milliseconds) must be forced to be numbers.
    var date = new Date(parts[1], parts[3] - 1, parts[5], parts[7] || 0,
      parts[8] || 0, parts[10] || 0, parts[12] || 0);

    // We now have a value that the Date object thinks is in the local
    // timezone but which actually represents the date/time in the
    // remote timezone (f.e. the value was "10:00 EST", and we have
    // converted it to "10:00 PST" instead of "07:00 PST").  We need to
    // correct that.  To do so, we're going to add the offset between
    // the remote timezone and UTC (to convert the value to UTC), then
    // add the offset between UTC and the local timezone //(to convert
    // the value to the local timezone).

    // Ironically, W3C-DTF gives us the offset between UTC and the
    // remote timezone rather than the other way around, while the
    // getTimezoneOffset() method of a Date object gives us the offset
    // between the local timezone and UTC rather than the other way
    // around.  Both of these are the additive inverse (i.e. -x for x)
    // of what we want, so we have to invert them to use them by
    // multipying by -1 (f.e. if "the offset between UTC and the remote
    // timezone" is -5 hours, then "the offset between the remote
    // timezone and UTC" is -5*-1 = 5 hours).

    // Note that if the timezone portion of the date/time string is
    // absent (which violates W3C-DTF, although ISO 8601 allows it), we
    // assume the value to be in UTC.

    // The offset between the remote timezone and UTC in milliseconds.
    var remoteToUTCOffset = 0;
    if (parts[13] && parts[13] != "Z") {
      var direction = (parts[14] == "+" ? 1 : -1);
      if (parts[15])
        remoteToUTCOffset += direction * parts[15] * HOURS_TO_MILLISECONDS;
      if (parts[16])
        remoteToUTCOffset += direction * parts[16] * MINUTES_TO_MILLISECONDS;
    }
    remoteToUTCOffset = remoteToUTCOffset * -1; // invert it

    // The offset between UTC and the local timezone in milliseconds.
    var UTCToLocalOffset = date.getTimezoneOffset() * MINUTES_TO_MILLISECONDS;
    UTCToLocalOffset = UTCToLocalOffset * -1; // invert it
    date.setTime(date.getTime() + remoteToUTCOffset + UTCToLocalOffset);

    return date;
  },

  create: function ISO8601_create(aDate) {
    function zeropad (s, l) {
      s = s.toString(); // force it to a string
      while (s.length < l) {
        s = '0' + s;
      }
      return s;
    }

    var myDate;
    // if d is a number, turn it into a date
    if (typeof aDate == 'number') {
      myDate = new Date()
      myDate.setTime(aDate);
    } else {
      myDate = aDate;
    }

    // YYYY-MM-DDThh:mm:ssZ
    var result = zeropad(myDate.getUTCFullYear (), 4) +
                 zeropad(myDate.getUTCMonth () + 1, 2) +
                 zeropad(myDate.getUTCDate (), 2) + 'T' +
                 zeropad(myDate.getUTCHours (), 2) + ':' +
                 zeropad(myDate.getUTCMinutes (), 2) + ':' +
                 zeropad(myDate.getUTCSeconds (), 2) + 'Z';

    return result;
  }
}


} ).call(this); 
