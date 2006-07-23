# -*- Mode: perl; indent-tabs-mode: nil -*-
#
# The contents of this file are subject to the Mozilla Public
# License Version 1.1 (the "License"); you may not use this file
# except in compliance with the License. You may obtain a copy of
# the License at http://www.mozilla.org/MPL/
#
# Software distributed under the License is distributed on an "AS
# IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
# implied. See the License for the specific language governing
# rights and limitations under the License.
#
# The Original Code is the Bugzilla Bug Tracking System.
#
# The Initial Developer of the Original Code is Netscape Communications
# Corporation. Portions created by Netscape are
# Copyright (C) 1998 Netscape Communications Corporation. All
# Rights Reserved.
#
# Contributor(s): Terry Weissman <terry@mozilla.org>
#                 Dan Mosedale <dmose@mozilla.org>
#                 Joe Robins <jmrobins@tgix.com>
#                 Dave Miller <justdave@syndicomm.com>
#                 Christopher Aillon <christopher@aillon.com>
#                 Gervase Markham <gerv@gerv.net>
#                 Christian Reis <kiko@async.com.br>
#                 Bradley Baetz <bbaetz@acm.org>

package Bugzilla::Auth::Verify::ROSCMS;

use strict;

use Bugzilla::Constants;

my $edit_options = {
    'new' => 0,
    'userid' => 0,
    'login_name' => 0,
    'realname' => 0,
};

sub can_edit {
    my ($class, $type) = @_;
    return $edit_options->{$type};
}

sub authenticate {
    my ($class, $username, $passwd) = @_;

    return (AUTH_ERROR, undef, "Internal error in Bugzilla::Auth::Verify::ROSCMS->authenticate");
}

1;

__END__

=head1 NAME

Bugzilla::Auth::Verify::ROSCMS - ROSCMS authentication for Bugzilla

=head1 SUMMARY

This is an L<authentication module|Bugzilla::Auth/"AUTHENTICATION"> for
Bugzilla, which logs the user in using the ROSCMS login system.

=head1 SEE ALSO

L<Bugzilla::Auth>
