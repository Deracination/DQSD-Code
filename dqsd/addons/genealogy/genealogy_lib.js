    var genealogy_debug = 0;
    var genealogy_lib_version = "1.5";

	var genealogy_month_names = new Array("JANUARY", "FEBRUARY", "MARCH", "APRIL", "MAY", "JUNE", "JULY", "AUGUST", "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER");
	var genealogy_date_format_1 = "([0-9]{1,2})\\s*/\\s*([0-9]{1,2})\\s*/\\s*([0-9]{4})";
	var genealogy_date_format_2 = "([0-9]{1,2})\\s+([a-zA-Z]{3,})\\s+([0-9]{4})";
	var genealogy_date_format_all = "(("+genealogy_date_format_1+")|("+genealogy_date_format_2+"))";

	var genealogy_states = new Array(
	"ALABAMA", "ALASKA", "ARIZONA", "ARKANSAS",
	"CALIFORNIA", "COLORADO", "CONNECTICUT",
	"DELAWARE", "DISTRICT OF COLUMBIA",
	"FLORIDA",
	"GEORGIA",
	"HAWAII",
	"IDAHO", "ILLINOIS", "INDIANA", "IOWA",
	"KANSAS", "KENTUCKY",
	"LOUISIANA",
	"MAINE", "MARYLAND", "MASSACHUSETTS", "MICHIGAN", "MINNESOTA", "MISSISSIPPI", "MISSOURI", "MONTANA",
	"NEBRASKA", "NEVADA", "NEW HAMPSHIRE", "NEW JERSEY", "NEW MEXICO", "NEW YORK", "NORTH CAROLINA", "NORTH DAKOTA",
	"OHIO", "OKLAHOMA", "OREGON",
	"PENNSYLVANIA",
	"RHODE ISLAND",
	"SOUTH CAROLINA", "SOUTH DAKOTA",
	"TENNESSEE", "TEXAS",
	"UTAH", 
	"VERMONT", "VIRGINIA",
	"WASHINGTON", "WEST VIRGINIA", "WISCONSIN", "WYOMING"
	);

    var genealogy_states_abbrev = new Array(
	"AL", "AK", "AZ", "AR",
	"CA", "CO", "CT", 
	"DE", "DC",
	"FL",
	"GA",
	"HI",
	"ID", "IL", "IN", "IA",
	"KS", "KY",
	"LA",
	"ME", "MD", "MA", "MI", "MN", "MS", "MO", "MT",
	"NE", "NV", "NH", "NJ", "NM", "NY", "NC", "ND",
	"OH", "OK", "OR", 
	"PA",
	"RI",
	"SC", "SD",
	"TN", "TX",
	"UT", 
	"VT", "VA",
	"WA", "WV", "WI", "WY"
	);

	var genealogy_countries = new Array(
	"ARGENTINA", "AUSTRALIA", "BELGIUM", "BRAZIL", "CANADA", "DENMARK", "ENGLAND", "FINLAND", "FRANCE", "GERMANY", "GREAT BRITAIN", "ICELAND", "ITALY",
	"LIECHTENSTEIN", "NETHERLANDS", "NEW ZELAND", "NORWAY", "PAPUA NEW GUINEA", "RUSSIA", "SOUTH AFRICA", "SWEDEN",
	"SWITZERLAND", "UNITED KINGDOM", "ZIMBABWE", "PUERTO RICO", "SCOTLAND"
	);

function genealogy_trim(str)
{
   var start = 0;
   var end = str.length-1;
   while (str.charAt(start) == ' ') {
	   start++;
   }
   if (start >= end)   {
	   return "";
   }
   while (str.charAt(end) == ' ') {
	   end--;
   }
   if (start != 0 || end != str.length-1) {
	   return str.substring(start, end+1);
   } else {
       return str;
   }
}

