var dragObserver = {
  origin        : null,
  overName      : false,
  externalFiles : new Array(),

  getSupportedFlavours : function() {
    var flavours = new FlavourSet();
    flavours.appendFlavour("application/x-moz-file", "nsILocalFile");
    flavours.appendFlavour("text/unicode");
    return flavours;
  },

  onDragStart : function (event, dragData, dragAction) {
    var targetID  = event.target.getAttribute('id');
    this.origin   = null;
    this.overName = false;

    if ((!gFtp.isConnected && targetID == 'remotetreechildren') || !treeHighlighter.dragSessionEnabled) {
      return;
    }

    if (targetID == 'localtreechildren' || targetID == 'remotetreechildren') {
      dragData.data = new TransferData();
      dragData.data.addDataForFlavour("text/unicode", targetID);
      this.origin = targetID;
    }

    event.preventDefault();
  },

  onDragOver : function (event, flavour, dragSession) {
    var targetID = event.target.getAttribute('id');
    var row = { }; var col = { }; var child = { };

    if (gFtp.isConnected && flavour.contentType == "application/x-moz-file"
                         && (targetID == 'remotetreechildren' || targetID == 'remotedirtreechildren' || targetID == 'queuetreechildren')) {
      this.externalFiles = new Array();

      var transObj = Components.classes["@mozilla.org/widget/transferable;1"].createInstance(Components.interfaces.nsITransferable);
      transObj.addDataFlavor("application/x-moz-file");       // only look at files

      for (var x = 0; x < dragSession.numDropItems; ++x) {    // iterate through dragged items getting any files
        try {
          dragSession.getData(transObj, x);
          var dataObj     = new Object();
          var dropSizeObj = new Object();
          transObj.getTransferData("application/x-moz-file", dataObj, dropSizeObj);

          var droppedFile = dataObj.value.QueryInterface(Components.interfaces.nsILocalFile);
          this.externalFiles.push(droppedFile);
        } catch (ex) {
          debug(ex);
          continue;
        }
      }

      this.origin         = "external";
      dragSession.canDrop = true;
    } else if (!gFtp.isConnected && flavour.contentType == "application/x-moz-file") {
      this.origin         = null;
      dragSession.canDrop = false;
    }

    if ((this.origin == 'remotetreechildren'                              && targetID == 'localtreechildren')
    || ((this.origin == 'localtreechildren' || this.origin == "external") && targetID == 'remotetreechildren')) {
      dragSession.canDrop = true;

      var x = { }; var y = { }; var width = { }; var height = { };

      if (targetID == 'localtreechildren') {
        gLocalTree.treeBoxObject.getCellAt(event.pageX, event.pageY, row, col, child);
        gLocalTree.treeBoxObject.getCoordsForCellItem(row.value, gLocalTree.columns["localname"], "text", x, y, width, height);
        this.overName = row.value != -1 && event.pageX - gLocalTreeChildren.boxObject.x < x.value + width.value;
        if (row.value != -1) {
          gLocalTree.treeBoxObject.invalidateCell(row.value, gLocalTree.columns["localname"]);
        }
      } else {
        gRemoteTree.treeBoxObject.getCellAt(event.pageX, event.pageY, row, col, child);
        gRemoteTree.treeBoxObject.getCoordsForCellItem(row.value, gRemoteTree.columns["remotename"], "text", x, y, width, height);
        this.overName = row.value != -1 && event.pageX - gRemoteTreeChildren.boxObject.x < x.value + width.value;
        if (row.value != -1) {
          gRemoteTree.treeBoxObject.invalidateCell(row.value, gRemoteTree.columns["remotename"]);
        }
      }
    } else if (targetID == 'localdirtreechildren' || targetID == 'remotedirtreechildren') {
      if (targetID == 'localdirtreechildren') {
        gLocalDirTree.treeBoxObject.getCellAt(event.pageX, event.pageY, row, col, child);
      } else {
        gRemoteDirTree.treeBoxObject.getCellAt(event.pageX, event.pageY, row, col, child);
      }

      dragSession.canDrop = row.value != -1;
    } else if ((this.origin == 'localtreechildren' || this.origin == "remotetreechildren" || this.origin == "external") && targetID == 'queuetreechildren') {
      dragSession.canDrop = true;
    } else {
      dragSession.canDrop = false;
    }
  },

  onDrop: function (event, dragData, dragSession) {
    event.preventDefault();
  }
};
