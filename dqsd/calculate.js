
function buildCalcMemPath()
{
  return DQSDLauncher.AppDataDirectory + '\\calcmem.txt';
}

// offline calculator
function calculate(expr)
{
  if (expr.match(/=ERR$/))
    return;

  try
  {
    with(Math)
    {
      var answer = eval(expr);
      if (typeof(answer) == "number")
      {
        // for the sake of pretty decimal numbers,
        // round numbers that are very close to a ten-millionth
        if (abs(answer) >= 0.001 &&
            abs(round(answer * 1e+7) - answer * 1e+7) < 0.001)
          answer = round(answer * 1e+7)/1e+7;
      }
      setSearchWindowText(answer, true);
    }
  }
  catch (exception)
  {
    setSearchWindowText(expr + "=ERR=", true);
  }

  savevars();
}

// based log functions
function log10(n) { return ln(n)/ln10; }
function log2(n) { return ln(n)/ln2; }

// hex conversion for use in calculator
function hex(i)
{
  hexChars = "0123456789abcdef";
  var h = "";
  var n = 256;

  while (i < 0)
  {
     if (i + n / 16 > 0) { i += n; break; }
     n = n * n;
  }

  while (i > 0)
  {
     h = hexChars.charAt(i % 16) + h;
     i = Math.floor(i / 16);
  }

  if (h == "")
     h = "0";

  return "0x" + h;
}

// octal conversion for use in calculator
function oct(i)
{
  octChars = "01234567";
  var h = "";
  var n = 512;

  while (i < 0)
  {
     if (i + n / 8 > 0) { i += n; break; }
     n = n * n;
  }

  while (i > 0)
  {
     h = octChars.charAt(i % 8) + h;
     i = Math.floor(i / 8);
  }

  if (h == "")
     h = "0";

  return "0" + h;
}

// binary conversion for use in calculator
function bin(i)
{
  binChars = "01";
  var h = "";
  var n = 16;

  while (i < 0)
  {
     if (i + n / 2 > 0) { i += n; break; }
     n = n * n;
  }

  while (i > 0)
  {
     h = binChars.charAt(i % 2) + h;
     i = Math.floor(i / 2);
  }

  if (h == "")
     h = "0";

  return h;
}


// detect numeric math expressions to execute right away
function mathexp(t)
{
  // chop out variables
  var noconst = t.replace(/\b(PI|E|LOG2E|LOG10E|LN2|LN10|SQRT2|SQRT1_2|pi)\b/g, "_");
  // experiment: also allow "longvarname="
  var noconst = noconst.replace(/^[a-zA-Z_]+\s*=/, "a=");
  // it's only an expression if it ends with a digit, paren, or variable
  if (!noconst.match(/\b[a-z]$|[_\d\)]$/)) return false;
  // chop out function names
  var nofunc = noconst.replace(/\b(ln|log10|log2|hex|oct|bin|abs|acos|asin|atan|atan2|ceil|cos|exp|floor|log|max|min|pow|random|round|sin|sqrt|tan)\(/g, "(");
  // it's only an expression if it begins with a digit, paren, or variable - or a minus
  if (!nofunc.match(/^[a-z_]\b|^[x\d\-\.\(]/)) return false;
  // detect numbers
  var nonum = nofunc.replace(/\b(\d+(\.\d*)?)([eE][+-]?\d+)?\b/g, "1");
  var nonum = nonum.replace(/(\.\d+)([eE][+-]?\d+)?\b/g, "1");
  // remove spsaces
  var nospace = nonum.replace(/\s/g, "");
  // only operators and recognized things are allowed
  if (!nospace.match(/^[1_a-z\*\-\+\/\(\),=]+$/)) return false;
  // constants cannot touch
  if (nospace.match(/[1_a-z\)][1_a-z\(]/)) return false;
  // operators cannot touch
  if (nospace.match(/[\*\-\+\/][\*\+\/=]/)) return false;
  // parens must match
  var paren = 0;
  var eq = 0;
  var i;
  for (i = nospace.length - 1; i >= 0; i--)
  {
    var ch = nospace.charAt(i);
    if (eq == 1)
    {
      if (ch < 'a' || ch > 'z') return false;
    }
    else
    {
      if (ch >= 'a' && ch <= 'z' && eval("typeof " + ch) == "undefined") return false;
    }
    if (ch == '=') { eq++; }
    if (ch == ')') { paren++; }
    if (ch == '(') { paren--; }
    if (paren < 0) return false;
    if (eq > 1) return false;
  }

  // looks like an expression: we can calc it
  calculate(t);
  return true;
}


function savevars()
{
  var savetext = "";
  for (var ii = 'a'.charCodeAt(0); ii <= 'z'.charCodeAt(0); ii++)
  {
    var varname = String.fromCharCode(ii);
    var vartype = eval("typeof " + varname);
    if (vartype == "number" || vartype == "string")
    {
       savetext += varname + "\t" + vartype + "\t" + escape(eval(varname)) + "\r\n";
    }
  }
  for (var ii = 'A'.charCodeAt(0); ii <= 'Z'.charCodeAt(0); ii++)
  {
    var varname = String.fromCharCode(ii);
    var vartype = eval("typeof " + varname);
    if (vartype == "number" || vartype == "string")
    {
       savetext += varname + "\t" + vartype + "\t" + escape(eval(varname)) + "\r\n";
    }
  }
  writeFile( buildCalcMemPath(), savetext);
}

function loadvars()
{
  var values = readTabDelimitedFile( buildCalcMemPath() )
  for (var ii = 0; ii < values.length; ii++)
  {
    var value = values[ii];
    if (value.length == 3)
    {
      if (value[1] == "number")
      {
        eval(value[0] + " = " + unescape(value[2]));
      }
      else if (value[1] == "string")
      {
        var strval = unescape(value[2]);
        eval(value[0] + " = strval");
      }
    }
  }
}

loadvars();

// constants for use in calculator
pi = Math.PI;
ln10 = Math.LN10;
ln2 = Math.LN2;
log10e = Math.LOG10E;
log2e = Math.LOG2E;
sqrt1_2 = Math.SQRT1_2;
sqrt2 = Math.SQRT2;
ln = Math.log;
e = Math.exp(1);


