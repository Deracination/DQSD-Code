function bookmarks_get_webpage(webpage_url, cache_file_name, refresh_time)
{
	// if cache_file_name not specified || refresh_time == 0 || cache_file_name does not exist || (current date - cache_file_name date) > refresh_time) Re-download from web
	var http_obj = new ActiveXObject("Microsoft.XMLHTTP");
	http_obj.Open('GET',webpage_url,false);
	try	{
  	  http_obj.Send();
	  return http_obj.responseText;
	} catch (e) {
	  return "";
	}
}

function bookmarks_get_file(filename)
{
	return readFile(filename);
}

function bookmarks_get_bookmarks_array_from_ie(fav_location) 
{
	var fso = new ActiveXObject("Scripting.FileSystemObject");
	var arr = new Array();
	bookmarks_process_ie_favorites_folder(fso, fso.GetFolder(fav_location), arr, 0);
	fso = null;
	return arr;
}

function bookmarks_process_ie_favorites_folder(fso, folder, bookmarks_array, level) 
{
    var fsoForReading = 1;
	var url_regexp = new RegExp(/URL=([^\n]+)/gim); 
	var fc, ff;

	fc = new Enumerator(folder.SubFolders);
	for (; !fc.atEnd(); fc.moveNext()) {
		var arr_item = "";
		for (var j=0; j < level; j++) {
		  arr_item += "\t";
		}
		arr_item += "Folder";
		arr_item += "\t";
		arr_item += fso.GetBaseName(fc.item());
		bookmarks_array.push(arr_item);
		bookmarks_process_ie_favorites_folder(fso, fc.item(), bookmarks_array, level+1);
	}

	ff = new Enumerator(folder.Files);
	for (; !ff.atEnd(); ff.moveNext()) {
		if (fso.GetExtensionName(ff.item()).toUpperCase() != "URL") {
			continue;
		}
		var objTextStream = fso.OpenTextFile (ff.item(), fsoForReading);
		var cur_url = ff.item();
		while (!objTextStream.AtEndOfStream) {
			var line = objTextStream.ReadLine();
			var url_results = url_regexp.exec(line);
			if (url_results != null) {
				cur_url = url_results[1];
				break;
			}
		}

		var arr_item = "";
		for (var j=0; j < level; j++) {
		  arr_item += "\t";
		}
		arr_item += cur_url;
		arr_item += "\t";
		arr_item += fso.GetBaseName(ff.item());
		bookmarks_array.push(arr_item);
	}
}


