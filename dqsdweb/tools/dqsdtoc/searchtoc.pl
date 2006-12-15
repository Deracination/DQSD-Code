#!/usr/bin/perl
############################################################################
# 
# Generate the html table file from the *.xml files in the searches directory 
#
# Monty Scroggins
#
############################################################################
## +++++++++++++++++++++++++++ Maintenance Log +++++++++++++++++++++++++++++
# Monty Scroggins Thu Sep 19 19:25:06 GMT 2002 Script created.

$|=1;    # set output buffering to off
$[=0;    # set array base to 0
$,=" ";  # set output field separator
$\="\n"; # set output record separator

#path to the searches directory
my $searchdir="/usr/home/dqsd/public_html/searches";
#my $gendate=localtime(time());

#instead of printing out the stylesheet data, we could link to a standalone stylesheet.
#<link rel='stylesheet' type='text/css' href='http://cvs.dqsd.sourceforge.net/cgi-bin/viewcvs.cgi/*checkout*/dqsd/dqsd/search.css?rev=HEAD&content-type=text/plain'>
#<link rel="stylesheet" type="text/css" href="dqsd.css">

#begin html
print "Content-type: text/html", "\n\n";

print q{
<html>
<head>
<title>DQSD Searches</title>
<link rel='stylesheet' type='text/css' href='searchtoc.css'>
<style>
th {
   text-align: left;
   font-weight: bold;
   border: 1px outset;
   padding-left:2px;
}
td {
   vertical-align:top;
}
.funcName {
   color: #600000;
}
.description {
   color: #606040;
   padding-left: 4px;
   padding-right: 4px;
}
.category {

   color: #a07040;
   padding-right: 4px;
   vertical-align:top;
}
</style>
</head>
<body>
};

#get the list of xml files

opendir(searchdir, "$searchdir") or die "searchdir not found: $!";
@datafiles = grep /\.xml$/,readdir searchdir;
closedir searchdir;

#start the table
my (@html,@categories);
#generate a record for each xml file
foreach $datafile (@datafiles) {
   next if ($datafile=~/^configure.xml/);
   my ($name,$desctext,$category,$link)=();
   my $filedata=read_file("$searchdir/$datafile");
   for ($t=0; $t<scalar(@$filedata); $t++) {
      #----------------------------------------------
      #trigger if the line contains the <name> tags
      if (@$filedata[$t]=~/<name.+\<\/name/i) {
         $name=(split(/<*\/*name>*/i,"@$filedata[$t]"))[1];
      }#if
      #----------------------------------------------
      #trigger if the line contains the <description> tag
      if (@$filedata[$t]=~/\<description/i) {
         if (@$filedata[$t]=~/\/description *>/i) {
            $desctext=(split(/<*\/*description>*/i,"@$filedata[$t]"))[1];
            }else{
            @$filedata[$t]=~s/\< *description *\>//i;
            $advance=0;
            until (@$filedata[$t+$advance]=~/\/description *>/i) {
               $desctext.="@$filedata[$t+$advance]"; 
               $advance++;
            }#until
         }#else
      }#if
      #----------------------------------------------
      #trigger if the line contains the category tags
      if (@$filedata[$t]=~/<category.+\<\/category/i) {
      $category=(split(/ *<*\/*category *>*/i,"@$filedata[$t]"))[1];
         $category =~ s/ *nomenu="true">//;
         push(@categories,"$category");
      }#if
      #----------------------------------------------
      #trigger if the line contains the link tags
      if (@$filedata[$t]=~/<link.+\<\/link/i) {
         $link=(split(/<*\/*link>*/i,"@$filedata[$t]"))[1];
         $link="<a href=\"$link\"</a>";
      }#if
   }#for
   push(@html,"<td class=\"category\"><a name=\"$category\">$category</a></td>\n<td><a href=\"
   http://dqsd.svn.sourceforge.net/viewvc/*checkout*/dqsd/trunk/dqsd/searches/$datafile\">$datafile</a></td>\n<td class=\"funcNamehelpboxCommands\">$link$name</td>\n<td class=description>$desctext<br></td>\n</tr>");
   #<a href=\"http://cvs.sourceforge.net/cgi-bin/viewcvs.cgi/*checkout*/dqsd/dqsd/searches/$datafile?rev=HEAD&content-type=text/xml\">$datafile</a></td>\n<td class=\"funcNamehelpboxCommands\">$link$name</td>\n<td class=description>$desctext<br></td>\n</tr>");
}#foreach

#sort by the categories
@html=sort(@html);
#remove any duplicates from the categories list
@categories=grep(($Last eq $_ ? 0 : ($Last = $_, 1)),sort @categories);

#build the category link string to go above and below the table
$categorylink="";
for ($t=0; $t<=scalar(@categories); $t++) {
   next if ($categories[$t]=~/^ *$/);
   $categorylink .="\n<a class=\"helpboxLink\" href=\"\#$categories[$t]\">$categories[$t]</a>";
}
print "<center><br><h3>DQSD Searches</h3><b>";
print scalar(@html)." Total</b> <br>$categorylink";

print "<table>\n<th style=background-color:#fcfcfc;></th><th>Category</th><th>File</th><th>Name</th><th>Description</th>";
for ($t=1; $t<=scalar(@html); $t++) {
   print "\n<tr>\n<td class=\"body\">$t</td>\n$html[$t-1]";
}
   
#end html
print qq{
</table>
$categorylink
<br><br>
</center>
</body>
</html>
};

#slurp in a file and return the data array
sub read_file {
   my($file) = @_;
   open(INF, "<$file") or return();  
   my(@data) = <INF>;
   close(INF);
   chomp @data;
   return(\@data);
   }#sub
