var Cc = Components.classes;
var Ci = Components.interfaces;

/*
 * 参考https://developer.mozilla.org/en/Code_snippets/File_I%2f%2fO
**/

function CreateFile(fileName){
    // the ProfD is defined at http://mxr.mozilla.org/seamonkey/source/xpcom/io/nsAppDirectoryServiceDefs.h
    // 文件创建相关函数 - https://developer.mozilla.org/en/nsIFile
    var profileDir = Cc["@mozilla.org/file/directory_service;1"].getService(Ci.nsIProperties).get("ProfD", Ci.nsIFile);
    var file = Cc["@mozilla.org/file/local;1"].createInstance(Ci.nsILocalFile);
    file.initWithPath(profileDir.path);
    file.append(fileName);

    if(!file.exists())
	file.createUnique(Ci.nsIFile.NORMAL_FILE_TYPE, 0666);

    //alert(file.path);
    return file;
}