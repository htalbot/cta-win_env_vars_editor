use strict;

use YAPE::Regex::Explain;
#~ print YAPE::Regex::Explain->new(qr/^\(\d{3}\) \d{3}-\d{4}$/)->explain;

print YAPE::Regex::Explain->new(qr/^([a-zA-Z]\:|\\\\[^\/\\:*?"<>|]+\\[^\/\\:*?"<>|]+)(\\[^\/\\:*?"<>|]+)+$/)->explain;

my $dir = "D:\\machine\\dpo";

if ($dir =~ /^([a-zA-Z]\:|\\\\[^\/\\:*?"<>|]+\\[^\/\\:*?"<>|]+)(\\[^\/\\:*?"<>|]+)*$/)
#~ if ($dir =~ /^([a-zA-Z]\:|\\\\[^\/\\:*?"<>|]+\\[^\/\\:*?"<>|]+)/)
{
    print "bon format\n";
}
else
{
    print "mauvais format\n";
}


#~ my $re = "^([a-zA-Z]\:|\\\\[^\/\\:*?"<>|]+\\[^\/\\:*?"<>|]+)(\\[^\/\\:*?"<>|]+)+$";

    #~ //~ if ($test =~ /^([a-zA-Z]\:|\\\\[^\/\\:*?"<>|]+\\[^\/\\:*?"<>|]+)(\\[^\/\\:*?"<>|]+)+$/)
