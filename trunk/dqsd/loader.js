// Load the contents of search.xml, aliases, and menu files

function addsearch(fname, name, desc, link, cat)
{
  try
  {
    if (name == null) name = fname;
    var newobj = {fname:fname, name:name, desc:desc, link:link, cat:cat, fun:eval(fname), aliases:[]};
    searches[fname] = newobj;
    if (!aliases[fname])
      addalias(fname, fname);

    addhelp(newobj);
  }
  catch (except)
  {
    alert("Error adding search - " + except.description +
           "\n\nfunction: \"" + fname + "\"\nname: \"" + name + "\"\ndescription: \"" + desc + "\"");
  }
}

internalShortcutIndex = 0;
INTERNAL_FUNC_PREFIX = "_dqsd_internal_fn_";

function addalias(alias, fname)
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
                           "var url = '" + url + "';" +
                           "if (url.search(/%s/) >= 0 && t == '')" + 
                           "  alert('An argument is required for " + alias + "  (" + url + ")');" + 
                           "else" + 
                           "  direct(url.replace( /%s/g, t ));"
                          );
      eval( fname + " = f;" );
      addsearch( fname, url, "", url.search(/%s/) < 0 ? url : "", "Shortcuts" );
    }
    else if ((res = fname.match(/^(\w+) +(.+)/)) && searches[res[1]]) // starts with a valid search function
    {
      var cmd = fname;
      fname = INTERNAL_FUNC_PREFIX + (++internalShortcutIndex);
      var f = new Function("t", 
                           "var cmd = '" + res[2] + "';" +
                           res[1] + "(cmd.replace( /%s/g, t ));"
                          );
      eval( fname + " = f;" );
      addsearch( fname, cmd, "", "", "Shortcuts" );
    }
    else
    {
      alert("Cannot add alias " + alias + " -> " + fname + ":\nThere is no search " + fname + ".");
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
  var cat = search.cat;
  if (!cat) cat = "Other";
  if (!categories[cat])
  {
    categories[cat] = [];
    categoryarray.push(cat);
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
  // Get optional searches in localsearch.xml
  var localSearches = document.all("localsearch").selectNodes("/searches/search");
  if (localSearches)
    for (var iPrivate = 0; iPrivate < localSearches.length; iPrivate++)
      searchRoot.appendChild(localSearches[iPrivate]);

  // Get searches in the 'searches' subdirectory
  var searches = getFiles( "searches\\*.xml" );
  searches = searches.split('\n');
  
  var xmldoc = new ActiveXObject( "MSXML.DOMDocument" );
  for ( var i = 0; i < searches.length; i++ )
  {
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
      if (searchRoot.selectSingleNode("/searches/search[@function='" + funcname + "']"))
      {
        alert('Search "' + funcname + '" found in ' + "searches\\" + searches[i] + ' already exists.');
        continue;
      }
      else
        searchRoot.appendChild(searchNode);
    }
  }
}
catch (except) {}


// 2. eval all the scripts and doc.write all the forms

if (searchRoot)
{
  var xscripts = searchRoot.selectNodes("search/script");
  for (var iPrivate = 0; iPrivate < xscripts.length; iPrivate++)
  {
    eval(xscripts[iPrivate].text);
  }
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
addAliasesFromFile( localaliases );

function addAliasesFromFile( aliasFile )
{
  var aliasTable = readTabDelimitedFile(aliasFile);
  
  for (var iPrivate = 0; iPrivate < aliasTable.length; iPrivate++)
  {
    var fields = aliasTable[iPrivate];
    if (fields.length != 2)
    {
      if (fields.length > 2 || !fields[0].match(/^\s*$/))
      {
        alert("Error on line " + (iPrivate + 1) + " of aliases.txt:\n\n" + aliasTable[iPrivate] + 
              "\n\n(Make sure there is a tab between the alias and command.)");
        break;
      }
    }
    else
    {
      addalias(fields[0], fields[1]);
    }
  }
}


// 5. load and execute the menu file

var menuTable = readTabDelimitedFile("menu");

for (var iPrivate = 0; iPrivate < menuTable.length; iPrivate++)
{
  var fields = menuTable[iPrivate];
  if (fields.length > 0 &&
      (fields[0].length > 0 || fields.length > 1))
  {
    menuarray.push(fields[0]);
  }
}
