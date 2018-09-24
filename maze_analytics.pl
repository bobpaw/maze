#!/usr/bin/perl

use warnings;
use strict;
use Digest::MD5 qw(md5 md5_hex md5_base64);

my $line = "";
my %mazes;
my $mazecount = 0;
my %mazes_count;
my $maze = "";
my $thismaze;

while (!eof()) {
    while ($line = <>, $line !~ /done/) {
        $maze .= $line;
    }
    $mazecount++;
    $thismaze = md5_base64($maze);
    if (!defined($mazes{$thismaze})) {
        $mazes{$thismaze} = 1;
    } else {
    $mazes{$thismaze} = $mazes{$thismaze}+1;
    }
    $maze = "";
}
my $i = 0;
print "Total mazes: $mazecount\n";
for my $thismaze (keys %mazes) {
    print "$i: $mazes{$thismaze}: $thismaze\n";
    $i++;
}
exit;
