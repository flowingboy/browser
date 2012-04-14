// 全局变量定义

var Cc = Components.classes;
var Ci = Components.interfaces;

var SS = Cc["@mozilla.org/storage/service;1"].getService(Ci.mozIStorageService);

var gDbHandler = null;
var gDbName = "TicketFinder.sqlite";

function Database(){

}

/*
 * the dbName must contain the file ext, for example: 'TicketFinder.sqlite'
**/ 
function openDatabase(dbName){
    // from file fileManager.js
    var dbFile = CreateFile(dbName);

    try{
	gDbHandler = SS.openDatabase(dbFile);
    }catch(e){
	alert("open database " + dbName + " failed!");
    }

    if(gDbHandler && !gDbHandler.connectionReady)
	gDbHandler = null;

    return gDbHandler;
}

function createTable(tbName, schema){
    if(gDbHandler && !gDbHandler.tableExists(tbName))
	gDbHandler.createTable(tbName, schema);
}

/* 执行select操作, 返回的是二维数组, 引用结果的时候需要注意 */
function select(sql){
    var statement = gDbHandler.createStatement(sql);

    try {
	var dataset = [];
	while (statement.executeStep()) {
	    var row = [];
	    var iCols = statement.columnCount;
	    for (var i = 0, k = iCols; i < k; i++){
		row[statement.getColumnName(i)] = statement.getUTF8String(i);
	    }
	    dataset.push(row);
	}
    }catch(e){
	alert(e);
    }

    return dataset;
}

/*
function selectQuery(sQuery, bBlobAsHex){
    this.aTableData = new Array();
    this.aTableType = new Array();
    this.aColumns = null;
    var bResult = false;
    
    var timeStart = Date.now();
    try {
	// mozIStorageStatement
	var stmt = this.dataConnection.createStatement(sQuery);
    }
    catch (e) {
	// statement will be undefined because it throws error);
	this.onSqlError(e, "Likely SQL syntax error: " + sQuery,
			this.dataConnection.lastErrorString);
	this.setErrorString();
	return false;
    }
    
    var iCols = 0;
    var iType, colName;
    try	{
	// do not use stmt.columnCount in the for loop, fetches the value again and again
	iCols = stmt.columnCount;
	this.aColumns = new Array();
	var aTemp, aType;
	for (var i = 0; i < iCols; i++)	{
            colName = stmt.getColumnName(i);
            aTemp = [colName, iType];
            this.aColumns.push(aTemp);
	}
    } catch (e) {
	this.onSqlError(e, "Error while fetching column name: " + colName, null);
    	this.setErrorString();
    	return false;
    }

    var cell;
    var bFirstRow = true;
    try {
	while (stmt.executeStep()) {
            aTemp = [];//new Array();
            aType = [];
            for (i = 0; i < iCols; i++) {
		iType = stmt.getTypeOfIndex(i);
	        if (bFirstRow) {
	            this.aColumns[i][1] = iType;
	        }
		switch (iType) {
		case stmt.VALUE_TYPE_NULL:
		    cell = g_strForNull;
		    break;
		case stmt.VALUE_TYPE_INTEGER:
		    //use getInt64, not getInt32 otherwise long int
		    // as in places.sqlite/cookies.sqlite shows funny values
		    cell = stmt.getInt64(i);
		    break;
		case stmt.VALUE_TYPE_FLOAT:
		    cell = stmt.getDouble(i);
		    break;
		case stmt.VALUE_TYPE_TEXT:
		    cell = stmt.getString(i);
		    break;
		case stmt.VALUE_TYPE_BLOB: //todo - handle blob properly
            	    if (bBlobAsHex) {
	              	var iDataSize = {value:0};
	              	var aData = {value:null};
	  		stmt.getBlob(i, iDataSize, aData);
	  		cell = sm_blob2hex(aData.value);
            	    }
            	    else {
			cell = g_strForBlob;
			if (g_showBlobSize) {
	              	    var iDataSize = {value:0};
	              	    var aData = {value:null};
	  		    stmt.getBlob(i, iDataSize, aData);
	  		    cell += " (Size: " + iDataSize.value + ")";
			}
	            }
		    break;
		default: sData = "<unknown>"; 
		}
		aTemp.push(cell);
		aType.push(iType);
            }
            this.aTableData.push(aTemp);
            this.aTableType.push(aType);
            bFirstRow = false;
	}
	this.miTime = Date.now() - timeStart;
    } catch (e) { 
	this.onSqlError(e, "Query: " + sQuery + " - executeStep failed", null);
    	this.setErrorString();
    	return false;
    } finally {
    	//must be reset after executeStep
    	stmt.reset();
    }
    this.setErrorString();
    return true;
}
*/

function insertQuery(sQuery, params){

}


function updateQuery(sQuery, params){

}