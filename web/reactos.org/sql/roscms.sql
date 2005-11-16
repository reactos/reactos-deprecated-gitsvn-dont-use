--
-- RosCMS - ReactOS Content Management System
-- Copyright (C) 2005  Ge van Geldorp <gvg@reactos.org>
-- 
-- This program is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation; either version 2 of the License, or
-- (at your option) any later version.
-- 
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
-- 
-- You should have received a copy of the GNU General Public License
-- along with this program; if not, write to the Free Software
-- Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
-- 
-- 
-- Database: `roscms`
-- 

-- --------------------------------------------------------

-- 
-- `content`
-- 

DROP TABLE IF EXISTS `content`;
CREATE TABLE `content` (
  `content_id` bigint(20) NOT NULL auto_increment,
  `content_name` varchar(100) collate utf8_unicode_ci NOT NULL default '',
  `content_lang` varchar(10) collate utf8_unicode_ci NOT NULL default 'all',
  `content_text` text collate utf8_unicode_ci NOT NULL,
  `content_version` int(11) NOT NULL default '0',
  `content_active` tinyint(4) NOT NULL default '0',
  `content_visible` tinyint(4) NOT NULL default '0',
  `content_description` varchar(255) collate utf8_unicode_ci NOT NULL default '',
  `content_editor` varchar(10) collate utf8_unicode_ci NOT NULL default '',
  `content_usrname_id` bigint(20) NOT NULL default '0',
  `content_date` date NOT NULL default '0000-00-00',
  `content_time` time NOT NULL default '00:00:00',
  PRIMARY KEY  (`content_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Contents' AUTO_INCREMENT=0;

-- --------------------------------------------------------

-- 
-- `dyn_content`
-- 

DROP TABLE IF EXISTS `dyn_content`;
CREATE TABLE `dyn_content` (
  `dyn_id` bigint(20) NOT NULL auto_increment,
  `dyn_content_id` bigint(20) NOT NULL default '0',
  `dyn_content_nr` int(11) NOT NULL default '0',
  `dyn_content_name` varchar(100) collate utf8_unicode_ci NOT NULL default '',
  `dyn_content_lang` varchar(10) collate utf8_unicode_ci NOT NULL default 'all',
  `dyn_content_text1` varchar(255) collate utf8_unicode_ci NOT NULL default '',
  `dyn_content_text2` varchar(255) collate utf8_unicode_ci NOT NULL default '',
  `dyn_content_text3` varchar(255) collate utf8_unicode_ci NOT NULL default '',
  `dyn_content_text4` longtext collate utf8_unicode_ci NOT NULL,
  `dyn_content_version` int(11) NOT NULL default '0',
  `dyn_content_active` tinyint(4) NOT NULL default '0',
  `dyn_content_visible` tinyint(4) NOT NULL default '0',
  `dyn_content_editor` varchar(10) collate utf8_unicode_ci NOT NULL default '',
  `dyn_content_usrname_id` bigint(20) NOT NULL default '0',
  `dyn_content_date` date NOT NULL default '0000-00-00',
  `dyn_content_time` time NOT NULL default '00:00:00',
  PRIMARY KEY  (`dyn_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Contents' AUTO_INCREMENT=0 ;

-- --------------------------------------------------------

-- 
-- `include_text`
-- 

DROP TABLE IF EXISTS `include_text`;
CREATE TABLE `include_text` (
  `inc_id` bigint(20) NOT NULL auto_increment,
  `inc_level` tinyint(4) NOT NULL default '0',
  `inc_word` varchar(255) collate utf8_unicode_ci NOT NULL default '',
  `inc_text` text collate utf8_unicode_ci NOT NULL,
  `inc_lang` varchar(10) collate utf8_unicode_ci NOT NULL default 'all',
  `inc_extra` varchar(20) collate utf8_unicode_ci NOT NULL default '',
  `inc_vis` tinyint(4) NOT NULL default '0',
  `inc_seclevel` tinyint(4) NOT NULL default '50',
  `inc_usrname_id` bigint(20) NOT NULL default '0',
  `inc_date` date NOT NULL default '0000-00-00',
  `inc_time` time NOT NULL default '00:00:00',
  PRIMARY KEY  (`inc_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Replace words with text phrases' AUTO_INCREMENT=0 ;

-- --------------------------------------------------------

-- 
-- `languages`
-- 

DROP TABLE IF EXISTS `languages`;
CREATE TABLE `languages` (
  `lang_id` varchar(10) collate utf8_unicode_ci NOT NULL default '',
  `lang_name` varchar(100) collate utf8_unicode_ci NOT NULL default '',
  `lang_level` int(11) NOT NULL default '0'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='available languages';

-- --------------------------------------------------------

-- 
-- `pages`
-- 

DROP TABLE IF EXISTS `pages`;
CREATE TABLE `pages` (
  `page_id` bigint(20) NOT NULL auto_increment,
  `page_name` varchar(50) collate utf8_unicode_ci NOT NULL default '',
  `page_language` varchar(50) collate utf8_unicode_ci NOT NULL default '',
  `pages_extra` varchar(20) collate utf8_unicode_ci NOT NULL default '',
  `page_text` text collate utf8_unicode_ci NOT NULL,
  `page_version` int(11) NOT NULL default '0',
  `page_active` tinyint(4) NOT NULL default '0',
  `page_visible` tinyint(4) NOT NULL default '0',
  `page_usrname_id` bigint(20) NOT NULL default '0',
  `page_date` date NOT NULL default '0000-00-00',
  `page_time` time NOT NULL default '00:00:00',
  `page_generate_usrid` bigint(20) NOT NULL default '0',
  `page_generate_timestamp` varchar(50) collate utf8_unicode_ci NOT NULL default '',
  `page_title` varchar(150) collate utf8_unicode_ci NOT NULL default '',
  `page_description` varchar(255) collate utf8_unicode_ci NOT NULL default '',
  PRIMARY KEY  (`page_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Pages' AUTO_INCREMENT=0 ;

-- --------------------------------------------------------

-- 
-- `roscms_security_log`
-- 

DROP TABLE IF EXISTS `roscms_security_log`;
CREATE TABLE `roscms_security_log` (
  `roscms_sec_log_id` bigint(20) NOT NULL auto_increment,
  `roscms_sec_log_section` varchar(100) collate utf8_unicode_ci NOT NULL default '',
  `roscms_sec_log_priority` int(11) NOT NULL default '100',
  `roscms_sec_log_reason` varchar(255) collate utf8_unicode_ci NOT NULL default '',
  `roscms_sec_log_user` varchar(100) collate utf8_unicode_ci NOT NULL default 'roscms_system',
  `roscms_sec_log_useraccount` varchar(100) collate utf8_unicode_ci NOT NULL default '',
  `roscms_sec_log_userip` varchar(255) collate utf8_unicode_ci NOT NULL default '',
  `roscms_sec_log_referrer` varchar(255) collate utf8_unicode_ci NOT NULL default '',
  `roscms_sec_log_text` text collate utf8_unicode_ci NOT NULL,
  `roscms_sec_log_date` date NOT NULL default '0000-00-00',
  `roscms_sec_log_time` time NOT NULL default '00:00:00',
  `roscms_sec_log_visible` int(11) NOT NULL default '1',
  PRIMARY KEY  (`roscms_sec_log_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='RosCMS Security Log' AUTO_INCREMENT=0 ;

-- --------------------------------------------------------

-- 
-- `subsys_mappings`
-- 

DROP TABLE IF EXISTS `subsys_mappings`;
CREATE TABLE `subsys_mappings` (
  `map_roscms_userid` bigint(20) NOT NULL default '0',
  `map_subsys_name` varchar(10) character set utf8 NOT NULL default '',
  `map_subsys_userid` int(7) NOT NULL default '0',
  PRIMARY KEY  (`map_roscms_userid`,`map_subsys_name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- --------------------------------------------------------

-- 
-- `user_sessions`
-- 

DROP TABLE IF EXISTS `user_sessions`;
CREATE TABLE `user_sessions` (
  `usersession_id` varchar(32) character set utf8 NOT NULL default '' COMMENT 'Unique ID of this session',
  `usersession_user_id` bigint(20) NOT NULL default '0' COMMENT 'User this session belongs to',
  `usersession_expires` datetime default NULL COMMENT 'Expiry date/time (NULL if does not expire)',
  `usersession_browseragent` varchar(255) character set utf8 NOT NULL default '' COMMENT 'HTTP_USER_AGENT when this session was created',
  `usersession_ipaddress` varchar(15) character set utf8 NOT NULL default '' COMMENT 'IP address from which this session was created',
  PRIMARY KEY  (`usersession_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci PACK_KEYS=0 COMMENT='Sessions';

-- --------------------------------------------------------

-- 
-- `usergroup_members`
-- 

DROP TABLE IF EXISTS `usergroup_members`;
CREATE TABLE `usergroup_members` (
  `usergroupmember_userid` bigint(20) NOT NULL default '0',
  `usergroupmember_usergroupid` varchar(10) collate utf8_unicode_ci NOT NULL default 'user'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- --------------------------------------------------------

-- 
-- `usergroups`
-- 

DROP TABLE IF EXISTS `usergroups`;
CREATE TABLE `usergroups` (
  `usrgroup_name_id` varchar(10) collate utf8_unicode_ci NOT NULL default '',
  `usrgroup_name` varchar(100) collate utf8_unicode_ci NOT NULL default '',
  `usrgroup_intern_id` varchar(25) collate utf8_unicode_ci NOT NULL default '',
  `usrgroup_securitylevel` tinyint(4) NOT NULL default '0',
  `usrgroup_description` varchar(255) collate utf8_unicode_ci NOT NULL default '',
  PRIMARY KEY  (`usrgroup_name_id`),
  UNIQUE KEY `usrgroup_name` (`usrgroup_name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- --------------------------------------------------------

-- 
-- `users`
-- 

DROP TABLE IF EXISTS `users`;
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
  `user_setting_multisession` varchar(5) collate utf8_unicode_ci NOT NULL default 'true',
  `user_setting_browseragent` varchar(5) collate utf8_unicode_ci NOT NULL default 'true',
  `user_setting_ipaddress` varchar(5) collate utf8_unicode_ci NOT NULL default 'true',
  `user_setting_timeout` varchar(5) collate utf8_unicode_ci NOT NULL default 'true',
  PRIMARY KEY  (`user_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='RosCMS User Table' AUTO_INCREMENT=0 ;


