
function getDisplayMonth(iMonth) {
  var monthArray = new Array("January","Febuary","March","April","May","June","July","August","September","October","November","December");
  return monthArray[iMonth];
}

function getDisplayDate(passedDate) {
  theDate = new Date(passedDate);
  sDisplayDate = getDisplayMonth(theDate.getMonth()) + " " + theDate.getDate() + ", " + theDate.getFullYear();
  return sDisplayDate;
}

function buildEmail( nm, sld, tld, txt )
{
  var mailto = '<a href="mailto:' + nm + '@' + sld + '.' + tld + '">' + txt + '</a>';
  return mailto;
}