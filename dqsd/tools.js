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

var DQSDLauncher = null;
var triedToCreateLauncher = false;
var useExternalBrowser = (launchmode > 0);

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
      DQSDLauncher = null;
    }
  }
  return (DQSDLauncher != null);
}

if (useExternalBrowser)
{
  if (ensureLauncher())
  {
    var browserPath = DQSDLauncher.pathDefaultBrowser
    if (launchmode == 2 && browserPath.toLowerCase().indexOf("iexplore.exe") > 0)
      useExternalBrowser = false;
  }
  else // Gracefully ignore problems creating the control.
  {
    useExternalBrowser = false;
  }
}

// Open a search window either with the default browser or with IE
function openSearchWindow(url)
{
  openNamedSearchWindow(url, reuseBrowserWindowMode ? DQSD_BROWSER_WINDOW_NAME : "_blank");
}

// Open a search window in an existing frame
function openNamedSearchWindow(url, name)
{
  if (useExternalBrowser && DQSDLauncher)
    DQSDLauncher.OpenDocument(url);
  else
    window.open(url, name);
}

// direct shellexecute
function openDocument(path)
{
  ensureLauncher();
  if (DQSDLauncher)
  {
    var params = path.match(/^(("[^"]*")|\S*)\s*(.*)/);
    if (params)
    {
      DQSDLauncher.OpenDocument(params[1], params[3]);
    }
    else
    {
    DQSDLauncher.OpenDocument(path);
  }
}
}

// Submit a form either with the default browser or with IE
function submitForm(form)
{
  // Here's a safeguard for forgetting to put a target in the FORM
  if (!form.target || (form.target && form.target == ''))
    form.target = '_blank';

  // Reuse a single window if the user wants to and the target is _blank
  // Don't override targets with any other name because some searches may want
  // their own window.
  if ((reuseBrowserWindowMode > 0) && form.target && (form.target == '_blank'))
  {
    // 1=same window always; 2=new window for each search type
    form.target = (reuseBrowserWindowMode == 1 ? DQSD_BROWSER_WINDOW_NAME : (DQSD_BROWSER_WINDOW_NAME + '_' + form.name));
  }
    
  if (useExternalBrowser && DQSDLauncher)
    DQSDLauncher.SubmitForm(form);
  else
    form.submit();
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
// should be modified to have something like an AppendToFile 
// method.
function appendToFile(filename, contents)
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

// read tab-delimited file
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
      //split on the pipe symbol or the tab character 
      fileTable.push(fileLines[i].split(/[\|\t]/));
  }
  return fileTable;
}
  
function protocolHandled(url)
{
  if (!ensureLauncher())
    return true; // what else can we do except assume the protocol is handled?
  
  var results = url.match(/(\w+):/);
  if (!results)
    return false; // not a valid url
    
  try
  {
    var handler = DQSDLauncher.GetProtocolHandler(results[1]);
  }
  catch (e)
  {
    return false;
  }

  return true;
}

// Returns an object with two properties:
//   q:        string with switches removed
//   switches: array of objects with these two properties:
//     name:   name of switch, expanded if it matches the prefix
//             of a switch in the list of expectedSwitches arg
//     value:  value of switch.  I.e., /switch:value
function parseArgs(
  q, /* string to be parsed */
  expectedSwitches /* list of expected switches, used to expand abbreviated switches */)
{
  var switches = [];
  var tmpq = q;
  
  // Grab each token that looks like a switch (/\S+)
  var reSwitch = /\/((\S+)(?::?(\S*)))\s*/;
  var res = null;
  expectedSwitches = ',' + expectedSwitches + ',';
  while (res = tmpq.match(reSwitch))
  {
    // Expand switch if it's in the list of expected switches...
    var fullswitch = expectedSwitches.match(new RegExp(',\\s*('+res[2]+'\\S*)\\s*,', 'i'));
  	if (fullswitch && fullswitch[1])
  	  switches.push( {name:fullswitch[1].toLowerCase(), value:res[3]} );
    else
    {
      var o = {name:res[2].toLowerCase(), value:res[3]};
      switches.push( o ); // ...otherwise just return the switch that was found
    }
      
    // Drop switch we just found
    tmpq = tmpq.replace(reSwitch, '');
  }
  
  // Trim the remaining string which now contains the string without switches
  q = tmpq.replace(/^\s*/, '').replace(/\s*$/, '');

  return { q:q, switches:switches };
}
