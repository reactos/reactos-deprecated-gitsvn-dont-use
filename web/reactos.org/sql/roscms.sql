-- phpMyAdmin SQL Dump
-- version 2.9.2
-- http://www.phpmyadmin.net
-- 
-- Host: localhost
-- Erstellungszeit: 27. Mai 2007 um 13:02
-- Server Version: 5.0.33
-- PHP-Version: 5.2.1
-- 
-- Datenbank: `roscms`
-- 

-- --------------------------------------------------------

-- 
-- Tabellenstruktur für Tabelle `content`
-- 

CREATE TABLE `content` (
  `content_id` bigint(20) NOT NULL auto_increment,
  `content_name` varchar(100) collate utf8_unicode_ci NOT NULL default '',
  `content_lang` varchar(10) collate utf8_unicode_ci NOT NULL default 'all',
  `content_text` text collate utf8_unicode_ci NOT NULL,
  `content_version` int(11) NOT NULL default '0',
  `content_active` tinyint(4) NOT NULL default '0',
  `content_visible` tinyint(4) NOT NULL default '0',
  `content_type` varchar(10) collate utf8_unicode_ci NOT NULL default 'default',
  `content_description` varchar(255) collate utf8_unicode_ci NOT NULL default '',
  `content_editor` varchar(10) collate utf8_unicode_ci NOT NULL default '',
  `content_usrname_id` bigint(20) NOT NULL default '0',
  `content_date` date NOT NULL default '0000-00-00',
  `content_time` time NOT NULL default '00:00:00',
  `content_altered` varchar(1) collate utf8_unicode_ci NOT NULL default '0',
  PRIMARY KEY  (`content_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Contents';

-- --------------------------------------------------------

-- 
-- Tabellenstruktur für Tabelle `data_`
-- 

CREATE TABLE `data_` (
  `data_id` int(11) NOT NULL auto_increment,
  `data_name` varchar(100) collate utf8_unicode_ci NOT NULL COMMENT 'together with page_type unique entries',
  `data_type` varchar(10) collate utf8_unicode_ci NOT NULL COMMENT '"page", "layout", "content", "script"',
  PRIMARY KEY  (`data_id`),
  KEY `data_name` (`data_name`,`data_type`),
  KEY `data_type` (`data_type`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='all RosCMS v2 "pages", "content", "dyncontent" and "inctext"';

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
  KEY `rev_version` (`rev_version`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

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
  KEY `data_id` (`data_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

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
  FULLTEXT KEY `stext_content` (`stext_content`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

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

-- --------------------------------------------------------

-- 
-- Tabellenstruktur für Tabelle `data_tag_name`
-- 

CREATE TABLE `data_tag_name` (
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
  FULLTEXT KEY `text_content` (`text_content`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

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
-- Tabellenstruktur für Tabelle `dyn_content`
-- 

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
  `dyn_altered` varchar(1) collate utf8_unicode_ci NOT NULL default '0',
  PRIMARY KEY  (`dyn_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Contents';

-- --------------------------------------------------------

-- 
-- Tabellenstruktur für Tabelle `include_text`
-- 

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
  `inc_altered` varchar(1) collate utf8_unicode_ci NOT NULL default '0',
  PRIMARY KEY  (`inc_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Replace words with text phrases';

-- --------------------------------------------------------

-- 
-- Tabellenstruktur für Tabelle `languages`
-- 

CREATE TABLE `languages` (
  `lang_id` varchar(10) collate utf8_unicode_ci NOT NULL default '',
  `lang_name` varchar(100) collate utf8_unicode_ci NOT NULL default '',
  `lang_level` int(11) NOT NULL default '0'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='available languages';

-- --------------------------------------------------------

-- 
-- Tabellenstruktur für Tabelle `pages`
-- 

CREATE TABLE `pages` (
  `page_id` bigint(20) NOT NULL auto_increment,
  `page_name` varchar(50) collate utf8_unicode_ci NOT NULL default '',
  `page_language` varchar(50) collate utf8_unicode_ci NOT NULL default '',
  `pages_extra` varchar(20) collate utf8_unicode_ci NOT NULL default '',
  `pages_extention` varchar(10) collate utf8_unicode_ci NOT NULL default 'default',
  `page_text` text collate utf8_unicode_ci NOT NULL,
  `page_version` int(11) NOT NULL default '0',
  `page_active` tinyint(4) NOT NULL default '0',
  `page_visible` tinyint(4) NOT NULL default '0',
  `page_usrname_id` bigint(20) NOT NULL default '0',
  `page_date` date NOT NULL default '0000-00-00',
  `page_time` time NOT NULL default '00:00:00',
  `page_generate_usrid` bigint(20) NOT NULL default '0',
  `page_generate_timestamp` varchar(50) collate utf8_unicode_ci NOT NULL default '',
  `page_generate_force` char(1) collate utf8_unicode_ci NOT NULL default '0',
  `page_title` varchar(150) collate utf8_unicode_ci NOT NULL default '',
  `page_description` varchar(255) collate utf8_unicode_ci NOT NULL default '',
  PRIMARY KEY  (`page_id`),
  KEY `page_generate_force` (`page_generate_force`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Pages';

-- --------------------------------------------------------

-- 
-- Tabellenstruktur für Tabelle `roscms_security_log`
-- 

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
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='RosCMS Security Log';

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
  `usrgroup_securitylevel` tinyint(4) NOT NULL default '0',
  `usrgroup_description` varchar(255) collate utf8_unicode_ci NOT NULL default '',
  PRIMARY KEY  (`usrgroup_name_id`),
  UNIQUE KEY `usrgroup_name` (`usrgroup_name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

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
