// Load the contents of search.xml, aliases, and menu files

function addsearch(fname, name, desc, link, cat)
{
  try
  {
    searches[fname] = {fname:fname, name:name, desc:desc, link:link, cat:cat, fun:eval(fname), aliases:[], enabled:!disabledsearches[fname]};
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

function addalias(alias, fname, name, desc, cat)
{
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
      addsearch( fname, name ? name : url, desc ? desc : "", url.search(/%s/) < 0 ? url : "", cat);
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
      addsearch( fname, name ? name : cmd, desc ? desc : "", "", cat);
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

// 1. load search.xml (if present), merge localsearch.xml (if present), merge files in 'searches' subdirectory

var searchRoot = null;
try
{
  searchRoot = document.all("searchxml").selectSingleNode("searches");
}
catch (e) {}

// If there's no searchxml XML, then create an empty search root
if (!searchRoot)
{
  var searchDOM = new ActiveXObject("MSXML.DOMDocument");
  searchRoot = searchDOM.createElement("searches");
}

try
{
  var xmldoc = new ActiveXObject( "MSXML.DOMDocument" );

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

try
{
  // Get searches in the 'searches' subdirectory
  var searches = getFiles( "searches\\*.xml" );
  searches = searches.split('\n');
  
  //sort the list of searches
  searches.sort(); 
  
  for ( var i = 0; i < searches.length; i++ )
  {
    // getFiles doesn't always work as expected; a problem with FindFirstFile/FindNextFile
    // especially with files named *.xml_sav or something similar.
    if ( !/\.xml$/.test( searches[i] ) )
    {
      continue;
    }

    var xml = readFile("searches\\" + searches[i]);
    if (!xmldoc.loadXML(xml))
    {
      alert('Unable to load search from ' + searches[i] + ':  ' + xmldoc.parseError.reason );
      continue;
    }
    else
    {
      var searchNode = xmldoc.selectSingleNode("/search");
      var funcname = searchNode.attributes.getNamedItem("function").text;
      if (alertmode && searchRoot.selectSingleNode("/searches/search[@function='" + funcname + "']"))
      {
        qualifiedalert('Search "' + funcname + '" found in ' + "searches\\" + searches[i] + ' already exists.');
        continue;
      }
      else if (alertmode && searchNode.selectSingleNode("/search/FORM"))
      {
        alert('Search "' + funcname + '" has a <FORM> element which probably needs to be lowercased (<form>)');
      }
      else
      {
        searchRoot.appendChild(searchNode);
      }
    }
  }
}
catch (except) {}


// 2. eval all the scripts and doc.write all the forms

if (searchRoot)
{
//  var xscripts = searchRoot.selectNodes("search[" + xpathquery + "]/script");
  var xscripts = searchRoot.selectNodes("search/script");
  for (var iPrivate = 0; iPrivate < xscripts.length; iPrivate++)
  {
    eval(xscripts[iPrivate].text);
  }
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
      addsearch(fn.text,
                (nameNode ? nameNode.text : fn.text),
                (descriptionNode ? descriptionNode.xml : null),
                (linkNode ? linkNode.text : null),
                (categoryNode ? categoryNode.text : null));
    }
  }
}


// 4. load and execute the alias file

addAliasesFromFile( "aliases" );
addAliasesFromFile( localaliases, "Shortcuts" );

function addAliasesFromFile( aliasFile, category )
{
  var aliasTable = readTabDelimitedFile(aliasFile);
  //sort the list of aliases
  //aliasTable.sort();
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
                 arguments.length >= 2 ? category : ((fields.length >= 5 && fields[4] != "") ? fields[4] : null) );
      }
    }
  }
