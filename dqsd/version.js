function versionCheck()
{
  var bSuccess = true;
  var testObject;

  // First, see if we can create the object at all
  try
  {
    testObject = new ActiveXObject("DQSDTools.Launcher");
  }
  catch(e)
  {
    alert("The DQSD helper DLL is not correctly installed");
    bSuccess = false;
  }

  // Try a version query on it
  if(bSuccess)
  {
    try
    {
      // The DLL version has to be great than or equal to
      // this number
      if(!testObject.VersionIsCorrect(3,1,8,0))
      {
         alert("The DQSD helper DLL is out of date.\nPlease reboot and run the setup program again.");
         bSuccess = false;
      }
    }
    catch(e)
    {
      alert("The DQSD helper DLL version couldn't be checked (error '" + e.description + "').\nPlease reboot and run the setup program again.");
      bSuccess = false;
    }
  }
  testObject = null;

  // Verify that the scripting engine version is what we need
  if(bSuccess)
  {
  	var reqMajor = 5;
  	var reqMinor = 6;
  	
  	var instMajor = ScriptEngineMajorVersion();
  	var instMinor = ScriptEngineMinorVersion();
  	
  	if(instMajor < reqMajor || (instMajor == reqMajor && instMinor < reqMinor))
  	{
  		alert("Dave's Quick Search Deskbar:\nYour version of the Windows Script Components (" + instMajor + "." + instMinor + ") is outdated.\nGet the latest from http://msdn.microsoft.com/downloads/list/webdev.asp");
  		bSuccess = false;
  	}
  }

  return bSuccess;
}

// Check for the correct ActiveX DLL version
if(!versionCheck())
{
  // This is not a nice thing to do, but does reinforce the fact that you
  // can't use the bar like it is
  // Unfortunately, you can't do a toolbar refresh to recover from this
  // so it might not be a great idea
  window.close();
}

function checkWebForUpdateNotifyAll()
{
  checkWebForUpdate(false)
}

// These are required to be global for the MSXML DOM
// asynchronous load to work properly.
var checkUpdateOnlineVersionDOM = null;
var checkUpdateQuiet = true;
var DQSD_CHECKUPDATE_URL = "http://www.dqsd.net/version.xml";

function checkWebForUpdate()
{
  checkUpdateQuiet = true;
  if ( arguments.length )
  {
    checkUpdateQuiet = arguments[0];
  }

  if ( checkUpdateQuiet && ( typeof checkForUpdate == 'undefined' || !checkForUpdate ) )
    return;
    
  try
  {
    // Load asynchronously to make sure explorer.exe doesn't lock up if this takes a while
    // (has been known to happen)
    checkUpdateOnlineVersionDOM = getMSXMLDOMDocumentInstance();
    checkUpdateOnlineVersionDOM.async = true;
    checkUpdateOnlineVersionDOM.onreadystatechange = onVersionLoadReadyStateChange;
    checkUpdateOnlineVersionDOM.load( DQSD_CHECKUPDATE_URL );
  }
  catch(e)
  {
    if ( !checkUpdateQuiet )
      alert("Dave's Quick Search Deskbar:\nUnable to check for update. Check your internet connection.");
  }
  
  checkForUpdate = false;  // only display once per session  
}

function onVersionLoadReadyStateChange()
{
  if( typeof checkUpdateOnlineVersionDOM != 'undefined' && checkUpdateOnlineVersionDOM.readyState == 4 ) // finished loading
  {
    if( checkUpdateOnlineVersionDOM.parseError.errorCode == 0 )
    {
      var rversion = getVersion( checkUpdateOnlineVersionDOM.documentElement );
      var lversion = getVersionFromVersionFile( "version.xml" );
  
      // If there's a later version and the user wants to be notified of it, or
      // there's a later version and there was an explicit update query, then show them
      // what's available
      if ( ( versioncmp( rversion, lversion ) > 0 ) && ( notifyUser( rversion ) || !checkUpdateQuiet ) )
      {
        window.showModalDialog("versiondialog.htm", { lversion:lversion, rversion:rversion }, "dialogHeight: 150px; dialogWidth: 300px; dialogTop: " + (screen.height / 2 - 75) + "px; dialogLeft: " + (screen.width / 2 - 150) + "px; edge: Raised; center: Yes; help: No; resizable: Yes; status: No; scroll: No;");
      }
      else if ( !checkUpdateQuiet )
      {
        rversion.htmlDescription = "You are using the latest version.<br/>For more details visit <a tabindex=-1 href='http://www.dqsd.net' onclick='window.close();'>www.dqsd.net</a>."
        window.showModalDialog("versiondialog.htm", { lversion:lversion, rversion:rversion }, "dialogHeight: 100px; dialogWidth: 300px; dialogTop: " + (screen.height / 2 - 50) + "px; dialogLeft: " + (screen.width / 2 - 150) + "px; edge: Raised; center: Yes; help: No; resizable: Yes; status: No; scroll: No;");
      }
    }
    else
    {
      alert( "Dave's Quick Search Deskbar:\nFailed to parse on-line version information. Error message: " + checkUpdateOnlineVersionDOM.parseError.reason );
    }
  }
}

function notifyUser( rversion )
{
  return ( checkForUpdateTypes.search( new RegExp("\\b" + rversion.type + "\\b", "i" ) ) >= 0 );
}

function getVersionFromVersionFile( versionXML )
{
  var version = null;

  var msxml = getMSXMLDOMDocumentInstance();
  msxml.async = false;
  if ( msxml.load( versionXML ) )
  {
    version = getVersion( msxml.documentElement );
  }

  return version;
}

function getVersion( xmldom )
{
  var version = new Object();
  version.majorHi = parseInt( xmldom.selectSingleNode( "/dqsd_version_info/version/majorhi" ).text );
  version.majorLo = parseInt( xmldom.selectSingleNode( "/dqsd_version_info/version/majorlo" ).text );
  version.minorHi = parseInt( xmldom.selectSingleNode( "/dqsd_version_info/version/minorhi" ).text );
  version.minorLo = parseInt( xmldom.selectSingleNode( "/dqsd_version_info/version/minorlo" ).text );
  version.htmlDescription = xmldom.selectSingleNode( "/dqsd_version_info/html_description" ).xml;
  version.date = xmldom.selectSingleNode( "/dqsd_version_info/date" ).text;
  version.type = xmldom.selectSingleNode( "/dqsd_version_info/type" ).text;
  return version;
}

// returns 1 if v1 > v2; -1 if v1 < v2; 0 if equal
function versioncmp( v1, v2 )
{
  if ( v1.majorHi > v2.majorHi )
    return 1;
  if ( v2.majorHi > v1.majorHi )
    return -1;
  if ( v1.majorLo > v2.majorLo )
    return 1;
  if ( v2.majorLo > v1.majorLo )
    return -1;
  if ( v1.minorHi > v2.minorHi )
    return 1;
  if ( v2.minorHi > v1.minorHi )
    return -1;
  if ( v1.minorLo > v2.minorLo )
    return 1;
  if ( v2.minorLo > v1.minorLo )
    return -1;
  
  return 0; // equal
}
