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

package Bugzilla::Auth::ROSCMS;

use strict;

use URI;
use URI::Escape;

use Bugzilla::Config;
use Bugzilla::Constants;
use Bugzilla::Error;
use Bugzilla::Util;

my $session_cookie_name = "roscmsusrkey";
my $roscms_db_name      = "roscms";
my $roscms_login_page   = "/roscms/?page=login&target=";

sub authenticate {
    my ($class, $username, $passwd) = @_;

    return (AUTH_NODATA) unless defined $username && defined $passwd;

    # We're just testing against the db: any value is ok
    trick_taint($username);

    my $userid = $class->get_id_from_username($username);
    return (AUTH_LOGINFAILED) unless defined $userid;

    return (AUTH_LOGINFAILED, $userid) 
        unless $class->check_password($userid, $passwd);

    # The user's credentials are okay, so delete any outstanding
    # password tokens they may have generated.
    require Bugzilla::Token;
    Bugzilla::Token::DeletePasswordTokens($userid, "user_logged_in");

    # Account may have been disabled
    my $disabledtext = $class->get_disabled($userid);
    return (AUTH_DISABLED, $userid, $disabledtext)
      if $disabledtext ne '';

    return (AUTH_OK, $userid);
}

sub can_edit { return 1; }

sub get_id_from_username {
    my ($class, $username) = @_;
    my $dbh = Bugzilla->dbh;
    my $sth = $dbh->prepare_cached("SELECT userid FROM profiles " .
                                   "WHERE login_name=?");
    my ($userid) = $dbh->selectrow_array($sth, undef, $username);
    return $userid;
}

sub get_disabled {
    my ($class, $userid) = @_;
    my $dbh = Bugzilla->dbh;
    my $sth = $dbh->prepare_cached("SELECT disabledtext FROM profiles " .
                                   "WHERE userid=?");
    my ($text) = $dbh->selectrow_array($sth, undef, $userid);
    return $text;
}

sub check_password {
    my ($class, $userid, $passwd) = @_;
    my $dbh = Bugzilla->dbh;
    my $sth = $dbh->prepare_cached("SELECT cryptpassword FROM profiles " .
                                   "WHERE userid=?");
    my ($realcryptpwd) = $dbh->selectrow_array($sth, undef, $userid);

    # Get the salt from the user's crypted password.
    my $salt = $realcryptpwd;

    # Using the salt, crypt the password the user entered.
    my $enteredCryptedPassword = crypt($passwd, $salt);

    return $enteredCryptedPassword eq $realcryptpwd;
}

sub change_password {
    my ($class, $userid, $password) = @_;
    my $dbh = Bugzilla->dbh;
    my $cryptpassword = Crypt($password);
    $dbh->do("UPDATE profiles SET cryptpassword = ? WHERE userid = ?", 
             undef, $cryptpassword, $userid);
}

sub login {
    my ($class, $type) = @_;

    # 'NORMAL' logins depend on the 'requirelogin' param
    if ($type == LOGIN_NORMAL) {
        $type = Param('requirelogin') ? LOGIN_REQUIRED : LOGIN_OPTIONAL;
    }

    my $cgi = Bugzilla->cgi;

    my $authres;
    my $userid;
    my $session_id = $cgi->cookie($session_cookie_name);
    if (! defined($session_id)) {
        $authres = AUTH_NODATA;
    } else {
        my $session_id_clean = $session_id;
        trick_taint($session_id_clean);
        my $remote_addr_clean;
        if ($ENV{'REMOTE_ADDR'} =~ m/^(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})/) {
            $remote_addr_clean = $1;
        } else {
            $remote_addr_clean = 'invalid';
        }
        my $browser_agent_clean = $ENV{'HTTP_USER_AGENT'};
        trick_taint($browser_agent_clean);
        my $query = "SELECT m.map_subsys_userid " .
                    "  FROM $roscms_db_name.user_sessions s, " .
                    "       $roscms_db_name.users u, " .
                    "       $roscms_db_name.subsys_mappings m " .
                    " WHERE s.usersession_id = ? " .
                    "   AND (s.usersession_expires IS NULL OR " .
                    "        NOW() <= s.usersession_expires) " .
                    "   AND u.user_id = s.usersession_user_id " .
                    "   AND (u.user_setting_ipaddress = 'false' OR " .
                    "        s.usersession_ipaddress = ?) " .
                    "   AND (u.user_setting_browseragent = 'false' OR " .
                    "        s.usersession_browseragent = ?) " .
                    "   AND m.map_roscms_userid = s.usersession_user_id " .
                    "   AND m.map_subsys_name = 'bugzilla'";
        my @params = ($session_id_clean, $remote_addr_clean,
                      $browser_agent_clean);
        my $dbh = Bugzilla->dbh;
        ($userid) = $dbh->selectrow_array($query, undef, @params);
        if ($userid) {
            $authres = AUTH_OK;

            # Update time of last session use
            $query = "UPDATE $roscms_db_name.user_sessions " .
                     "   SET usersession_expires = DATE_ADD(NOW(), INTERVAL 30 MINUTE) " .
                     " WHERE usersession_id = ? " .
                     "   AND usersession_expires IS NOT NULL";
            @params = ($session_id_clean);
            $dbh->do($query, undef, @params);
        } else {
            $authres = AUTH_NODATA;
        }
    }

    # We can load the page if the login was ok, or there was no data
    # but a login wasn't required
    if ($authres == AUTH_OK ||
        ($authres == AUTH_NODATA && $type == LOGIN_OPTIONAL)) {

        # login succeded, so we're done
        return $userid;
    }

    # No login details were given, but we require a login if the
    # page does
    if ($authres == AUTH_NODATA && $type == LOGIN_REQUIRED) {
        # Throw up the login page

        my $this_uri = uri_escape($cgi->url(-absolute=>1, -path_info=>1,
                                            -query=>1));
        print $cgi->redirect($roscms_login_page .  $this_uri);
        exit 0;
    }

    # If we get here, then we've run out of options, which shouldn't happen
    ThrowCodeError("authres_unhandled", { authres => $authres,
                                          type => $type, });
}

1;

__END__

=head1 NAME

Bugzilla::Auth::ROSCMS - ROSCMS authentication for Bugzilla

=head1 SUMMARY

This is an L<authentication module|Bugzilla::Auth/"AUTHENTICATION"> for
Bugzilla, which logs the user in using the ROSCMS login system.

=head1 SEE ALSO

L<Bugzilla::Auth>
