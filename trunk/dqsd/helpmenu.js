function showpop()
{
  document.deff.q.focus();
  document.deff.q.createTextRange().select();
  var mb = new ActiveXObject("DQSDTools.MenuBuilder");

  // Align the menu with the button
  mb.HorizontalAlignment = ( buttonalign == 'left' ? 1 : 2 ); // 1 = left, 2 = right (default)
  
  mb.AppendMenuItem( local(ABOUT_TEXT), "about", "Help on using Dave's Quick Search Deskbar" );
  mb.AppendSeparator( 0 );
  for (i = 0; i < categoryarray.length; i++)
  {
    var categoryName = categoryarray[i];
    var helparray = categories[categoryName];
    
    // Are there any enabled searches in this category?
    for (var k = 0; k < helparray.length && !helparray[k].enabled; k++)
    {
    }

    // If all the searches are disabled in the category, don't add a submenu
    if ( k == helparray.length )
      continue;

    var hsubmenu = mb.AppendSubMenu( categoryName );
    helparray.sort( searchCompare );
    for (var k = 0; k < helparray.length; k++)
    {
      var search = helparray[k];
      var alias = getSearchAliases( search );

      if ( search.enabled )
        mb.AppendMenuItem( search.name + '\t' + alias.replace(/&/g, '&&'), search.aliases[0], makeToolTipString(search), hsubmenu );
    }
  }
  
  appendMRUMenuSelections( mb );

  if(helpMenuToolTipsEnabled)
  {
    try
    {
      if(typeof helpMenuToolTipsDisplayTime == 'undefined')
      {
         helpMenuToolTipsDisplayTime = 3;
      }

      mb.InitialiseTooltips(helpMenuToolTipsDisplayTime);
    }
    catch(e)
    {
      alert("Error initialising tooltips for menus: " + e.description);
    }
  }

  var fn = mb.Display(document);
  if ( fn )
  {
    var alias = null;
    if ( isInternalSearch( fn ) )
      alias = searches[fn].aliases[1];
    mnu( fn, alias );
    if ( fn != 'about' )
      saveMenuHistory( alias ? alias : fn );
  }
  return false;
}

function saveMenuHistory( alias )
{
  var menuContent = null;
  try
  {
    menuContent = readFile( "mrumenu.txt" );
  }
  catch (e) {}
  var menus = menuContent ? menuContent.replace(/\r\n/g, '\n').split('\n') : new Array(0);
  
  var exists = false;
  var newmenus = new Array(0);
  for ( var i = 0; i < menus.length; i++ )
  {
    if ( menus[i].toLowerCase() != alias )
      newmenus.push( menus[i] );
    else
      exists = true;
  }
  if ( exists )
    menus = (alias + '\n' + newmenus.join('\n')).split('\n');
  
  menus.push( alias );
  if ( menus.length > menuMRUlength )
    menus.splice( 0, menus.length - menuMRUlength );
  writeFile( "mrumenu.txt", menus.join('\r\n') );
}

function appendMRUMenuSelections( mb )
{
  var menuContent = null;
  try
  {
    menuContent = readFile( "mrumenu.txt" );
  }
  catch (e) {}
  if ( menuContent )
  {
    mb.AppendSeparator( 0 );
    var menus = menuContent.replace(/\r\n/g, '\n').split('\n');
    for ( var i = 0; i < menus.length; i++ )
    {
      for ( var iSearch in searches )
      {
        var search = searches[iSearch];
        if ( search.aliases )
        for ( var iAlias = 0; iAlias < search.aliases.length; iAlias++ )
        {
          if ( menus[i] == search.aliases[iAlias] )
          {
            var alias = getSearchAliases( search );
            
            // Create 'mneumonic' using the last digit of the number
            var mnemonic = (menus.length - i).toString();
            mnemonic = mnemonic.substr( 0, mnemonic.length - 1 ) + "&" + mnemonic.substr( mnemonic.length - 1 );
            mb.AppendMenuItem( mnemonic + "  " + search.name + '\t' + alias, search.aliases[0], makeToolTipString(search) );
            break;
          }
        }
      }
    }
  }
  
}

function makeToolTipString(search)
{
  var tip = '';
  if(helpMenuToolTipsEnabled && search.desc != '')
  {
    tip = search.desc
                .replace( /<[^>]+>/g, '' )          // any tags
                .replace( /(^\s*)|(\s*$)/g, '' )    // leading/trailing whitespace
                .replace( /\r\n\s*\r\n/g, '\r\n' )  // blank lines
                .replace( /&gt;/g, '>' )            // HTML gunk
                .replace( /&lt;/g, '<' )
                .replace( /&amp;/g, '&' )
                .replace( /\t/g, '    ' )           // reduce indentation
  }
  return tip;
}
