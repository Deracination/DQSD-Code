// fancy UI: history and cut and paste (via arrow and control keys)

histarray = [];
histedit = new Array(historylength + 1);
histcurr = 0;

restoreHistory();

// save the history array
function saveHistory()
{
  writeFile("history.txt", histarray.join('\r\n'));
}

// restore at most historylength entries from the saved history
function restoreHistory()
{
  var loaded = readFile("history.txt").split('\r\n');
  for (var i = (loaded.length < historylength ? 0 : historylength - loaded.length);
           i < loaded.length; i++)
  {
    if (loaded[i] && loaded[i] != "")
      histarray.push(loaded[i]);
  }
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
  histedit[histcurr] = t;
}

// get the current history entry
function currhistedit()
{
  if (histedit[histcurr])
      return histedit[histcurr];
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
    
  if (i == 0) return;
  if (i > 0 && histcurr >= histarray.length) return;
  if (i < 0 && histcurr <= 0) return;

  histcurr = histcurr + i;
}


