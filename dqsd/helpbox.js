// code to handle the about/help dialog

aboutWindow = null;

// the help dialog box
function closeabout()
{
  var unopened = true;
  try
  {
    unopened = (!aboutWindow || aboutWindow.closed);

    if (!unopened)
    {
      aboutWindow.close();
      unopened = true;
    }
  }
  catch (exception) {}
}

function searchCompare( s1, s2 )
{
  // by default, sort by first alias
  var key1 = getFirstAlias( s1 ).toLowerCase();
  var key2 = getFirstAlias( s2 ).toLowerCase();
  switch ( helpSortKey.toLowerCase() )
  {
  case 'name':
    key1 = s1.name.toLowerCase().replace( /<[^<]*>/g, '' );
    key2 = s2.name.toLowerCase().replace( /<[^<]*>/g, '' );
    break;
  }
  if ( key1 < key2 ) return -1;
  if ( key1 > key2 ) return 1;

  return 0;
}


function about()
{
  if (window.event)
    window.event.cancelBubble = true;

  closeabout();

  // WGD - I've moved this lot up to the top of the function, so that there's some quick action
  // when you click on the help button
  opts = "height=480, width=441, menubar=no, scrollbars=yes, resizable=yes, toolbar=no, status=no";
  if (typeof helpoptions != 'undefined' && helpoptions != "")
    opts += ", " + helpoptions;

  aboutWindow = window.open("about:blank", "_blank", opts);
  aboutWindow.document.write("<body bgcolor=threedface style=border:0><table height=90% width=100%><tr><td><center style=font:menu>Generating Help...</td></tr></table>");
 
  {
    var i;
    var txt;
    var helptable = "";
    helptable += "<tr><td align=center colspan=50><table><tr class='localSearch'><td class='helpboxCommands' style='width: 12px;'></td><td>&nbsp;- indicates a local search or alias</td></tr></table></td></tr>\n";

    for (i = 0; i < 10; i++)
    {
      txt = document.all.tags("!").item(i).text.slice(4, -3);
      if (txt.match(/Copyright/)) break;
    }
    if (i >= 10)
    {
      txt = "Cannot find documentation.";
    }

    for (i = 0; i < categoryarray.length; i++)
    {
      var helparray = categories[categoryarray[i]];
      helparray.sort( searchCompare );
    
      // Are there any enabled searches in this category?
      for (var k = 0; k < helparray.length && (!helparray[k].enabled || helparray[k].nomenu); k++)
      {
      }

      // If all the searches are disabled in the category, don't display this category
      if ( k == helparray.length )
        continue;

      var categoryText = '';
      categoryText += "<tr><td class=helpboxCategoryExpanderRow colspan=3>\n<table width='100%' border=0 cellpadding=0 cellspacing=0>\n<tr><td class=helpboxCategory><span id='categoryExpander' category='" + categoryarray[i] + "' class='helpboxCategoryExpanderSign'></span>\n"
      categoryText += "&nbsp;" + categoryarray[i] + "&nbsp;";
      categoryText += "<span id='categoryCount' class=helpboxCategoryCount /><a name=\""+categoryarray[i]+"\"></a></td><td align=right class=helpboxCategoryTop>[ <a class=helpboxLinkTop href='#top'>top</a> ]</td></tr></table></td></tr>";
      for (var k = 0; k < helparray.length; k++)
      {
        var search = helparray[k];
        var rowclassname = '';
        var sortText = '';
        
        if (search.nomenu) {
          continue;
        }

        if (search.local)
        {
          rowclassname = 'class="localSearch"';
        }

        var checked = '';
        if (search.enabled)
        {
          checked = 'checked';
        }
        else
        {
          rowclassname = 'class="disabledSearch" ';
        }
        sortText += "<tr " + rowclassname + "id='" + categoryarray[i] + "'><td class=helpboxCommands>";

        for (var j = 0; j < search.aliases.length; j++)
        {
          var alias = search.aliases[j].replace(/&/g, "&amp;").replace(/</g, "&lt;");
          if (isInternalSearch(alias))
            continue;
          if (alias == "")
            alias = "<em>Enter</em>";
          if (j > 0)
            sortText += "<a class='helpboxAlias'>";
          sortText += alias;
          if (j > 0)
            sortText += "</a>";
          if (j < search.aliases.length)
            sortText += "<br />"
        }

        sortText += "</td><td class=helpboxToggle><input title='Enable/disable search' name='" + search.fname + "' id='search_enabled' type='checkbox' onclick='toggleEnabledSearch()' " + checked + " /></td><td class='helpboxDescriptions'>";
        if (search.link)
          sortText += "<a target='info' class='helpboxLink' href=\"" + search.link + "\">" + search.name + "</a>";
        else
          sortText += "<a class='helpboxSearchName'>" + search.name + "</a>";
        if (search.desc)
        {
          sortText += " - " + search.desc;
        }
        sortText += "</td></tr>\n";
        categoryText += sortText;
      }
      helptable += categoryText;
    }

    sVersion = '';
    sVersionDate = '';
    // Try to read the version information several times, in case there's a problem
    for ( var iGetVersion = 0; (iGetVersion < 5) && (typeof sVersion == 'undefined' || sVersion == null || sVersion == ''); iGetVersion++ )
    {
      try {
        var oVersion = getVersionFromVersionFile( "version.xml" );
        sVersion = new Array( oVersion.majorHi, oVersion.majorLo, oVersion.minorHi, oVersion.minorLo ).join('.');
        sVersionDate = oVersion.date;
      } catch(e) {}
    }

    txt = txt.replace(/\r\n/g, '\n');
    txt = txt.replace(/%lv/g, sVersion );
    txt = txt.replace(/%ld/g, sVersionDate );
    txt = txt.replace(/\n\*/g, '<li />');
    txt = txt.replace(/\nVer/, '</b><br />Ver');
    txt = txt.replace(/\nCop/, '<p />Cop');
    txt = txt.replace(/\nDis/, '<br />Dis');
    txt = txt.replace(/(David Bau)/, '<a href="mailto:davidbau@hotmail.com">$1</a>');
    txt = txt.replace(/(GNU.*2)\s\((.*txt)\)/, '<br /><a href="$2" target=GNU>$1</a>');
    txt = txt.replace(/\n#[^\n]*/g, '');
    txt = txt.replace(/<table/, "<div align='center' style='margin: 0px 0px 5px 0px;'>[ <span class='helpboxCategoryExpander' type='expandall'>expand all</span> ]  [ <span class='helpboxCategoryExpander' type='collapseall'>collapse all</span> ]</div><table width='100%' border=0 cellspacing=1 cellpadding=2");
    txt = txt.replace(/----/, '</center>');
    
    var basedir = window.location.href;
    basedir = basedir.substring(0, basedir.lastIndexOf('/') + 1);
    txt = txt.replace(/(view-source:|file:)/g, '$1' + basedir);
    txt = txt.replace(/file:file:/g, 'file:');
 
    // dollar signs are match variables for replace, so any literal dollar signs need to be
    // converted to HTML code so they will display properly
    helptable = helptable.replace(/\$/g, "&#036;");

    txt = txt.replace(/<\/table>/, helptable + "</table>");

    txt = txt.replace(/\n\n/g, '<p />\n');
// WGD I've removed this because I don't think it's relevant - the browser will deal quite happily with \n's
    txt = txt.replace(/\n/g, ' ');

    var mesg = "<!DOCTYPE HTML PUBLIC '-//W3C//DTD HTML 4.0 Transitional//EN'>\n<html><head><title>About Dave's Quick Search Deskbar</title>\n";

    mesg += "<link rel='stylesheet' type='text/css' href='search.css'>\n";
    // Add a call to the overwriting stylesheet if necessary
    if (typeof localstylesurl != "undefined" && localstylesurl != "")
      mesg += "<link rel='stylesheet' type='text/css' href='" + localstylesurl + "'>\n";
    mesg += "<script type='text/Jscript' src='savesearchsettings.js'></script>\n";
    mesg += "</head>\n"
    mesg += "<body scroll=yes onunload='saveSearchSettings()' class=helpboxBody>\n<a name='#top'></a>\n<table height='100%' width='100%' cellpadding=20>\n<tr><td class=helpbox><center><b>";

    try
    {
      // Remove the 'Please wait' message
      aboutWindow.document.write("");
      aboutWindow.document.close();

      // Display the help - this multiple write method is _significantly_ faster than catting the string
      // at this point, txt is > 100Kbytes, so string operations involving copying shouldn't be undertaken lightly
      aboutWindow.document.write(mesg);
      aboutWindow.document.write(txt);
      aboutWindow.document.write('</td></tr></table></body></html>');
      aboutWindow.document.close();
      aboutWindow.focus();
    }
    catch(e)
    {
    }
  }
}
