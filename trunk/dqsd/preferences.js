// Configuration for Dave's Quick Search Deskbar features.
// After you've saved your changes, enter "!" in the deskbar to reload.

clock=true;          // false disables the clock
clockdelay=5000;     // clock shows after 5 secs
clockupdate=1000;    // clock updates every 1 sec
clocklongwidth=139;  // long form is used if this pixel width is exceeded

launchmode=2;        // 0=use ie; 1=use default browser; 2=auto
// language="en";    // uncomment to override default language
//defaultsearch="gg"; // use google search
cal=true;            // right-click shows popup calendar
defaultcal="yhocal"; // click on a calendar day uses yahoo calendar

// Note: valid calendars include
// yhocal - Yahoo Calendar
// msncal - MSN Calendar
// aolcal - AOL Calendar
// nsccal - Netscape Calendar
// mycal  - MyCalendar.com
// evcal  - Evite.com


// The clock is formatted according to the following format strings:

clocklongform=" EEE d MMM yyyy h:mm A ";
clockshortform=" h:mm A ";
clocktooltipform=null; // tooltip turned off by default


/* show the menu popup button?
 *   0 = never
 *   1 = always
 *   2 = if search bar width is greater than clocklongwidth [DEFAULT]
 */
showbutton = 2;

/*
Some useful formatting codes:

yyyy - four digit year (2001)
yy   - two digit year (01)
MMMM - fully spelled month name (January)
MMM  - abbreviated month name (Jan)
MM   - zero-padded month number (01)
M    - month number (1)
dd   - zero-padded day-of-month number (01)
d    - day-of-month number (1)
hh   - zero-padded 12-hour clock hour (01)
h    - 12-hour clock hour (1)
HH   - zero-padded 24-hour clock hour (01, 13)
H    - 24-hour clock hour (13)
mm   - zero-padded minutes value (00)
ss   - zero-padded seconds value (00)
a    - 12-hour clock am/pm lowercase (am)
A    - 12-hour clock am/pm uppercase (AM)
EEEE - day-of-week full name (Monday)
EEE  - day-of-week abbreviation (Mon)
*/