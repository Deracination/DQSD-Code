// Load the contents of search.xml, aliases, and menu files

function addsearch(fname, name, desc, link, cat, local, subcats, nomenu)
{
  try
  {
    searches[fname] = {fname:fname, 
                       name:name, 
                       desc:desc, 
                       link:link, 
                       cat:cat, 
                       subcats:subcats,
                       fun:eval(fname), 
                       aliases:[], 
                       enabled:!disabledsearches[fname], 
                       aliasmenudisplay:true, 
                       local:(typeof local != 'undefined' ? local : false),
                       nomenu:(typeof nomenu != 'undefined' ? nomenu : false)};
    if( !aliases[fname] )
      addalias( fname, fname );
    addhelp( searches[fname] );
  }
  catch( except )
  {
    qualifiedalert("Error adding search: " + except.description + 
          "\n\nfunction: \"" + fname + "\"\nname: \"" + name + "\"\ndescription: \"" + desc + "\"");
  }
}

internalShortcutIndex = 0;
INTERNAL_FUNC_PREFIX = "_dqsd_internal_fn_";

function addalias(alias, fname, name, desc, cat, subcats)
{
  // If this alias is already defined, then remove it from the search it was previously defined for
  // so that the last alias definition wins (i.e., localaliases.txt overrides aliases.txt)
  if (aliases[alias])
  {
    try 
    {
      var prevfname = aliases[alias];
      var newaliases = [];
      for ( var i = 0; i < searches[prevfname].aliases.length; i++ )
      {
        if ( searches[prevfname].aliases[i] != alias )
        {
          newaliases.push( searches[prevfname].aliases[i] );
        }
      }
      // Gotta have one alias; even if the local alias overrides a non-local alias
      if ( newaliases.length > 0 )
      {
        searches[prevfname].aliases = newaliases;
      }
      else
      {
        // reset the one and only alias to the search name
        // but don't display the alias on the menu because
        // there's a local alias that is overriding it.
        searches[prevfname].aliases = [ prevfname ];
        searches[prevfname].aliasmenudisplay = false;
      }
    }
    catch (e)
    {
      alert("Error adding alias " + alias + " for function " + fname + ": " + e.description);
      return;
    }
  }

  if (!searches[fname]) // no matching searches
  {
    var res;
    if (aliases[fname])
    {
      fname=aliases[fname];
    }
    else if (fname.search(/^\w+:/) >= 0) // resembles URL
    {
      var url = fname;
      fname = INTERNAL_FUNC_PREFIX + (++internalShortcutIndex);
      var f = new Function("t", 
                           "var url = unescape('" + escape(url) + "');" +
                           "if (url.search(/%s/) >= 0 && t == '')" + 
                           "  alert('An argument is required for " + alias + "  (" + url + ")');" + 
                           "else" + 
                           "  direct(url.replace( /%s/g, t ));"
                          );
      eval( fname + " = f;" );
      addsearch( fname, name ? name : url, desc ? desc : url, url.search(/%s/) < 0 ? url : "", cat, true, subcats);
    }
    else if ((res = fname.match(/^(\w+) +(.+)/)) && searches[res[1]]) // starts with a valid search function
    {
      var cmd = fname;
      fname = INTERNAL_FUNC_PREFIX + (++internalShortcutIndex);
      var f = new Function("t", 
                           "var cmd = unescape('" + escape(res[2]) + "');" +
                           res[1] + "(cmd.replace( /%s/g, t ));"
                          );
      eval( fname + " = f;" );
      addsearch( fname, name ? name : cmd, desc ? desc : cmd, "", cat, true, subcats);
    }
    else
    {
      qualifiedalert("Cannot add alias " + alias + " -> " + fname + ":\nThere is no search " + fname + ".");
      return;
    }
  }

  aliases[alias] = fname;
  searches[fname].aliases.push(alias); 
  if (fname == "ia" || fname == "ggc")
    specialaliasarray.push(alias);
}

function addhelp(search)
{
  var cat = (search.cat ? search.cat : "Other");
  if (!categories[cat])
  {
    categories[cat] = [];
    categoryarray.push(cat);
    //sort the list of categories
    categoryarray.sort(); 
  }
  var helparray = categories[cat];
  helparray.push(search);
}


// the following structures will be defined

var searches = {};
var aliases = {};
var specialaliasarray = [];
var menuarray = [];
var categories = {};
var categoryarray = [];

// 0. Get disabled searches

var disabledfnames = readTabDelimitedFile("disabledsearches.txt");
var disabledsearches = new Object();
var xpathquery = '';
var delim = ''
for ( var i = 0; i < disabledfnames.length; i++ )
{
  disabledsearches[disabledfnames[i]] = true;
  xpathquery += delim + "@function!='" + disabledfnames[i] + "'";
  delim = ' and ';
}

