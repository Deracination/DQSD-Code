    var genealogy_debug = 0;

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
	"RHODE_ISLAND",
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
	"ME", "MD", "MA", "MI", "MN", "MI", "MS", "MT",
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
	}

	// it must be a relative link
	var last_backslash = cur_url.lastIndexOf('/');
	if (last_backslash > 5) {
		// not the :// <-
		return cur_url.substring(0, last_backslash+1)+link_url;
	} else {
		genealogy_error("Problem building url {cur_url: "+cur_url+", link_url: "+link_url);
		return link_url;
	}

}


function genealogy_get_webpage(webpage_url, search_name, refresh_time)
{
	// if search_name not specified || refresh_time == 0 || search_name.cache does not exist || (current date - search_name.cache date) > refresh_time) Re-download from web
	var http_obj = new ActiveXObject("Microsoft.XMLHTTP");
	http_obj.Open('GET',webpage_url,false);
	http_obj.Send();
	return http_obj.responseText;
}

function genealogy_get_page_links(webpage_source)
{
	var links_regex = new RegExp("<a href=\"(.*?)\".*?>(.*?)<\/a>", "gim");
	//var links_regex = new RegExp("<a.*?\/a>", "gim");
	var link;
	var arr = new Array();

	// replace cr/lf with space for regex to work better
	var new_source = webpage_source.replace(/\r|\n/g, ' '); 

	// add links to array
	var i = 0;
	while ( (link = links_regex.exec(new_source)) != null) {
		arr[i] = link[0];
		i++;
	}
	return arr;
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
	var links_regex = new RegExp("<a href=\"(.*?)\".*?>(.*?)<\/a>", "i");
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
		genealogy_alert(link[2] + " "+link[1]);
		var place = genealogy_trim(genealogy_strip_tags(link[2].replace(replace_spaces_regex, " ").replace(replace_nothing_regex, "")));
		if (match_regex != null) {
			if (!place.match(match_regex))
				continue;
			var place_results = match_regex.exec(place);
			if (place_results != null && place_results.length > 1)
			{
				place = place_results[1];
			}
		}
		genealogy_alert("\""+place+"\"");
		var place_num = genealogy_lookup_place(place);
		if (place_num == -1)
			continue;

		var place_url = genealogy_build_url(webpage_url, link[1]);

		results[place_num] = place_url;
	}
	return results;
}

genealogy_alert("genealogy_lib.js loaded");