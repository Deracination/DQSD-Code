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
  openNamedSearchWindow(url, "_blank");
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
    DQSDLauncher.OpenDocument(path);
  }
}

// Submit a form either with the default browser or with IE
function submitForm(form)
{
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
  



