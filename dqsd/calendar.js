/**
 * script to display a popup calendar
 * added by Sidney Chong (sidney@gamebox.net) 12/12/2001
 * modified by Sidney Chong (sidney@gamebox.net) 15/01/2002
 *  - added display of events (eg holidays) specified
 *    in a file referenced by the var 'eventsfileurl'
 * modified by Sidney Chong (sidney@gamebox.net) 16/01/2002
 *  - changed event month to run from 1-12 instead of 0-11
 *  - assume event date to be recurring if month or year attribute
 *    is not specified.
 *
 * $Revision$ 
 * $Date$
 *
 * TODO:
 *  - add listing of public holidays in other countries
 *  - add ability to define personal events
 *  - add ability to distingush b/w different kind of events
 *  - add ability to input events directly from search box
 */

var ents = null;

function loadeventslist()
{
  try
  {
    document.write("<xml id='eventslist' src='" + eventsfileurl + "'></xml>");
    ents = document.all("eventslist").selectSingleNode("events");
  }
  catch (ex) {}
}

loadeventslist();

function yhocal(dat)
{
  // adjust for the local time zone
  dat = dat - (new Date()).getTimezoneOffset() * 1000 * 60;

  // round down to closest day (in minutes)
  dat = Math.floor(dat / 60 / 60 / 24 / 1000) * 60 * 60 * 24;
  openNamedSearchWindow("http://calendar.yahoo.com/srt:0/?v=0&t=" + dat, "dqsdcal");
}

function msncal(dat)
{
  var dobj = new Date(dat);
  var yy = dobj.getYear();
  if (yy < 100) yy += 1900;
  openNamedSearchWindow("http://calendar.msn.com/calendar/isapi.dll?calendarView=day&day=" + dobj.getDate() + "&month=" + (dobj.getMonth()+1) + "&year=" + yy, "dqsdcal");
}

function nsccal(dat)
{
  var dobj = new Date(dat);
  var yy = dobj.getYear();
  if (yy < 100) yy += 1900;
  openNamedSearchWindow("http://calendar.netscape.com/cgi-bin/gx.cgi/AppLogic+DayGridView?_Date=" + (dobj.getMonth() + 1) + "/" + dobj.getDate() + "/" + yy, "dqsdcal");
}

function aolcal(dat)
{
  var dobj = new Date(dat);
  var yy = dobj.getYear();
  if (yy < 100) yy += 1900;
  openNamedSearchWindow("http://calendar.aol.com/cgi-bin/gx.cgi/AppLogic+DayGridView?_Date=" + (dobj.getMonth() + 1) + "/" + dobj.getDate() + "/" + yy, "dqsdcal");
}

function mycal(dat)
{
  var dobj = new Date(dat);
  var yy = dobj.getYear();
  if (yy < 100) yy += 1900;
  openNamedSearchWindow("http://www.mycalendar.com/?act=thisday&curdate=" + (dobj.getMonth() + 1) + "/" + dobj.getDate() + "/" + yy, "dqsdcal");
}


function evcal(dat)
{
  var dobj = new Date(dat);
  var yy = dobj.getYear();
  if (yy < 100) yy += 1900;
  openNamedSearchWindow("http://www.evite.com/welcome?date=" + (dobj.getMonth() + 1) + "%2F" + dobj.getDate() + "%2F" + yy, "dqsdcal");
}




//vars used by the calendar script
var DAYS_OF_WEEK = 7;    // "constant" for number of days in a week
var DAYS_OF_MONTH = 31;    // "constant" for number of days in a month

var Calendar = new Date(); //stores the date the user is looking at
var Today = new Date(); // stores the date marked as today
var popcal = null;

function showcal()
{

  if (!dopopup || !cal)
    return true;

  if (popcal == null)
  {
    popcal = window.createPopup();
    popcal.document.body.innerHTML = buildcal();
    popcal.document.body.style.border="outset 2px";
    popcal.document.body.style.background=calbackground;
    popcal.document.body.style.color=caltextcolor;
  }
  else if (Today.getDate() != (new Date()).getDate())
  {
    popcal.document.body.innerHTML = buildcal();
  }

  popcal.show(document.body.clientWidth - calwidth, -calheight, calwidth, calheight, document.body);
  return false;
}

calupdatetimer = null;

function movecal(nMths)
{
  Calendar.setMonth(Calendar.getMonth()+nMths);
  popcal.document.all["calhead"].innerHTML = buildcalheader();
  if (calupdatetimer == null)
    calupdatetimer = setTimeout("updatecal()", 0);
}

function updatecal()
{
  calupdatetimer = null;
  popcal.document.body.innerHTML = buildcal();
}

function opencal(d)
{
  if (defaultcal)
  {
    if (popcal)
      popcal.hide();
    eval(defaultcal + '(' + d + ')');
  }
}

