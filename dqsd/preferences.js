/* Configuration for Dave's Quick Search Deskbar features.
*
*  In order to avoid losing configuration changes during upgrades
*  the preferred way to make changes is to redefine the variable in
*  your localprefs file (defaulted to localpreferences.js).  If this file
*  does not exist, you should create it.
*
*  After you've saved your changes, enter "!" in the deskbar
*  to reload.
*/

/* Local files.  The following files are used to overwrite default
*  settings.  They will not be replaced during upgrade installations.
*
*  Defaults:
*  localeventsfileurl	=	"localevents.xml"
*  localsearchurl		=	"localsearch.xml"
*  localaliases			=	"localaliases.txt"
*  localstylesurl		=	"localsearch.css"
*  localprefs			=	"localprefs.js"
*
*  NOTE:
*  localeventsfileurl - the format of this file is just like a standard events file (holidays.us.xml)
*  localsearchurl - the format of this file is <searches>[search definition like in /searches/amax.xml]</searches>
*  localaliases - the format of this file is just like aliases.txt
*  localstylesurl - this is a standard cascading stylesheet
*  localprefs - this is a standard javascript source file
*/
localeventsfileurl	=	"localevents.xml";		// local events to be added to the calendar
localsearchurl		=	"localsearch.xml"; 		// local search file
localaliases		=	"localaliases.txt";		// local aliases file
localstylesurl		=	"localsearch.css";		// local stylsheet (to overwrite defaults in search.css)
localprefs			=	"localprefs.js";		// local preferences (to overwrite defaults in this file)


/* Personalization variables
*  Variables can be used in searches as default parameters.
*  Uncomment the following lines and change the values of the variables.
*/
/*
user_address = "1600 Pennsylvania Ave"
user_city = "Washington"
user_state = "DC"
user_zip = "20509"
*/

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


/* Default calendar. clicking on a calendar day will spawn the defined online calendar.
*  Valid calendars include:
*  yhocal - Yahoo Calendar [default]
*  msncal - MSN Calendar
*  aolcal - AOL Calendar
*  nsccal - Netscape Calendar
*  mycal  - MyCalendar.com
*  evcal  - Evite.com
*/
defaultcal="yhocal";


/* Events file for the popup calendar events
*  holidays.us.xml - United States [default]
*  holidays.ca.xml - Canada
*  holidays.sg.xml - Singapore
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


/* help message duration in milliseconds
*  every 1000=1 second ex: 5000=5 seconds
*  default = 8500
*/
helptime = 8500;


/* Changes to the size of the help window.
*  uncomment the bolow line and set the values that you wish to change.
*/
//helpoptions = "width=800, height=550";


/* Turn on/off [default] auto complete.  Values are true (for on) and false (for off)
*/
autocomplete = false;


/* Number of searches to remember
*  default = 50;
*/
historylength=50;


/* Changes the default alert sensitivity:
*  0=no qualified alerts [default]
*  1=all qualified alerts
*/
alertmode=0;