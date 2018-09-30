function doGet(e) { 
  Logger.log( JSON.stringify(e) );

  var result = 'Ok';

  if (e.parameter == undefined) {
    result = 'No Parameters';
  }
  else {
    var id = 'your spreadsheetURL ID';//docs.google.com/spreadsheetURL/d
    var sheet = SpreadsheetApp.openById(id).getActiveSheet();
    var newRow = sheet.getLastRow() + 1;
    var rowData = [];
    //var waktu = new Date();
    rowData[0] = new Date();
    
    for (var param in e.parameter) {
      Logger.log('In for loop, param='+param);
      var value = stripQuotes(e.parameter[param]);
      switch (param) {
        case 'Data1':
          rowData[1] = value;
          break;
        case 'Data2':
          rowData[2] = value;
          break;
        case 'Data3':
          rowData[3] = value;
          break;
        case 'Data4':
          rowData[4] = value;
          break;
        case 'Data5':
          rowData[5] = value;
          break;
        case 'Data6':
          rowData[6] = value;
          break;
        default:
          result = "unsupported parameter";
      }
    }
    Logger.log(JSON.stringify(rowData));
    var newRange = sheet.getRange(newRow, 1, 1, rowData.length);
    newRange.setValues([rowData]);
  }

  return ContentService.createTextOutput(result);
}

function stripQuotes( value ) {
  return value.replace(/^["']|['"]$/g, "");
}
