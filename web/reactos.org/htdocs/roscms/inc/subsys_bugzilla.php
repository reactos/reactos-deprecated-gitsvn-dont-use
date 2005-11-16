<?php
    /*
    RosCMS - ReactOS Content Management System
    Copyright (C) 2005  Ge van Geldorp <gvg@reactos.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
    */

// To prevent hacking activity:
if ( !defined('ROSCMS_SYSTEM'))
{
  if ( !defined('ROSCMS_SYSTEM_LOG') ) {
    define ("ROSCMS_SYSTEM_LOG", "Hacking attempt");
  }
  $seclog_section="subsys_bugzilla";
  $seclog_level="50";
  $seclog_reason="Hacking attempt: subsys_bugzilla.php";
  define ("ROSCMS_SYSTEM", "Hacking attempt");
  include('securitylog.php'); // open security log
  die("Hacking attempt");
}

require_once("subsys_utils.php");

define('SUBSYS_BUGZILLA_DBNAME', "bugs");

function subsys_bugzilla_info_check()
{
  $inconsistency_count = 0;
  $query = "SELECT u.user_id, u.user_name, u.user_email, " .
           "       p.realname, p.login_name " .
           "  FROM users u, " .
           "       subsys_mappings m, " .
                   SUBSYS_BUGZILLA_DBNAME . ".profiles p " .
           " WHERE m.map_roscms_userid = u.user_id " .
           "   AND m.map_subsys_name = 'bugzilla' " .
           "   AND p.userid = m.map_subsys_userid " .
           "   AND (u.user_name != p.realname OR " .
           "        p.realname IS NULL OR " .
           "        u.user_email != p.login_name) ";
  $query_set = mysql_query($query) or die("DB error (subsys_bugzilla #1)");
  while ($result_row = mysql_fetch_array($query_set))
    {
    echo "Info mismatch for RosCMS userid " . $result_row['user_id'] . ": ";
    if ($result_row['user_name'] <> $result_row['realname'])
    {
      echo "user_name " . $result_row['user_name'] . "/" .
           $result_row['realname'] . " ";
    }
    if ($result_row['user_email'] <> $result_row['login_name'])
    {
      echo "user_email " . $result_row['user_email'] . "/" .
           $result_row['login_name'];
    }
    echo "<br>\n";
    $inconsistency_count++;
    }

  return $inconsistency_count;
}

function subsys_bugzilla_check()
{
?>
<h2>Bugzilla</h2>
<?php
  $inconsistency_count = 0;
  $inconsistency_count += subsys_mapping_check('bugzilla');
  $inconsistency_count += subsys_userids_exist('bugzilla',
                                               SUBSYS_BUGZILLA_DBNAME .
                                               '.profiles',
                                               'userid');
  $inconsistency_count += subsys_bugzilla_info_check();
  $fix_url = htmlentities('?page=admin&sec=subsys&sec2=fix&subsys=bugzilla');
  if (0 == $inconsistency_count)
    {
      echo "No problems found.<br/>\n";
    }
  else if (1 == $inconsistency_count)
    {
      echo '<br />1 problem found. <a href="' . $fix_url .
           '">Fix this</a><br/>' . "\n";
    }
  else
    {
      echo "<br />$inconsistency_count problems found." .
           ' <a href="' . $fix_url . '">Fix these</a><br/>' . "\n";
    }

  return $inconsistency_count;
}

function subsys_bugzilla_add_roscms_users()
{
  $fix_count = 0;

  $query = "SELECT p.userid AS bz_user_id, p.login_name AS bz_user_email, " .
           "       p.realname AS bz_user_name " .
           "  FROM " . SUBSYS_BUGZILLA_DBNAME . ".profiles p " .
           " WHERE p.userid NOT IN " .
           "       (SELECT m.map_subsys_userid " .
           "          FROM subsys_mappings m " .
           "         WHERE m.map_subsys_userid = p.userid " .
           "           AND m.map_subsys_name = 'bugzilla')";
  $query_set = mysql_query($query) or die("DB error (subsys_bugzilla #13)");
  while ($result_row = mysql_fetch_array($query_set))
    {
      if (subsys_add_roscms_user($result_row['bz_user_name'],
                                 '*UNKNOWN*',
                                 $result_row['bz_user_email'],
                                 $result_row['bz_user_name'],
                                 'bugzilla',
                                 $result_row['bz_user_id']))
        {
          $fix_count++;
        }
    }

  return $fix_count;
}

