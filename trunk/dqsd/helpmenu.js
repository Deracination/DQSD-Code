// code to take advantage of IE 5.5 popup window

function mi(name, func, sym)
{
  popheight += 18;
  qsym = sym;
  if (sym == "\\")
    qsym = "\\\\";

  return "<tr " +
    "onmouseover=\"this.style.background='highlight'; this.style.color='highlighttext';\"" +
    "onmouseout=\"this.style.background='menu'; this.style.color='menutext';\"" +
    "onmouseup=\"this.style.background='menu'; this.style.color='menutext';parent.mnu('" + func + "', &quot;" + qsym + "&quot;);\"" +
    "style=\"" + actualmenustyle + "\">" +
    "<td nowrap style=padding-left:20px;>" +
    name +
    "</td><td nowrap style=padding-left:10px;padding-right:10px;>" + sym + "</td></tr>";
}

function dv()
{
  if (shrink) return "";
  popheight += 10;
  return "<tr><td colspan=2 style='height:10px;padding-left:2px;padding-right:2px'><hr style='position:absolute;top=" + (popheight-10) + "px;'></td></tr>";
}

function buildpop()
{
  pophelp = window.createPopup();
  popheight = 4;
  popwidth = 240;
  shrink = (window.screen.height <= 700);
  var menucode = "<table cellpadding=0 cellspacing=0 width=100%><tr><td valign=top>" +
      "<table cellpadding=0 cellspacing=0 width=100%>" +
      mi("About This Toolbar", "about", "");
  var i;
  for (i = menuarray.length - 1; i >= 0; i--)
  {
    var entry = menuarray[i];
    if (entry == "separator")
    {
       menucode += dv();
    }
    else
    {
       var search = searches[aliases[entry]];
       menucode += mi(search.name, search.fname, entry);
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
    var rows = pophelp.document.all.tags("TR");
    var i;
    for (i = 0; i < rows.length; i++)
    {
       rows[i].style.background="menu";
       rows[i].style.color="menutext";
    }
    pophelp.show(document.body.clientWidth - popwidth, -popheight, popwidth, popheight, document.body);
    window.setTimeout("queuefocus();", 0);
    watchpopup("pophelp");
    return false;
  }
}

function queuefocus()
{
  document.deff.q.focus();
}

