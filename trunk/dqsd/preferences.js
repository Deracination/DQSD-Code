/* Configuration for Dave's Quick Search Deskbar features.
*
*  In order to avoid losing configuration changes during upgrades
*  the preferred way to make changes is to redefine the variable in
*  your localprefs file (defaulted to localprefs.js).  If this file
*  does not exist, you should create it.
*
*  After you've saved your changes, enter "!" in the deskbar
*  to reload.
*/

/* Local files.  The following files are used to overwrite default
*  settings.  They will not be replaced during upgrade installations.
*
*  Defaults:
*  localeventsfileurl = "localevents.xml"
*  localsearchurl     = "localsearch.xml"
*  localaliases       = "localaliases.txt"
*  localstylesurl     = "localsearch.css"
*
*  NOTE:
*  localeventsfileurl - the format of this file is just like a standard events file (holidays.us.xml)
*  localsearchurl - the format of this file is <searches>[search definition like in /searches/amax.xml]</searches>
*  localaliases - the format of this file is just like aliases.txt
*  localstylesurl - this is a standard cascading stylesheet
*/
localeventsfileurl = "localevents.xml";   // local events to be added to the calendar
localsearchurl     = "localsearch.xml";   // local search file
localaliases       = "localaliases.txt";  // local aliases file
localstylesurl     = "localsearch.css";   // local stylsheet (to overwrite defaults in search.css)


/* Personalization variables
*  Variables can be used in searches as default parameters.
*  Uncomment the following lines and change the values of the variables.
*/
/*
user_address = "1600 Pennsylvania Ave"
user_city    = "Washington"
user_state   = "DC"
user_zip     = "20509"
*/

/* Startup delay
*  Increasing this might help if your tray icons don't all show up when you startup windows.
*  [milliseconds (1000 milliseconds = 1 second)]
*/
startupdelay=0;

/* Display the clock?
*  true = display
*  false = do not display
*/
clock=true;


/* clock display delay in milliseconds
*  every 1000=1 second ex: 5000=5 seconds
*  default = 5000
*/
clockdelay=5000;


/* clock update rate in milliseconds
*  every 1000=1 second
*  default = 1000
*/
clockupdate=1000;


/* clock long form width.
*  long form is used if this pixel width is exceeded
*  default = 139
*/
clocklongwidth=139;


/* The clock is formatted according to the following format strings:
*  yyyy - four digit year (2001)
*  yy   - two digit year (01)
*  MMMM - fully spelled month name (January)
*  MMM  - abbreviated month name (Jan)
*  MM   - zero-padded month number (01)
*  M    - month number (1)
*  dd   - zero-padded day-of-month number (01)
*  d    - day-of-month number (1)
*  hh   - zero-padded 12-hour clock hour (01)
*  h    - 12-hour clock hour (1)
*  HH   - zero-padded 24-hour clock hour (01, 13)
*  H    - 24-hour clock hour (13)
*  mm   - zero-padded minutes value (00)
*  ss   - zero-padded seconds value (00)
*  a    - 12-hour clock am/pm lowercase (am)
*  A    - 12-hour clock am/pm uppercase (AM)
*  EEEE - day-of-week full name (Monday)
*  EEE  - day-of-week abbreviation (Mon)
*
*  default long form = " EEE d MMM yyyy h:mm A "
*  default short form = " h:mm A "
*/
clocklongform  = " EEE MMM d, yyyy h:mm A ";
clockshortform = " h:mm A ";


/* Display clock tooltip text turned off by default
*  default = ""
*  common tool tip text = "MMMM d, yyyy"
*/
clocktooltipform="";


/* Reuse browser window mode
*  NOTE: reuse browser mode is only valid when IE is the default browser.
*  If any other browser is the default and the launchmode=1, a new browser
*  window will be spawned for each search regardless of category.
*
*  0=new window always [default]
*  1=single window for all
*  2=single window for each search type
*/
reuseBrowserWindowMode = 0;


/* Browser launch mode
*  0=use IE
*  1=use default browser
*  2=auto (launches in IE directly if IE is default browser) [default]
*/
launchmode=2;

/* Page template to use to initialize search results window
* Used only when launching in IE. Set pagetemplate to "maximized.htm" to maximize results.
* With things set up as is currently, reuses the browser size that was most recently closed.
*/
pagetemplate="";


