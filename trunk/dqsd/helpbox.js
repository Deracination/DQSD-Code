// code to handle the about/help dialog

aboutWindow = null;

// the help dialog box
function closeabout()
{
  var unopened = true;
  try
  {
    unopened = (!aboutWindow || aboutWindow.closed);
  }
  catch (exception) {}

  if (!unopened)
  {
    aboutWindow.close();
    unopened = true;
  }
}


function about()
{
  if (window.event)
    window.event.cancelBubble = true;

  closeabout();

  {
    var i;
    var txt;
    var helptable = "";
    var jumplist = "<center>";

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
      helptable += "<tr><td valign=top class=lb colspan=2><table width='100%' border=0 cellpadding=0 cellspacing=0><tr><td><span id='categoryExpander' category='" + categoryarray[i] + "' class='categoryExpander'></span><b>"
      helptable += categoryarray[i];
      helptable += "</b> <span id='categoryCount'></span><a name=\""+categoryarray[i]+"\"></td><td align=right class='top'>[ <a href='#top'>top</a> ]</td></tr></table></td></tr>";
      var helparray = categories[categoryarray[i]];
      for (var k = 0; k < helparray.length; k++)
      {
        var search = helparray[k];
        helptable += "<tr id='" + categoryarray[i] + "'><td valign=top class=lb>";
        for (var j = 0; j < search.aliases.length; j++)
        {
          var alias = search.aliases[j].replace(/&/g, "&amp;").replace(/</g, "&lt;");
          if (alias.substr(0,INTERNAL_FUNC_PREFIX.length) == INTERNAL_FUNC_PREFIX)
            continue;
          if (alias == "")
            alias = "<em>Enter</em>";
          helptable += alias;
          if (j < search.aliases.length)
            helptable += "<br>"
        }
        helptable += "</td><td valign=top class=lg>";
        if (search.link)
          helptable += "<a target=info href=\"" + search.link + "\">" + search.name + "</a>";
        else
          helptable += search.name;
        if (search.desc)
        {
          helptable += " - " + search.desc;
        }
        helptable += "</td></tr>";
      }
      jumplist += (i > 0 ? " | " : "") + "<a href=\"\#"+categoryarray[i]+"\">"+ categoryarray[i]+"</a>";
    }
    jumplist += "</center>";
    jumplist += "<div align='center' style='margin: 5px 0px 5px 0px;'>[ <span class='categoryExpander' type='expandall'>expand all</span> ]  [ <span class='categoryExpander' type='collapseall'>collapse all</span> ]</div>";

    txt = txt.replace(/\r\n/g, '\n');
    txt = txt.replace(/\n\*/g, '<li>');
    txt = txt.replace(/\nVer/, '</b><br>Ver');
    txt = txt.replace(/\nCop/, '<p>Cop');
    txt = txt.replace(/\nDis/, '<br>Dis');
    txt = txt.replace(/(David Bau)/, '<a href=mailto:davidbau@hotmail.com>$1</a>');
    txt = txt.replace(/(GNU.*2)\s\((.*txt)\)/, '<br><a href=$2 target=GNU>$1</a>');
    txt = txt.replace(/\n#[^\n]*/g, '');
    txt = txt.replace(/<table/, jumplist + "<table width='100%'");

	// dollar signs are match variables for replace, so any literal dollar signs need to be
	// converted to HTML code so they will display properly
    helptable = helptable.replace(/\$/g, "&#036;");
    txt = txt.replace(/<\/table>/, helptable + "</table>");

    txt = txt.replace(/\n\n/g, '<p>');
    txt = txt.replace(/\n/g, ' ');
    txt = txt.replace(/----/, '</center>');
    txt +=jumplist;
    
    
    var basedir = window.location.href;
    basedir = basedir.substring(0, basedir.lastIndexOf('/') + 1);
    txt = txt.replace(/(view-source:)/g, '$1' + basedir);
    var mesg = "<title>About Dave's Quick Search Deskbar</title>";
    mesg += "<head><style>body{margin:20px;border:0;padding:0;background-color:threedface;font-family:Tahoma,Arial;scrollbar-track-color:threedface}" +
           "td{font-size:8pt;border-collapse:collapse} TD.lg{background:lightgreen;width:99%} .lb{background:skyblue;} .top{font-size:60%} SPAN.categoryExpander { font-size: 80%; behavior: url(categoryExpander.htc); } #categoryCount { font-size: 80% }</style>"
    if (typeof helpstyle == 'undefined')
      helpstyle = '';
    mesg += "<style>TD {" + helpstyle + "}</style></head>";
    mesg += "<body scroll=yes><a name='#top'><table height=100% width=100%><tr><td><center><b>";
    mesg += txt + "</td></tr></table>";
    opts = "height=480, width=441, menubar=no, scrollbars=yes, resizable=yes, toolbar=no, status=no";
    if (typeof helpoptions != 'undefined' && helpoptions != "")
      opts += ", " + helpoptions;
    aboutWindow = window.open("about:blank", "_blank", opts);
    aboutWindow.document.write(mesg);
    aboutWindow.document.close();
  }
  aboutWindow.focus();
}
