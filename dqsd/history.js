// fancy UI: history and cut and paste (via arrow and control keys)

histarray = new Array(historylength + 1);
histedit = new Array(histarray.length);
histarray[0] = "";
histcount = 1;
histcurr = 0;
histend = 0;

restoreHistory();

function saveHistory()
{
  writeFile("history.txt", histarray.join('\n'));
}

function restoreHistory()
{
  var loaded = readFile( "history.txt" ).split('\n');
  for (var i = 1; i < histarray.length; i++)
  {
    if (!loaded[i] || loaded[i] == "")
      break;
    histarray[i] = loaded[i];
  }
  histcount = histend = i;
}

function historyItemExists(t)
{
  for (var i = histend-1; i >= 0; i--)
    if (t == histarray[i])
      return true;
  return false;
}

// add an item to the history
function addhist(t)
{
  // Don't add if already exists
  if (histend > 0 && historyItemExists(t))
    return;
  var i;
  for (i = 0; i < histcount; i++)
    histedit[i] = null;
  histarray[histend] = t;
  histend = histend + 1;
  if (histend >= histarray.length)
      histend = 0;
  histcurr = histend;
  histarray[histend] = "";
  if (histcount < histarray.length)
    histcount = histcount + 1;
    
  saveHistory();
}

// get the current history entry
function currhist()
{
  if (histedit[histcurr])
      return histedit[histcurr];
  return histarray[histcurr];
}

// advance the currently viewed history entry +1 or -1
function advhist(i)
{
  if (i > 0 && histcurr == histend) i = 0;
  if (i < 0 && (histcurr == histend + 1 || histend == histcount - 1 && histcurr == 0)) i = 0;

  if (document.deff.q.value != currhist())
    histedit[histcurr] = document.deff.q.value;

  histcurr = histcurr + i;
  if (histcurr >= histcount)
    histcurr = 0;
  if (histcurr < 0)
    histcurr = histcount - 1;
}