/* Default language - uncomment to override default language (which is determined programatically)*/
// language="en";


/* Default search engine
*  gg=google [default]
*/
defaultsearch="gg";


/* Display the calendar on right-click?
*  true = display calendar [default]
*  false = do not display
*/
cal=true;


/* Calendar popup dimensions.
*  default calwidth=179
*  default calheight=156
*/
calwidth=179;
calheight=156;


/* Calendar start method.
*  0 = right click
*  1 = double click
*  default is 0
*/
calStart=0;


/* Default calendar. clicking on a calendar day will spawn the defined online calendar.
*  Valid calendars include:
*  yhocal - Yahoo Calendar [default]
*  msncal - MSN Calendar
*  aolcal - AOL Calendar
*  nsccal - Netscape Calendar
*  mycal  - MyCalendar.com
*  evcal  - Evite.com
*  outlook - MS Outlook default calendar (won't go to the current day, yet)
*  mozillacal - Mozilla calendar (won't go to the current day, yet)
*/
defaultcal="yhocal";


/* Events file for the popup calendar events
*  holidays.us.xml - United States [default]
*  holidays.ca.xml - Canada
*  holidays.sg.xml - Singapore
*  holidays.it.xml - Italy
*  holidays.fr.xml - France
*  holidays.nl.NL.xml - Netherlands (Dutch language)
*  holidays.nl.EN.xml - Netherlands (English language)
*/
eventsfileurl="holidays.us.xml";


/* Use multi-line searchbar?  Set this to false to prevent word wraps.
*  true = display multiline [default]
*  false = do not display multiline
*/
multiline=true;


/*  Show the menu '>>' popup button?
 *  0 = never
 *  1 = always
 *  2 = if search bar width is greater than clocklongwidth [default]
 */
showbutton = 2;


/*  Align the menu button right or left
*/
buttonalign="right";


/* Display duration for the help message "Press ? for help"
*  after the initial toolbar startup.
*  every 1000=1 second ex: 5000=5 seconds
*  default = 3000
*/
helptime = 3000;


/* Changes to the size of the help window.
*  uncomment the bolow line and set the values that you wish to change.
*/
//helpoptions = "width=800, height=550, top=100, left=100";


/* Turn on/off [default] auto complete.  Values are true (for on) and false (for off)
*/
autocomplete = false;


/* Number of searches to remember.  Lower this if you notice poor performance
*  after a search.  (The entire history is written to disk after each search.)
*/
historylength=1000;


/* Changes the default alert sensitivity:
*  Qualified alerts are alerts that will not display unless a particular variable is set.
*  This setting is mainly used for debugging purposes.
*  0=no qualified alerts [default]
*  1=all qualified alerts
*/
alertmode=0;


/* Length of menu Most Recently Used list
*/
menuMRUlength = 5;


/* The character we use with the 'Windows key' as a hotkey to jump to the searchbar
*  This should be a single UPPER CASE letter
*/
hotkeyCharacter = "S";

