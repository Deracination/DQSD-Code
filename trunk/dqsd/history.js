// fancy UI: history and cut and paste (via arrow and control keys)

histarray = new Array(historylength);
histedit = new Array(histarray.length);
histarray[0] = "";
histcount = 1;
histcurr = 0;
histend = 0;

// add an item to the history
function addhist(t)
{
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


