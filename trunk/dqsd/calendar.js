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
 * modified by Monty Scroggins  31/03/2002
 *  - added support for a local events file with referenced by
 *    a the var 'localeventsfileurl'
 * modified by Neel Doshi 31/03/2002
 *  - configured the calendar to use the stylesheet
 *  - fixed the bug where "today" was not being highlighted if today is Sunday
 * modified by Harry-Anton Talvik 03/11/2002
 *  - configured the calender to use week start day
 *
 * TODO:
 *  - add ability to distingush b/w different kind of events
 *  - add ability to input events directly from search box
 */


var ents = null;
var lents = null;

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

function loadlocaleventslist()
{
  try
  {
    document.write("<xml id='localeventslist' src='" + localeventsfileurl + "'></xml>");
    lents = document.all("localeventslist").selectSingleNode("events");
  }
  catch (ex) {}
}

loadlocaleventslist();

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

function outlook(dat)
{
  // TODO - find out how to open at a particular date
  openDocument("outlook:calendar");
}

function mozillacal(dat)
{
  // TODO - find out how to open at a particular date
  openDocument("mozilla.exe -calendar");
  //openDocument("mozilla.exe chrome://calendar/content/calendar.xul");
}

//vars used by the calendar script
var DAYS_OF_WEEK = 7;    // "constant" for number of days in a week
var DAYS_OF_MONTH = 31;    // "constant" for number of days in a month
var CAL_DAY_START = 1;    // "constant" to indicate which date starts a month (usually people like it to be 1. day of the month)
var WEEK_START_DAY = weekStartDay;    // "constant" to indicate a day which starts a week -- 1=Sun, 2=Mon, .. ; weekStartDay is declared in preferences.js

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
    popcal.document.body.className="cal";
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

function buildcalhtmlhead()
{
  return '<table class=cal width=100% height=100% cellpadding=0 cellspacing=0 border=0><tr><td><style>' + convertStylesToInline() + '</style>';
}

function buildcalheader()
{
  return getMonthName(Calendar.getMonth()+1)  + ' ' + Calendar.getFullYear();
}

function buildcal()
{
  var year = Calendar.getFullYear();  // Returns year in 4 digits
  var month = Calendar.getMonth();    // Returns month (0-11)

  Today = new Date();
  var weekday = -1;  // Just init weekday

  if (Today.getMonth() == Calendar.getMonth() && Today.getYear() == Calendar.getYear())
    weekday = Today.getDay();

  var cal;    // Used for printing

  Calendar.setDate(CAL_DAY_START);    // Start the calendar day at '1', '2', ..

  var TR_start = '<tr>';
  var TR_end = '</tr>';
  var day_start = '<td width=14.2857% class=caldow>';
  var day_end = '</td>';

  function eventday_start(d, ttl)
  {
    return '<td class=caleventday' +
      (defaultcal ? ' onmouseover=this.className="caleventdayhigh" onmouseout=this.className="caleventday" onmouseup=this.className="caleventday";parent.opencal(' + d + ');' : ' ') +
      ' title="' + ttl + '">';
  }
  var eventday_end   = '</td>';

  function todayevent_start(d, ttl)
  {
    return '<td class=caltodayevent' +
      (defaultcal ? ' onmouseover=this.className="caltodayeventhigh" onmouseout=this.className="caltodayevent" onmouseup=this.className="caltodayevent";parent.opencal(' + d + ');' : ' ') +
      ' title="' + ttl + '"' + '>';
  }
  var todayevent_end   = '</td>';

  function today_start(d)
  {
    return '<td class=caltoday' +
        (defaultcal ? ' onmouseover=this.className="caltodayhigh" onmouseout=this.className="caltoday" onmouseup=this.className="caltoday";parent.opencal(' + d + ');' : ' ') + '>';
  }
  var today_end   = '</td>';

  function gray_start(d)
  {
     return '<td class=calgray' +
         (defaultcal ? ' onmouseover=this.className="calgrayhigh" onmouseout=this.className="calgray" onmouseup=this.className="calgray";parent.opencal(' + d + ');' : ' ') + '>';
  }
  var gray_end   = '</td>';

  function TD_start(d)
  {
    return '<td class=calday' +
        (defaultcal ? ' onmouseover=this.className="caldayhigh" onmouseout=this.className="calday" onmouseup=this.className="calday";parent.opencal(' + d + ');' : ' ') + '>';
  }
  var TD_end = '</td>';

  cal = buildcalhtmlhead();

  cal += '<table width=100% height=100% border=0 cellspacing=0 cellpadding=0><tr>';
  cal += '<td colspan=7><table width=100% height=100% border=0 cellspacing=0 cellpadding=0><tr>';
  cal += '<td><table align=left height=100% border=0 cellspacing=0 cellpadding=0><tr>';
  cal += '<td class=calnavyear width=0 title="' + local(PREV_YEAR) + '" ' +
         ' onmouseup="parent.movecal(-12);return false;" ' +
         ' onmouseover=this.className="calnavyearhigh" ' +
         ' onmouseout=this.className="calnavyear" >&nbsp;&laquo;&nbsp;</td>';
  cal += '<td class=calnavmonth width=0 title="' + local(PREV_MONTH) + '" ' +
         ' onmouseup="parent.movecal(-1);return false;" ' +
         ' onmouseover=this.className="calnavmonthhigh" ' +
         ' onmouseout=this.className="calnavmonth" >&nbsp;&lsaquo;&nbsp;</td></tr></table></td>';

  if (weekday < 0)
  {
    cal += '<td id=calhead class=calmonth title="' + local(GO_TO_TODAY) + '" ' +
      ' onmouseup="parent.Calendar=new Date();parent.movecal(0);" ' +
      ' onmouseover=this.className="calmonthhigh" ' +
      ' onmouseout=this.className="calmonth" >';
  }
  else
  {
    cal += '<td id=calhead class=calmonth colspan=' + (DAYS_OF_WEEK - 2) + '>';
  }

  cal += buildcalheader();

  cal += '</td>';
  cal += '<td><table align=RIGHT height=100% border=0 cellspacing=0 cellpadding=0><tr>';
  cal += '<td class=calnavmonth width=0 title="' + local(NEXT_MONTH) + '" ' +
         ' onmouseup="parent.movecal(+1);return false;" ' +
         ' onmouseover=this.className="calnavmonthhigh" ' +
         ' onmouseout=this.className="calnavmonth" >&nbsp;&rsaquo;&nbsp;</td>';
  cal += '<td class=calnavyear width=0 title="' + local(NEXT_YEAR) + '" ' +
         ' onmouseup="parent.movecal(+12);return false;" ' +
         ' onmouseover=this.className="calnavyearhigh" ' +
         ' onmouseout=this.className="calnavyear" >&nbsp;&raquo;&nbsp;</td></tr></table></td>';
  cal += '</td></tr></table>';
  cal += TR_end + TR_start;

  //printing day labels
  for(index=0; index < DAYS_OF_WEEK; index++)
  {
    cal += day_start;
    cal += getDayName(DAYS_OF_WEEK + ((index + WEEK_START_DAY - 1) % DAYS_OF_WEEK) + 1);
    cal += day_end;
  }

  cal += TD_end;

  //fill spaces until first day in the month
  var whichmonth = -1;
  var filldays = Calendar.getDay() - (WEEK_START_DAY - 1);
  if (filldays < 0) filldays += DAYS_OF_WEEK;

  Calendar.setDate(Calendar.getDate() - filldays);

  //draw six weeks loop for each day in the month
  for (index=0; index < DAYS_OF_WEEK * 6; index++)
  {
    var month_day = Calendar.getDate();
    if (month_day == CAL_DAY_START) whichmonth += 1;

    week_day = Calendar.getDay();

    // start new row if we hit a new week
    if (week_day == WEEK_START_DAY - 1) cal += TR_end + TR_start;
    var ent = null;
    var lent = null;
    querystr = 'event[date[(number(@year)="'+year+'" || not(@year)) && (number(@month)="'+(month+1)+'" || not(@month)) && number(@day)="'+month_day+'"]]';
    if (ents)
      ent = ents.selectSingleNode(querystr);
    if (lents)
      lent = lents.selectSingleNode(querystr);


    var highlight = false;
    if( weekday>=0 && Today.getDate()==month_day)
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
            if (lent)
              cal += eventday_start(Calendar.getTime(), lent.getAttribute("name"));
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
  cal += '</table>'
  cal += '</td></tr></table>';

  //return the html string
  return cal;
}

