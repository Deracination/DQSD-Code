// clock code

var clocktimer = null;
var clockrunning = false;
var clockformat = clocklongform;

// Date formatting code adapted from
// http://www.mattkruse.com/javascript/date/source.html


function formatdate(date,format)
{
  format = format+"";
  var result = "";
  var i_format = 0;
  var c = "";
  var token = "";
  var y = date.getYear()+"";
  var M = date.getMonth()+1;
  var d = date.getDate();
  var H = date.getHours();
  var m = date.getMinutes();
  var s = date.getSeconds();
  var E = date.getDay() + 1;
  var yyyy,yy,MMMM,MMM,MM,dd,hh,h,mm,ss,ampm,AMPM,HH,H,EEE,EEEE,E;

  // Convert real date parts into formatted versions

  // Year
  if (y.length < 4)
    y = y-0+1900;
  y = ""+y;
  yyyy = y;
  yy = y.substring(2,4);

  // Month
  MM = ((M < 10) ? "0" + M : M);
  MMMM = getMonthName(M);
  MMM = getMonthName(M+12);

  // Day of month
  dd = ((d < 10) ? "0" + d : d);

  // Day of week
  EEEE = getDayName(E);
  EEE = getDayName(E+7);

  // Hour
  h = ((H > 12) ? H - 12 : H);
  if (h == 0) h = 12;
  hh = ((h < 10) ? "0"+h : h);
  HH = ((H < 10) ? "0"+H : H);

  // AM/PM
  AMPM = ((H >= 12) ? "PM" : "AM");
  ampm = ((H >= 12) ? "pm" : "am");

  // Minute
  mm = ((m < 10) ? "0" + m : m);

  // Second
  if (s < 10) { ss = "0"+s; }
    else { ss = s; }

  // Now put them all into an object!
  var value = new Object();
  value["yyyy"] = yyyy;
  value["yy"] = yy;
  value["y"] = y;
  value["MMMM"] = MMMM;
  value["MMM"] = MMM;
  value["MM"] = MM;
  value["M"] = M;
  value["dd"] = dd;
  value["d"] = d;
  value["hh"] = hh;
  value["h"] = h;
  value["HH"] = HH;
  value["H"] = H;
  value["mm"] = mm;
  value["m"] = m;
  value["ss"] = ss;
  value["s"] = s;
  value["a"] = ampm;
  value["A"] = AMPM;
  value["E"] = E;
  value["EEE"] = EEE;
  value["EEEE"] = EEEE;
  while (i_format < format.length)
  {
    // Get next token from format string
    c = format.charAt(i_format);
    token = "";

	// Basic character escaping
    if ((format.charAt(i_format) == '\\') && (i_format < format.length - 1))
    {
      result = result + format.charAt(++i_format);
      i_format++;
	  continue;
    }

    while ((format.charAt(i_format) == c) && (i_format < format.length))
    {
      token += format.charAt(i_format);
      i_format++;
    }
    if (value[token] != null)
    {
      result = result + value[token];
    }
    else
    {
      result = result + token;
    }
  }
  return result;
}

function advanceclock()
{
  stopbanner();
  clocktimer = setTimeout("advanceclock()", clockupdate);
  clockrunning = true;
  paintclock();
}

function paintclock()
{
  // Display date and time
  document.selection.empty();
  document.deff.q.className = "clock";
  var now = new Date();
  setSearchWindowText(formatdate(now, clockformat));
  if (typeof clocktooltipform != "undefined" && clocktooltipform != "")
  {
    document.deff.q.title = formatdate(now, clocktooltipform);
  }
}

function startclock(delay)
{
  if (clock && !clockrunning)
  {
    clocktimer = setTimeout("advanceclock()", delay);
    clockrunning = true;
  }
}

function stopclock()
{
  if (clockrunning)
    clearTimeout(clocktimer);
  document.deff.q.title = "";
  clockrunning = false;
}