function subsys_bugzilla_add_bugzilla_user($roscms_user_id,
                                           $roscms_user_name,
                                           $roscms_user_email)
{
  /* These default settings were taken from bugzilla/Bugzilla/Constants.pm,
     DEFAULT_EMAIL_SETTINGS */
  $default_email_settings = "ExcludeSelf~on" .

                            "~FlagRequestee~on" .
                            "~FlagRequester~on" .

                            "~emailOwnerRemoveme~on" .
                            "~emailOwnerComments~on" .
                            "~emailOwnerAttachments~on" .
                            "~emailOwnerStatus~on" .
                            "~emailOwnerResolved~on" .
                            "~emailOwnerKeywords~on" .
                            "~emailOwnerCC~on" .
                            "~emailOwnerOther~on" .
                            "~emailOwnerUnconfirmed~on" .
  
                            "~emailReporterRemoveme~on" .
                            "~emailReporterComments~on" .
                            "~emailReporterAttachments~on" .
                            "~emailReporterStatus~on" .
                            "~emailReporterResolved~on" .
                            "~emailReporterKeywords~on" .
                            "~emailReporterCC~" .
                            "~emailReporterOther~on" .
                            "~emailReporterUnconfirmed~on" .
  
                            "~emailQAcontactRemoveme~on" .
                            "~emailQAcontactComments~on" .
                            "~emailQAcontactAttachments~on" .
                            "~emailQAcontactStatus~on" .
                            "~emailQAcontactResolved~on" .
                            "~emailQAcontactKeywords~on" .
                            "~emailQAcontactCC~" .
                            "~emailQAcontactOther~on" .
                            "~emailQAcontactUnconfirmed~on" .
  
                            "~emailCClistRemoveme~on" .
                            "~emailCClistComments~on" .
                            "~emailCClistAttachments~on" .
                            "~emailCClistStatus~on" .
                            "~emailCClistResolved~on" .
                            "~emailCClistKeywords~on" .
                            "~emailCClistCC~" .
                            "~emailCClistOther~on" .
                            "~emailCClistUnconfirmed~on" .
  
                            "~emailVoterRemoveme~on" .
                            "~emailVoterComments~on" .
                            "~emailVoterAttachments~on" .
                            "~emailVoterStatus~on" .
                            "~emailVoterResolved~on" .
                            "~emailVoterKeywords~on" .
                            "~emailVoterCC~" .
                            "~emailVoterOther~on" .
                            "~emailVoterUnconfirmed~on";

  $query = "INSERT INTO " . SUBSYS_BUGZILLA_DBNAME . ".profiles " .
           "       (login_name, cryptpassword, realname, emailflags) " .
           "       VALUES ('" . mysql_real_escape_string($roscms_user_email) .
                           "', " .
                           "'*', " .
                           "'" . mysql_real_escape_string($roscms_user_name) .
                           "', " .
                           "'" . $default_email_settings . "')";
  mysql_query($query) or die("DB error (subsys_bugzilla #10)");

  /* Finally, insert a row in the mapping table */
  $query = "INSERT INTO subsys_mappings " .
           "       (map_roscms_userid, map_subsys_name, map_subsys_userid) " .
           "       VALUES($roscms_user_id, 'bugzilla', LAST_INSERT_ID())";
  mysql_query($query) or die("DB error (subsys_bugzilla #11)");

  return TRUE;
}

