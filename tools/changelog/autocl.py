#!/usr/bin/python

# Creates basic changelog wiki page from svn log
# - argument is the generated xml log file with log entries
#   created by 'svn log -v --xml -r revision1:revision2 repository'
# - needs internet access for retrieving the author names from wiki
#   (http://www.reactos.org/wiki/index.php?title=Developer_Roles&action=edit)
# - needs a section file 'sections'
#   this file contains all generated wiki-sections and the according paths
#   to assign the log messages correctly
#
# (c) Matthias Kupfer 2009

import xml.sax # for the parser
import sys # for system functions
import string # for string operations
import re # for regular expressions
import time # for sorting
import urllib # for downloading the developer wiki page

class XMLLogParser:
	class XMLLogHandler(xml.sax.handler.ContentHandler):
		__logentries = [] # log entries list
		__logentry = {}
		def startElement(self, name, attrs):
			"Process all start tags."
			self.__data = ''
			attr = {} # temporary dictionary of attributes
			for i in attrs.items(): # iterate over all attributes
				tmp = i[1] # save attribute value
				if tmp: # if value exists
					tmp = tmp.encode('utf-8') # encode as utf-8
				attr[i[0].encode('utf-8')] = tmp # encode attribute name too
			if name == 'logentry':
			# save the log entry revision number (currently unused)
				self.__logentry["revision"] = attr.get("revision")
			elif name == 'paths':
				# collect files and paths
				self.__logentry["paths"] = [] # reset path
			elif name == 'path':
				# save file or path attributes
				self.__pathattr = (attr.get("action"),
					attr.get("copyfrom-path"),
					attrs.get("copyfrom-rev")) # copy path attributes
		def endElement(self, name):
			"Process all end tags."
			if name == 'logentry':
				# append the created log entry and clear the temporary one
				self.__logentries.append(self.__logentry)
				self.__logentry = {} # clear current log entry
			elif name == 'author':
				# save the author name in the current log entry
				self.__logentry["author"] = self.__data # copy author 
			elif name == 'date':
				# extract timestamp and save it in the current log entry
				m = re.match(r"(\d{4})-(\d{2})-(\d{2})T(\d{2}):(\d{2}):" \
				"(\d{2})\.(\d{6})Z", self.__data) # regular expression for time
				self.__logentry["time"] = time.mktime((int(m.group(1)),
					int(m.group(2)), int(m.group(3)), int(m.group(4)),
					int(m.group(5)), int(m.group(6)), -1, -1, -1)) + \
					(float("0."+m.group(7))) # build time entry
			elif name == 'path':
				# append a file or path entry to current log entry
				self.__logentry["paths"].append((self.__data, self.__pathattr))
			elif name == 'msg':
				# save message in current log entry
				self.__logentry["msg"] = self.__data
		def characters(self, ch):
			"Save character data."
			self.__data += ch.encode('utf-8')
		def get_entry_list(self):
			"Sort created list by revision in ascending order."
			self.__logentries.sort(lambda x,y : cmp(y["revision"], x["revision"]))
			return self.__logentries
	__Handler = XMLLogHandler() # instance of content handler
	get_entry_list = __Handler.get_entry_list
	def __init__(self, file = None):
		"Create a XML parser, set content handler and load the file."
		self.parser = xml.sax.make_parser() # create a xml parser
		self.parser.setContentHandler(self.__Handler) # set content handler
		if file:
			self.load(file)
	def load(self, file):
		"Parse a file."
		self.parser.parse(file) # parse file

def get_common_part(paths):
	"returns the longest common path part of all file entries"
	index = 0 # length
	mypaths = [] # list of all paths
	for i in paths: # extract all path from list
		mypaths.append(i[0][0:string.rfind(i[0],'/')])
	if len(mypaths) == 1: # if only one path
		return mypaths[0] # the return the whole path
	else:
		cmp = mypaths[0] # set first path for comparison
		while index < len(cmp): 
			isequal = True
			for i in mypaths[1:]: # for all remaining paths
				if index >= len(i) or i[index] != cmp[index]:
					isequal = False
					break
			if isequal == True: # if equal then next character
				index += 1
			else: # else finish
				break;
		return cmp[0:index]

def find_section(i,sections):
	"find the best fitting (longest path) section"
	section = "" # no section found
	path = "" # no path
	for k in sections.keys(): # iterate over all section paths
		if i.startswith(k) and len(k) > len(path): # if i starts with path and
			#is longer then the previous path then update path and section
			section = sections[k]
			path = k
	return section # return wiki section name

if __name__ == '__main__':
	if len(sys.argv) != 2:
		print "This script generates a template for the wiki changelog"
		print
		print "Syntax: %s <svn-log-xml-file>" % sys.argv[0]
		print "log file should be created with:"
		print "svn log -v --xml [-r revision1:revision2] repository > svn-log-xml-file"
		exit(1)
	sec_order = [] # list for the sections (to preserve) order
	authors = {} # dictionary for commit name -> real names
	sections = {} # dictionary for path -> sections
	messages = {} # dictionary for section -> log messages
	author = urllib.urlopen("http://www.reactos.org/wiki/index.php?title=Developer_Roles&action=edit").readlines()
	for i in author:
		m = re.match("^.*\[\[(.*)\]\].*\|\|[^|]*\|\|\s([^ ]*)\s\|\|.*$",i)
		if m:
			authors[m.group(2)] = m.group(1) # fill author dictionary
	section = open('sections').readlines() # read sections
	for i in section:
		m = re.match("^(/.*?) (.*)$",i)
		if m:
			sections[m.group(1)] = m.group(2) # fill section dictionary
			sec_order.append(m.group(2)) # and section list
	log = XMLLogParser(sys.argv[1]) # read log entries
	for i in log.get_entry_list():
		lns = []
		lines = string.split(i['msg'],'\n');
		for k in lines:
			m = re.match(r"^\s*[-*]*\s*(.*)\s*$",k)
			if m:
				l = string.strip(m.group(1))
				if len(l) > 0:
					lns.append(l)
		msg = string.join(lns)
		author = authors[i['author']] 
		if not i.has_key('paths'):
			print "Error: no path information found"
			print "Did you really use the option '-v' in 'svn log'?"
			print "Can't create template without path information"
			exit(1)
		path = get_common_part(i['paths'])
		this_section = find_section(path,sections)
		if messages.has_key(this_section):
			messages[this_section].append((msg,author))
		else:
			messages[this_section] = [(msg,author)]
	for i in sec_order:
		print i
		if messages.has_key(i):
			for k in messages[i]:
				print '*',k[0],'([['+k[1]+']])'

# vim: set ts=4 sw=4:
