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
      if(!testObject.VersionIsCorrect(3,1,5,0))
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

function checkWebForUpdate()
{
  var quiet = true;
  if ( arguments.length )
  {
    var quiet = arguments[0];
  }

  if ( quiet && ( typeof checkForUpdate == 'undefined' || !checkForUpdate ) )
    return;
    
  checkForUpdate = false;  // only display once per session

  try
  {
    var rversion = getVersionFromVersionFile( "http://cvs.sourceforge.net/cgi-bin/viewcvs.cgi/*checkout*/dqsd/dqsd/version.xml?content-type=text/xml" );
    var lversion = getVersionFromVersionFile( "version.xml" );

    // If there's a later version and the user wants to be notified of it, or
    // there's a later version and there was an explicit update query, then show them
    // what's available
    if ( ( versioncmp( rversion, lversion ) > 0 ) && ( notifyUser( rversion ) || !quiet ) )
    {
      window.showModalDialog("versiondialog.htm", { lversion:lversion, rversion:rversion }, "dialogHeight: 150px; dialogWidth: 300px; dialogTop: " + (screen.height / 2 - 75) + "px; dialogLeft: " + (screen.width / 2 - 150) + "px; edge: Raised; center: Yes; help: No; resizable: Yes; status: No; scroll: No;");
    }
    else if ( !quiet )
    {
      rversion.htmlDescription = "You are using the latest version.<br/>For more details visit <a tabindex=-1 href='http://www.dqsd.net' onclick='window.close();'>www.dqsd.net</a>."
      window.showModalDialog("versiondialog.htm", { lversion:lversion, rversion:rversion }, "dialogHeight: 100px; dialogWidth: 300px; dialogTop: " + (screen.height / 2 - 50) + "px; dialogLeft: " + (screen.width / 2 - 150) + "px; edge: Raised; center: Yes; help: No; resizable: Yes; status: No; scroll: No;");
    }
  }
  catch(e)
  {
    if ( !quiet )
      alert("Unable to check for update.  Check your internet connection.");
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