function genealogy_strip_tags(str)
{
	var start = 0;
	var end = str.length-1;
	var retval = "";
	var in_tag = 0;
	var in_quotes = 0;
	while (start <= end)
	{
		var ch = str.charAt(start);
		if (in_tag) {
			if (ch == '>') {
				if (!in_quotes) {
					in_tag = 0;
				}
			} else if (ch == '"') {
				in_quotes = (in_quotes == 1) ? 0 : 1;
			}
		} else {
			if (ch == '<') {
				if (in_quotes) {
					retval += ch;
				} else {
					in_tag = 1;
				}
			} else if (ch == '"') {
				in_quotes = (in_quotes == 1) ? 0 : 1;
				retval += ch; 
			} else {
				retval += ch;
			}
		}
		start++;
	}
	return retval;
}

function genealogy_log(msg)
{
	if (genealogy_debug != null && genealogy_debug != 0) {
  	  appendFile("genealogy.log", msg+"\r\n");
	}
}

function genealogy_error(msg)
{
	alert(msg);
}

function genealogy_alert(msg)
{
	if (genealogy_debug != null && genealogy_debug != 0) {
  	  alert(msg);
	}
}

function genealogy_lookup_place(place)
{
	search_place = place.toUpperCase();
	for (var i=0; i < genealogy_states.length; i++) {
		if (genealogy_states[i] == search_place) {
			return i;
		}
	}
	for (var i=0; i < genealogy_states_abbrev.length; i++) {
		if (genealogy_states_abbrev[i] == search_place) {
			return i;
		}
	}
	for (var i=0; i < genealogy_countries.length; i++) {
		if (genealogy_countries[i] == search_place) {
			return (genealogy_states.length + i);
		}
	}
	return -1;
}

function genealogy_lookup_place_abbrev(place)
{
	var place_num= genealogy_lookup_place(place);
	if (place_num >= 0)	{
		return genealogy_states_abbrev[place_num];
	}
	return "";
}

function genealogy_lookup_place_name(place)
{
	var place_num= genealogy_lookup_place(place);
	if (place_num >= 0)	{
		return genealogy_states[place_num];
	}
	return "";
}

function genealogy_parse_surname(name)
{
	var lngn_regex = new RegExp("^\\s*(.*?)\\s*,\\s*(.*?)$");
	if (name.match(lngn_regex)) {
		var name_results = lngn_regex.exec(name);
		if (name_results != null && name_results.length == 3) {
			return genealogy_trim(name_results[1]);
		}
	}
	return genealogy_trim(name);
}

function genealogy_parse_givenname(name)
{
	var lngn_regex = new RegExp("^\\s*(.*?)\\s*,\\s*(.*?)$");
	if (name.match(lngn_regex)) {
		var name_results = lngn_regex.exec(name);
		if (name_results != null && name_results.length == 3) {
			return genealogy_trim(name_results[2]);
		}
	}
	return "";
}

function genealogy_soundex(name)
{
	var i = 0;
	var j = 0;
	var SCode = '0';
	var PrevCode = '0';
	var strResult = '';
	var CharTemp = '0';
	var tempName = genealogy_trim(name).toUpperCase();

	for (i = 0; (i < tempName.length && j < 4); i++) {
		CharTemp = tempName.charAt(i);
		if (CharTemp == 'R') {
			SCode = '6';
        } else if (CharTemp == 'M' || CharTemp == 'N') {
			SCode = '5';
		} else if (CharTemp == 'L') {
			SCode = '4';
		} else if (CharTemp == 'D' || CharTemp == 'T') {
			SCode = '3';
		} else if (CharTemp == 'C' || CharTemp == 'G' ||
                   CharTemp == 'J' || CharTemp == 'K' ||
                   CharTemp == 'Q' || CharTemp == 'S' ||
                   CharTemp == 'X' || CharTemp == 'Z') {
			SCode = '2';
		} else if (CharTemp == 'B' || CharTemp == 'F' ||
                   CharTemp == 'P' || CharTemp == 'V') {
			SCode = '1';
		} else {
			SCode = '0';
		}

		if (SCode > '0' || j == 0) {
			if (j == 0 || SCode != PrevCode) {
				strResult += SCode;
				j++;
            }
        }

		if (j == 0)	{
			j++;
		}

		if (CharTemp == 'H' || CharTemp == 'W') {
			SCode = PrevCode;
		}
		PrevCode = SCode;
	}

	for (i = j; i <= 4; i++) {
		strResult += '0';
	}
	if (name.length > 0) {
		return name.charAt(0).toUpperCase()+strResult.substring(1,4);
	} else {
		return "";
	}
}

