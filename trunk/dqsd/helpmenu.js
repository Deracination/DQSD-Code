// code to take advantage of IE 5.5 popup window

function mi(name, func, sym)
{
  popheight += 18;
  qsym = sym;
  if (sym == "\\")
    qsym = "\\\\";
  return "<tr class=menuRow " +
    " onmouseover=\"this.className='menuRowHigh';\"" +
    " onmouseout=\"this.className='menuRow';\"" +
    " onmouseup=\"this.className='menuRow';parent.mnu('" + func + "', &quot;" + qsym + "&quot;);\"" +
     " style=\"" + actualmenustyle + "\">" +
    "<td nowrap class=menuCommands>" +
    name +
    "</td><td nowrap class=menuAlias>" + sym + "</td></tr>";
}

function dv()
{
  if (shrink) return "";
  popheight += 16;
  return "<tr><td colspan=2 class=menuSeparator><hr class=menuHR></td></tr>";
}

function buildpop()
{
  pophelp = window.createPopup();
  popheight = 5;
  popwidth = 240;
  shrink = (window.screen.height <= 700);
  var menucode = "<table cellpadding=0 cellspacing=0 width=100%><tr><td valign=top><style>" + convertStylesToInline() + "</style>" +
      "<table cellpadding=0 cellspacing=0 width=100%>" +
      mi("About This Toolbar", "about", "");
  var i;
  var div = false;

  //for (i = 0; i < menuarray.length; i++)
  for (i = menuarray.length - 1; i >= 0; i--)
  {
    var entry = menuarray[i];
    if (entry == "separator")
    {
       if (!div) // don't add consecutive separators
       {
         menucode += dv();
         div = true;
       }
    }
    else
    {
       var search = searches[aliases[entry]];
       if (search)
       {
         menucode += mi(search.name, search.fname, entry);
         div = false;
       }
    }
  }
  menucode += "</table></td><tr></table>";

  pophelp.document.body.innerHTML = menucode;
  pophelp.document.body.style.border="outset 2px";
  pophelp.document.body.style.background='menu';
}

pophelp = null;
pophelpisopen=false;

function watchpopup(popvarname)
{
  var isopen = eval(popvarname + "isopen = " + popvarname + ".isOpen");
  if (isopen)
  {
    window.setTimeout("watchpopup('" + popvarname + "');", 100);
  }
}

function showpop()
{
  if (!dopopup)
    return false;

  if (window.event.button == 2)
    return false;

  if (!pophelp)
    buildpop();

  window.event.cancelBubble = true;
  if (pophelpisopen)
  {
    pophelp.hide();
    return false;
  }
  else
  {
    pophelp.show((buttonalign == "left" ? 0 : document.body.clientWidth - popwidth),
                 -popheight, popwidth, popheight, document.body);

    window.setTimeout("queuefocus();", 0);
    watchpopup("pophelp");
    return false;
  }
}

function queuefocus()
{
  document.deff.q.focus();
}

