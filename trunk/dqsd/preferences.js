
/* Configuration for Dave's Quick Search Deskbar features.
*  After you've saved your changes, enter "!" in the deskbar 
*  to reload.
*/

/* Display the clock?
*  true = display
*  false = do not display
*/
clock=true;          


/* clock display delay in seconds
*  every 1000=1 second ex: 5000=5 seconds
*/
clockdelay=5000;


/* clock update rate in seconds
*  every 1000=1 second
*/
clockupdate=1000;

/* clock long form width.  
*  long form is used if this pixel width is exceeded
*/
clocklongwidth=139;


/* Reuse browser window mode
* note - reuse browser mode is only valid when IE is the default browser.
* If any other browser is the default and the launchmode=1, a new browser
* window will be spawned for each search regardless of category.
*
*  0=new window always [default]
*  1=single window for all
*  2=single window for each search type
*/
reuseBrowserWindowMode = 2;


/* Browser launch mode
*  0=use IE
*  1=use default browser
*  2=auto (launches in IE directly if IE is default browser)
*/
launchmode=2;


/* Default language - uncomment to override default language */
// language="en";


/* Default search engine
*  gg=google (default)
*/
//defaultsearch="gg";


/* Display the calendar on right-click?
*  true = display calendar (default)
*  false = do not display
*/
cal=true;


/* Events file for the popup calendar events 
* holidays.us.xml - United States
* holidays.ca.xml - Canada
* holidays.sg.xml - Singapore
*/
eventsfileurl="holidays.us.xml";


/* Use multi-line searchbar?  Set this to false to prevent word wraps.
*  true = display multiline (default)
*  false = do not display multiline
*/
//multiline=false;


/* Default calendar. clicking on a calendar day will spawn the defined online calendar. 
*  Valid calendars include:
*  yhocal - Yahoo Calendar
*  msncal - MSN Calendar
*  aolcal - AOL Calendar
*  nsccal - Netscape Calendar
*  mycal  - MyCalendar.com
*  evcal  - Evite.com
*/
defaultcal="yhocal";


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
*/
clocklongform=" EEE d MMM yyyy h:mm A ";
clockshortform=" h:mm A ";


/* Display clock tooltip text  turned off by default*/
clocktooltipform=true;


/*  Show the menu '>>' popup button?
 *  0 = never
 *  1 = always
 *  2 = if search bar width is greater than clocklongwidth (default)
 */
showbutton = 2;

/* 
 * Styles that can be used to modify the color, font, etc. 
 */
//txtfldstyle = "background:yellow";
//clockstyle = "background:threedface;text-align:center;border-width:1px;font-weight:bold";
//helpstyle = "font-size:12pt;";
//helpoptions = "width=800, height=550" // These are not CSS styles, but window.open() options

/*
 * Turn on/off(default) auto complete
 */
 autocomplete = false;
 