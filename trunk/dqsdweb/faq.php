<?php // FAQ.php -- simple caching for DQSD HTML FAQ
// This script assumes that the XSLT extension is enabled in PHP
// and that the PHP version is 4.0.6 or newer.
// The xslt_process function for older versions used different parameters
// and is incompatible with the new function declaration.


// change these to the correct filenames.
$xsl_filename = "faq.xsl";
$xml_filename = "faq.xml";
$html_filename = "faq.htm";



// shouldn't have to modify anything below this line


$html_timestamp = filemtime($html_filename); // avoid calling twice

if (($html_timestamp < filemtime($xsl_filename)) || ($html_timestamp < filemtime($xml_filename)))
{
	$xh = xslt_create();
	if(! @xslt_process($xh, $xml_filename, $xsl_filename, $html_filename))
	{
		// error in xslt_process! Write the error to the HTML file
		// so that it will be displayed instead of the old FAQ.
		// This is, of course, totally unnecessary, but might help
		// debug problems while writing the XML and/or XSL files.

		$fp = fopen($html_filename, 'w');
		fwrite($fp, "Error processing stylesheet: " . xslt_error($xh));
		fclose($fp);
	}
}

// HTML file has, if outdated, been rewritten. Now just echo the
// contents to the client.
if(version_compare(php_version(), "4.3.0", ">="))
{
	echo file_get_contents($html_filename);
}
else
{
	echo implode('', file($html_filename));
}

?>
