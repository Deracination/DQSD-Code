RELEASE PROCEDURE FOR DQSD - Kim Gräsman, December 2003
========================================================================
- Prepare for build
	- Update version resource in DQSDTools DLL
	- Make sure the paths in build.cmd match your private VC6 and 
		NSIS paths

- Update version stuff in scripts
	- Update version in version.xml
	- Set date for latest changes in ChangeLog.txt
	- Commit changes

- Build DLL and installer
	- Run build.cmd
	or
	- Build manually
		- Build DQSDTools.dll from inside Visual Studio
		- NSIS-compile search.nsi
	- Generates dqsd.exe
	- Rename to dqsd-<ver>-<type>.exe, e.g. dqsd-318-beta.exe or 
		dqsd-318.exe for a final version
		
- Test newly-built installer

- Put a new release up on SF
	- Transfer installer to SF via anonymous FTP
		- Connect to ftp://upload.sourceforge.net/ as anonymous
		- Upload your installer (e.g. dqsd-318-beta.exe)
	- Go to http://sourceforge.net/projects/dqsd
	- Admin -> File Releases
	- Add Release
		- Name it "<version> <type>", e.g. "3.1.8.0 beta" or "3.1.8.0" 
			for a final version
		- Set status to Hidden
		- Paste in change log from ChangeLog.txt
		- [Submit/Refresh]
	- Attach file
		- Your uploaded installer should be listed under Step 2, check 
			the filename
		- [Add Files and/or Refresh View]
	- Set file metadata
		- Processor: i386
		- Release Date: Today's date
		- File Type: .exe (32-bit Windows)
		- [Update/Refresh]
	- Activate Release
		- Set status to Active
		- [Submit/Refresh]
	- [Send Notice]
	
- Commit all DQSD files
- Set a CVS tag on the DQSD module called rel-<ver>-<type>, e.g. 
	"rel-3-1-8-0-beta" or "rel-3-1-8-0" for a final version

- Update the site
	- Check out the dqsdweb module from CVS
	- Modify doc.htm
		- If this is a beta, point the beta link to the 
			Sourceforge release
		- If this is a final version, comment out previous beta link 
			and change version number of link
	- If final version
		- Upload dqsd.exe (latest release must have this name)
		- Upload dqsd<ver>.exe, e.g. dqsd318.exe
		- Add general release notes under What's New
  - Copy version.xml from dqsd module
	- Commit changes to CVS - the files will be automatically distributed
		to dqsd.net and update notification will be triggered by the new 
		version.xml, in due time.

- Send an announcement to dqsd-announce@lists.sourceforge.net 
	(see: http://sourceforge.net/mailarchive/forum.php?forum_id=8603)