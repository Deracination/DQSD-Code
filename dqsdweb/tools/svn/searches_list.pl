my $listing = `svn log list --xml https://dqsd.svn.sourceforge.net/svnroot/dqsd/trunk/dqsd/searches`;
print "$listing";
