var DQSD_BROWSER_WINDOW_NAME = "DQSDBrowserWindow";

// IE 5.0 and earlier don't have splice, push
if (!Array.prototype.splice)
{
  function array_splice(ind,cnt)
  {
    removeArray = this.slice(ind,ind+cnt);
    endArray = this.slice(ind+cnt);
    this.length = ind;
    for (var i=2; i < arguments.length; i++)
      this[this.length] = arguments[i];
    for (var i=0; i < endArray.length; i++)
      this[this.length] = endArray[i];
    return removeArray;
  }
  Array.prototype.splice = array_splice;
}

if (!Array.prototype.push)
{
  function array_push()
  {
    for (var i=0; i<arguments.length; i++)
      this[this.length] = arguments[i];
    return this.length;
  }
  Array.prototype.push = array_push;
}

// Tools that rely on DQSDTools.dll

function ensureLauncher()
{
  if (!triedToCreateLauncher)
  {
    triedToCreateLauncher = true;

    try
    {
      DQSDLauncher = new ActiveXObject("DQSDTools.Launcher");
    }
    catch (e)
    {
      alert('Unable to instantiate DQSDTools.Launcher:\n' + e.description);
      DQSDLauncher = null;
    }
  }
  return (DQSDLauncher != null);
}

// file exists
function fileExists(filename)
{
  if (ensureLauncher())
  {
    return DQSDLauncher.FileExists(filename);
  }
  else
    throw "Unable to check if file " + filename + " exists";
}

// rename file
function renameFile(fromFilename, toFilename)
{
  if (ensureLauncher())
  {
	  try
	  {
		DQSDLauncher.RenameFile(fromFilename, toFilename);
		return true;
	  }
	  catch (e)
	  {
		  return false;
	  }
  }
  else
    throw "Unable to rename file " + filename;
}

// read file
function readFile(filename)
{
  if (ensureLauncher())
  {
    return DQSDLauncher.ReadFile(filename);
  }
  else
    throw "Unable to read file " + filename;
}

// write file
function writeFile(filename, contents)
{
  if (ensureLauncher())
  {
    DQSDLauncher.WriteFile(filename, contents);
  }
  else
    throw "Unable to write file " + filename;
}

// Here's a quick hack to append to an existing file, but
// is very slow because it rewrites the whole file.  If
// this is needed for anything but debugging, the component
// should be modified to have something like an AppendFile 
// method.
function appendFile(filename, contents)
{
  if (ensureLauncher())
  {
    var curr = '';
    try
    {
      curr = DQSDLauncher.ReadFile(filename);
    }
    catch (e) {}
    DQSDLauncher.WriteFile(filename, curr + contents);
  }
  else
    throw "Unable to append to file " + filename;
}

// get files
function getFiles(directory)
{
  if (ensureLauncher())
  {
    try
    {
      return DQSDLauncher.GetFiles(directory);
    }
    catch (e)
    {
      return "";
    }
  }
  else
    throw "Unable to get files from directory " + directory;
}

function getFolders(directory)
{
  if (ensureLauncher())
  {
    try
    {
      return DQSDLauncher.GetFolders(directory);
    }
    catch (e)
    {
      return getFiles(directory);
    }
  }
  else
    throw "Unable to get folders from directory " + directory;
}


// read tab-delimited, or pipe (|) delimited file
function readTabDelimitedFile(filename)
{
  var fileText = "";
  try {fileText = readFile(filename);} catch (e) {}
  fileText = fileText.replace(/\r\n/g,"\n");
  var fileLines = fileText.split("\n");
  var fileTable = [];
  for (var i = 0; i < fileLines.length; i++)
  {
    if (!fileLines[i].match(/^\s*($|\/\/)/))  // ignore comments (//) and blank lines
    {
      //must prepend a space or the split will complain on the gg search
      if (fileLines[i].match(/^[\|\t]/))
      {
        fileLines[i]=" "+fileLines[i];
      }
      //split on the pipe symbol or the tab character 
      fileTable.push(fileLines[i].split(/\s*[\|\t]\s*/));
    }
  }
  return fileTable;
}

