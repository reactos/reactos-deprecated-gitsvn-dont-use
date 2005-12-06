<?php
    /*
    RosCMS - ReactOS Content Management System
    Copyright (C) 2005  Klemens Friedl <frik85@reactos.org>

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
?>
<p>&nbsp;</p>
<?php 
if ($roscms_infotable == "" || $roscms_infotable == "page") { ?>
<table width="100%" border="1" cellpadding="5" cellspacing="0" bordercolor="#000000">
  <tr> 
    <td colspan="2" valign="top"><strong><font size="2">Table Legend - "Static Pages"</font></strong> 
    </td>
  </tr>
  <tr> 
    <td width="50%" valign="top"> <table width="100%" border="0">
        <tr> 
          <td width="25" valign="top"><img src="images/view.gif" width="19" height="18" border="0"></td>
          <td valign="middle"><font size="1"><strong>View/Edit</strong> entry</font></td>
        </tr>
        <tr> 
          <td valign="top"><img src="images/tool.gif" width="19" height="18" border="0"></td>
          <td valign="middle"><font size="1"><strong>Translate</strong> entry</font></td>
        </tr>
        <tr> 
          <td valign="top"><img src="images/genpage.gif" width="19" height="18"></td>
          <td valign="middle"><font size="1"><strong>Generate</strong> a single 
            <strong> static page</strong></font></td>
        </tr>
        <tr> 
          <td valign="top"><img src="images/delete.gif" width="19" height="18"></td>
          <td valign="middle"><font size="1"><strong>Delete</strong> entry</font></td>
        </tr>
        <tr> 
          <td valign="top"><img src="images/history.gif" width="19" height="18"></td>
          <td valign="middle"><font size="1">History of one entry: list all<strong> 
            revisions of the entry</strong></font></td>
        </tr>
        <tr> 
          <td valign="top"><img src="images/lock.gif" width="19" height="18"></td>
          <td valign="middle"><font size="1">Locked: if you have not the<strong> 
            required account level </strong></font></td>
        </tr>
      </table></td>
    <td width="50%" valign="top"> <table width="100%" border="0">
        <tr valign="top"> 
          <td width="25"><img src="images/active.gif" width="19" height="18"></td>
          <td valign="middle"><font size="1"><strong>active</strong> entry: only 
            active entries will go through the static generator</font></td>
        </tr>
        <tr valign="top"> 
          <td><img src="images/notactive.gif" width="19" height="18"></td>
          <td valign="middle"><font size="1"><strong>NOT active</strong> entry: 
            only one entry each &quot;entry id&quot; can be active</font></td>
        </tr>
        <tr valign="top"> 
          <td><img src="images/visible.gif" width="19" height="18"></td>
          <td valign="middle"><font size="1"><strong>visible</strong> entry</font></td>
        </tr>
        <tr valign="top"> 
          <td><img src="images/notvisible.gif" width="19" height="18"></td>
          <td valign="middle"><font size="1"><strong>NOT visible</strong> entry</font></td>
        </tr>
        <tr valign="top"> 
          <td><img src="images/richtexteditor.gif" width="19" height="18"></td>
          <td valign="middle"><font size="1"><strong>Rich Text Editor</strong></font></td>
        </tr>
        <tr valign="top"> 
          <td><img src="images/bbcode.gif" width="19" height="18"></td>
          <td valign="middle"><font size="1"><strong>bbcode Editor</strong></font></td>
        </tr>
      </table></td>
  </tr>
</table>
<?php } elseif ($roscms_infotable == "user") { ?>
<table width="100%" border="1" cellpadding="5" cellspacing="0" bordercolor="#000000">
  <tr> 
    <td colspan="2" valign="top"><strong><font size="2">Table Legend - "User"</font></strong> 
    </td>
  </tr>
  <tr> 
    <td width="50%" valign="top"> <table width="100%" border="0">
        <tr> 
          <td width="25" valign="top"><img src="images/view.gif" width="19" height="18" border="0"></td>
          <td valign="middle"><font size="1"><strong>View/Edit</strong> entry</font></td>
        </tr>
        <tr>
          <td valign="top"><img src="images/tool.gif" width="19" height="18" border="0"></td>
          <td valign="middle"><font size="1"><strong>Add</strong> entry</font></td>
        </tr>
        <tr> 
          <td valign="top"><img src="images/delete.gif" width="19" height="18"></td>
          <td valign="middle"><font size="1"><strong>Delete</strong> entry</font></td>
        </tr>
        <tr valign="top"> 
          <td><img src="images/mail.gif" width="19" height="18"></td>
          <td valign="middle"><font size="1"><strong>Email address</strong></font></td>
        </tr>
        <tr valign="top"> 
          <td><img src="images/home.gif" width="19" height="18"></td>
          <td valign="middle"><font size="1"><strong>Website</strong></font></td>
        </tr>
      </table></td>
    <td width="50%" valign="top"> <table width="100%" border="0">
        <tr valign="top"> 
          <td width="25"><img src="images/active.gif" width="19" height="18"></td>
          <td valign="middle"><font size="1"><strong>active</strong> account: 
            only active accounts are useable</font></td>
        </tr>
        <tr valign="top"> 
          <td><img src="images/notactive.gif" width="19" height="18"></td>
          <td valign="middle"><font size="1"><strong>NOT active</strong> account</font></td>
        </tr>
        <tr valign="top"> 
          <td><img src="images/visible.gif" width="19" height="18"></td>
          <td valign="middle"><font size="1"><strong>visible</strong> account</font></td>
        </tr>
        <tr valign="top"> 
          <td><img src="images/notvisible.gif" width="19" height="18"></td>
          <td valign="middle"><font size="1"><strong>hidden</strong> account</font></td>
        </tr>
      </table></td>
  </tr>
</table>
<?php } ?>