var searchRoot = null;

// Create an empty search root
if (!searchRoot)
{
  var searchDOM = getMSXMLDOMDocumentInstance();
  searchDOM.async = false;
  searchRoot = searchDOM.createElement("searches");
}

try
{
  var xmldoc = getMSXMLDOMDocumentInstance();
  xmldoc.async = false;

  // Get optional searches in localsearch.xml
  var localSearches = document.all("localsearch").selectNodes("/searches/search");
  if (localSearches)
  {
    var localSearchFile = document.all("localsearch").src;
    if (!xmldoc.loadXML(readFile(localSearchFile)))
    {
      alert('Unable to load search from ' + localSearchFile + ':  ' + xmldoc.parseError.reason );
    }
    else
    {
      for (var iPrivate = 0; iPrivate < localSearches.length; iPrivate++)
      {
        var searchNode = localSearches[iPrivate];
        if (alertmode && searchNode.selectSingleNode("FORM") && !searchNode.selectSingleNode("form"))
        {
          qualifiedalert('Search "' + searchNode.attributes.getNamedItem("function").text + '" needs lowercase <form> element');
        }
        else
        {
          searchRoot.appendChild(localSearches[iPrivate]);
        }
      }
    }
  }
}
catch (except) {}

// Load searches from search directory
loadSearchesFromDir( "searches" );

// Load optional add-ons from addons directory
loadAddons();

// Load local searches
loadSearchesFromDir( "localsearches", true );

// 2. eval all the scripts and doc.write all the forms

if (searchRoot)
{

// ??? This XQuery will only allow the searches that are enabled to be evaluated.  This should
// decrease load time, but it needs to be tested and determined if it makes sense first.
//  var xscripts = searchRoot.selectNodes("search[" + xpathquery + "]/script");

  var xscripts = searchRoot.selectNodes("search/script");
  var loadedScripts = new Object();
  for (var iPrivate = 0; iPrivate < xscripts.length; iPrivate++)
  {
    // ??? This seems like a hack, but the only way I can determine to load and evaluate
    // external scripts (referenced with the 'src' attribute), is by manually loading them.
    var externalScriptRef = xscripts[iPrivate].attributes.getNamedItem("src");
    var xScriptVal = null;

    if ( externalScriptRef )
    {
      var externalScriptName = externalScriptRef.text;
      if ( loadedScripts[ externalScriptName ] ) // External script is already loaded
        continue;
      
      xScriptVal = readFile( externalScriptName );

      loadedScripts[ externalScriptName ] = true;
    }
    else
    {
      xScriptVal = xscripts[iPrivate].text;
    }

    try
    {
      eval(xScriptVal);
    }
    catch(e)
    {
      alert("Error '" + e.description + "', " + (e.number & 0xffff) + " occurred loading script '" + xScriptVal + "'");
    }
  }

// ??? This XQuery will only allow the searches that are enabled to be evaluated.  This should
// decrease load time, but it needs to be tested and determined if it makes sense first.
//  var xforms = searchRoot.selectNodes("search[" + xpathquery + "]/form");

  var xforms = searchRoot.selectNodes("search/form");
  for (var iPrivate = 0; iPrivate < xforms.length; iPrivate++)
  {
    document.write(xforms[iPrivate].xml);
  }
}


// 3. define all the searches

if (searchRoot)
{
  var searchNodes = searchRoot.selectNodes("search");
  for (var iSearch = 0; iSearch < searchNodes.length; iSearch++)
  {
    var searchNode = searchNodes[iSearch];
    if (searchNode != null)
    {
      var fn = searchNode.attributes.getNamedItem("function");
      var nameNode = searchNode.selectSingleNode("name");
      var descriptionNode = searchNode.selectSingleNode("description");
      var linkNode = searchNode.selectSingleNode("link");
      var categoryNode = searchNode.selectSingleNode("category");
      var nomenu = false;
      if ( categoryNode )
      {
        nomenu = categoryNode.attributes.getNamedItem("nomenu");
      }
      var searchCategories = new Array();
      getCategories( categoryNode, searchCategories );
      var descriptonXml = null;
      if(descriptionNode)
      {
        // There may be a better way to do this - I'm trying to remove 
        // the <description> tags which bracket the description XML
        descriptionXml = descriptionNode.xml.replace(/\<\/?description\>/g, '')
      }
      var localsearch = searchNode.attributes.getNamedItem("localsearch") ? true : false;
      addsearch(fn.text,
                (nameNode ? nameNode.text : fn.text),
                descriptionXml,
                (linkNode ? linkNode.text : null),
                searchCategories.length ? searchCategories[0] : null,
                localsearch,
                searchCategories.slice( 1 ),
                nomenu ? true : false);
    }
  }
}

