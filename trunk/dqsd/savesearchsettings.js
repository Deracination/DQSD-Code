function saveSearchSettings()
{
//  alert('in saveSearchSettings.js: saveSearchSettings');
}

function toggleEnabledSearch()
{
  //alert( event.srcElement.tagName );
  var el = event.srcElement;
  while ( el.tagName != 'TR' )
    el = el.parentElement;
    
  if ( event.srcElement.checked )
    el.className = '';
  else
    el.className = 'disabledSearch';
}