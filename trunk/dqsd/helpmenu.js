function registerMenuHook( menuHook )
{
  if ( typeof menuHooks == 'undefined' )
    menuHooks = new Array();
    
  menuHooks.push( menuHook );
}

function searchCompareWithSubcatPath( s1, s2 )
{
  var s1_subcatPath = "";
  var s2_subcatPath = "";

  if ( s1.subcats && s1.subcats.length >= 1 ) {
    for ( var isubmenu = 0; isubmenu < s1.subcats.length; isubmenu++ ) {
      s1_subcatPath += '~' + s1.subcats[ isubmenu ].toLowerCase();
	}
  }

  if ( s2.subcats && s2.subcats.length >= 1 ) {
    for ( var isubmenu = 0; isubmenu < s2.subcats.length; isubmenu++ ) {
      s2_subcatPath += '~' + s2.subcats[ isubmenu ].toLowerCase();
	}
  }

  if (s1_subcatPath == "" && s2_subcatPath != "") return 1;
  if (s1_subcatPath != "" && s2_subcatPath == "") return -1;
  if (s1_subcatPath < s2_subcatPath) return -1;
  if (s1_subcatPath > s2_subcatPath) return 1;

  return searchCompare(s1, s2)
}

function showpop()
{
  document.deff.q.focus();
  document.deff.q.createTextRange().select();
  var mb = new ActiveXObject("DQSDTools.MenuBuilder");

  // Align the menu with the button
  mb.HorizontalAlignment = ( buttonalign == 'left' ? 1 : 2 ); // 1 = left, 2 = right (default)
  
  mb.AppendMenuItem( local(ABOUT_TEXT), "about", "Help on using Dave's Quick Search Deskbar" );
  mb.AppendMenuItem( "Check for update...", "checkWebForUpdateNotifyAll", "Check for update to Dave's Quick Search Deskbar" );
  mb.AppendSeparator( 0 );
  
  menuHandles = new Object();

  for (i = 0; i < categoryarray.length; i++)
  {
    var categoryName = categoryarray[i];
    var helparray = categories[categoryName];
    
    // Are there any enabled searches in this category?
    for (var k = 0; k < helparray.length && (!helparray[k].enabled || helparray[k]).nomenu; k++)
    {
    }

    // If all the searches are disabled in the category, don't add a submenu
    if ( k == helparray.length )
      continue;

    var hcatmenu = mb.AppendSubMenu( categoryName );
    helparray.sort( searchCompareWithSubcatPath );
    for (var k = 0; k < helparray.length; k++)
    {
      var search = helparray[k];
      var alias = getSearchAliases( search );

      if ( search.enabled && !search.nomenu )
      {
      
        // If there are subcategories, nest them
        if ( search.subcats && search.subcats.length >= 1 )
        {
          subcatPath = categoryName;
          hsubmenu = hcatmenu;
          for ( var isubmenu = 0; isubmenu < search.subcats.length; isubmenu++ )
          {
            subcatPath += '~' + search.subcats[ isubmenu ];
            if ( menuHandles[ subcatPath ] )
              hsubmenu = menuHandles[ subcatPath ]
            else
            {
              hsubmenu = mb.AppendSubMenu( search.subcats[ isubmenu ], hsubmenu );
              menuHandles[ subcatPath ] = hsubmenu;
            }
          }
        }
        else // no subcategories, append menu items
        {
          hsubmenu = hcatmenu;
        }
        
        // Remove HTML tags from name
        var searchname = search.name.replace( /<[^<]*>/g, '' );
      
        mb.AppendMenuItem( searchname + (search.local ? ' [local]' : '' )  + '\t' + (search.aliasmenudisplay ? alias.replace(/&/g, '&&') : ""),  // menu text along with alias
                           search.aliases[0],         // function invoked when user selects menu item
                           makeToolTipString(search), 
                           hsubmenu );
      }
    }
  }
  
  // Add any hooks created by searches or add-ons
  if ( typeof menuHooks != 'undefined' )
  {
    for ( var i = 0; i < menuHooks.length; i++ )
    {
      menuHooks[i]( mb );        
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
    if (searches[fn])
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
  if ( helpMenuToolTipsEnabled )
  {
    if ( search.desc != '' && search.desc != search.name )
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
  }
  return tip;
}