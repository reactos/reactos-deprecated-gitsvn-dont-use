-- Database: `roscms`
--
-- Date: 2007-08-13 11:12
-- MySQL-Version: 5.0.33
-- PHP-Version: 5.2.1
-- 

-- --------------------------------------------------------

-- 
-- Tabellenstruktur für Tabelle `data_`
-- 

CREATE TABLE `data_` (
  `data_id` int(11) NOT NULL auto_increment,
  `data_name` varchar(100) collate utf8_unicode_ci NOT NULL COMMENT 'together with page_type unique entries',
  `data_type` varchar(10) collate utf8_unicode_ci NOT NULL COMMENT '"page", "layout", "content", "script"',
  `data_acl` varchar(50) collate utf8_unicode_ci NOT NULL default 'default',
  PRIMARY KEY  (`data_id`),
  KEY `data_name` (`data_name`,`data_type`),
  KEY `data_type` (`data_type`),
  KEY `data_acl` (`data_acl`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='all RosCMS v1 "pages", "content", "dyncontent" and "inctext"';

INSERT INTO `data_` VALUES (1, 'index', 'page', 'defaultpage');
INSERT INTO `data_` VALUES (2, 'default', 'template', 'layout');
INSERT INTO `data_` VALUES (3, 'index', 'content', 'default');
INSERT INTO `data_` VALUES (4, 'news', 'script', 'script');
INSERT INTO `data_` VALUES (5, 'news_page', 'content', 'default');


-- --------------------------------------------------------

-- 
-- Tabellenstruktur für Tabelle `data_a`
-- 

CREATE TABLE `data_a` (
  `data_id` int(11) NOT NULL auto_increment,
  `data_name` varchar(100) collate utf8_unicode_ci NOT NULL COMMENT 'together with page_type unique entries',
  `data_type` varchar(10) collate utf8_unicode_ci NOT NULL COMMENT '"page", "layout", "content", "script"',
  `data_acl` varchar(50) collate utf8_unicode_ci NOT NULL default 'default',
  PRIMARY KEY  (`data_id`),
  KEY `data_name` (`data_name`,`data_type`),
  KEY `data_type` (`data_type`),
  KEY `data_acl` (`data_acl`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='all RosCMS v1 "pages", "content", "dyncontent" and "inctext"';

-- --------------------------------------------------------

-- 
-- Tabellenstruktur für Tabelle `data_revision`
-- 

CREATE TABLE `data_revision` (
  `rev_id` int(11) NOT NULL auto_increment COMMENT 'unique revisions id',
  `data_id` int(11) NOT NULL,
  `rev_version` int(11) NOT NULL COMMENT 'version number increase with every entry change',
  `rev_language` varchar(10) collate utf8_unicode_ci NOT NULL COMMENT 'language code ("en", "de", etc.)',
  `rev_usrid` int(11) NOT NULL COMMENT 'user id who saved the entry',
  `rev_datetime` datetime NOT NULL,
  `rev_date` date NOT NULL,
  `rev_time` time NOT NULL,
  PRIMARY KEY  (`rev_id`),
  KEY `data_id` (`data_id`),
  KEY `rev_language` (`rev_language`),
  KEY `rev_version` (`rev_version`),
  KEY `rev_datetime` (`rev_datetime`),
  KEY `rev_usrid` (`rev_usrid`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

INSERT INTO `data_revision` VALUES (1, 1, 1, 'en', 1, '2007-08-13 17:03:44', '2007-08-13', '17:03:44');
INSERT INTO `data_revision` VALUES (2, 2, 1, 'en', 1, '2007-08-13 17:05:03', '2007-08-13', '17:05:03');
INSERT INTO `data_revision` VALUES (6, 3, 2, 'en', 1, '2007-08-13 17:15:15', '2007-08-13', '17:15:15');
INSERT INTO `data_revision` VALUES (5, 4, 2, 'en', 1, '2007-08-13 17:11:36', '2007-08-13', '17:11:36');
INSERT INTO `data_revision` VALUES (7, 5, 1, 'en', 1, '2007-08-13 17:15:20', '2007-08-13', '17:15:20');
INSERT INTO `data_revision` VALUES (8, 5, 1, 'en', 1, '2007-08-13 17:15:45', '2007-08-13', '17:15:45');


-- --------------------------------------------------------

-- 
-- Tabellenstruktur für Tabelle `data_revision_a`
-- 

CREATE TABLE `data_revision_a` (
  `rev_id` int(11) NOT NULL auto_increment COMMENT 'unique revisions id',
  `data_id` int(11) NOT NULL,
  `rev_version` int(11) NOT NULL COMMENT 'version number increase with every entry change',
  `rev_language` varchar(10) collate utf8_unicode_ci NOT NULL COMMENT 'language code ("en", "de", etc.)',
  `rev_usrid` int(11) NOT NULL COMMENT 'user id who saved the entry',
  `rev_datetime` datetime NOT NULL,
  `rev_date` date NOT NULL,
  `rev_time` time NOT NULL,
  PRIMARY KEY  (`rev_id`),
  KEY `data_id` (`data_id`),
  KEY `rev_language` (`rev_language`),
  KEY `rev_version` (`rev_version`),
  KEY `rev_datetime` (`rev_datetime`),
  KEY `rev_usrid` (`rev_usrid`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- --------------------------------------------------------

-- 
-- Tabellenstruktur für Tabelle `data_security`
-- 

CREATE TABLE `data_security` (
  `sec_name` varchar(50) collate utf8_unicode_ci NOT NULL,
  `sec_fullname` varchar(100) collate utf8_unicode_ci NOT NULL,
  `sec_description` varchar(255) collate utf8_unicode_ci NOT NULL,
  `sec_branch` varchar(10) collate utf8_unicode_ci NOT NULL,
  `sec_lev1_read` tinyint(4) NOT NULL default '0',
  `sec_lev1_write` tinyint(4) NOT NULL default '0',
  `sec_lev1_add` tinyint(4) NOT NULL default '0',
  `sec_lev1_pub` tinyint(4) NOT NULL default '0',
  `sec_lev1_trans` tinyint(4) NOT NULL default '0',
  `sec_lev2_read` tinyint(4) NOT NULL default '0',
  `sec_lev2_write` tinyint(4) NOT NULL default '0',
  `sec_lev2_add` tinyint(4) NOT NULL default '0',
  `sec_lev2_pub` tinyint(4) NOT NULL default '0',
  `sec_lev2_trans` tinyint(4) NOT NULL default '0',
  `sec_lev3_read` tinyint(4) NOT NULL default '0',
  `sec_lev3_write` tinyint(4) NOT NULL default '0',
  `sec_lev3_add` tinyint(4) NOT NULL default '0',
  `sec_lev3_pub` tinyint(4) NOT NULL default '0',
  `sec_lev3_trans` tinyint(4) NOT NULL default '0',
  `sec_allow` varchar(255) collate utf8_unicode_ci NOT NULL,
  `sec_deny` varchar(255) collate utf8_unicode_ci NOT NULL,
  PRIMARY KEY  (`sec_name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='RosCMS content security';

INSERT INTO `data_security` VALUES ('default', 'Content (default)', 'Default content security ACL; normal translator cannot publish nor add new content', 'website', 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, '', '');
INSERT INTO `data_security` VALUES ('script', 'Script or Variable', 'Entry contain scripts, variables or similar content which can only be accessed and altered by admins.', 'website', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, '', '');
INSERT INTO `data_security` VALUES ('layout', 'Layout', 'Website layout related content, changing may destroy the website layout, so be careful!', 'website', 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, '', '|transmaint|');
INSERT INTO `data_security` VALUES ('announcement', 'Official announcement', 'Official announcement, which shouldn''t be altered by anyone except admins and devs', 'website', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, '|developer|', '');
INSERT INTO `data_security` VALUES ('notrans', 'No translation needed', 'Content don''t need a translation.', '', 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, '', '');
INSERT INTO `data_security` VALUES ('rosversion', 'ReactOS versions number', 'Currrent ReactOS version number, change the related contents on every release day!', 'website', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, '|developer|', '');
INSERT INTO `data_security` VALUES ('defaultpage', 'Page (default)', 'Default page security ACL', 'website', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, '|developer|', '');
INSERT INTO `data_security` VALUES ('defaultmenu', 'Menu (default)', 'Menu content which can be translated by language maintainers', 'website', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, '|transmaint|', '');
INSERT INTO `data_security` VALUES ('system', 'System', 'RosCMS system', 'website', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '|ros_sadmin|', '');
INSERT INTO `data_security` VALUES ('readonly', 'Read-Only', 'Read only, noone can alter this', 'website', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, '', '');
INSERT INTO `data_security` VALUES ('langmaint', 'Language Maintainer', 'Language-Group related content', 'website', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, '|transmaint|', '');


-- --------------------------------------------------------

-- 
-- Tabellenstruktur für Tabelle `data_stext`
-- 

CREATE TABLE `data_stext` (
  `stext_id` int(11) NOT NULL auto_increment,
  `data_rev_id` int(11) NOT NULL,
  `stext_name` varchar(100) collate utf8_unicode_ci NOT NULL,
  `stext_content` varchar(255) collate utf8_unicode_ci NOT NULL,
  PRIMARY KEY  (`stext_id`),
  KEY `data_rev_id` (`data_rev_id`),
  FULLTEXT KEY `stext_content` (`stext_content`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

INSERT INTO `data_stext` VALUES (14, 1, 'description', 'test page');
INSERT INTO `data_stext` VALUES (13, 1, 'comment', '');
INSERT INTO `data_stext` VALUES (12, 2, 'description', 'test template');
INSERT INTO `data_stext` VALUES (15, 1, 'title', 'Index');
INSERT INTO `data_stext` VALUES (19, 6, 'description', 'test content');
INSERT INTO `data_stext` VALUES (18, 5, 'description', 'news php script');
INSERT INTO `data_stext` VALUES (22, 7, 'description', 'news issue 1');
INSERT INTO `data_stext` VALUES (23, 7, 'title', 'News_page');
INSERT INTO `data_stext` VALUES (26, 8, 'description', 'news issue 2');
INSERT INTO `data_stext` VALUES (27, 8, 'title', 'News_page');


-- --------------------------------------------------------

-- 
-- Tabellenstruktur für Tabelle `data_stext_a`
-- 

CREATE TABLE `data_stext_a` (
  `stext_id` int(11) NOT NULL auto_increment,
  `data_rev_id` int(11) NOT NULL,
  `stext_name` varchar(100) collate utf8_unicode_ci NOT NULL,
  `stext_content` varchar(255) collate utf8_unicode_ci NOT NULL,
  PRIMARY KEY  (`stext_id`),
  KEY `data_rev_id` (`data_rev_id`),
  FULLTEXT KEY `stext_content` (`stext_content`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- --------------------------------------------------------

-- 
-- Tabellenstruktur für Tabelle `data_tag`
-- 

CREATE TABLE `data_tag` (
  `tag_id` int(11) NOT NULL auto_increment,
  `data_id` int(11) NOT NULL,
  `data_rev_id` int(11) NOT NULL,
  `tag_name_id` int(11) NOT NULL,
  `tag_value_id` int(11) NOT NULL,
  `tag_usrid` int(11) NOT NULL,
  PRIMARY KEY  (`tag_id`),
  KEY `data_id` (`data_id`),
  KEY `data_rev_id` (`data_rev_id`),
  KEY `tag_name_id` (`tag_name_id`),
  KEY `tag_value_id` (`tag_value_id`),
  KEY `tag_usrid` (`tag_usrid`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

INSERT INTO `data_tag` VALUES (11, 1, 1, 1, 5, -1);
INSERT INTO `data_tag` VALUES (2, 1, 1, 2, 2, -1);
INSERT INTO `data_tag` VALUES (10, 2, 2, 1, 5, -1);
INSERT INTO `data_tag` VALUES (15, 4, 5, 5, 7, -1);
INSERT INTO `data_tag` VALUES (14, 4, 5, 1, 5, -1);
INSERT INTO `data_tag` VALUES (24, 3, 6, 1, 5, -1);
INSERT INTO `data_tag` VALUES (22, 5, 7, 1, 5, -1);
INSERT INTO `data_tag` VALUES (18, 5, 7, 6, 9, -1);
INSERT INTO `data_tag` VALUES (21, 5, 8, 1, 5, -1);
INSERT INTO `data_tag` VALUES (20, 5, 8, 6, 10, -1);


-- --------------------------------------------------------

-- 
-- Tabellenstruktur für Tabelle `data_tag_a`
-- 

CREATE TABLE `data_tag_a` (
  `tag_id` int(11) NOT NULL auto_increment,
  `data_id` int(11) NOT NULL,
  `data_rev_id` int(11) NOT NULL,
  `tag_name_id` int(11) NOT NULL,
  `tag_value_id` int(11) NOT NULL,
  `tag_usrid` int(11) NOT NULL,
  PRIMARY KEY  (`tag_id`),
  KEY `data_id` (`data_id`),
  KEY `data_rev_id` (`data_rev_id`),
  KEY `tag_name_id` (`tag_name_id`),
  KEY `tag_value_id` (`tag_value_id`),
  KEY `tag_usrid` (`tag_usrid`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- --------------------------------------------------------

-- 
-- Tabellenstruktur für Tabelle `data_tag_name`
-- 

CREATE TABLE `data_tag_name` (
  `tn_id` int(11) NOT NULL auto_increment,
  `tn_name` varchar(100) collate utf8_unicode_ci NOT NULL,
  PRIMARY KEY  (`tn_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

INSERT INTO `data_tag_name` VALUES (1, 'status');
INSERT INTO `data_tag_name` VALUES (2, 'extension');
INSERT INTO `data_tag_name` VALUES (4, 'star');
INSERT INTO `data_tag_name` VALUES (5, 'kind');
INSERT INTO `data_tag_name` VALUES (6, 'number');


-- --------------------------------------------------------

-- 
-- Tabellenstruktur für Tabelle `data_tag_name_a`
-- 

CREATE TABLE `data_tag_name_a` (
  `tn_id` int(11) NOT NULL auto_increment,
  `tn_name` varchar(100) collate utf8_unicode_ci NOT NULL,
  PRIMARY KEY  (`tn_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- --------------------------------------------------------

-- 
-- Tabellenstruktur für Tabelle `data_tag_value`
-- 

CREATE TABLE `data_tag_value` (
  `tv_id` int(11) NOT NULL auto_increment,
  `tv_value` varchar(255) collate utf8_unicode_ci NOT NULL,
  PRIMARY KEY  (`tv_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

INSERT INTO `data_tag_value` VALUES (7, 'php');
INSERT INTO `data_tag_value` VALUES (2, 'html');
INSERT INTO `data_tag_value` VALUES (4, 'off');
INSERT INTO `data_tag_value` VALUES (5, 'stable');
INSERT INTO `data_tag_value` VALUES (9, '1');
INSERT INTO `data_tag_value` VALUES (10, '2');


-- --------------------------------------------------------

-- 
-- Tabellenstruktur für Tabelle `data_tag_value_a`
-- 

CREATE TABLE `data_tag_value_a` (
  `tv_id` int(11) NOT NULL auto_increment,
  `tv_value` varchar(255) collate utf8_unicode_ci NOT NULL,
  PRIMARY KEY  (`tv_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- --------------------------------------------------------

-- 
-- Tabellenstruktur für Tabelle `data_text`
-- 

CREATE TABLE `data_text` (
  `text_id` int(11) NOT NULL auto_increment,
  `data_rev_id` int(11) NOT NULL,
  `text_name` varchar(100) collate utf8_unicode_ci NOT NULL,
  `text_content` text collate utf8_unicode_ci NOT NULL,
  PRIMARY KEY  (`text_id`),
  KEY `data_rev_id` (`data_rev_id`),
  FULLTEXT KEY `text_content` (`text_content`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

INSERT INTO `data_text` VALUES (9, 1, 'content', '[#templ_default]');
INSERT INTO `data_text` VALUES (8, 2, 'content', '[#cont_head][#cont_menu_top][#cont_menu_side][#cont_contmenu_[#%NAME%]][#cont_menu_search][#cont_menu_language][#cont_menu_misc][#cont_%NAME%][#cont_bottom]');
INSERT INTO `data_text` VALUES (13, 6, 'content', '<h1>Welcome to RosCMS v3</h1><p>RosCMS test page</p>[#inc_news]');
INSERT INTO `data_text` VALUES (15, 7, 'content', 'news #1 ...');
INSERT INTO `data_text` VALUES (17, 8, 'content', 'news #2');
INSERT INTO `data_text` VALUES (12, 5, 'content', '<h3>News</h3> <?php $tmp_year = "2085"; $query_content = mysql_query("SELECT d.data_name, r.rev_usrid, s1.stext_content AS ''title'', s2.stext_content AS ''description'', v.tv_value as ''numb'', r.rev_date  FROM data_ d, data_revision r, data_stext s1, data_stext s2, data_tag a, data_tag_name n, data_tag_value v  WHERE d.data_name = ''news_page''  AND d.data_type = ''content''  AND r.data_id = d.data_id  AND (r.rev_language = ''".mysql_real_escape_string($roscms_template_var_lang)."'' OR r.rev_language = ''en'')   AND r.rev_version > 0  AND s1.data_rev_id = r.rev_id  AND s1.stext_name = ''title''  AND s2.data_rev_id = r.rev_id  AND s2.stext_name = ''description''  AND r.data_id = a.data_id  AND r.rev_id = a.data_rev_id  AND a.tag_usrid = ''-1''  AND a.tag_name_id = n.tn_id  AND n.tn_name = ''number''  AND a.tag_value_id = v.tv_id  ORDER BY r.rev_datetime DESC  LIMIT 3;"); while($result_content = mysql_fetch_array($query_content)) { 
if ($tmp_year > substr($result_content[''rev_date''], 0, 4)) { $tmp_year = substr($result_content[''rev_date''], 0, 4); echo "<h3>".$tmp_year."</h3>"; } $query_usraccount= mysql_query("SELECT user_name, user_fullname   FROM users   WHERE user_id = ''".mysql_real_escape_string($result_content["rev_usrid"])."''   LIMIT 1;"); $result_usraccount=mysql_fetch_array($query_usraccount); ?>  <p align="left"><b><font color="#666666"><?php echo $result_content[''rev_date'']; ?>,  <?php  if ($result_usraccount[''user_fullname'']) { echo $result_usraccount[''user_fullname''];  }  else { echo $result_usraccount[''user_name''];  }  ?>  </font></b><br />  <b><a href="<?php echo "[#link_".$result_content["data_name"]."_".$result_content["numb"]."]"; ?>"><?php echo $result_content["title"]; ?></a></b><br />  <?php echo $result_content["description"]; ?></p> <?php } // end while ?> ');


-- --------------------------------------------------------

-- 
-- Tabellenstruktur für Tabelle `data_text_a`
-- 

CREATE TABLE `data_text_a` (
  `text_id` int(11) NOT NULL auto_increment,
  `data_rev_id` int(11) NOT NULL,
  `text_name` varchar(100) collate utf8_unicode_ci NOT NULL,
  `text_content` text collate utf8_unicode_ci NOT NULL,
  PRIMARY KEY  (`text_id`),
  KEY `data_rev_id` (`data_rev_id`),
  FULLTEXT KEY `text_content` (`text_content`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- --------------------------------------------------------

-- 
-- Tabellenstruktur für Tabelle `data_user_filter`
-- 

CREATE TABLE `data_user_filter` (
  `filt_id` int(11) NOT NULL auto_increment,
  `filt_usrid` int(11) NOT NULL,
  `filt_title` varchar(50) collate utf8_unicode_ci NOT NULL,
  `filt_type` smallint(6) NOT NULL default '1',
  `filt_string` varchar(255) collate utf8_unicode_ci NOT NULL,
  `filt_datetime` datetime NOT NULL,
  `filt_usage` int(11) NOT NULL,
  `filt_usagedate` datetime NOT NULL,
  PRIMARY KEY  (`filt_id`),
  KEY `filt_usrid` (`filt_usrid`),
  KEY `filt_name` (`filt_title`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- --------------------------------------------------------

-- 
-- Tabellenstruktur für Tabelle `languages`
-- 

CREATE TABLE `languages` (
  `lang_id` varchar(10) collate utf8_unicode_ci NOT NULL default '',
  `lang_name` varchar(100) collate utf8_unicode_ci NOT NULL default '',
  `lang_name_org` varchar(100) collate utf8_unicode_ci NOT NULL,
  `lang_level` int(11) NOT NULL default '0',
  PRIMARY KEY  (`lang_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='available languages';

INSERT INTO `languages` VALUES ('en', 'English', 'English', 10);
INSERT INTO `languages` VALUES ('de', 'German', 'Deutsch', 8);


-- --------------------------------------------------------

-- 
-- Tabellenstruktur für Tabelle `subsys_mappings`
-- 

CREATE TABLE `subsys_mappings` (
  `map_roscms_userid` bigint(20) NOT NULL default '0',
  `map_subsys_name` varchar(10) character set utf8 NOT NULL default '',
  `map_subsys_userid` int(7) NOT NULL default '0',
  PRIMARY KEY  (`map_roscms_userid`,`map_subsys_name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- --------------------------------------------------------

-- 
-- Tabellenstruktur für Tabelle `user_sessions`
-- 

CREATE TABLE `user_sessions` (
  `usersession_id` varchar(32) character set utf8 NOT NULL default '' COMMENT 'Unique ID of this session',
  `usersession_user_id` bigint(20) NOT NULL default '0' COMMENT 'User this session belongs to',
  `usersession_expires` datetime default NULL COMMENT 'Expiry date/time (NULL if does not expire)',
  `usersession_browseragent` varchar(255) character set utf8 NOT NULL default '' COMMENT 'HTTP_USER_AGENT when this session was created',
  `usersession_ipaddress` varchar(15) character set utf8 NOT NULL default '' COMMENT 'IP address from which this session was created',
  `usersession_created` datetime NOT NULL default '0000-00-00 00:00:00' COMMENT 'session created (date/time) - all session will get deleted ... by date',
  PRIMARY KEY  (`usersession_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci PACK_KEYS=0 COMMENT='Sessions';

-- --------------------------------------------------------

-- 
-- Tabellenstruktur für Tabelle `usergroup_members`
-- 

CREATE TABLE `usergroup_members` (
  `usergroupmember_userid` bigint(20) NOT NULL default '0',
  `usergroupmember_usergroupid` varchar(10) collate utf8_unicode_ci NOT NULL default 'user'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- --------------------------------------------------------

-- 
-- Tabellenstruktur für Tabelle `usergroups`
-- 

CREATE TABLE `usergroups` (
  `usrgroup_name_id` varchar(10) collate utf8_unicode_ci NOT NULL default '',
  `usrgroup_name` varchar(100) collate utf8_unicode_ci NOT NULL default '',
  `usrgroup_intern_id` varchar(25) collate utf8_unicode_ci NOT NULL default '',
  `usrgroup_seclev` tinyint(4) NOT NULL default '0',
  `usrgroup_securitylevel` tinyint(4) NOT NULL default '0',
  `usrgroup_description` varchar(255) collate utf8_unicode_ci NOT NULL default '',
  PRIMARY KEY  (`usrgroup_name_id`),
  UNIQUE KEY `usrgroup_name` (`usrgroup_name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

INSERT INTO `usergroups` VALUES ('ros_admin', 'Administrator', 'roscms_usrgrp_admin', 3, 70, 'Administrator-Group');
INSERT INTO `usergroups` VALUES ('user', 'User', 'roscms_usrgrp_user', 0, 0, 'Normal Visitors');
INSERT INTO `usergroups` VALUES ('developer', 'Developer', 'roscms_usrgrp_dev', 2, 50, 'Developer');
INSERT INTO `usergroups` VALUES ('moderator', 'Moderator', 'roscms_usrgrp_team', 2, 20, 'Website Moderators');
INSERT INTO `usergroups` VALUES ('translator', 'Translator', 'roscms_usrgrp_trans', 1, 10, 'Translators');
INSERT INTO `usergroups` VALUES ('ros_sadmin', 'Super-Administrator', 'roscms_usrgrp_sadmin', 3, 100, 'Super-Administrator-Group');
INSERT INTO `usergroups` VALUES ('transmaint', 'Tranlation Maintainer', 'roscms_usrgrp_trans', 2, 15, 'Translation Maintainer');


-- --------------------------------------------------------

-- 
-- Tabellenstruktur für Tabelle `users`
-- 

CREATE TABLE `users` (
  `user_id` bigint(20) NOT NULL auto_increment,
  `user_name` varchar(20) collate utf8_unicode_ci NOT NULL default '',
  `user_roscms_password` varchar(32) collate utf8_unicode_ci NOT NULL default '',
  `user_roscms_getpwd_id` varchar(50) collate utf8_unicode_ci NOT NULL default '',
  `user_timestamp_touch` varchar(50) collate utf8_unicode_ci NOT NULL default '',
  `user_timestamp_touch2` timestamp NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  `user_login_counter` bigint(20) NOT NULL default '0',
  `user_account_enabled` varchar(10) collate utf8_unicode_ci NOT NULL default 'no',
  `user_account_hidden` varchar(5) collate utf8_unicode_ci NOT NULL default 'no',
  `user_register` timestamp NULL default NULL,
  `user_fullname` varchar(100) collate utf8_unicode_ci NOT NULL default '',
  `user_email` varchar(150) collate utf8_unicode_ci NOT NULL default '',
  `user_website` varchar(150) collate utf8_unicode_ci NOT NULL default '',
  `user_language` varchar(2) collate utf8_unicode_ci NOT NULL default '',
  `user_country` varchar(2) collate utf8_unicode_ci NOT NULL default '',
  `user_timezone` varchar(3) collate utf8_unicode_ci NOT NULL default '',
  `user_occupation` varchar(50) collate utf8_unicode_ci NOT NULL default '',
  `user_description` varchar(255) collate utf8_unicode_ci NOT NULL default '',
  `user_setting_multisession` varchar(5) collate utf8_unicode_ci NOT NULL default 'false',
  `user_setting_browseragent` varchar(5) collate utf8_unicode_ci NOT NULL default 'true',
  `user_setting_ipaddress` varchar(5) collate utf8_unicode_ci NOT NULL default 'true',
  `user_setting_timeout` varchar(5) collate utf8_unicode_ci NOT NULL default 'true',
  PRIMARY KEY  (`user_id`),
  UNIQUE KEY `user_name` (`user_name`),
  UNIQUE KEY `user_email` (`user_email`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='RosCMS User Table';

