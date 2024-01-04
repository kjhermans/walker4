#!/usr/bin/perl

##
## Turns (parts of) a picture into a bitmap, usable by C.
##

use GD;

my ($picture, $dimension, $xoff, $yoff) = (undef, 1<<7, 0, 0);
my ($rshift, $gshift, $bshift) = (0, 0, 0);
my $img;

if (scalar(@ARGV)) { $picture = shift @ARGV; }
if (scalar(@ARGV)) { $dimension = shift @ARGV; }
if (scalar(@ARGV)) { $xoff = shift @ARGV; }
if (scalar(@ARGV)) { $yoff = shift @ARGV; }
if (scalar(@ARGV)) { $rshift = shift @ARGV; }
if (scalar(@ARGV)) { $gshift = shift @ARGV; }
if (scalar(@ARGV)) { $bshift = shift @ARGV; }

open IMG, "< $picture" || die "Could not open image file $picture.";

if ($picture =~ /\.jpe?g$/i) {
  $img = GD::Image->newFromJpeg(\*IMG) || die "Could not decode JPEG";
} elsif ($picture =~ /\.png$/i) {
  $img = GD::Image->newFromPng(\*IMG) || die "Could not decode PNG";
} else {
  die "Unknown image format from '$picture'\n";
}
close IMG;

print "#define IMGWIDTH $dimension\n";
print "#define IMGHEIGHT $dimension\n";
print "#define IMG { \\\n";
for (my $i=0; $i < $dimension; $i++) {
  print "  /* Row $i */ \\\n";
  for (my $j=0; $j < $dimension; $j++) {
    my $index = $img->getPixel($xoff + $j, $yoff + $i);
    my ($r,$g,$b) = $img->rgb($index);
    if ($rshift) { $r = int($r * $rshift); if ($r >= 256) { $r = 255; } }
    if ($gshift) { $g = int($g * $gshift); if ($g >= 256) { $g = 255; } }
    if ($bshift) { $b = int($b * $bshift); if ($b >= 256) { $b = 255; } }
    print "  ($r<<16)|($g<<8)|$b";
    if ($j < $dimension-1) {
      print ",";
    }
    print " \\\n";
  }
  if ($i < $dimension-1) {
    printf ",";
  }
  print " \\\n";
}
print "}\n";

close IMG;

exit 0;

1;
