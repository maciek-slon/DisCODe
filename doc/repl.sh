#!/bin/bash

#
# Script to tidy-up generated documentation. Pass directory name and let
# it do the magic for you. Every action is described in it's section.
#
# Example usage:
#    ./repl.sh ~/discode/doc/html
#

# Test whether command-line argument is present (non-empty).
if [ -n "$1" ]
then
	cd $1
else
	echo "Usage: ./repl.sh DOC_DIR"
	exit 1
fi

#
# remove namespaces form class index
#

sed -e's/(<a class="el" href="namespace[^)]*<\/a>)//g' classes.html > classes2.html && mv classes2.html classes.html


#
# Change column headers in class index
#

sed -n '
# if the first line copy the pattern to the hold buffer
1h
# if not the first line then append the pattern to the hold buffer
1!H
# if the last line then ...
$ {
        # copy from the hold to the pattern buffer
        g
        # do the search and replace
        s|<td>[^<]*<a name="\([^"]*\)"></a><table[^<]*>[^<]*<tr>[^<]*<td>[^<]*<div class="ah">&nbsp;&nbsp;\(\w\)&nbsp;&nbsp;</div>[^<]*</td>[^<]*</tr>[^<]*</table>[^<]*</td>|<td class="index_header"><a name="\1"></a>\2</td>\n|g
        # print
        p
}
' classes.html > classes2.html && mv classes2.html classes.html


#
# Change appearance od main page (remove header)
#

sed -n '
# if the first line copy the pattern to the hold buffer
1h
# if not the first line then append the pattern to the hold buffer
1!H
# if the last line then ...
$ {
        # copy from the hold to the pattern buffer
        g
        # do the search and replace
        s|<div class="header">.*<div class="contents">|<div class="contents" style="padding-top: 5px">|g
        # print
        p
}
' index.html > index2.html.html && mv index2.html.html index.html

sed -n '
# if the first line copy the pattern to the hold buffer
1h
# if not the first line then append the pattern to the hold buffer
1!H
# if the last line then ...
$ {
        # copy from the hold to the pattern buffer
        g
        # do the search and replace
        s|<h3 class="version">.*</h3>||g
        # print
        p
}
' index.html > index2.html.html && mv index2.html.html index.html
