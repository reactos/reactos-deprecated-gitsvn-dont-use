import hbin
import struct

FLG_ADDREG_BINVALUETYPE = 1
FLG_ADDREG_NOCLOBBER = 2
FLG_ADDREG_DELVAL = 4
FLG_ADDREG_APPEND = 8
FLG_ADDREG_KEYONLY = 16
FLG_ADDREG_OVERWRITEONLY = 32
FLG_ADDREG_TYPE_SZ = 0
FLG_ADDREG_TYPE_MULTI_SZ  = 0x00010000
FLG_ADDREG_TYPE_EXPAND_SZ = 0x00020000
FLG_ADDREG_TYPE_BINARY    = FLG_ADDREG_BINVALUETYPE
FLG_ADDREG_TYPE_DWORD     = 0x00010000 | FLG_ADDREG_BINVALUETYPE
FLG_ADDREG_TYPE_NONE      = 0x00020000 | FLG_ADDREG_BINVALUETYPE
FLG_ADDREG_TYPE_MASK      = 0xffff0000 | FLG_ADDREG_BINVALUETYPE

UNICODE = 'utf-16-be'

regkeys = {
        "HKCR" : "\\Registry\\Machine\\SOFTWARE\\Classes\\",
        "HKCU" : "\\Registry\\User\\.DEFAULT\\",
        "HKLM" : "\\Registry\\Machine\\",
        "HKU"  : "\\Register\\User\\"
        }

def stripcomment(text):
        n = text.find(';')
        if n != -1:
                text = text[:n].strip()
        return text

def nextline(linelist,curline):
        rs = ""
        stop = False

        while curline < len(linelist) and \
                  (len(stripcomment(linelist[curline]).strip()) == 0 or \
                   linelist[curline] == '['):
                curline = curline + 1

        while not stop and curline < len(linelist):
                theline = linelist[curline]
                if len(theline) and theline[-1] == '\\':
                        theline = theline[:-1]
                else:
                        stop = True
                rs = rs + theline
                curline = curline + 1
                rs = stripcomment(rs).strip()

        return rs,curline

def hexpand(st):
        if len(st) > 2 and st[:2] == '0x':
                return st
        else:
                return '0x'+st

class regelt:
        def __init__(self,text):
                result = []
                curword = ""
                quoted = 0
                for ch in text:
                        if ch == ',':
                                if quoted == 2:
                                        curword = curword + '"'
                                curword = curword.strip()
                                if len(curword) > 0 and curword[0] == '"':
                                        curword = curword[1:-1]
                                result.append(curword.strip())
                                quoted = 0
                                curword = ""
                        elif ch == '"':
                                if quoted == 0:
                                        quoted = 1
                                        curword = curword + ch
                                elif quoted == 1:
                                        quoted = 2
                                elif quoted == 2:
                                        quoted = 1
                                        curword = curword + '"'
                        else:
                                if quoted == 2:
                                        quoted = 0
                                        curword = curword + '"'
                                curword = curword + ch

                if quoted == 2:
                        curword = curword + '"'
                        curword = curword.strip()
                if len(curword) > 0 and curword[0] == '"':
                        curword = curword[1:-1]

                result.append(curword.strip())

                self.parsed = result

        def empty(self):
                return len(self.parsed) < 3 or len(self.parsed[0]) == 0

        def keyname(self):
                return regkeys[self.parsed[0]] + self.parsed[1]

        def valuename(self):
                return self.parsed[2]

        def iskey(self):
                return self.flags() & FLG_ADDREG_KEYONLY

        def type(self):
                return self.flags() & FLG_ADDREG_TYPE_MASK

        def flags(self):
                if len(self.parsed[3]) == 0:
                        return FLG_ADDREG_KEYONLY | FLG_ADDREG_NOCLOBBER
                else:
                        return eval(hexpand(self.parsed[3]))

        # Accumulate binary data for this value
        def bindata(self):
                f = self.type()
                result = ''
                unul = chr(0) + chr(0)
                if f == FLG_ADDREG_TYPE_SZ or \
                   f == FLG_ADDREG_TYPE_EXPAND_SZ:
                        result = unicode(self.parsed[4]).encode(UNICODE) + unul
                elif f == FLG_ADDREG_TYPE_MULTI_SZ:
                        for i in xrange(4,len(self.parsed)):
                                result += unicode(self.parsed[i]).encode(UNICODE) + unul
                        result += unul
                elif f == FLG_ADDREG_BINVALUETYPE:
                        for i in xrange(4,len(self.parsed)):
                                result += chr(eval(hexpand(self.parsed[i])))
                elif f == FLG_ADDREG_TYPE_DWORD:
                        result = struct.pack(hbin.ULONG, eval(hexpand(self.parsed[4])))
                else:
                        result = self.parsed[4:]
                return result

class reginf:
        def __init__(self):
                self.content = []

        def addentry(self,data):
                re = regelt(data)
                if not re.empty():
                        self.content.append(re)