function isInternalSearch( fn )
{
  return (fn.substr(0,INTERNAL_FUNC_PREFIX.length) == INTERNAL_FUNC_PREFIX)
}

function getSearchAliases( search )
{
  return isInternalSearch(search.aliases[0]) ? search.aliases.slice(1).join(', ') : search.aliases.join(', ');
}

function getFirstAlias( search )
{
  var alias = '';
  
  // If it's an internal search, then the first name will be the internal name (_dqsd_internal_fn_??)
  // so return the second alias which should always be there, _unless_ this is a shortcut (mapping to 
  // an internal function) defined aliases.txt that was overridden by a shortcut in localaliases.txt.
  if ( isInternalSearch(search.aliases[0]) )
  {
    if ( search.aliases.length > 1 )
    {
      alias = search.aliases[1];
    }
    else
    {
      alias = ''; // could return the internal name here, but that would be weird.
    }
  }
  else
  {
    alias = search.aliases[0];
  }
  return alias;
}

// This method just is just a convenience for instantiating the same
// same MSXML DOM each time. This method is now changed
// to probe for older versions of MSXML, in case the newest
// version is not installed on the machine.
function getMSXMLDOMDocumentInstance()
{
  try
  {
    var xmlDom = new ActiveXObject("Msxml2.DOMDocument.4.0");
    xmlDom.setProperty("NewParser", true); // this is much faster, but doesn't support asynchronous parsing
    return xmlDom;
  }
  catch(e)
  {
    try
    {
      return new ActiveXObject("Msxml2.DOMDocument");
    }
    catch(e)
    {
      return new ActiveXObject("Msxml.DOMDocument");
    }
  }
}

function getDefaultLanguage()
{
  if (typeof language == "undefined" || language == "")
  {
    language = navigator.userLanguage.substr(0,2);
  }
}

function initGlobals()
{
  init = false;
  DQSDLauncher = null;
  triedToCreateLauncher = false;

  firsterror = null;
  // window.onerror=handleerror;

  var uaMatch = navigator.userAgent.toLowerCase().match("msie\\s+(\\d+\\.\\d*)");
  var ie_version = (uaMatch ? parseFloat(uaMatch[1]) : 0.0);
  dopopup = (ie_version >= 5.5);

  useExternalBrowser = (launchmode > 0);
  if (useExternalBrowser)
  {
    if (ensureLauncher())
    {
      var browserPath;
      try
      {
        browserPath = DQSDLauncher.pathDefaultBrowser;
        if (launchmode != 0 && browserPath.toLowerCase().indexOf("iexplore.exe") > 0)
          useExternalBrowser = false;
      }
      catch(e)
      {
		// Just ignore errors here, which seem to be caused by bad external browser setup
		// (Happens on some 0.9x builds of FireFox)
      }
    }
    else // Gracefully ignore problems creating the control.
    {
      useExternalBrowser = false;
    }
  }

  reuseBrowserWindowModeOverride = reuseBrowserWindowMode;
  searchPrefix = '';

  init = true;
  selfdrag = false;

  bant = null;

  // eliminate Go button if window is narrow
  gowidth = -1;
  textheight = -1;

  keyboardHook = null;
}

// catch errors
function handleerror(msg, url, lineno)
{
  if (!firsterror)
  {
     firsterror = msg;
     alert("Error (line " + lineno + "):\n" + msg);
  }
  return true;
}

function qualifiedalert(s)
{
  alertmode && alert(s);
}

// Function used to allow startup options for DQSD.
function startup()
{
	if (typeof startupCommands == "undefined" || startupCommands == "")
		return true;

	eval(startupCommands);
	return true;
}