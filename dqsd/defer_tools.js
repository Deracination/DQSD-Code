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
  
  // Grab each token that looks like a switch (/\w+)
  var reSwitch = /\/((\w+)(?::?(\w*)))\s*/;
  var res = null;
  expectedSwitches = ',' + expectedSwitches + ',';
  while (res = tmpq.match(reSwitch))
  {
    // Expand switch if it's in the list of expected switches...
    var fullswitch = expectedSwitches.match(new RegExp(',\\s*('+res[2]+'\\w*)\\s*,', 'i'));
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