function genealogy_parse_place(name, fmt)
{
   var fmtparts = fmt.split(",");
   if (fmtparts == null || fmtparts.length == 0) {
	   return null;
   }
   for (var i=0; i < fmtparts.length; i++) {
	   fmtparts[i] = genealogy_trim(fmtparts[i]).toLowerCase();
   }
   var placePieces = [];
   var fmtitem = fmtparts.length-1;
   var nameparts = name.split(",");
   var namepart = '';
   var fmtpart = '';

   if (nameparts != null) {
	   for (var j=nameparts.length-1; j >= 0; j--) {
			namepart = genealogy_trim(nameparts[j]);
			fmtpart = fmtparts[fmtitem];
			placePieces[fmtpart] = namepart;
			fmtitem--;
			if (fmtitem < 0) {
				break;
			}
       }
   }
   while (fmtitem >= 0) {
		fmtpart = fmtparts[fmtitem];
		placePieces[fmtpart] = "";
		fmtitem--;
   }
   return placePieces;
}

function genealogy_parse_place_state(name, fmt)
{
	if (fmt == null || fmt.length == 0)	{
		fmt = "county, state";
	}
	var placePieces = genealogy_parse_place(name, fmt);
	return placePieces["state"];
}

function genealogy_parse_place_county(name, fmt)
{
	if (fmt == null || fmt.length == 0)	{
		fmt = "county, state";
	}
	var placePieces = genealogy_parse_place(name, fmt);
	return placePieces["county"];
}


