#!/bin/sh
chown -R reactos.roscms .
find . -exec chmod g+w,o+r \{\} \;
find . -name "*.cgi" -exec chmod o+x \{\} \;
find . -name "*.pl" -exec chmod o+x \{\} \;
find . -name "*.sh" -exec chmod o+x \{\} \;
find . -type d -exec chmod o+rx \{\} \;
find . -type d -exec chmod g+s \{\} \;
chown -R apache graphs data
