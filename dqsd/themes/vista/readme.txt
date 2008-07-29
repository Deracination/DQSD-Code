/***********************************************************
* Readme File for updating DQSD look and feel on Windows
* Vista. This document, and the files it describes, are
* provided as is, with no warrantee expressed or implied. 
* But hey, it worked for me, and it looks a lot better than
* it did before!
*
* Version: 1.0, created 28 December, 2006 by Charlie Russel
* ModHist: 1.1, added alternate widescreen theme (cpr)
* 
*/

The default colour schemes shipped with DQSD are designed to look 
right on Windows XP. But Windows Vista has an all new look and feel
and the colours for DQSD just don't look good on Windows Vista.
Actually, if you've installed DQSD on Vista, you'll already know
that they're just plain butt ugly. 

The included files here will fix that. They're not perfect, but they're 
a whole lot better than the defaults. I did the first pass at fixing
this colour problem, and then got some great help from Matt. The
result is what you see in this group of files. There should be five
files here. This readme, plus:
   Vistatoolbar1_vista.bmp
   Vistatoolbar2_vista.bmp
   Theme_vista.css
   Theme_vista_widescreen_alt.css
   DQSD.png

The file that does all the heavy lifting is Theme_Vista.css, or the 
alternate file Theme_vista_widescreen_alt.css. These are the files that
control the theme for DQSD and set all the colours and stuff. The alternate
version uses essentially similar settings, but with a larger font since 
screen real estate is less of an issue. Borders are also adjusted.

The changed DQSD.png file changes the colour of the search icon on the left 
of the DQSD taskbar to blend into the Vista background. You may want to 
preserve the old one rather than just overwrite it, if you like a more
pronounced icon, but I think the new one looks better in this theme.

In Windows XP and earlier versions of Windows, you could use the built-in
abilities of DQSD to change your theme on the fly. Unfortunately, with 
Windows Vista you can't easily change themes on the fly - User Account 
Control (UAC) gets in the way. And you should not turn that off. So, what
you need to do is first save off your existing Theme.css file to something 
that won't get overwritten.

Next, copy the 6 files here to your default DQSD installation directory. 
(Normally, C:\Program Files\Quick Search Deskbar.) Now, replace your 
exisiting Theme.css with the Theme_Vista.css (or alternate widescreen 
version) file by deleting Theme.css and renaming Theme_vista.css (or 
Theme_vista_widescreen_alt.css) to Theme.css. 

Once you've done that, simply reload DQSD using the ! command. Or 
click on the chevron (">>") and select Configure from the menu. 

That should be sufficient. But if it still doesn't look quite right, 
just reboot and it should be fine. Enjoy. Please post any comments, 
suggestions, or bugs to the DQSD Users list. 

Charlie. 
