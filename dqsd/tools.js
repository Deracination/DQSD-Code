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
useExternalBrowser = (launchmode > 0);

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
    if (launchmode != 0 && browserPath.toLowerCase().indexOf("iexplore.exe") > 0)
      useExternalBrowser = false;
  }
  else // Gracefully ignore problems creating the control.
  {
    useExternalBrowser = false;
  }
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
    {
      //must prepend a space or the split will complain on the gg search
      if (fileLines[i].match(/^[\|\t]/))
      {
        fileLines[i]=" "+fileLines[i];
      }
      //split on the pipe symbol or the tab character 
      fileTable.push(fileLines[i].split(/[\|\t]/));
  	   }
  }
  return fileTable;
}
