if (WScript.Arguments.length < 2)
{
   WScript.echo("MSXML XSLT Utility\n");
   WScript.echo("usage: xslt inputFile xsltFile [options]");
   WScript.echo("  -o outputFile");
   WScript.echo("  -p paramName paramValue");
   WScript.Quit(1);
}
var source = WScript.Arguments.Item(0);
var stylesheet = WScript.Arguments.Item(1);
var xslTemplate = new ActiveXObject("MSXML2.XSLTemplate");
var xmlSource = new ActiveXObject("MSXML2.FreeThreadedDOMDocument");
var xmlStylesheet = new ActiveXObject("MSXML2.FreeThreadedDOMDocument");
var xslProcessor = null;
var outputfile = "";

// load the source document
if (xmlSource.load(source))
{
   // load the stylesheet document
   if (xmlStylesheet.load(stylesheet)) 
   {
       try {
          // associate the stylesheet with the template
          xslTemplate.stylesheet = xmlStylesheet;
          // create the XSLProcessor for this transformation
          xslProcessor = xslTemplate.createProcessor();
          // specify the input and output streams
          xslProcessor.input = xmlSource;
          //xslProcessor.output = WScript.StdOut;

          if (WScript.Arguments.length > 2)
          {
             var i;
             for (i=2; i<WScript.Arguments.length; i++)
             {
  		option = WScript.Arguments.Item(i);
                if (option == "-o")
                {
                   outputfile = WScript.Arguments.Item(i+1);
                }
                else if (option == "-p")
                {
                   var paramName = WScript.Arguments.Item(i+1);
                   var paramValue = WScript.Arguments.Item(i+2);
                   xslProcessor.addParameter(paramName, paramValue);
                }
             }
          }
          // call transform
          b = xslProcessor.transform(); 

          if (outputfile == "")
             WScript.echo(xslProcessor.output);
          else
          {
             var fso = new ActiveXObject("Scripting.FileSystemObject");
             var txtFile = fso.CreateTextFile(outputfile, true);
             txtFile.WriteLine(xslProcessor.output);
             txtFile.Close();
          }
       }
       catch(e) 
       {
	  WScript.echo("### tranform error: " + e.description);
       }
   }
   else WScript.echo("### parse error: " + xmlStylesheet.parseError.reason);    
}
else WScript.echo("### parse error: " + xmlSource.parseError.reason);