/* An alternative if you want to do something more elaborate - 
*  set the hotkeyCharacter to "" and use this as a direct vkCode number
  VK_LBUTTON = 1;
  VK_RBUTTON = 2;
  VK_CANCEL = 3;
  VK_MBUTTON = 4;  { NOT contiguous with L & RBUTTON }
  VK_BACK = 8;
  VK_TAB = 9;
  VK_CLEAR = 12;
  VK_RETURN = 13;
  VK_CONTROL = 17;
  VK_MENU = 18;
  VK_PAUSE = 19;
  VK_CAPITAL = 20;
  VK_ESCAPE = 27;
  VK_SPACE = $20;
  VK_PRIOR = 33;
  VK_NEXT = 34;
  VK_END = 35;
  VK_HOME = 36;
  VK_LEFT = 37;
  VK_UP = 38;
  VK_RIGHT = 39;
  VK_DOWN = 40;
  VK_SELECT = 41;
  VK_PRINT = 42;
  VK_EXECUTE = 43;
  VK_SNAPSHOT = 44;
  VK_INSERT = 45;
  VK_DELETE = 46;
  VK_HELP = 47;
{ VK_0 thru VK_9 are the same as ASCII '0' thru '9' ($30 - $39) }
{ VK_A thru VK_Z are the same as ASCII 'A' thru 'Z' ($41 - $5A) }
  VK_LWIN = 91;
  VK_RWIN = 92;
  VK_APPS = 93;
  VK_NUMPAD0 = 96;
  VK_NUMPAD1 = 97;
  VK_NUMPAD2 = 98;
  VK_NUMPAD3 = 99;
  VK_NUMPAD4 = 100;
  VK_NUMPAD5 = 101;
  VK_NUMPAD6 = 102;
  VK_NUMPAD7 = 103;
  VK_NUMPAD8 = 104;
  VK_NUMPAD9 = 105;
  VK_MULTIPLY = 106;
  VK_ADD = 107;
  VK_SEPARATOR = 108;
  VK_SUBTRACT = 109;
  VK_DECIMAL = 110;
  VK_DIVIDE = 111;
  VK_F1 = 112;
  VK_F2 = 113;
  VK_F3 = 114;
  VK_F4 = 115;
  VK_F5 = 116;
  VK_F6 = 117;
  VK_F7 = 118;
  VK_F8 = 119;
  VK_F9 = 120;
  VK_F10 = 121;
  VK_F11 = 122;
  VK_F12 = 123;
  VK_F13 = 124;
  VK_F14 = 125;
  VK_F15 = 126;
  VK_F16 = 127;
  VK_F17 = 128;
  VK_F18 = 129;
  VK_F19 = 130;
  VK_F20 = 131;
  VK_F21 = 132;
  VK_F22 = 133;
  VK_F23 = 134;
  VK_F24 = 135;
  VK_NUMLOCK = 144;
  VK_SCROLL = 145;
{ VK_L & VK_R - left and right Alt, Ctrl and Shift virtual keys.
  Used only as parameters to GetAsyncKeyState() and GetKeyState().
  No other API or message will distinguish left and right keys in this way.
}
  VK_LSHIFT = 160;
  VK_RSHIFT = 161;
  VK_LCONTROL = 162;
  VK_RCONTROL = 163;
  VK_LMENU = 164;
  VK_RMENU = 165;
  VK_PROCESSKEY = 229;
  VK_ATTN = 246;
  VK_CRSEL = 247;
  VK_EXSEL = 248;
  VK_EREOF = 249;
  VK_PLAY = 250;
  VK_ZOOM = 251;
  VK_NONAME = 252;
  VK_PA1 = 253;
  VK_OEM_CLEAR = 254;
*/
hotkeyVkCode = 0;

/* The modifiers for the hotkey
  Options are combinations of 
                WIN for the windows key (default)
                ALT for ALT
                CONTROL for CONTROL
                SHIFT for SHIFT
  e.g.    hotkeyModifiers="WIN ALT";
  Spaces are optional, so ALTSHIFTWIN is just as good
  You wouldn't want to use anything on its own other than WIN, unless you're deliberately trying to upset people
*/
hotkeyModifiers="WIN";

/* History search default key assignment.
   <historySearchVkCode> searches for the typed string in any part of a previous command
   Shift-<historySearchVkCode> searches for the typed string in any part of the next command
*/
historySearchVkCode = 119 // F8 (kinda' like DOSKEY)

/* How to sort the searches in the help window and the popup menu
*  'name'  = sort by search name
*  'alias' = sort by first alias
*/
helpSortKey = 'name';

/* Enable tooltips on menu help menu items 
    The options are 'true' or 'false'
*/
helpMenuToolTipsEnabled = true;

/* A multiplier of  the time for which the menu tooltips are displayed
 1 would be the system default, which is a bit fast for some of the long tips
*/
helpMenuToolTipsDisplayTime = 3;


/* Display help text setting. Setting this to false will cause the 'Quick Search'
* and 'Display ? for help' strings to not be displayed at all.
*/
displayHelp = true;


/* Check web for an updated version each time the toolbar is started
*/
checkForUpdate = true;
checkForUpdateDelay = 10000; // millisecond delay after first search

/* Notify for only final ('final') or both beta and final ('beta,final').
*  This is ignored if 'checkForUpdateTypes' is false
*/
checkForUpdateTypes = 'final';


/* Broadcast a message to refresh the tray icons.  This may solve the problems
*  for some with disappearing tray icons at startup.
*/
refreshIconsAtStartup = true;