function getCategories( categoryNode, categories )
{
  var nodes = categoryNode.childNodes;
  if ( !nodes )
    alert( categoryNode.text );
  for ( var i = 0; i < nodes.length; i++ )
  {
    if ( nodes[i].nodeType == 3 ) // NODE_TEXT
    {
      var text = nodes[i].nodeValue
                  .replace( /(^\s*)|(\s*$)/g, '' )    // leading/trailing whitespace
                  .replace( /\r\n\s*\r\n/g, '\r\n' );  // blank lines

      categories.push( text );
    }
    else if ( nodes[i].nodeType == 1 ) // NODE_ELEMENT
    {
      getCategories( nodes[i], categories );
    }
  }
}

// 4. load and execute the alias file

addAliasesFromFile( "aliases" );
addAliasesFromFile( localaliases, "Shortcuts" );

function addAliasesFromFile( aliasFile, category )
{
  var aliasTable = readTabDelimitedFile(aliasFile);
  for (var iPrivate = 0; iPrivate < aliasTable.length; iPrivate++)
  {
    var fields = aliasTable[iPrivate];
      if (fields.length < 2)
      {
        alert("Error on line " + (iPrivate + 1) + " of aliases.txt:\n\n" + aliasTable[iPrivate] + 
              "\n\n(Make sure there is a tab or \| symbol between the alias and command.)");
        break;
      }
      else
      {
        addalias(fields[0],
                 fields[1],
                 (fields.length >= 3 && fields[2] != '') ? fields[2] : null,   // name
                 (fields.length >= 4 && fields[3] != '') ? fields[3] : null,   // description
                 (fields.length >= 5 && fields[4] != "") ? fields[4] : ((arguments.length >= 2) ? category : null), // category
                 (fields.length >= 6 && fields[5] != "") ? fields[5].split(',') : new Array() // subcategories
                 );
      }
    }
  }


function loadSearchesFromDir( directory, local )
{

  try
  {
    // Get searches in the 'searches' subdirectory
    var fileSearches = getFiles( directory + "\\*.xml" ).split('\n');
    
    for ( var i = 0; i < fileSearches.length; i++ )
    {
      // getFiles doesn't always work as expected; a problem with FindFirstFile/FindNextFile
      // especially with files named *.xml_sav or something similar.
      if ( !/\.xml$/.test( fileSearches[i] ) )
        continue;
     
      loadSearchFile( directory + "\\" + fileSearches[i], local );

    }
  }
  catch (except) {}
}

function loadAddons()
{
  try
  {
    // Get searches in the 'addons' subdirectory
    var addonDirs = getFiles( "addons\\*" ).split('\n');
    
    for ( var i = 0; i < addonDirs.length; i++ )
    {
    
      if ( addonDirs[i] == "." )
        continue;
  
      // load all addon xml search files
      var addonSearches = getFiles("addons\\"+addonDirs[i]+"\\*.xml").split('\n');
      for (var j = 0; j < addonSearches.length; j++)
      {
        // getFiles doesn't always work as expected; a problem with FindFirstFile/FindNextFile
        // especially with files named *.xml_sav or something similar.
        if ( !/\.xml$/.test( addonSearches[j] ) )
          continue;

        loadSearchFile( "addons\\" + addonDirs[i] + "\\" + addonSearches[j] );
      }

    }

  }
  catch (except) {}
}

function loadSearchFile( path, local )
{
  var xml = readFile(path);
  if (!xmldoc.loadXML(xml))
  {
    alert('Unable to load search from ' + path + ':  ' + xmldoc.parseError.reason );
    return;
  }
  else
  {
    var searchNode = xmldoc.selectSingleNode("/search");
    var funcname = searchNode.attributes.getNamedItem("function").text;

    // If the search already exists, override with this new one
    var existingSearchNode = searchRoot.selectSingleNode("/search[@function='" + funcname + "']");
    if ( existingSearchNode )
    {
      if ( alertmode )
      {
        alert('Search "' + funcname + '" found in ' + path + ' already exists.  It will override the existing search.');
      }
      searchRoot.removeChild( existingSearchNode );
    }

    // Check to see if the element is lowercase (which it needs to be)
    if (alertmode && searchNode.selectSingleNode("/search[@function='" + funcname + "']/FORM"))
    {
      alert('Search "' + funcname + '" found in ' + path + ' has a <FORM> element which needs to be lowercased (<form>)');
    }
    
    if ( typeof local != 'undefined' && local == true )
    {
      searchNode.attributes.setNamedItem( xmldoc.createAttribute("localsearch") );
    }
    
    searchRoot.appendChild(searchNode);
  }
}