function genealogy_build_url(cur_url, link_url)
{
	genealogy_alert("cur_url="+cur_url+", link_url="+link_url);
	if (link_url.match("://")) {
		// has a protocol, etc it's ok just return it
		return link_url;
	}
	if (link_url.match("^/")) {
		// absolute page but no domain name and protocol
		if (!cur_url.match(/.*?:\/\/(.*?)\//)) {
			return cur_url + link_url;
		} else {
			var url_parse_regex = new RegExp("(.*?):\/\/(.*?)\/");
			var url_parse_results = url_parse_regex.exec(cur_url);
			if (url_parse_results != null && url_parse_results.length == 3)	{
				return ""+url_parse_results[1]+"://"+url_parse_results[2]+link_url;
			} else {
				genealogy_error("Problem building url {cur_url: "+cur_url+", link_url: "+link_url);
				return link_url;
			}
		}
	} else if (link_url.match("^#")) {
		return cur_url + link_url;
	} 

	// it must be a relative link
	var last_backslash = cur_url.lastIndexOf('/');
	var last_period = cur_url.lastIndexOf('.');
	if (last_backslash > 6 && last_period > last_backslash) {
		// not the :// and has a filename at the end of the url
		return cur_url.substring(0, last_backslash+1)+link_url;
	} else {
		if (last_backslash == cur_url.length-1) {
			return cur_url+link_url;
		} else {
			return cur_url+"/"+link_url;
		}
	}

}
function genealogy_get_webpage(webpage_url, search_name, refresh_time)
{
	// TODO: caching - 
	//   if search_name not specified || 
	//      refresh_time == 0 || 
	//      search_name.cache does not exist || 
	//      (current date - search_name.cache date) > refresh_time)
	//   then Re-download from web
	return genealogy_webpage_get(webpage_url);
}

function genealogy_webpage_get(webpage_url)
{
	var xmlhttp = false;
	try	{
		xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");		
	} catch (e) {
		xmlhttp = false;
	}
	if (!xmlhttp) {
		return "";
	}
    xmlhttp.Open('GET',webpage_url,false);
	try	{
		xmlhttp.Send();
	}
	catch (e) {
		xmlhttp = null;
		return "";
	}
	var page_src = xmlhttp.responseText;
	xmlhttp = null;
	return page_src;
}

function genealogy_webpage_exists(webpage_url)
{
	var http_obj = new ActiveXObject("Microsoft.XMLHTTP");
	http_obj.Open('HEAD',webpage_url,false);
	try {
		http_obj.Send();
	}
	catch (e) {
		return false;
	}
	var status = http_obj.status;
	http_obj = null;
	return (status == 200) ? true : false;
}

function genealogy_webpage_get_links(webpage_src)
{
	var list = new Array();
	var links_regex = new RegExp("<a (.*?)>.*?<\/a>", "gim");
	var new_source = webpage_src.replace(/\r|\n/g, ' '); 
	var link = null;
	var i = 0;
	while ( (link = links_regex.exec(new_source)) != null) {
		list[i] = link[0];
		i++;
	}
	return list;
}

function genealogy_links_get_matching_named_links(links, matching_regex, not_matching_regex)
{
	var list = new Array();
	var j = 0;
	for (var i=0; i < links.length; i++) {
		var name = genealogy_link_get_name(links[i]);
		var matching = (matching_regex == null || (matching_regex != null && matching_regex.test(name))) ? true : false;
		var not_matching = (not_matching_regex == null || !not_matching_regex.test(name)) ? true : false;
		genealogy_alert("name: "+name+", matching: "+matching+", not_matching: "+not_matching);
		if (matching && not_matching) {
			list[j] = links[i];
			j++;
		}
	}
	return list;
}

function genealogy_link_get_href(link)
{
	var links_regex = new RegExp("href=\"(.*?)\"", "gim");
	var parts = links_regex.exec(link);
	if (parts != null && parts.length > 1) {
		return parts[1];
	}
	return "";
}

function genealogy_link_get_name(link)
{
	var links_regex = new RegExp("<a.*?>(.*?)<\/a>", "gim");
	var parts = links_regex.exec(link);
	if (parts != null && parts.length > 1) {
		// replace <br> tags with space, strip rest of tags, replace &nbsp; with space, replace &amp; with &, replace multiple spaces with one space
		var name = genealogy_trim(genealogy_strip_tags(parts[1].replace(/\<br\>/gim, " ")).replace(/&nbsp;/gim, " ").replace(/&amp;/gim, "&").replace(/\s{2,}/gim, " "));
		return name;
	}
	return "";
}

function genealogy_get_page_links(webpage_source, matching_name)
{
	var links = genealogy_webpage_get_links(webpage_source);
	if (matching_name != null && matching_name.length > 0){
		return genealogy_links_get_matching_named_links(links, new RegExp(matching_name, "i"), null);
	} else {
		return links;
	}
}

function genealogy_get_page_link_named(webpage_url, matching_name)
{
	var webpage = genealogy_get_webpage(webpage_url);
	var links = genealogy_get_page_links(webpage, matching_name);
	genealogy_log("done with genealogy_get_page_links for matching_name: "+matching_name);
	if (links != null && links.length > 0) {
		genealogy_log("calling genealogy_build_url for matching_name: "+matching_name);
		return genealogy_build_url(webpage_url, genealogy_link_get_href(links[0]));
	} else {
		return "";
	}
}

function genealogy_check_lib_version(required_version)
{
	var cur_version_parts = genealogy_lib_version.split(".");
	var required_version_parts = required_version.split(".");
	
	var major_cur_version = cur_version_parts[0];
	var minor_cur_version = (cur_version_parts.length > 1) ? cur_version_parts[1] : 0;

	var major_required_version = required_version_parts[0];
	var minor_required_version = (required_version_parts.length > 1) ? required_version_parts[1] : 0;
	
	if (major_cur_version >= major_required_version) {
		if (minor_cur_version >= minor_required_version) {
			return true;
		}
	}
	genealogy_error("This search requires version "+required_version+" of genealogy_lib.js");
	return false;
}

function genealogy_in_array(needle, haystack)
{
  if (haystack != null) {
	  for (var i=0; i < haystack.length; i++) {
		  if (needle.toUpperCase() == haystack[i].toUpperCase()) {
			  return 1;
		  }
	  }
  }
  return 0;
}

function genealogy_get_known_page_links(webpage_url, match_regex_str, search_name, refresh_time)
{
	var results = new Array();
	for (var i=0; i < genealogy_states.length + genealogy_countries.length; i++)
	{
		// initialize all to webpage_url
		results[i] = webpage_url;
	}
	var webpage_source = genealogy_get_webpage(webpage_url, search_name, refresh_time);
	var links_regex = new RegExp("<a.*?href=\"(.*?)\".*?>(.*?)<\/a>", "i");
	var replace_multiple_spaces = new RegExp("\\s{2,}", "gim");
	var replace_spaces_regex = new RegExp("_");
	var replace_nothing_regex = new RegExp("\n\.");
	var links = genealogy_get_page_links(webpage_source);
	if (links == null)
		return null;

	// setup match_regex if it's wanted
	var match_regex = null;
	if (match_regex_str != null && match_regex_str.length > 0)
	{
		match_regex = new RegExp(match_regex_str);
	}

	for (var i=0; i < links.length ; i++)
	{
		var link = links_regex.exec(links[i]);
		if (link == null || link.length != 3)
			continue;
		genealogy_log(link[2] + " "+link[1]);
		var place = genealogy_trim(genealogy_strip_tags(link[2].replace(replace_spaces_regex, " ").replace(replace_nothing_regex, "")).replace(replace_multiple_spaces, " "));
		if (match_regex != null) {
			if (!place.match(match_regex))
				continue;
			var place_results = match_regex.exec(place);
			if (place_results != null && place_results.length > 1)
			{
				place = place_results[1];
			}
		}
		genealogy_log("\""+place+"\"");
		var place_num = genealogy_lookup_place(place);
		if (place_num == -1)
			continue;

		var place_url = genealogy_build_url(webpage_url, link[1]);

		results[place_num] = place_url;
	}
	return results;
}

function genealogy_get_month_length(month, year)
{
	var len;
	if(month==1 || month==3 || month==5 || month==7 || month==8 || month==10||month==12) {
		len = 31;
	} else {
		if(month==2) {
			len = 28;
			if(!(year%4) && (year%100 || !(year%400)))
				len++;
		} else {
			len = 30;
		}
   }
   return len;    
}


function genealogy_age_calc(startDate, endDate)
{
	if (endDate == null || endDate == "" || typeof endDate == "undefined") {
		endDate = new Date();
	}

	var startDay = startDate.getDate();
	var startMonth = startDate.getMonth()+1;
	var startYear = startDate.getFullYear();

	var endDay = endDate.getDate();
	var endMonth = endDate.getMonth()+1;
	var endYear = endDate.getFullYear();


	var ageDays = endDay - startDay;
	if (ageDays < 0) {
		endMonth--;
		if (endMonth < 1) {
			endYear--;
			endMonth += 12;
		}
		var monthLength = genealogy_get_month_length(endMonth, endYear);
		ageDays += monthLength;
	}

	var ageMonths = endMonth - startMonth;
	if (ageMonths < 0) {
		endYear--;
		ageMonths += 12;
	}

	var ageYears = endYear - startYear;
	var result = "";
	if (ageYears > 0){
		result += (ageYears  + " year"  + ((ageYears != 1)  ? "s" : "") + ", ");
	}
	if (ageMonths > 0 || result != "") {
		result += (ageMonths + " month" + ((ageMonths != 1) ? "s" : "") + ", "); 
	}
	result += (ageDays   + " day"   + ((ageDays != 1)   ? "s" : ""));

	return result;
}

function genealogy_parse_date(dateString)
{
	var year;
	var month;
	var day;
	var r;
	var valid_fmt = false;
	var dateFmt1RegExp = new RegExp(genealogy_date_format_1);
	var dateFmt2RegExp = new RegExp(genealogy_date_format_2);
	if ( (r = dateFmt1RegExp.exec(dateString)) != null) {
		month = r[1]-1;
		day = r[2];
		year = r[3];
		valid_fmt = true;
	} else if ( (r = dateFmt2RegExp.exec(dateString)) != null) {
		day = r[1];
		month = -1;
		year = r[3];
		var monthName = (r[2]).toUpperCase();
		for (var i=0; i < genealogy_month_names.length; i++) {
			if (monthName == genealogy_month_names[i] ||
				monthName == genealogy_month_names[i].substring(0,3)) {
				month = i;
				valid_fmt = true;
				break;
			}
		}
	}
	if (valid_fmt) {
		if (month < 0 || month > 11) {
			valid_fmt = false;
		}
	}
	if (valid_fmt) {
		return new Date(year, month, day);
	} else {
		return false;
	}
}

genealogy_alert("genealogy_lib.js loaded");