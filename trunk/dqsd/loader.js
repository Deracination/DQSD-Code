// Load the contents of search.xml, aliases, and menu files

function addsearch(fname, name, desc, link, cat)
{
  try
  {
    var newobj = {fname:fname, name:name, desc:desc, link:link, cat:cat, fun:eval(fname), aliases:[]};
    searches[fname] = newobj;
    if (!aliases[fname])
      addalias(fname, fname);

    addhelp(newobj);
  }
  catch (ex)
  {
    alert("Error adding search - " + ex.description +
           "\n\nfunction: \"" + fname + "\"\nname: \"" + name + "\"\ndescription: \"" + desc + "\"");
  }
}

function addalias(alias, fname)
{
  if (!searches[fname])
  {
    if (aliases[fname])
    {
      fname=aliases[fname];
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


// 1. load search.xml and merge localsearch.xml

var searchRoot = null;
try
{
  searchRoot = document.all("searchxml").selectSingleNode("searches");
  var localSearches = document.all("localsearch").selectNodes("/searches/search");
  for (var iPrivate = 0; iPrivate < localSearches.length; iPrivate++)
    searchRoot.appendChild(localSearches[iPrivate]);
}
catch (ex) {}



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

var aliasTable = readTabDelimitedFile("aliases");

for (var iPrivate = 0; iPrivate < aliasTable.length; iPrivate++)
{
  var fields = aliasTable[iPrivate];
  if (fields.length != 2)
  {
    if (fields.length > 2 || !fields[0].match(/^\s*$/))
    {
      alert("Error on line " + (i + 1) + " of aliases.txt.");
      break;
    }
  }
  else
  {
    addalias(fields[0], fields[1]);
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