function subsys_bugzilla_update_bugzilla($roscms_user_id,
                                         $roscms_user_name,
                                         $roscms_user_email,
                                         $bz_user_id)
{
  /* Make sure that the email address and/or user name are not already in
     use in bugzilla */
  $query = "SELECT COUNT(*) AS inuse " .
           "  FROM " . SUBSYS_BUGZILLA_DBNAME .  ".profiles " .
           " WHERE (LOWER(login_name) = LOWER('" .
           mysql_real_escape_string($roscms_user_email) . "') OR " .
           "        LOWER(realname) = LOWER('" .
           mysql_real_escape_string($roscms_user_name) . "')) " .
           "   AND userid <> $bz_user_id ";
  $bz_check_set = mysql_query($query)
                  or die("DB error (subsys_bugzilla #7)");
  $bz_check_row = mysql_fetch_array($bz_check_set);
  if (0 != $bz_check_row['inuse'])
    {
      echo "User name ($roscms_user_name) and/or email address " .
           "($roscms_user_email) collision<br>\n";
      return FALSE;
    }

  /* Now, make sure that info in bugzilla matches info in roscms */
  $query = "UPDATE " . SUBSYS_BUGZILLA_DBNAME .  ".profiles " .
           "   SET login_name = '" .
                   mysql_real_escape_string($roscms_user_email) . "', " .
           "       realname = '" .
                   mysql_real_escape_string($roscms_user_name) . "' " .
           " WHERE userid = $bz_user_id";
  mysql_query($query) or die("DB error (subsys_bugzilla #8)");

  return TRUE;
}

function subsys_bugzilla_add_mapping($roscms_user_id)
{
  if (! subsys_get_roscms_info($roscms_user_id,
                               $roscms_user_name,
                               $roscms_user_email,
                               $roscms_user_fullname,
                               $roscms_user_password,
                               $roscms_user_register))
    {
      return FALSE;
    }

  /* First, try to match on email address */
  $query = "SELECT userid " .
           "  FROM " . SUBSYS_BUGZILLA_DBNAME .  ".profiles " .
           " WHERE LOWER(login_name) = LOWER('" .
           mysql_real_escape_string($roscms_user_email) . "')";
  $bz_email_set = mysql_query($query)
                  or die("DB error (subsys_bugzilla #5)");
  if ($bz_email_row = mysql_fetch_array($bz_email_set))
    {
      $bz_user_id = $bz_email_row['userid'];
    }
  else
    {
      /* That failed. Let's try to match on user name then */
      $query = "SELECT userid " .
               "  FROM " . SUBSYS_BUGZILLA_DBNAME .  ".profiles " .
               " WHERE LOWER(realname) = LOWER('" .
               mysql_real_escape_string($roscms_user_name) . "')";
      $bz_name_set = mysql_query($query)
                     or die("DB error (subsys_bugzilla #6)");
      if ($bz_name_row = mysql_fetch_array($bz_name_set))
        {
          $bz_user_id = $bz_name_row['userid'];
        }
    }

  if (! isset($bz_user_id))
    {
      /* We haven't found a match, so we need to add a new bugzilla user */
      $fixed = subsys_bugzilla_add_bugzilla_user($roscms_user_id,
                                                 $roscms_user_name,
                                                 $roscms_user_email);
    }
  else
    {
      /* Synchronize the info in bugzilla */
      if (! subsys_bugzilla_update_bugzilla($roscms_user_id,
                                            $roscms_user_name,
                                            $roscms_user_email,
                                            $bz_user_id))
        {
          return FALSE;
        }

      /* Insert a row in the mapping table */
      $query = "INSERT INTO subsys_mappings " .
               "       (map_roscms_userid, map_subsys_name, map_subsys_userid) " .
               "       VALUES($roscms_user_id, 'bugzilla', $bz_user_id)";
      mysql_query($query) or die("DB error (subsys_bugzilla #9)");

      $fixed = TRUE;
    }

  return $fixed;
}

