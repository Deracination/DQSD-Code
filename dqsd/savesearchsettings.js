var dirty = false;

function saveSearchSettings()
{
  if ( !dirty )
    return;
  
  var toggles = document.all('search_enabled');
  var disabled = new Array();
  for ( var i = 0; i < toggles.length; i++ )
  {
    if ( !toggles[i].checked )
      disabled.push( toggles[i].name );
    opener.searches[toggles[i].name].enabled = toggles[i].checked;
  }
  opener.writeFile( "disabledsearches.txt", disabled.join("\n") );
}

function toggleEnabledSearch()
{
  dirty = true;
  var el = event.srcElement;
  while ( el.tagName != 'TR' )
    el = el.parentElement;
    
  el.className = event.srcElement.checked ? '' : 'disabledSearch';
}