// IE 5.0 and earlier don't have splice, push
if (!Array.prototype.splice)
{
  function array_splice(ind,cnt)
  {
    removeArray = this.slice(ind,ind+cnt);
    endArray = this.slice(ind+cnt);
    this.length = ind;
    for (var i=2; i < arguments.length; i++)
      this[this.length] = arguments[i];
    for (var i=0; i < endArray.length; i++)
      this[this.length] = endArray[i];
    return removeArray;
  }
  Array.prototype.splice = array_splice;
}

if (!Array.prototype.push)
{
  function array_push()
  {
    for (i=0; i<arguments.length; i++)
      this[this.length] = arguments[i];
    return this.length;
  }
  Array.prototype.push = array_push;
}

function addsearch(fname, name, desc, link)
{
  try
  {
    var newobj = {fname:fname, name:name, desc:desc, link:link, fun:eval(fname), aliases:[]};
    searches[fname] = newobj;
    if (!aliases[fname])
      addalias(fname, fname);

    helparray.push(newobj);
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




// the following structures will be defined

var searches = {};
var aliases = {};
var specialaliasarray = [];
var menuarray = [];
var helparray = [];



// 1. load search.xml and merge localsearch.xml

var searchRoot = null;
try
{
  searchRoot = document.all("searchxml").selectSingleNode("searches");
  var localSearches = document.all("localsearch").selectNodes("/searches/search");
  for (var i = 0; i < localSearches.length; i++)
    searchRoot.appendChild(localSearches[i]);
}
catch (ex) {}



// 2. eval all the scripts and doc.write all the forms

if (searchRoot)
{
  var xscripts = searchRoot.selectNodes("search/script");
  for (var i = 0; i < xscripts.length; i++)
  {
    eval(xscripts[i].text);
  }
  var xforms = searchRoot.selectNodes("search/form");
  for (var i = 0; i < xforms.length; i++)
  {
    document.write(xforms[i].xml);
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
      addsearch(fn.text, nameNode.text, descriptionNode.xml, (linkNode ? linkNode.text : null));
    }
  }
}



// 4. load the alias file (TBD)

var aliasFileText = "";
try {aliasFileText = readFile("aliases");} catch (e) {}
aliasFileText = aliasFileText.replace(/\r\n/g,"\n");
var aliasFileLines = aliasFileText.split("\n");



// 5. execute the alias file

for (var i = 0; i < aliasFileLines.length; i++)
{
  var aliasFields = aliasFileLines[i].split("\t");
  if (aliasFields.length != 2)
  {
    if (aliasFields.length > 2 || !aliasFields[0].match(/^\s*$/))
    {
      alert("Error on line " + (i + 1) + " of aliases.txt:\n" + aliasFileLines[i]);
      break;
    }
  }
  else if (aliasFields[0] == "menu")
  {
    menuarray.push(aliasFields[1]);
  }
  else
  {
    addalias(aliasFields[0], aliasFields[1]);
  }
}