function subsys_bugzilla_update_existing($roscms_user_id, $bz_user_id)
{
  if (! subsys_get_roscms_info($roscms_user_id,
                               $roscms_user_name,
                               $roscms_user_email,
                               $roscms_user_fullname,
                               $roscms_user_password,
                               $roscms_user_register))
    {
      return FALSE;
    }

  if (! subsys_bugzilla_update_bugzilla($roscms_user_id,
                                        $roscms_user_name,
                                        $roscms_user_email,
                                        $bz_user_id))
    {
      return FALSE;
    }

  return TRUE;
}

function subsys_bugzilla_update_user($roscms_user_id)
{
  $query = "SELECT map_subsys_userid " .
           "  FROM subsys_mappings " .
           " WHERE map_roscms_userid = $roscms_user_id " .
           "   AND map_subsys_name = 'bugzilla'";
  $query_set = mysql_query($query) or die("DB error (subsys_bugzilla #2)");
  if ($result_row = mysql_fetch_array($query_set))
    {
      $fixed = subsys_bugzilla_update_existing($roscms_user_id,
                                               $result_row['map_subsys_userid']);
    }
  else
    {
      $fixed = subsys_bugzilla_add_mapping($roscms_user_id);
    }

  return $fixed;
}

function subsys_bugzilla_fix_mappings()
{
  $fix_count = 0;
  $query = "SELECT u.user_id " .
           "  FROM users u " .
           " WHERE u.user_id NOT IN " .
           "       (SELECT m.map_roscms_userid " .
           "          FROM subsys_mappings m " .
           "         WHERE m.map_roscms_userid = u.user_id " .
           "           AND m.map_subsys_name = 'bugzilla')";
  $query_set = mysql_query($query) or die("DB error (subsys_bugzilla #3)");
  while ($result_row = mysql_fetch_array($query_set))
    {
      if (subsys_bugzilla_update_user($result_row['user_id']))
        {
          echo "Fixed mapping of RosCMS userid " . $result_row['user_id'] .
               "<br>\n";
          $fix_count++;
        }
      else
        {
          echo "Unable to fix mapping of RosCMS userid " .
               $result_row['user_id'] .  "<br>\n";
        }
    }

  return $fix_count;
}

function subsys_bugzilla_fix_info()
{
  $fix_count = 0;
  $query = "SELECT u.user_id " .
           "  FROM users u, " .
           "       subsys_mappings m, " .
                   SUBSYS_BUGZILLA_DBNAME . ".profiles p " .
           " WHERE m.map_roscms_userid = u.user_id " .
           "   AND m.map_subsys_name = 'bugzilla' " .
           "   AND p.userid = m.map_subsys_userid " .
           "   AND (u.user_name != p.realname OR " .
           "        p.realname IS NULL OR " .
           "        u.user_email != p.login_name) ";
  $query_set = mysql_query($query) or die("DB error (subsys_bugzilla #12)");
  while ($result_row = mysql_fetch_array($query_set))
    {
      if (subsys_bugzilla_update_user($result_row['user_id']))
        {
          echo "Fixed info for RosCMS userid " . $result_row['user_id'] .
               "<br>\n";
          $fix_count++;
        }
      else
        {
          echo "Unable to fix info for RosCMS userid " .
               $result_row['user_id'] .  "<br>\n";
        }
    }

  return $fix_count;
}

function subsys_bugzilla_fix_all()
{
?>
<h2>Bugzilla</h2>
<?php
  $fix_count = 0;
  $fix_count += subsys_bugzilla_fix_mappings();
  $fix_count += subsys_bugzilla_add_roscms_users();
  $fix_count += subsys_bugzilla_fix_info();
  if (0 == $fix_count)
    {
      echo "No problems fixed.<br/>\n";
    }
  else if (1 == $fix_count)
    {
      echo "<br>1 problem fixed.<br/>\n";
    }
  else
    {
      echo "<br>$fix_count problems fixed.<br/>\n";
    }

  return $fix_count;
}

?>
