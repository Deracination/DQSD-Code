function versionCheck()
{
  var bSuccess = true;
  var testObject;

  // First, see if we can create the object at all
  try
  {
    testObject = new ActiveXObject("DQSDTools.Launcher");
  }
  catch(e)
  {
    alert("The DQSD helper DLL is not correctly installed");
    bSuccess = false;
  }

  // Try a version query on it
  if(bSuccess)
  {
    try
    {
      if(!testObject.VersionIsCorrect(2,5,8,9))
      {
        throw "DQSDTools out of date";
      }
    }
    catch(e)
    {
      alert("The DQSD helper DLL is out of date");
      bSuccess = false;
    }
  }
  testObject = null;


  return bSuccess;
}

// Check for the correct ActiveX DLL version
if(!versionCheck())
{
  // This is not a nice thing to do, but does reinforce the fact that you
  // can't use the bar like it is
  // Unfortunately, you can't do a toolbar refresh to recover from this
  // so it might not be a great idea
  window.close();
}
