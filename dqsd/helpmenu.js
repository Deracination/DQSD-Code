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
    " >" +
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

function saveMenuHistory( alias )
{
  var menuContent = null;
  try
  {
    menuContent = readFile( "mrumenu.txt" );
  }
  catch (e) {}
  var menus = menuContent ? menuContent.replace(/\r\n/g, '\n').split('\n') : new Array(0);
  
  var exists = false;
  var newmenus = new Array(0);
  for ( var i = 0; i < menus.length; i++ )
  {
    if ( menus[i].toLowerCase() != alias )
      newmenus.push( menus[i] );
    else
      exists = true;
  }
  if ( exists )
    menus = (alias + '\n' + newmenus.join('\n')).split('\n');
  
  menus.push( alias );
  if ( menus.length > menuMRUlength )
    menus.splice( 0, menus.length - menuMRUlength );
  writeFile( "mrumenu.txt", menus.join('\r\n') );
}

function appendMRUMenuSelections( mb )
{
  var menuContent = null;
  try
  {
    menuContent = readFile( "mrumenu.txt" );
  }
  catch (e) {}
  if ( menuContent )
  {
    mb.AppendSeparator( 0 );
    var menus = menuContent.replace(/\r\n/g, '\n').split('\n');
    for ( var i = 0; i < menus.length; i++ )
    {
      for ( var iSearch in searches )
      {
        var search = searches[iSearch];
        if ( search.aliases )
        for ( var iAlias = 0; iAlias < search.aliases.length; iAlias++ )
        {
          if ( menus[i] == search.aliases[iAlias] )
          {
            var alias = getSearchAliases( search );
            mb.AppendMenuItem( "&" + (menus.length - i) + "  " + search.name + '\t' + alias, search.aliases[0], 0 );
            break;
          }
        }
      }
    }
  }
  
}

function showpop()
{
  document.deff.q.focus();
  document.deff.q.createTextRange().select();
  var mb = new ActiveXObject("DQSDTools.MenuBuilder");
  mb.AppendMenuItem( "About DQSD...", "about", 0 );
  mb.AppendSeparator( 0 );
  for (i = 0; i < categoryarray.length; i++)
  {
    var hsubmenu = mb.AppendSubMenu( categoryarray[i] );
    var helparray = categories[categoryarray[i]];
    helparray.sort( searchCompare );
    for (var k = 0; k < helparray.length; k++)
    {
      var search = helparray[k];
      var alias = getSearchAliases( search );
      mb.AppendMenuItem( search.name + '\t' + alias, search.aliases[0], hsubmenu );
    }
  }
  
  appendMRUMenuSelections( mb );

  var fn = mb.Display();
  if ( fn )
  {
    var alias = null;
    if ( isInternalSearch( fn ) )
      alias = searches[fn].aliases[1]
    mnu( fn, alias );
    if ( fn != 'about' )
      saveMenuHistory( alias ? alias : fn );
  }
  return false;
}

function queuefocus()
{
  document.deff.q.focus();
}