function buildcalheader()
{
  return '<CENTER>' + getMonthName(Calendar.getMonth()+1)  + ' ' + Calendar.getFullYear() + '</CENTER>';
}


function buildcal()
{
  var year = Calendar.getFullYear();  // Returns year in 4 digits
  var month = Calendar.getMonth();    // Returns month (0-11)

  Today = new Date();
  var weekday = -1;

  if (Today.getMonth() == Calendar.getMonth() && Today.getYear() == Calendar.getYear())
    weekday = Today.getDay();

  var cal;    // Used for printing

  Calendar.setDate(1);    // Start the calendar day at '1'


  var TR_start = '<TR style=font:menu>';
  var TR_end = '</TR>';
  var day_start = '<TD width=14.2857% style="' + caldowstyle + '"><CENTER>';
  var day_end = '</CENTER></TD>';
  function eventday_start(d, ttl)
  {
    return '<TD style="cursor:default;background-color:' + caleventdaybackground + ';' + caleventdaystyle + '" ' +
           (defaultcal ? ' onmouseover=this.style.background="' + calhighbackground +
               '";this.style.color="' + calhightext +
               '"; onmouseout=this.style.background="' + caleventdaybackground +
               '";this.style.color="' + caleventdaytext +
               '" onmouseup=parent.opencal(' + d + ');' +
               'this.style.background="' + caleventdaybackground +
               '";this.style.color="' + caleventdaytext + '"' : '') +
               ' title="' + ttl + '"' +
           '><CENTER>';
  }
  var eventday_end   = '</CENTER></TD>';
  function todayevent_start(d, ttl)
  {
    return '<TD style="cursor:default;background-color:' + caltodayeventbackground + ';' + caltodayeventstyle + '" ' +
           (defaultcal ? ' onmouseover=this.style.background="' + calhighbackground +
               '";this.style.color="' + calhightext +
               '"; onmouseout=this.style.background="' + caltodayeventbackground +
               '";this.style.color="' + caltodayeventtext +
               '" onmouseup=parent.opencal(' + d + ');' +
               'this.style.background="' + caltodayeventbackground +
               '";this.style.color="' + caltodayeventtext + '"' : '') +
               ' title="' + ttl + '"' +
           '><CENTER>';
  }
  var todayevent_end   = '</CENTER></TD>';
  function today_start(d)
  {
    return '<TD style="cursor:default;background-color:' + caltodaybackground + ';' + caltodaystyle + '" ' +
           (defaultcal ? ' onmouseover=this.style.background="' + calhighbackground +
               '";this.style.color="' + calhightext +
               '"; onmouseout=this.style.background="' + caltodaybackground +
               '";this.style.color="' + caltodaytext +
               '" onmouseup=parent.opencal(' + d + ');' +
               'this.style.background="' + caltodaybackground +
               '";this.style.color="' + caltodaytext + '"' : '') +
           '><CENTER>';
  }
  var today_end   = '</CENTER></TD>';
  function gray_start(d)
  {
     return '<TD style="cursor:default;background-color:' + calgraybackground + ';' + 'color:' + calgraytext + ';' + caldaystyle + '" ' +
           (defaultcal ? ' onmouseover=this.style.background="' + calhighbackground +
               '";this.style.color="' + calhightext +
               '"; onmouseout=this.style.background="' + calgraybackground +
               '";this.style.color="' + calgraytext +
               '" onmouseup=parent.opencal(' + d + ');'  +
               'this.style.background="' + calbackground +
               '";this.style.color="' + calgraytext + '"' : '') +
           '><CENTER>';
  }
  var gray_end   = '</CENTER></TD>';
  function TD_start(d)
  {
    return '<TD style="cursor:default;' + caldaystyle + '" ' +
           (defaultcal ? ' onmouseover=this.style.background="' + calhighbackground +
               '";this.style.color="' + calhightext +
               '"; onmouseout=this.style.background="' + calbackground +
               '";this.style.color="' + caltextcolor +
               '" onmouseup=parent.opencal(' + d + ');' +
               'this.style.background="' + calbackground +
               '";this.style.color="' + caltextcolor + '"' : '') +
           '><CENTER>';
  }
  var TD_end = '</CENTER></TD>';

  cal = '<TABLE WIDTH=100% HEIGHT=100% BORDER=0 CELLSPACING=0 CELLPADDING=0><TR style=font:menu>';
  cal += '<TD COLSPAN=7><TABLE WIDTH=100% HEIGHT=100% BORDER=0 CELLSPACING=0 CELLPADDING=0><TR style=font:menu>';
  cal += '<TD><TABLE ALIGN=LEFT HEIGHT=100% BORDER=0 CELLSPACING=0 CELLPADDING=0><TR style=font:menu>';
  cal += '<TD WIDTH=0 title="' + local(PREV_YEAR) +
         '" style="cursor:hand;' + calnavstyle +
         '" onmouseup="parent.movecal(-12);return false;" ' +
         'onmouseover=this.style.background="' + calhighbackground +
         '";this.style.color="' + calhightext +
         '" onmouseout=this.style.background="' + calbackground +
         '";this.style.color="' + caltextcolor +
         '"><CENTER>&nbsp;&lt;&nbsp;</CENTER></TD>';
  cal += '<TD WIDTH=0 title="' + local(PREV_MONTH) +
         '" style="cursor:hand;' + calnavstyle +
         '" onmouseup="parent.movecal(-1);" ' +
         'onmouseover=this.style.background="' + calhighbackground +
         '";this.style.color="' + calhightext +
         '" onmouseout=this.style.background="' + calbackground +
         '";this.style.color="' + caltextcolor +
         '"><CENTER>&nbsp;&lt;&nbsp;</CENTER></TD></TR></TABLE></TD>';
  if (weekday <0)
  {
    cal += '<TD ID=calhead title="' + local(GO_TO_TODAY) +
           '" style="cursor:hand;' + calmonthstyle +
           '" onmouseup="parent.Calendar=new Date();parent.movecal(0);" ' +
           'onmouseover=this.style.background="' + calhighbackground +
           '";this.style.color="' + calhightext +
           '" onmouseout=this.style.background="' + calbackground +
           '";this.style.color="' + caltextcolor +
           '"><CENTER>';
  }
  else
  {
    cal += '<TD ID=calhead COLSPAN="' + (DAYS_OF_WEEK - 2) +
           '" style="' + calmonthstyle + '">';
  }
  cal += buildcalheader();
  cal += '</TD>';
  cal += '<TD><TABLE ALIGN=RIGHT HEIGHT=100% BORDER=0 CELLSPACING=0 CELLPADDING=0><TR style=font:menu>';
  cal += '<TD WIDTH=0 title="' + local(NEXT_MONTH) +
         '" style="cursor:hand;' + calnavstyle +
         '" onmouseup="parent.movecal(+1);" ' +
         'onmouseover=this.style.background="' + calhighbackground +
         '";this.style.color="' + calhightext +
         '" onmouseout=this.style.background="' + calbackground +
         '";this.style.color="' + caltextcolor +
         '"><CENTER>&nbsp;&gt;&nbsp;</CENTER></TD>';
  cal += '<TD WIDTH=0 title="' + local(NEXT_YEAR) +
         '" style="cursor:hand;' + calnavstyle +
         '" onmouseup="parent.movecal(+12);" ' +
         'onmouseover=this.style.background="' + calhighbackground +
         '";this.style.color="' + calhightext +
         '" onmouseout=this.style.background="' + calbackground +
         '";this.style.color="' + caltextcolor +
         '"><CENTER>&nbsp;&gt;&nbsp;</CENTER></TD></TR></TABLE>';
  cal += '</TD></TR></TABLE>';
  cal += TR_end + TR_start;

  //printing day labels
  for(index=0; index < DAYS_OF_WEEK; index++)
  {
    cal += day_start;
    cal += getDayName(index+7+1);
    cal += day_end;
  }

  cal += TD_end;

  //fill spaces until first day in the month
  var whichmonth = -1;
  var filldays = Calendar.getDay();

  Calendar.setDate(Calendar.getDate() - filldays);

  //draw six weeks loop for each day in the month
  for (index=0; index < DAYS_OF_WEEK * 6; index++)
  {
    var month_day = Calendar.getDate();
    if (month_day == 1) whichmonth += 1;

    week_day = Calendar.getDay();

    // start new row if we hit a new week
    if (week_day == 0) cal += TR_end + TR_start;

    var ent = null;
    querystr = 'event[date[(@year="'+year+'" || not(@year)) && (@month="'+(month+1)+'" || not(@month)) && @day="'+month_day+'"]]';
    if (ents)
      ent = ents.selectSingleNode(querystr);

    var highlight = false;
    if( weekday>0 && Today.getDate()==month_day)
      highlight = true;

    // highlight appropriately
    if (whichmonth != 0)
      cal += gray_start(Calendar.getTime()) + month_day + gray_end;
    else
    {
      if (highlight && ent)
        cal += todayevent_start(Calendar.getTime(), ent.getAttribute("name"));
      else
        if (highlight)
          cal += today_start(Calendar.getTime());
        else
          if (ent)
            cal += eventday_start(Calendar.getTime(), ent.getAttribute("name"));
          else
            cal += TD_start(Calendar.getTime());

      cal += month_day;

      if (highlight && ent)
        cal += todayevent_end;
      else
        if (highlight)
          cal += today_end;
        else 
          if (ent)
            cal += eventday_end;
          else
            cal += TD_end;
    }

    //advance the date for next iteration
    Calendar.setDate(Calendar.getDate()+1);

  }// end for loop

  cal += TR_end;

  //reset back the calendar's month
  Calendar.setMonth(Calendar.getMonth()-1);

  cal += '</TABLE>';

  //return the html string
  return cal;
}

