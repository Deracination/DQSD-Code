// Open a search window either with the default browser or with IE
function openSearchWindow(url)
{
  openNamedSearchWindow(url, getReuseBrowserWindowMode() ? DQSD_BROWSER_WINDOW_NAME : "_blank");
}

// Open a search window in an existing frame
function openNamedSearchWindow(url, name)
{
  if (useExternalBrowser && DQSDLauncher)
    DQSDLauncher.OpenDocument(url);
  else if (typeof windowOpenFeatures != "undefined") {
    window.open(url, name, windowOpenFeatures);
  } else {
    window.open(url, name);
  }
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

var submitcount = 0;

// Submit a form either with the default browser or with IE
function submitForm(form, dontChangeTarget)
{
  var oldtarget;
  var newtarget;

  //allow the Override
  if ( !dontChangeTarget )
  {
    // Here's a safeguard for forgetting to put a target in the FORM
    if ( (getReuseBrowserWindowMode() == 0) || !form.attributes["target"].nodeValue || (form.attributes["target"].nodeValue && form.attributes["target"].nodeValue == '') )
    {
      form.attributes["target"].nodeValue = '_blank';
    }

    // Reuse a single window if the user wants to and the target is _blank
    // Don't override targets with any other name because some searches may want
    // their own window.

    oldtarget = form.attributes["target"].nodeValue;
    newtarget = oldtarget;

    if ((getReuseBrowserWindowMode() > 0) && oldtarget && (oldtarget == '_blank'))
    {
      // 1=same window always; 2=new window for each search type
      newtarget = ((getReuseBrowserWindowMode() == 1) ? DQSD_BROWSER_WINDOW_NAME : (DQSD_BROWSER_WINDOW_NAME + '_' + form.name));
    }
    else
    {
      newtarget = DQSD_BROWSER_WINDOW_NAME + "_" + submitcount;
      submitcount = submitcount + 1;
    }
    form.attributes["target"].nodeValue = newtarget;
  }

  if (useExternalBrowser && DQSDLauncher)
    DQSDLauncher.SubmitForm(form);
  else
  {
    if (newtarget && pagetemplate)
    {
      var w = null;
      if (typeof windowOpenFeatures != "undefined") {
        w = window.open(url, name, windowOpenFeatures);
      } else {
        w = window.open(url, name);
      }
      w.history.back(1);
    }
    form.submit();
  }

  if (oldtarget)
    form.attributes["target"].nodeValue = oldtarget;
}

// Used by the calendar and the menu (or anything that uses IE window.createPopup method), which cannot call
// the stylesheet directly.
function convertStylesToInline()
{
  var stylestring = new String();
  for (var j = 0; j < document.styleSheets.length; j++)
  {
	var intermediateString = '';
	var rules = document.styleSheets[j].rules;
	var rulesLength = rules.length;
	
    for (var i = 0; i < rulesLength; i++)
    {
      intermediateString += rules[i].selectorText + ' {' + rules[i].style.cssText + '}'
    }
    
    stylestring += intermediateString;
  }
  return(stylestring);
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

// isURL
//
// Takes a string as a parameter and determines if it is a http URL (using either http or https protocol)
// If the string is not a URL, then it attempts to detect obvious dns names, and if so, it prepends http://
// to the string.  In either of the above to cases, the final string is returned, otherwise, 'false' is
// returned.
function isURL(t)
{
  // detect strings that look like URLs or filenames
  prot = new RegExp("^(http://|https://|ftp://)([\\-a-z0-9]+\\.)*[\\-a-z0-9]+" +
          "|^[a-z]:($|\\\\)" +
          "|^\\\\\\\\[a-z0-9]+($|\\\\($|[a-z0-9]+($|\\\\)))", "i");
  if (prot.exec(t))
    return t;

  // detect strings that look like DNS names
  dns = new RegExp("^www\.([\\-a-z0-9]+\\.)+[\\-a-z0-9]+(/\\S*)?$" +
          "|^([\\-a-z0-9]+\\.)+(com|net|org|edu|gov|mil|[a-z]{2})(/\\S*)?$", "i");
  if (dns.exec(t))
  {
    t = "http://" + t;
    return t;
  }
  return false;
}

// Used to parse standard switches (/foo or /foo:bar).  Takes the following parameters:
//    q - string from the search function
//    expectedSwitches - list or array of the expected switch values
//    expandSwitches - optional parameter used to determine if the switch shortcuts should be expanded (i.e. /f becomes /foo)
// Function returns an object with three properties
//    q - the input string with the switches removed
//    switches - array of objects with these two properties:
//           name:   expanded name of the matched switch (i.e. foo as in /foo:bar)
//            value:  value of switch (i.e. bar as in /foo:bar)
//    switch_val - associative array with the switch name as the key with the switch value as the value. (i.e. switch_val["foo"] = "bar" as in /foo:bar)
//
function parseArgs(q, expectedSwitches, expandSwitches)
{
  // In case the caller does not pass in a value
  if (typeof expandSwitches == 'undefined')
    expandSwitches = 1;

  // In case the caller uses a delimited (;,<space>) string
  if (typeof expectedSwitches[0] == 'undefined')
    expectedSwitches = expectedSwitches.split( /[,;/\s]/ );

  var switches = [];
  var switch_val = [];

  // Split the query to allow for easy looping.
  var args_array = q.split(' ');

  // Regular expression that defines switches
  var re_switch = /\/(([-.\w]+)(?::?(\S*)))\s*/;
  var re_res_args;
  var re_res_switch;

  // Loop through the q array and see if any of the q's look like switches
  for (var i = 0; i < args_array.length; i++)
  {
    // If a q looks like a switch, loop through the switch array and see if any of the switches match.
    re_res_args = args_array[i].match(re_switch);
    if (re_res_args)
    {
      for (var j = 0; j < expectedSwitches.length && !re_res_switch; j++)
      {
        var expect_regex = new RegExp(
            '^(' + re_res_args[2].replace('.', '\\.') +
             ')' + (expandSwitches ? '' : '$'), 'i');

        re_res_switch = expectedSwitches[j].match(expect_regex);

        //  If there is a match, adjust the args_array, and save the values.
        if (re_res_switch)
        {
          switch_val[expectedSwitches[j]] = re_res_args[3];
          switches.push( {name:expectedSwitches[j].toLowerCase(), value:re_res_args[3]} );
          args_array.splice(i, 1);
          i--;
        }
      }
      re_res_switch = "";
    }
  }
  q = args_array.join(' ');
  return { q:q, switches:switches, switch_val:switch_val };
}

/*
 * parseArgsEx is similar to parseArgs except that
 * it isn't split on spaces but instead the /
 * This means that args.q will be the value before the
 * first slash and the parameter value is everything after
 * the arg name until the next slash.  Also the arg name
 * and value are separated by a space not a : like parseArgs.
 * This will also allow you to have spaces in the argument value
 * as well.
 * [Brent Beardsley - 10/31/2002]
 */
function parseArgsEx(q, expectedSwitches, expandSwitches)
{
  // In case the caller does not pass in a value
  if (typeof expandSwitches == 'undefined')
    expandSwitches = 1;

  // In case the caller uses a delimited (;,<space>) string
  if (typeof expectedSwitches[0] == 'undefined')
    expectedSwitches = expectedSwitches.split( /[,;/\s]/ );

  var switches = [];
  var switch_val = [];
  var args_array = q.split('/');
  for (var i=0; i < args_array.length; i++)
  {
    var one_arg = trimWhitespace(args_array[i]);
    if (one_arg == "") 
    {
      continue;
    }
    var argRegExp = /(\S+)(\s+(.*))?/g;
    var reResult = argRegExp.exec(one_arg);
    if (reResult == null || reResult[1] == "") 
    {
      continue;
    }
    var argName = reResult[1];
    var argValue = trimWhitespace(reResult[2]);
    var re_res_switch;

    for (var j = 0; j < expectedSwitches.length && !re_res_switch; j++)
    {
      var expect_regex = new RegExp(
          '^(' + argName.replace('.', '\\.') +
           ')' + (expandSwitches ? '' : '$'), 'i');

      re_res_switch = expectedSwitches[j].match(expect_regex);

      //  If there is a match, adjust the args_array, and save the values.
      if (re_res_switch) 
      {
        argName = expectedSwitches[j];
        switch_val[argName] = argValue;
        switches.push( {name:argName.toLowerCase(), value:argValue} );
      }
    }
    re_res_switch = "";
  }

  if (q.match(/^\s*\//))
  {
    // if it starts with a slash there are no non-switch values
    q = "";
  } 
  else 
  {
    // only the part before the first slash is returned in args.q
    var firstSlash = q.indexOf('/');
    if (firstSlash != -1) 
    {
      q = q.substring(0, firstSlash);
    }
    q = trimWhitespace(q);
  }
  return { q:q, switches:switches, switch_val:switch_val };
}


function trimWhitespace( s )
{
  return s.replace(/^\s+/, "").replace(/\s+$/, "");
}


// nullArgs (Neel Doshi - 04/09/2002)
//
// Many functions will open the <link> url when the query is null or the description if the query is "?".  This function automates this process.
//    func - string name of the function
//    q - string from the search function
// The function returns false if the query was not null or "?" and true otherwise
//
function nullArgs(func, q)
{
  if (q == "")
  {
    if (searches[func].link)
    {
      openSearchWindow(searches[func].link);
      return true;
    }
  }
  if (q == "?" || q == "")
  {
    if (searches[func].desc)
    {

      if ((typeof qsfind) == 'function')
      {
        // If QSFind exists use that to display the help
        var qsarg = func + " /function";
        qsfind(qsarg);
      }
      else
      {
        // Otherwise use an alert box
        var search_desc = searches[func].desc;

        // convery <br> tags to newlines
        var re = new RegExp("(<br)( )?(\/>)", "g");
        search_desc = search_desc.replace(re, "\n");

        // Replace tabs and multiple spaces in the description with one space
        var re = new RegExp("((\\t)|( ))+", "g");
        search_desc = search_desc.replace(re, " ");

        // Replace newline space with nothing
        var re = new RegExp("\n( )", "g");
        search_desc = search_desc.replace(re, "");

        // Remove any more HTML tags
        var re = new RegExp("<(\/)?\\w+( )?[\\w\"\=\ ]*(\/)?>", "g");
        var search_desc = search_desc.replace(re, "");
        alert(search_desc);
      }
      return true;
    }
  }
  else
    return false;
}

function getReuseBrowserWindowMode()
{
  return reuseBrowserWindowModeOverride;
}

function displayPopupMessage( msg, headHTML )
{
  var dqsdMessagePopup = window.createPopup();
  var windowW = 350;
  var dqsdMessagePopup = window.createPopup();
  var dqsdMessagePopupBody = dqsdMessagePopup.document.body;
  var dqsdMessageBodyCode = "<html><head>" + (typeof headHTML == 'undefined' ? '' : headHTML ) + "</head><body>";
  dqsdMessageBodyCode += "<table id=qstable border=0 cellspacing=1 cellpadding=2 width=100% height=100%>";
  dqsdMessageBodyCode += "<tr><td valign=top><style>" + convertStylesToInline() + "</style>";
  dqsdMessageBodyCode += "<tr><td valign=top style='text-align: center' class=helpboxDescriptions>" + msg;
  dqsdMessageBodyCode += "</tr></td></table></body></html>";
  dqsdMessagePopupBody.innerHTML = dqsdMessageBodyCode;
  dqsdMessagePopup.document.body.style.border="outset 2px";
  dqsdMessagePopup.document.body.style.background='menu';
  dqsdMessagePopup.document.body.style.overflowY='auto';

  // Temporatily show the popup to determine the proper final
  // height for the popup.
  dqsdMessagePopup.show(0, 0, windowW, 0);
  var windowH = dqsdMessagePopupBody.scrollHeight + 6;
  dqsdMessagePopup.hide();

  // Put a cap on the popup height
  windowH = windowH > window.screen.height-100 ? window.screen.height-100 : windowH;

  dqsdMessagePopup.show((buttonalign == "left" ? 0 : document.body.clientWidth - windowW), -windowH, windowW, windowH, document.body);
}

function setSearchWindowText(text, autoselect)
{
  document.deff.q.value = text;
  if (typeof autoselect != "undefined" && autoselect == true) {
    document.deff.q.createTextRange().select();
  }
}