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
	
	if ($rpm_page != "admin" && $rpm_page != "dev") {
		die("");
	}

?><div class="contentSmall"> <span class="contentSmallTitle"><?php echo $roscms_langres['ContTrans_Interface_Pages']; ?></span> 
  <ul>
    <li><strong><a href="?page=<?php echo $rpm_page; ?>&sec=pages&sec2=view&amp;<?php echo 'sort='.$rpm_sort.'&amp;filt='.$rpm_filt.'&amp;langid='.$rpm_lang_id ; ?>"><?php echo $roscms_langres['ContTrans_Pages']; ?></a></strong> 
      <ul>
        <li><?php echo $roscms_langres['ContTrans_ViewEditpage']; ?></li>
      </ul>
    </li>
  </ul>
  <?php

	$query_page = mysql_query("SELECT * 
			FROM pages
			WHERE page_id = '$rpm_db_id'
			ORDER BY 'page_language' ASC") ;

	$farbe=$roscms_intern_color1;
	
	$result_page = mysql_fetch_array($query_page);
?>
  <form name="cms_pages" method="post" action="<?php 
	/*if ($rpm_option == "update") {
		echo '?page=admin&amp;sec=pages&amp;sec2=save&amp;opt=update&amp;db_id='.$rpm_db_id;
	}
	else {
		echo '?page=admin&amp;sec=pages&amp;sec2=save&amp;opt=insert&amp;db_id='.$rpm_db_id;
	}*/
		echo '?page='.$rpm_page.'&amp;sec=pages&amp;sec2=save&amp;db_id='.$rpm_db_id;
	 ?>">
    <table width="600" border="0" cellpadding="1" cellspacing="1">
      <tr bgcolor="<?php echo $roscms_intern_color0; ?>"> 
        <td width="80" bgcolor="<?php echo $roscms_intern_color0; ?>"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_PageID']; ?></strong></font></div></td>
        <td bgcolor="#E2E2E2"> <div align="left"><font face="Arial, Helvetica, sans-serif"> 
            <input name="txt_pageid" type="text" id="txt_pageid" value="<?php echo $result_page['page_name']; ?>" size="50" maxlength="50">
            (e.g. &quot;about&quot;)</font></div></td>
      </tr>
      <tr bgcolor="<?php echo $roscms_intern_color0; ?>"> 
        <td bgcolor="<?php echo $roscms_intern_color0; ?>"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_PageTitle']; ?></strong></font></div></td>
        <td bgcolor="#E2E2E2"><font face="Arial, Helvetica, sans-serif"> 
          <input name="txt_pagetitle" type="text" id="txt_pagetitle" value="<?php echo $result_page['page_title']; ?>" size="50" maxlength="150">
          (e.g. &quot;About xyz&quot;) </font></td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="<?php echo $roscms_intern_color0; ?>"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_Description']; ?></strong></font></div></td>
        <td valign="top" bgcolor="#E2E2E2"><font face="Arial, Helvetica, sans-serif"> 
          <input name="txt_pagedesc" type="text" id="txt_pagedesc" value="<?php echo $result_page['page_description']; ?>" size="50" maxlength="255">
          (-&gt; Sitemap) </font></td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="<?php echo $roscms_intern_color0; ?>"> <div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_Language']; ?></strong></font></div></td>
        <td valign="top" bgcolor="#EEEEEE"><div align="left"> 
            <select id="txt_langa" size="1" name="txt_langa" class="selectbox">
              <?php 
				//echo $result_page['page_language'];
				$cmsros_intern_temp_lang_short=$result_page['page_language'];
				$sql_lang="SELECT * 
					FROM languages
					WHERE lang_level != '0' AND lang_id = '$cmsros_intern_temp_lang_short'
					ORDER BY 'lang_level' DESC Limit 1";
				$sql_query_lang=mysql_query($sql_lang);
				$myrow_lang=mysql_fetch_row($sql_query_lang); // Languages
			?>
              <optgroup label="languages"> 
              <option value="all"<?php if ($result_page['page_language'] == "all") { echo ' selected="selected"'; } ?>>All</option>
              <?php
			  
		// Languages
		$sql_langa="SELECT * 
					FROM languages
					WHERE lang_level != '0'
					ORDER BY 'lang_level' DESC";
		$sql_query_langa=mysql_query($sql_langa);
		while($myrow_langa=mysql_fetch_row($sql_query_langa)) {
              echo '<option value="'.$myrow_langa[0].'"';
			  if ($myrow_lang[0] == $myrow_langa[0]) {
					echo ' selected="selected"';
			  }
			  echo '>'.$myrow_langa[1].'</option>';
		}
              ?>
              </optgroup>
              <optgroup label="other"> 
              <option value="xhtml"<?php if ($result_page['page_language'] == "xhtml") { echo ' selected="selected"'; } ?>>XHTML</option>
              <option value="html"<?php if ($result_page['page_language'] == "html") { echo ' selected="selected"'; } ?>>HTML</option>
              </optgroup>
            </select>
          </div></td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="<?php echo $roscms_intern_color0; ?>"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_Content']; ?></strong></font></div></td>
        <td valign="top" bgcolor="#E2E2E2"><font face="Arial, Helvetica, sans-serif"> 
          <textarea name="textarea_pages" cols="60" rows="15" id="textarea_pages"><?php 
			echo $result_page['page_text'];
		?></textarea>
          </font></td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="<?php echo $roscms_intern_color0; ?>"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_Rev']; ?></strong></font></div></td>
        <td valign="top" bgcolor="#EEEEEE"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			echo $result_page['page_version'];
		?>
          </font></td>
      </tr>
      <tr>
        <td valign="top" bgcolor="<?php echo $roscms_intern_color0; ?>"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_Visible']; ?></strong></font></div></td>
        <td valign="top" bgcolor="#E2E2E2"><font face="Arial, Helvetica, sans-serif">
          <?php 
			if ($result_page['page_visible'] == 1) {
				echo ' <input name="page_visible" type="radio" value="yes" checked> yes &nbsp; <input type="radio" name="page_visible" value="no"> no';
			}
			else {
				echo ' <input name="page_visible" type="radio" value="yes"> yes &nbsp; <input type="radio" name="page_visible" value="no" checked> no';
			}	
		?>
</font></td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="<?php echo $roscms_intern_color0; ?>"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_Active']; ?></strong></font></div></td>
        <td valign="top" bgcolor="#EEEEEE"><font face="Arial, Helvetica, sans-serif">
          <?php 
			if ($result_page['page_active'] == 1) {
				echo ' <input name="page_active" type="radio" value="yes" checked> yes &nbsp; <input type="radio" name="page_active" value="no"> no';
			}
			else {
				echo ' <input name="page_active" type="radio" value="yes"> yes &nbsp; <input type="radio" name="page_active" value="no" checked> no';
			}	
		?> 
          </font></td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="<?php echo $roscms_intern_color0; ?>"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_Extra']; ?></strong></font></div></td>
        <td valign="top" bgcolor="#E2E2E2"> <select id="txt_extra" size="1" name="txt_extra" class="selectbox">
            <optgroup label="current"> 
            <?php   
				echo '<option value="'.$result_page["pages_extra"].'"';
				echo ' selected="selected">';
				
				if ($result_page["pages_extra"]=="") {
					echo "no";
				}
				else {
					echo $result_page["pages_extra"];
				}
				echo '</option>';
				?>
            </optgroup>
            <optgroup label="extra option"> 
            <option value="">no</option>
            <option value="dynamic">dynamic</option>
            </optgroup>
          </select> </td>
      </tr>
      <tr>
        <td valign="top" bgcolor="<?php echo $roscms_intern_color0; ?>"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_Extention']; ?></strong></font></div></td>
        <td valign="top" bgcolor="#E2E2E2"><font face="Arial, Helvetica, sans-serif">
          <input name="txt_pageextent" type="text" id="txt_pageextent" value="<?php echo $result_page['pages_extention']; ?>" size="10" maxlength="10">
(&quot;default&quot;, &quot;xml&quot;, ...) </font></td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="<?php echo $roscms_intern_color0; ?>"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_Date']; ?></strong></font></div></td>
        <td valign="top" bgcolor="#EEEEEE"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			echo $result_page['page_date'];
		?>
          </font></td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="<?php echo $roscms_intern_color0; ?>"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_Time']; ?></strong></font></div></td>
        <td valign="top" bgcolor="#E2E2E2"><font face="Arial, Helvetica, sans-serif"> 
          <?php 
			echo $result_page['page_time'];
		?>
          </font></td>
      </tr>
      <tr> 
        <td valign="top" bgcolor="<?php echo $roscms_intern_color0; ?>"><div align="center"><font color="#FFFFFF" face="Arial, Helvetica, sans-serif"><strong><?php echo $roscms_langres['ContTrans_Action']; ?></strong></font></div></td>
        <td valign="top" bgcolor="#EEEEEE"> 
          <?php if($roscms_intern_usrgrp_sadmin == true) { ?>
          <script type="text/javascript">
			<!--
				function DeletePage() {
					var chk = window.confirm("Do you really want to delete this page?");
					if (chk == true) {
						parent.location.href = "?page=<?php echo $rpm_page; ?>&amp;sec=pages&amp;sec2=delete&amp;db_id=<?php echo $result_page['page_id']; ?>";
					}
				}
			-->
			</script> <a href="javascript:DeletePage()"><img src="images/delete.gif" alt="Delete" width="19" height="18" border="0"></a> 
          <?php  } ?>
        </td>
      </tr>
    </table>
    <p> 
      <input name="page_rad_opt" type="radio" value="insert" checked>
      <?php echo $roscms_langres['ContTrans_SAVE']; ?> &nbsp; 
      <?php 
			//if($roscms_intern_account_level>50 AND $result_page['page_name'] != "") {
		?>
      <input name="page_rad_opt" type="radio" value="update">
      <?php echo $roscms_langres['ContTrans_UPDATE']; ?> 
      <?php //} ?>
    </p>

      <?php 
			//if($roscms_intern_account_level>20) {
		?>
      <input type="submit" name="Submit" value="Submit">
      <?php // }  
	  /*if($roscms_intern_account_level<=20) { ?>
      <img src="images/lock.gif" alt="Locked" width="19" height="18"> (you need 
      a higher account level to save the content to the database)</p>
    <?php } */?>
    <p>&nbsp;</p>
    <p><strong>Info:</strong><br>
      for each [#cont_xyz] or [#inc_xyz] tag the RosCMS will include the code 
      that is linked with this tag from database.<br>
      <br>
      <strong>Code snips</strong></p>
    <ul>
      <li> space = &amp;nbsp; </li>
      <li>&lt;p&gt;&amp;nbsp;&lt;/p&gt;</li>
    </ul>
    <p>&nbsp;</p>
  </form>
</div>
