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
    window.open(url, name).focus();
}

// direct shellexecute
function openDocument(path)
{
  ensureLauncher();
  if (DQSDLauncher)
  {
    DQSDLauncher.OpenDocument(path);
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
    fileTable.push(fileLines[i].split("\t"));
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
