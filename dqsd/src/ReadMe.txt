Build Instructions

- Update version resource in DQSDTools DLL

- Update version in version.xml (DON'T CHECK IN YET!)

- If the public methods of DQSDTools have changed and any
  script requires those new methods, then update version
  numbers in the VersionIsCorrect call.
  
- Build the install, put a new release up on SF

- Check in the version.xml.  This will cause the update
  notifications to start happening.