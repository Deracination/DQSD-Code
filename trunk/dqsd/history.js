// fancy UI: history and cut and paste (via arrow and control keys)

histarray = [];
histedit = new Array(historylength + 1);
histcurr = 0;

restoreHistory();

// save the history array
function saveHistory()
{
  try
  {
    writeFile("history.txt", histarray.join('\r\n'));
  }
  catch(e) { }
}

// restore at most historylength entries from the saved history
function restoreHistory()
{
  var historyFileContent = null;
  try
  {
    historyFileContent = readFile("history.txt");
  }
  catch (e) {}
  
  var loaded = null;
  if (historyFileContent)
    loaded = historyFileContent.replace(/\r\n/g, '\n').split('\n');
  else // history.txt probably doesn't exist
    loaded = new Array(0);

  for (var i = loaded.length - 1; i >= 0; i--)
  {
    if (loaded[i] && loaded[i] != "")
      histarray.push(loaded[i]);
    if (histarray.length >= historylength)
      break;
  }
  histarray.reverse();
  histcurr = histarray.length;
  histedit[histarray.length] = "";
}

// add an item to the history
function addhist(t)
{
  // See if it's already in the history so we can remove it
  var i;
  for (i = 0; i < histarray.length; i++)
    if (histarray[i] == t) break;

  // Not there, but history is maxed out: remove the 0th.
  if (i == histarray.length && histarray.length == historylength)
    i = 0;
    
  // Shift down all the entries after the one to be removed
  for (; i < histarray.length - 1; i++)
    histarray[i] = histarray[i + 1];

  // Store the new item in the history
  histarray[i] = t;
    
  // Save it baby
  saveHistory();

  // History edits get cleared whenever something new goes in
  clearhistedits();
  histcurr = i + 1;
}

// get the current history entry
function currhistedit()
{
  if (histedit[histcurr] != null)
    return histedit[histcurr];
  if (histcurr >= histarray.length)
    return (histcurr > 0 ? histarray[histarray.length - 1] : "");
  return histarray[histcurr];
}

// clear the history edits
function clearhistedits()
{
  for (var i = 0; i < histedit.length; i++)
  {
    histedit[i] = null;
  }
}

// advance the currently viewed history entry +1 or -1
function histeditmove(t, i)
{
  if (t != currhistedit())
    histedit[histcurr] = t;

  // special case: first ctrl-p and no edits were done
  if (i == -1 && histcurr == histarray.length && histedit[histcurr] == null)
  {
    histedit[histcurr] = "";
    if (histcurr > 0) histcurr--;
  }
    
  // no movement
  if (i == 0) return;

  // filter when somebody has typed !prefix then ctrl-p
  var scan = histcurr + i;
  if (scan > histarray.length || scan < 0) return;

  var filter = histedit[histarray.length];
  if (filter != null && filter.match(/^!\S/) && filter != "!!")
    filter = "\\b" + filter.substring(1);
  else if (filter != null && filter.match(/\S!$/) && filter != "!!")
    filter = "\\b" + filter.substring(0,filter.length - 1);
  else filter = "";

  while (scan < histarray.length && !histarray[scan].match(filter))
  {
    scan += i;
    if (scan < 0) return;
  }

  histcurr = scan;
}

function histsearch( t, shift )
{
  if ( searchPrefix == '' )
    searchPrefix = t;

  // Escape every letter in the searchPrefix, because it might contain chars with special meaning in a regexp
  var escapedString = escapeString( searchPrefix );

  var re;
  try
  {
    re = new RegExp( escapedString, "i" );
  }
  catch(e)
  {
    alert("An error (" + e.description + ") occurred during the history search");
    return;
  }
  
  if (!shift) // Search back through history
  {
    for ( var i = histcurr - 1; i >= 0; i-- )
    {
      if ( histarray[i].match( re ) )
      {
        histcurr = i;
        document.deff.q.value = currhistedit();
        return;
      }
    }

    for ( var i = histarray.length - 1; i >= histcurr; i-- )
    {
      if ( histarray[i].match( re ) )
      {
        histcurr = i;
        document.deff.q.value = currhistedit();
        return;
      }
    }
  }
  else // Search forward through history
  {
    for ( var i = histcurr + 1; i < histarray.length; i++ )
    {
      if ( histarray[i].match( re ) )
      {
        histcurr = i;
        document.deff.q.value = currhistedit();
        return;
      }
    }

    for ( var i = 0; i <= histcurr; i++ )
    {
      if ( histarray[i].match( re ) )
      {
        histcurr = i;
        document.deff.q.value = currhistedit();
        return;
      }
    }
  }
}


// recent changes a !foo textbox into whatever it means
// it returns false if there is no match

function recent()
{
  var t = escapeString( document.deff.q.value );

  if (!t.match(/^!\S/)) return true;

  if (t == "!!")
  {
    histcurr = histarray.length;
    histeditmove(t, -1);
    if (histcurr == histarray.length) return false;
  }
  else
  {
    // this bit of code searches backwards for !foo
 
    // set the filter then behave like a ctrl-p
    histcurr = histarray.length;
    histeditmove(t, -1);

    // return true if no match
    if (histcurr == histarray.length) return false;
  }

  document.deff.q.value = currhistedit();
  return true;
}

function escapeString( s )
{
  if ( !s.length )
    return s;
  var es = '';
  for (var i = 0; i < s.length; i++)
  {
    es += "\\x" + s.charCodeAt(i).toString(16);
  }
  return es;
}
