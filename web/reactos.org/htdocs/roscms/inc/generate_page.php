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

/*

	ReactOS CMS - Page Generator
	
	(c) by Klemens Friedl <frik85>
	
*/


	// To prevent hacking activity:
	if ( !defined('ROSCMS_SYSTEM') && $rpm_page != "generate_fast")
	{
		die(" ");
	}
	
	if ($rpm_page == "generate_fast") {
		$rpm_sec="generator";
		$rpm_sec2="output";
		$roscms_intern_account_id = "1";
	}
	

	function eval_template($code, $dyncontid) // function code from: http://www.zend.com/zend/art/buffering.php
	{ 
		//echo "<br>ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ".$dyncontid."<br>";
		//ob_end_clean(); 
		ob_start(); 
		$roscms_template_var_pageid="";
		$roscms_template_var_pageid=$dyncontid;
		eval(' ?'.'>'.$code.' <'.'?php '); 
		$output = ob_get_contents(); 
		ob_end_clean(); 
		//echo ob_get_status();
		//$roscms_template_var_pageid="";
		return $output; 
	} 



	// RosCMS Config File:
	include("roscms_config.php");

	//ini_set("max_execution_time","false");
	set_time_limit(0);

	
	$rpm_gen_lang="";
	if (array_key_exists("gen_lang", $_GET)) $rpm_gen_lang=htmlspecialchars($_GET["gen_lang"]);
	if ($rpm_gen_lang != "") {
		$rpm_temp_gen_lang = " AND lang_id = '".mysql_escape_string($rpm_gen_lang)."'";
	}
	else {
		$rpm_temp_gen_lang = "";
	}
	
	//include("db/connect_db.inc.php"); // database connection script


	$roscms_intern_page_counter = 1; // page counter

	// This page was generated in ...
	$gentime = microtime(); 
	$gentime = explode(' ',$gentime); 
	$gentime = $gentime[1] + $gentime[0]; 
	$pg_start = $gentime; 
	
	if ($rpm_site == "" || $rpm_sec2=="genpage") {
		echo "<p><b>RosCMS - HTML & XHTML static page generation</b></p><br>";
	}
	
	// HTML & XHTML
	for($counter=1;$counter<3;$counter++) {
		if ($rpm_site != "" && $rpm_sec2=="view") {
			echo "<!-- Generated with ".$roscms_intern_version." -->\n\n";
			switch ($rpm_forma) {
				case "html":
					$fileformat=$roscms_intern_fileformat_html;
					$w3cformat=$roscms_intern_w3cformat_html;
					//echo "<b>HTML</b><br>";
					$roscms_path_formatfile="../".$roscms_intern_path_html; // output path
					break;
				default:
				case "xhtml":
					$fileformat=$roscms_intern_fileformat_xhtml;
					$w3cformat=$roscms_intern_w3cformat_xhtml;
					//echo "<br><br><b>XHTML</b><br>";
					$roscms_path_formatfile="../".$roscms_intern_path_xhtml; // output path
					break;
			}
		}
		else {
			if ($roscms_standard_output_format == "xhtml") {
				$counter=2;
			}
			
			switch ($counter) {
				case 1:
					$fileformat=$roscms_intern_fileformat_html;
					$w3cformat=$roscms_intern_w3cformat_html;
					echo "<b>HTML</b><br>";
					$roscms_path_formatfile="../".$roscms_intern_path_html; // output path
					
					if ($roscms_standard_output_format == "html") {
						$counter=3;
					}
					break;
				case 2:
					$fileformat=$roscms_intern_fileformat_xhtml;
					$w3cformat=$roscms_intern_w3cformat_xhtml;
					echo "<br><br><b>XHTML</b><br>";
					$roscms_path_formatfile="../".$roscms_intern_path_xhtml; // output path
					break;
			}
		}
		
		// Languages:
			if ($rpm_site != "" && $rpm_sec2=="view") {
				$sql_lang="SELECT * 
							FROM languages
							WHERE lang_level != '0' AND lang_id = '".mysql_escape_string($rpm_lang)."'
							ORDER BY 'lang_level' DESC";
				//echo "<br>!!".$rpm_lang;
			}
			else if ($rpm_site != "" && $rpm_sec2=="genpage") {
				$sql_lang="SELECT * 
							FROM languages
							WHERE lang_level != '0' ".$rpm_temp_gen_lang."
							ORDER BY 'lang_level' DESC";
			}
			else {
				$sql_lang="SELECT * 
							FROM languages
							WHERE lang_level != '0' ".$rpm_temp_gen_lang."
							ORDER BY 'lang_level' DESC";
			}
		$sql_query_lang=mysql_query($sql_lang);
		
		while($myrow_lang=mysql_fetch_row($sql_query_lang)) { // Languages
			$roscms_TEMP_curlang = $myrow_lang[0];
			// Pages:

			if ($rpm_site != "") {
				$query_page = mysql_query("SELECT * 
						FROM pages
						WHERE page_visible != 0 AND page_active = '1' AND page_name = '$rpm_site' AND (page_language = '$myrow_lang[0]' OR page_language = 'all')
						ORDER BY 'page_name' ASC") ;
//						echo "<br>=".$rpm_site;
			}
			else {
				if ($rpm_newcontent == true) {
					echo "<br><br>&nbsp;&nbsp;<b>".$myrow_lang[1]."</b><br>";
					$query_page = mysql_query("SELECT * 
							FROM pages
							WHERE page_visible = '1' AND page_active = '1' AND page_generate_force = '1' AND (page_language = '$myrow_lang[0]' OR page_language = 'all') 
							ORDER BY 'page_name' ASC");
				}
				else {
					echo "<br><br>&nbsp;&nbsp;<b>".$myrow_lang[1]."</b><br>";
					$query_page = mysql_query("SELECT * 
							FROM pages
							WHERE page_visible = '1' AND page_active = '1' AND (page_language = '$myrow_lang[0]' OR page_language = 'all') 
							ORDER BY 'page_name' ASC");
				}
			}
	//		$result = mysql_fetch_array($query); AND pages_extra = ''
			while($result_page = mysql_fetch_array($query_page)) { // Pages
				$data_page=$result_page['page_text'];
				$data_page_extra=$result_page['pages_extra'];
				$data_page_title="";
				$data_page_title=$result_page['page_title'];

				
				
				// Pages Dynamic:
				
				$current_pagedyn=$result_page['page_name'];
				$current_pageid=$result_page['page_id'];
				
				$query_page_dynamic="";
				$query_page_dynamic_cont_id="";
				if ($result_page['pages_extra'] == "dynamic") {
					//echo "<br><br>--Dynamic--";
					$query_page_dynamic = mysql_query("SELECT * 
											FROM `dyn_content` 
											WHERE 1 AND `dyn_content_nr` = 1 AND `dyn_content_name` = '$current_pagedyn'
											ORDER BY `dyn_content_id` ASC ");
					//echo "!!".$query_page_dynamica."-!";
				}
				else {
					//echo "<br><br>--NOTDynamic--";
					$query_page_dynamic = mysql_query("SELECT * 
														FROM `pages` 
														WHERE 1 AND `page_id` = $current_pageid LIMIT 1 "); // temp
				}

				while($result_page_dynamic = mysql_fetch_array($query_page_dynamic)) { // Pages Dynamic
					if (array_key_exists("dyn_content_id", $result_page_dynamic)) $query_page_dynamic_cont_id=$result_page_dynamic['dyn_content_id'];

					if ($query_page_dynamic_cont_id == "") {
						$roscms_pagedyn="0";
						//echo "+NO+";
					}
					else {
						$roscms_pagedyn="1";
						$data_page=$result_page['page_text'];
						//echo "+YES+";
					}

					// This page was generated in (for every page) ...
					$gentimea = microtime(); 
					$gentimea = explode(' ',$gentimea); 
					$gentimea = $gentimea[1] + $gentimea[0]; 
					$pg_starta = $gentimea; 
		
		
		
					// Content:
					
					// Known Bug: language names that begin with "a" will not work; reason -> ORDER BY `content_lang` DESC" <=> content_lang = 'all'
					$query_content = mysql_query("SELECT * 
							FROM content
							WHERE content_visible != 0 AND content_active = '1' AND (content_lang = '$myrow_lang[0]' OR content_lang = '$w3cformat' OR content_lang = 'all')
							ORDER BY `content_lang` DESC");
					while($result_content = mysql_fetch_array($query_content)) { // Content
						/*if ($result_content['content_lang'] == "") {
							continue;
						}
						if ($result_content['content_lang'] != $myrow_lang[0] || $result_content['content_lang'] != $w3cformat) {
							if ()continue;
						}*/
						$data_content=$result_content['content_text'];
						$data_page = str_replace("[#cont_".$result_content['content_name']."]",$data_content,$data_page);

						// RosCMS specific tags (only very important tags!):
						$data_page = str_replace("[#roscms_path_homepage]",$roscms_intern_path_server,$data_page); // homepage path
						if (preg_match("=roscms_content_author=", $data_page)) { // to same generation time
							$res_content_authorid=$result_content['content_usrname_id'];
							//echo "<br>br><br><b>===".$res_content_authorid."</b><br><br><br>";
							$query_usraccountc= mysql_query("SELECT * 
												FROM `users` 
												WHERE `user_id` = ".$res_content_authorid." LIMIT 0 , 1");
							$result_usraccountc=mysql_fetch_array($query_usraccountc);
							$data_page = str_replace("[#roscms_content_author]",$result_usraccountb['user_name'],$data_page); // account that changed the content
						}
					} // end while - Content
					
					// PHP templates for DynContent:
					$query_dyninctexta = mysql_query("SELECT * 
													FROM `include_text` 
													WHERE 1 AND `inc_extra` = 'template_php' AND `inc_vis` = 1");
					while($result_dyninctexta = mysql_fetch_array($query_dyninctexta)) { // PHP templates for DynContent
						if (preg_match("=inc_".$result_dyninctexta["inc_word"]."=", $data_page)) { // to same generation time
							$ev_buffer=eval_template($result_dyninctexta["inc_text"],$query_page_dynamic_cont_id);
							$data_page = str_replace("[#inc_".$result_dyninctexta["inc_word"]."]",$ev_buffer,$data_page);
							$ev_buffer="";
						}
					} // end while - PHP template for DynContent
					
					// Include_Text:
					$query_inc_text = mysql_query("SELECT inc_word, inc_text, inc_lang, inc_vis
							FROM include_text
							WHERE inc_vis=1 AND `inc_extra` = '' AND (inc_lang = '$myrow_lang[0]' OR inc_lang = '$w3cformat' OR inc_lang = 'all')
							ORDER BY 'inc_level' ASC") ;
					while($result_inc_text = mysql_fetch_array($query_inc_text)) { // Include_Text
						$data_page = str_replace("[#inc_".$result_inc_text['inc_word']."]",$result_inc_text['inc_text'],$data_page);
	
						// RosCMS specific tags (only very important tags!):
						$data_page = str_replace("[#roscms_path_homepage]",$roscms_intern_path_server,$data_page); // homepage path
					} // end while - Include_Text
	
					// RosCMS specific tags:
					if ($data_page_extra == "dynamic") {
						$data_page = str_replace("[#roscms_filename]",$result_page['page_name']."_".$query_page_dynamic_cont_id.".".$fileformat,$data_page); // current filename
						$data_page = str_replace("[#roscms_pagename]",$result_page['page_name']."_".$query_page_dynamic_cont_id,$data_page); // current page name
						$data_page = str_replace("[#roscms_pagetitle]",ucfirst($result_page['page_name']."_".$query_page_dynamic_cont_id),$data_page); // current page title
					}
					else {
						$data_page = str_replace("[#roscms_filename]",$result_page['page_name'].".".$fileformat,$data_page); // current filename
						$data_page = str_replace("[#roscms_pagename]",$result_page['page_name'],$data_page); // current page name
						$data_page = str_replace("[#roscms_pagetitle]",$data_page_title,$data_page); // current page title
					}
					$data_page = str_replace("[#roscms_language]",$myrow_lang[1],$data_page); // current language
					$data_page = str_replace("[#roscms_language_short]",$myrow_lang[0],$data_page); // current language
					$data_page = str_replace("[#roscms_format]",$w3cformat,$data_page); // current page format (xhtml/html)
					$data_page = str_replace("[#roscms_date]",date("Y-m-d"),$data_page); // current date
						$zeit = localtime(time() , 1);
					$data_page = str_replace("[#roscms_time]", sprintf("%02d",$zeit['tm_hour']).":".sprintf("%02d",$zeit['tm_min']),$data_page); // current time
						$query_usraccountb= mysql_query("SELECT * 
											FROM `users` 
											WHERE `user_id` = ".$roscms_intern_account_id." LIMIT 0 , 1");
						$result_usraccountb=mysql_fetch_array($query_usraccountb);
						$roscms_intern_usrnameid = $result_page['page_usrname_id'];
						$query_usraccount= mysql_query("SELECT * 
											FROM `users` 
											WHERE `user_id` = ".$roscms_intern_usrnameid." LIMIT 0 , 1");
						$result_usraccount=mysql_fetch_array($query_usraccount);
					$data_page = str_replace("[#roscms_user]",$result_usraccountb['user_name'],$data_page); // account that generate the page
					$data_page = str_replace("[#roscms_inc_author]",$result_usraccount['user_name'],$data_page); // account that changed the include text
					$data_page = str_replace("[#roscms_page_version]",$result_page['page_version'],$data_page); // current page version


					// Add extern link symbol: (code by Dr. Fred)
 
					/* 1.) check if href does begin with http:// */
					$data_page = str_replace ('(<a [^h]*)(href\s*=\s*"(http|ftp)://)', '\1class="extern" \2', $data_page); 
					/* 2.) remove extern-class again if the link goes to http://*.reactos.org */
					$data_page = str_replace ('class="extern" (href="http://([^\.]*.)?([^\.]*.)?reactos.org)', '\1', $data_page);
					/* 3.) remove extern if img */
					$data_page = str_replace ('class="extern" (href[^>]*>([^<]*<img|[^<]*<[^<\/]*<img|[^>]*<[^<\/]]*<[^<a]*<img))', '\1', $data_page);


					$roscms_pagename_file=$result_page['page_name'];
					if ($roscms_pagedyn=="1") { // dynamic page
						$roscms_pagename_file=$result_page['page_name']."_".$query_page_dynamic_cont_id; // add the dynamic page number to the filename
					}

					if ($rpm_debug != "yes") {		
						// Add correct links:
						$query_page_link = mysql_query("SELECT * 
								FROM pages
								WHERE page_visible != 0 AND page_active = '1' AND (page_language = '$myrow_lang[0]' OR page_language = 'all')
								ORDER BY 'page_name' ASC") ;
						while($result_page_link = mysql_fetch_array($query_page_link)) { // Pages
						
							if ($result_page_link['pages_extra']=="dynamic") { // dynamic page
								//echo "<br><br>--222Dynamic--";
								$query_page_dynamic_link = mysql_query("SELECT * 
									FROM `dyn_content` 
									WHERE 1 AND `dyn_content_nr` = 1 AND `dyn_content_active` = 1 AND `dyn_content_visible` = 1");
							}
							else {
								//echo "<br><br>--222NOTDynamic--";
								$query_page_dynamic_link = mysql_query("SELECT * 
																	FROM `pages` 
																	WHERE 1 AND `page_id` = $current_pageid LIMIT 1 "); // temp
							}
			
							while($result_page_dynamic_link = mysql_fetch_array($query_page_dynamic_link)) { // Pages Dynamic
								if ($result_page_link['pages_extra']=="dynamic") { // dynamic page
									$roscms_pagename=$result_page_link['page_name']."_".$result_page_dynamic_link['dyn_content_id']; // add the dynamic page number to the filename
								}
								else {
									$roscms_pagename=$result_page_link['page_name'];
								}
								//echo "<br><br>".$roscms_pagename."<br>";
								if ($w3cformat=="html") {
									// replace HTML tags with valid HTML 4.01 tags:
									$data_page = str_replace("<br />","<br>",$data_page); // current page version
									$data_page = str_replace(" />",">",$data_page); // current page version
									
									// replace links:
									if ($rpm_site != "" && $rpm_sec2=="view") {
										$data_page = str_replace("[#link_".$roscms_pagename."]", $roscms_intern_path_server.$roscms_intern_path_gererator."?page=$rpm_page&amp;sec=generator&amp;sec2=view&amp;site=".$roscms_pagename."&amp;lang=".$myrow_lang[0]."&amp;forma=".$w3cformat."&amp;skin=".$rpm_skin,$data_page); // correct link
									}
									else {
										if ($result_page_link['pages_extention'] == "default") {
											$data_page = str_replace("[#link_".$roscms_pagename."]", $roscms_intern_path_server.$roscms_intern_path_html.$myrow_lang[0]."/".$roscms_pagename.".".$fileformat,$data_page); // correct link
										}
										else {
											$data_page = str_replace("[#link_".$roscms_pagename."]", $roscms_intern_path_server.$roscms_intern_path_html.$myrow_lang[0]."/".$roscms_pagename.".".$result_page_link['pages_extention'],$data_page); // correct link
										}
									}
									//echo " HTML: [#link_".$result_page_link['page_name']."]<br>";
								}
								else if ($w3cformat=="xhtml") {
									// replace HTML 4.01 tags with valid XHTML tags:
									$data_page = str_replace("<br>","<br />",$data_page); // current page version
									//$data_page = preg_replace("/<img(.*)>/sU","<img\\1/>",$data_page); // current page version
									
									// replace links:
									if ($rpm_site != "" && $rpm_sec2=="view") {
										$data_page = str_replace("[#link_".$roscms_pagename."]", $roscms_intern_path_server.$roscms_intern_path_gererator."?page=$rpm_page&amp;sec=generator&amp;sec2=view&amp;site=".$roscms_pagename."&amp;lang=".$myrow_lang[0]."&amp;forma=".$w3cformat."&amp;skin=".$rpm_skin,$data_page); // correct link
									}
									else {
										if ($result_page_link['pages_extention'] == "default") {
											$data_page = str_replace("[#link_".$roscms_pagename."]", $roscms_intern_path_server.$roscms_intern_path_xhtml.$myrow_lang[0]."/".$roscms_pagename.".".$fileformat,$data_page); // correct link
										}
										else {
											$data_page = str_replace("[#link_".$roscms_pagename."]", $roscms_intern_path_server.$roscms_intern_path_xhtml.$myrow_lang[0]."/".$roscms_pagename.".".$result_page_link['pages_extention'],$data_page); // correct link
										}
									}
									//echo " XHTML: [#link_".$result_page_link['page_name']."]<br>";
								}
							}
						}
					}
					
					// redirect all bad links to the frontpage:
					$data_page = str_replace("[#link_",$roscms_intern_path_server."?page=index&amp;temp=",$data_page); // correct link
					// Replace high chars by their html-escaped version
					$data_page = roscms_unicode_escape($data_page);

					// Output & View:
					switch ($rpm_sec2) {
						case "genpage": // generate a single page
						case "output":
							// create HTML file: 
							if ($result_page['pages_extention'] == "default") {
								$output_fileformat = $fileformat;
							}
							else {
								$output_fileformat = $result_page['pages_extention'];
							}
							$filename_page_html = $roscms_path_formatfile.$myrow_lang[0]."/".$roscms_pagename_file.".".$output_fileformat; // create HTML file
							$fp = fopen($filename_page_html,"w");
							flock($fp,2);
							fputs($fp,$data_page); // write content
							fputs($fp,"\n\n<!-- Generated with ".$roscms_intern_version." -->");
							flock($fp,3);
							fclose($fp);
							echo "<br>&nbsp;&nbsp;&nbsp;&nbsp;* &nbsp; \"".$roscms_path_formatfile.$myrow_lang[0]."/".$roscms_pagename_file.".".$output_fileformat."\" generated";
							$page_gen_id=$result_page['page_id'];
							$page_gen_timestamp=time();
							$page_gen_timestamp_query="UPDATE `pages` SET `page_generate_usrid` = '$roscms_intern_account_id', `page_generate_timestamp` = '$page_gen_timestamp' WHERE `page_id` = '$page_gen_id' LIMIT 1 ;";
							$page_gen_timestamp_list=mysql_query($page_gen_timestamp_query);								 // This page was generated in (for every page) ...
								$gentimea = microtime(); 
								$gentimea = explode(' ',$gentimea); 
								$gentimea = $gentimea[1] + $gentimea[0]; 
								$pg_enda = $gentimea; 
								$totaltimea = ($pg_enda - $pg_starta); 
								$showtimea = number_format($totaltimea, 4, '.', ''); 
								echo(" &nbsp; (" . $showtimea . " sec; #".$roscms_intern_page_counter.")");
							$roscms_intern_page_counter++;
							break;
						default:
						case "view":
							echo $data_page;
							echo "\n\n<!-- Generated with ".$roscms_intern_version." -->";
							break;				
					} // end switch
					if ($roscms_pagedyn=="0") {
						break;
					}
				} // end while - PagesDynamic
				if ($rpm_site != "") {
					break;
				}
			} // end while - Pages
			if ($rpm_site != "" && $rpm_sec2=="view") {
				break;
			}
		} // end while - Languages
		if ($rpm_site != "" && $rpm_sec2=="view") {
			break;
		}
	} // end for - html & xhtml


 // This page was generated in ...
$gentime = microtime(); 
$gentime = explode(' ',$gentime); 
$gentime = $gentime[1] + $gentime[0]; 
$pg_end = $gentime; 
$totaltime = ($pg_end - $pg_start); 
$showtime = number_format($totaltime, 4, '.', ''); 
if ($rpm_site == "") {
	echo("<br><br><p>The static HTML & XHTML pages were generated in " . $showtime . " seconds.</p>");
}
else {
	echo("\n\n\n<!-- information for dynamic version -->\n<br><center><font size='1'>This dynamic <b>".$w3cformat." page</b> was <b>generated</b> with ".$roscms_intern_version." in <b>" . $showtime . " seconds.</b></font></center>");
}

if ($rpm_site == "" && $rpm_sec2 != "view" && $rpm_sec2 !="genpage") {
	$query_pagegen_status = mysql_query("SELECT * 
			FROM pages
			WHERE page_visible = '1' AND page_active = '1' 
			ORDER BY 'page_name' ASC ;") ;
	while($result_pagegen_status = mysql_fetch_array($query_pagegen_status)) { // Pages
		$content_postd="UPDATE `pages` SET `page_generate_force` = '0' WHERE `page_id` = '". $result_pagegen_status['page_id'] ."' ;";
		$content_post_listd=mysql_query($content_postd);
	}
}

set_time_limit(30);

?>