function bookmarks_get_bookmarks_array_from_netscape(webpage_source)
{
	// tokenize the page
    var parsed_items = new Array();
    var i = 0;
	var max_len = webpage_source.length;
	var cur_item = '';
	var cur_item_start = 0;
	var cur_item_end = 0;
	var ch = '';

	while (i < max_len) {
		ch = webpage_source.charAt(i);
		if (ch == '<') {
			if (cur_item_end > cur_item_start) {
				cur_item = webpage_source.substring(cur_item_start, cur_item_end);
				parsed_items.push(cur_item.replace(/\n\t/, " "));
			}
			cur_item_start = i;
			cur_item_end = cur_item_start;
		} else if (ch == '>') {
			cur_item_end = i+1;
			cur_item = webpage_source.substring(cur_item_start, cur_item_end);
			parsed_items.push(cur_item.replace(/\n\t/, " "));
			cur_item_start = i+1;
			cur_item_end = cur_item_start;
		} else {
			if (ch == '\n' || ch == '\r') {
			  if (cur_item_start == i) {
				  cur_item_start++;
				  cur_item_end = cur_item_start;
			  } else {
				  cur_item_end++;
			  }
			} else if (ch == '\t' || ch == ' ') {
				if (cur_item_start == i) {
					cur_item_start++;
				    cur_item_end = cur_item_start;
				} else {
                    cur_item_end++;
				}
			} else {
			  cur_item_end++;
			}
		}
		i++;
	}
	if (cur_item_end > cur_item_start) {
      cur_item = webpage_source.substring(cur_item_start, cur_item_end);
	  parsed_items.push(cur_item.replace(/\n\t/, " "));
	}

    // assemble the array of levels, bookmarks, and folders
	var arr = new Array();
	var arr_count = 0;
	var level = 0;
	var last_tag = '';
	var cur_tag = '';
	var cur_href = '';
	var first_dl = true;
	var tag_item = null;
	var state = 0; // 0=nothing, 1=folder, 2=bookmark
	var tag_regexp = new RegExp(/^\<\s*([\/]?\w+)\b(.*?)\>/);
	var href_regexp = new RegExp(/href=\"(.*?)\"/i);
	for (i = 0; i < parsed_items.length; i++) {
		cur_item = parsed_items[i];
		tag_item = tag_regexp.exec(cur_item);
		if (tag_item != null) {
		    // tag
			last_tag = cur_tag;
			cur_tag = tag_item[1].toLowerCase();
			if (cur_tag == 'dl') {
				if (first_dl) {
					// first dl is still level 0
					first_dl = false;
				} else {
					level++;
				}
				state = 1;
			} else if (cur_tag == '/dl') {
				level--;
                if (level >= 0) {
					state = 1;
                } else {
                    state = 0;
				}
			} else if (cur_tag == 'dt')	{
				// assume folder until you hit an a tag
				state = 1;
				cur_href = "Folder";
			} else if (cur_tag == 'dd')	{
				// ignore description
				state = 0;
			} else if (cur_tag == 'a') {
				// bookmark
				state = 2;
				cur_href = '';
                var href_results = href_regexp.exec(tag_item[2]);
				if (href_results != null) {
				  cur_href = href_results[1];
				} else {
				  cur_href="Failed";
				}
			}
		} else {
			// text
			if (state == 1 || state == 2) {
			  var arr_item = "";
			  for (var j=0; j < level; j++) {
				  arr_item += "\t";
			  }
			  arr_item += cur_href;
			  arr_item += "\t";
			  arr_item += cur_item.replace("&amp;", "and");
			  //alert(""+level+", "+cur_item);
			  arr[arr_count++] = arr_item;
			}
		}
	}

    // clear parsed items
    parsed_items = null;

	return arr;
}

function bookmarks_get_menu(bookmarks_array)
{
	// build the menu
	var mb = new ActiveXObject("DQSDTools.MenuBuilder");
    // Align the menu with the button
    mb.HorizontalAlignment = ( buttonalign == 'left' ? 1 : 2 ); // 1 = left, 2 = right (default)

    var hsubmenu_stack = new Array();
	var hsubmenu = null;
    var last_level = 0;
    var arr_regexp = new RegExp(/(\t*)([^\t]+)\t(.*)/);

	hsubmenu_stack[last_level] = null;
	for (i=0; i < bookmarks_array.length; i++) {
		var arr_item = arr_regexp.exec(bookmarks_array[i]);
		if (arr_item == null || arr_item.length < 4)
			continue;

		var cur_level = arr_item[1].length;
		var cur_url = arr_item[2];
		var cur_name = arr_item[3];

		hsubmenu = hsubmenu_stack[cur_level];
		if (cur_url == 'Failed') {
			continue;
		} else if (cur_url == 'Folder') {
			hsubmenu = mb.AppendSubMenu(cur_name, hsubmenu);
			hsubmenu_stack[cur_level+1] = hsubmenu;
		} else {
			mb.AppendMenuItem(cur_name, cur_url, '', hsubmenu);
		}
	}
    return mb;
}

function bookmarks_show_menu(bookmarks_array)
{
	var bookmarks_menu = bookmarks_get_menu(bookmarks_array);
	if (bookmarks_menu != null) {
		var fn = bookmarks_menu.Display(document);
		if (fn) {
			openSearchWindow(fn);
		}
		bookmarks_menu = null;
	}

}