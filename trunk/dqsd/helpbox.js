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

    for (i = 0; i < 10; i++)
    {
      txt = document.all.tags("!").item(i).text.slice(4, -3);
      if (txt.match(/Copyright/)) break;
    }
    if (i >= 10)
    {
      txt = "Cannot find documentation.";
    }

    for (i = 0; i < helparray.length; i++)
    {
      var search = helparray[i];
      helptable += "<tr><td valign=top class=lb>";
      for (var j = 0; j < search.aliases.length; j++)
      {
        helptable += search.aliases[j].replace(/&/g, "&amp;").replace(/</g, "&lt;");
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

    txt = txt.replace(/\r\n/g, '\n');
    txt = txt.replace(/\n\*/g, '<li>');
    txt = txt.replace(/\nVer/, '</b><br>Ver');
    txt = txt.replace(/\nCop/, '<p>Cop');
    txt = txt.replace(/\nDis/, '<br>Dis');
    txt = txt.replace(/(David Bau)/, '<a href=mailto:davidbau@hotmail.com>$1</a>');
    txt = txt.replace(/(GNU.*2)\s\((.*txt)\)/, '<br><a href=$2 target=GNU>$1</a>');
    txt = txt.replace(/\n#[^\n]*/g, '');
    txt = txt.replace(/<\/table>/, helptable + "</table>");
    txt = txt.replace(/\n\n/g, '<p>');
    txt = txt.replace(/\n/g, ' ');
    txt = txt.replace(/----/, '</center>');
    var basedir = window.location.href;
    basedir = basedir.substring(0, basedir.lastIndexOf('/') + 1);
    txt = txt.replace(/(view-source:)/g, '$1' + basedir);
    var mesg = "<title>About Dave's Quick Search Deskbar</title>";
    mesg += "<style>body{margin:20px;border:0;padding:0;background-color:threedface;font-family:Tahoma,Arial;scrollbar-track-color:threedface}" +
           "td{font-size:8pt;border-collapse:collapse} .lg{background:lightgreen;} .lb{background:skyblue;}</style>";
    mesg += "<body scroll=yes><table height=100% width=100%><tr><td><center style=font-size:9pt><b>";
    mesg += txt + "</td></tr></table>";
    opts = "height=480, width=420, menubar=no, scrollbars=yes, resizable=yes, toolbar=no, status=no";
    aboutWindow = window.open("about:blank", "_blank", opts);
    aboutWindow.document.write(mesg);
    aboutWindow.document.close();
  }
  aboutWindow.focus();
}
