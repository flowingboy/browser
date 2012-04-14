/*
 * www.kuxun.com火车票查询接口
 * http://piao.kuxun.cn/search.php?T=Ticket&Cat=sale&From=%E5%8C%97%E4%BA%AC&q=d25&x=61&y=23
**/

// 别名定义
var Cc = Components.classes;
var Ci = Components.interfaces;

var AS = Cc["@mozilla.org/alerts-service;1"].getService(Ci.nsIAlertsService);
var ioservice = Cc["@mozilla.org/network/io-service;1"].getService(Ci.nsIIOService);

var htmlContent = '';

var TicketFinder = {
    /*****************************************************************/
    /**************************** 属性声明 ****************************/
    /*****************************************************************/
    tickets : [],

    isRuning : false,
    interval : 10000,		// 默认是10秒钟执行一次查询
    toIdTrain : 0,
    toIdAero : 0,

    channel : null,
    totalTicketNumber : 0,
    //htmlContent : "",

    /******************************************************************/
    /****************************** 函数声明 ***************************/
    /******************************************************************/

    /*
     * 仅仅当插件加载之后初始化一次，不管插件是否启动, 如数据库表的初始化等
    **/
    init : function(tv){
	// 运行间隔初始化
	this.interval = tv;
	
	// 数据库初始化
	try{
	    gDbHandler = openDatabase(gDbName);
	    // 这里需要判断是否存在着两个表
	    createTable("TrainTicket", 
				   "id INTEGER PRIMARY KEY, url STRING, num STRING, date STRING, class STRING"
				  );
	    createTable("AeroTicket",
				   "id INTEGER PRIMARY KEY, url STRING, num STRING, date STRING, class STRING"
				  );
	}catch(e){
	    alert(e);
	}

	this.totalTicketNumber = select("select count(*) from TrainTicket", false)[0]['count(*)'];
    },
    /* 
     * Manage the http request.
     * Reference the mozilla official page: https://developer.mozilla.org/En/Creating_Sandboxed_HTTP_Connections
    **/
    getUrl : function(url){
	var uri = ioservice.newURI(url, null, null);
	channel = ioservice.newChannelFromURI(uri);

	var listener = new StreamListener(this.callbackFunc);

	channel.notificationCallbacks = listener;
	channel.asyncOpen(listener, null);
    },

    callbackFunc : function(data){
	htmlContent = data;
    },

    /*
     * 调用getUrl获取查询结果页面内容
     * startCity=>出发城市
     * number=>车次/航班号
     * dateRange=>接受的日期或者日期范围
     * pubTime=>可以接受的最早发布时间
    **/
    getTickets : function(startCity, number, dateRange, pubTime){

	var url = "http://piao.kuxun.cn/search.php?T=Ticket&Cat=sale&From=%E5%8C%97%E4%BA%AC&q=d25&x=61&y=23";

	// 这里因为setTimeout调用的getTickets，所以导致getTickets的上下文变为全局的，而不是TicketFinder的上下文
	this.TicketFinder.getUrl(url);
	//这里需要使用\\对(做转义，如果使用\则得到的正则表达式中没有\
	var re = new RegExp("onclick=\"return clickTicket\\('[A-Za-z0-9]+', '[^']*', '[^']*', '[^']*'\\).*$", "igm");
	this.TicketFinder.findNewTickets(htmlContent, re);
    },

    /*
     * 获取htmlContent中包含的ticket链接，并判断是否有新的链接
    **/
    findNewTickets : function(htmlCnt, re){
	var ticketLinks = htmlCnt.match(re);
	var tmpArr = [];
	if(ticketLinks){
	    // 获取车票各个字段信息
	    var re2 = new RegExp("\\('([A-Za-z0-9]+)', '([^']*)', '([^']*)', '([^']*)'\\)", "igm");
	    var ma;
	    for(var i = 0; i < ticketLinks.length; i++){
		ma = ticketLinks[i].match(re2);
		this.tickets[i] = [];
		this.tickets[i]['TicketNum'] = RegExp.$1;
		this.tickets[i]['TicketClass'] = RegExp.$2;
		this.tickets[i]['StartDay'] = RegExp.$3;
		var url = RegExp.$4;
		this.tickets[i]['TicketUrl'] = url.substring(url.indexOf("go=")+3, url.indexOf("&ppz="));
		tmpArr.push(this.tickets[i]['TicketUrl']);
	    }

	    tmpArr = this.getUniqeTicket(tmpArr);
	    tmpArr = this.getUnsavedTicket(tmpArr);
	    if(tmpArr.length > 0)
		this.storeTicket(tmpArr);

	    // 每次弹窗调用两次，一次是弹出，一次是消失
	    var listener = {
		observe: function(subject, topic, data){
		    //openNewTab("http://www.yahoo.cn");
		}
	    };

	    for(var i=0; i<tmpArr.length; i++){
		AS.showAlertNotification("chrome://TicketFinder/skin/icons/ticket.png",
					 "最近"+this.interval/1000+"秒新增车票",
					 tmpArr[i],
					 true,
					 "",
					 listener
					);
	    }
	}

	// 清除之前的计时器，重新开始计时
	clearTimeout(this.toIdTrain);
	this.toIdTrain = setTimeout(this.getTickets, this.interval, "", "", "", "");

	// 这里每次调用完findNewTicket必须清空该数组
	this.tickets = [];
    },

    /* 去除页面中的重复ticket，根据ticket的实际地址判断 */
    /* arr => 新提取的tickets */
    getUniqeTicket : function(arr){
	arr.sort();
	var i;
	var newArr = [];
	for(i=0; i<arr.length; i++){
	    if(0 == i || i > 0 && arr[i] != arr[i-1])
		newArr.push(arr[i]);
	}

	return newArr;
    },

    /* 获取当前页面中没有存入数据库中的ticket信息 */
    /* arr => 新提取的tickets */
    getUnsavedTicket : function(arr){
	var storedTickets = select("select url from TrainTicket", false);

	var tmpArr = [];
	if(0 == storedTickets.length)
	    return arr;

	var len1 = arr.length;
	var len2 = storedTickets.length;
	var i, j, found = false;
	var el;

	for(i=0; i < len1; i++){
	    el = arr.shift();
	    // TODO: 这里还可以优化，将storedTickets数组排序，然后比较，就可以避免每次都做len2长度的遍历
	    for(j=0; j < len2; j++){
		if(el == storedTickets[j]['url']){
		    // 数据库中已经存在了这个url，直接开始下一次循环
		    found = true;
		    break;
		}
	    }

	    // 数据库中没有这个url，则压入结果数组中
	    if(!found){
		tmpArr.push(el);
	    }else{
		found = false;
	    }
	}

	return tmpArr;
    },

    // 将tickets信息存入数据库中
    storeTicket : function(arr){
	var tmpArr = [];
	var len1 = arr.length;
	var len2 = this.tickets.length;
	var i,j;
	for(i=0; i<len1; i++){
	    for(j=0; j<len2; j++){
		if(arr[i] == this.tickets[j]['TicketUrl']){
		    tmpArr.push(this.tickets[j]);
		    break;
		}
	    }
	}

	len1 = tmpArr.length;
	var statement = null;
	for(i=0; i<len1; i++){
	    var sQuery = "insert into TrainTicket(id, url, num, date, class) values(";
	    var str = "";
	    str = (++this.totalTicketNumber).toString() + ",?1,?2,?3,?4)";
	    sQuery += str;

	    try{
		statement = gDbHandler.createStatement(sQuery);
		statement.bindUTF8StringParameter(0, tmpArr[i]['TicketUrl']);
		statement.bindUTF8StringParameter(1, tmpArr[i]['TicketNum']);
		statement.bindUTF8StringParameter(2, tmpArr[i]['StartDay']);
		statement.bindUTF8StringParameter(3, tmpArr[i]['TicketClass']);
		statement.execute();
	    }catch(e){
		alert(e);
	    }
	}
    },


    //改变statusbar上的图标的图案
    changeIcon : function(){
	var e = document.getElementById('tfStatusIcon');
	if(!this.isRuning){
	    e.setAttribute("findTrainTicket", "on");
	}else{
	    e.removeAttribute("findTrainTicket");
	}
    },

    resumeFinder : function(intervalTime){
	this.toIdTrain = setTimeout(this.getTickets, intervalTime, "", "", "", "");
    },
    
    suspendFinder : function(){
	clearTimeout(this.toIdTrain);
	var d = document.getElementById("tfStatusIcon");
	d.tooltiptext = "TicketFinder is stopped.";
    },

    toggleStatus : function(){
	this.changeIcon();
	if(this.isRuning)
	    this.suspendFinder();
	else
	    this.resumeFinder(this.interval);
	
	this.isRuning = !(this.isRuning);
    },
};


// StreamListener Class defination.
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
	var scriptableInputStream = Cc["@mozilla.org/scriptableinputstream;1"]
        .createInstance(Ci.nsIScriptableInputStream);
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
	channel = null;
    },

    onChannelRedirect: function (aOldChannel, aNewChannel, aFlags) {
	// if redirecting, store the new channel
	channel = aNewChannel;
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
	if (aIID.equals(Ci.nsISupports) ||
            aIID.equals(Ci.nsIInterfaceRequestor) ||
            aIID.equals(Ci.nsIChannelEventSink) ||
            aIID.equals(Ci.nsIProgressEventSink) ||
            aIID.equals(Ci.nsIHttpEventSink) ||
            aIID.equals(Ci.nsIStreamListener))
	    return this;

	throw Components.results.NS_NOINTERFACE;
    }

};