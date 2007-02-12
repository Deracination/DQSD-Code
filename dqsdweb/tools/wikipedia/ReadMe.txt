There are two files... 
- search_wikipedia.html
- search_wikipedia_ie_context_menu.reg

The .reg file adds a context menu item "Search Wikipedia" to Internet
Explorer when you have text selected in the browser. I've only tested this
on XP, but it appears to work ok with Avante and IE.

Installation...
- Extract the files to some directory
- Edit the .reg file and correct the path to the extracted .html file (or 
just go ahead and merge to the registry and make the changes thru regedit).  
I.e., if you extracted the files to "C:\Documents and Settings\Glenn\My Documents\search_wikipedia.html", 
change the path in the .reg to "C:\\Documents and Settings\\Glenn\\My Documents\\search_wikipedia.html".
- Double click the .reg file to make the changes in the registry.
- You'll need to restart any open browser windows.
- With Avante I had to create an popup exception rule for *wikipedia* windows.