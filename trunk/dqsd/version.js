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
      var version = getVersionFromVersionFile( "version.xml" );
      if(!testObject.VersionIsCorrect( version.majorHi, version.majorLo, version.minorHi, version.minorLo))
      {
         alert("The DQSD helper DLL is out of date");
         bSuccess = false;
      }
    }
    catch(e)
    {
      alert("The DQSD helper DLL version couldn't be checked (error '" + e.description + "')");
      bSuccess = false;
    }
  }
  testObject = null;


  return bSuccess;
}

function getVersionFromVersionFile( versionXML )
{
  var version = null;

  var msxml = new ActiveXObject( "MSXML.DOMDocument" );
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

function checkWebForUpdate()
{
  if ( typeof checkForUpdate == 'undefined' || !checkForUpdate )
    return;

  try
  {
    var rversion = getVersionFromVersionFile( "http://cvs.sourceforge.net/cgi-bin/viewcvs.cgi/*checkout*/dqsd/dqsd/version.xml?content-type=text/xml" );
    var lversion = getVersionFromVersionFile( "version.xml" );
    if ( versioncmp( rversion, lversion ) > 0 )
    {
      window.showModalDialog("versiondialog.htm", { lversion:lversion, rversion:rversion }, "dialogHeight: 150px; dialogWidth: 300px; dialogTop: px; dialogLeft: px; edge: Raised; center: Yes; help: No; resizable: Yes; status: No; scroll: No;");
    }
  }
  catch(e)
  {
    // ignore any errors for now
  }
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

// Check for the correct ActiveX DLL version
if(!versionCheck())
{
  // This is not a nice thing to do, but does reinforce the fact that you
  // can't use the bar like it is
  // Unfortunately, you can't do a toolbar refresh to recover from this
  // so it might not be a great idea
  window.close();
}
