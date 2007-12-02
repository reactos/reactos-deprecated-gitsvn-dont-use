import struct

HCELL_NULL = 0xffffffff
HCELL_TYPE_MASK = 0x80000000
HCELL_BLOCK_MASK = 0x7ffff000
HCELL_OFFSET_MASK = 0x00000fff
HCELL_TYPE_SHIFT = 31
HCELL_BLOCK_SHIFT = 12
HCELL_OFFSET_SHIFT = 0

REG_KEY_VOLATILE_CELL = 1
REG_KEY_ROOT_CELL = 12
REG_KEY_LINK_CELL = 16
REG_KEY_NAME_PACKED = 32
REG_VALUE_NAME_PACKED = 1

USHORT = ">H"
ULONG = ">I"
QUAD = ">Q"

def roundup(n,gran):
    return (n + (gran - 1)) & ~(gran - 1)

def pad(block,expsize):
    if len(block) >= expsize:
        return block
    else:
        return block + struct.pack(str(expsize - len(block))+"s","")

def round_pad(data,padto):
    return pad(data,roundup(len(data),padto))

def chop(data,padto):
    if len(data) > padto:
        return data[:padto]
    else:
        return pad(data,padto)

class regval:
    def __init__(self,name,type,value):
        self.name = name
        self.flags = REG_VALUE_NAME_PACKED
        self.type = type
        self.value = value
        self.datacell = HCELL_NULL

    def encode(self):
        result = ""
        for type,value in [
            ('2s',      'kv'),
            (USHORT,    len(self.name)),
            (ULONG,     len(self.value)),
            (ULONG,     self.datacell),
            (ULONG,     self.type),
            (USHORT,    self.flags),
            (USHORT,    0)]:
            result = result + struct.pack(type, value)
        return result + \
                 round_pad(self.name, 4)

    def makecell(self,regf):
        self.datacell = regf.claim_write(self.value)
        return regf.claim_write(self.encode())

class regkey:
    def __init__(self,name,classname,flags):
        self.name = name
        self.classname = classname
        self.flags = flags | REG_KEY_NAME_PACKED
        self.values = []
        self.subkeys = []
        self.parentcell = HCELL_NULL
        self.keylist = HCELL_NULL
        self.valuelist = HCELL_NULL
        self.seccell = HCELL_NULL
        self.classnamecell = HCELL_NULL
        self.maxname = 0
        self.maxclass = 0
        self.maxvaluename = 0
        self.maxvaluedata = 0
        self.maxdatalen = 0

    def addsubkey(self,key):
        self.subkeys.append(key)

    def addvalue(self,val):
        self.values.append(val)
        self.maxvaluename = max(len(val.name),self.maxvaluename)
        self.maxvaluedata = max(len(val.value),self.maxvaluedata)

    def navigate(self,name):
        for key in self.subkeys:
            if key.name.lower() == name.lower():
                return key
        key = regkey(name,'',0)
        self.addsubkey(key)
        return key

    def encode_header(self):
        result = ""
        for type,value in [
            ('2s',     'kn'),
            (USHORT,   self.flags),
            (QUAD,     0),
            (ULONG,    0),
            (ULONG,    self.parentcell),
            (ULONG,    len(self.subkeys)), # Persistent subkey count
            (ULONG,    0),
            (ULONG,    self.keylist),
            (ULONG,    0),
            (ULONG,    len(self.values)),
            (ULONG,    self.valuelist),
            (ULONG,    self.seccell),
            (ULONG,    self.classnamecell),
            (ULONG,    self.maxname),
            (ULONG,    self.maxclass),
            (ULONG,    self.maxvaluename),
            (ULONG,    self.maxvaluedata),
            (ULONG,    0),
            (USHORT,   len(self.name)),
            (USHORT,   len(self.classname))
            ]:
            result = result + struct.pack(type, value)
        return result + \
               round_pad(self.name,4) + \
               round_pad(self.classname,4)

    def size_header(self):
        return len(self.encode_header())

    def make_key_list_cell(self,regf,keycells):
        len_of_cell = 4 + (8 * len(keycells))
        cellid = regf.claim(len_of_cell)
        data = struct.pack(">2sH", 'fl', len(keycells))
        for name,cell in keycells:
            data = data + struct.pack(">I4s", cell, chop(name,4))
        regf.write(cellid, data)
        return cellid

    def make_value_list_cell(self,regf,valuecells):
        len_of_cell = 4 * len(valuecells)
        cellid = regf.claim(len_of_cell)
        data = struct.pack(">"+str(len(valuecells))+"I",*valuecells)
        regf.write(cellid, data)
        return cellid

    def makecell(self,regf):
        valcells = []
        keycells = []

        mycell = regf.claim(self.size_header())

        for val in self.values:
            valcells.append(val.makecell(regf))

        for key in self.subkeys:
            key.parentcell = mycell
            keycells.append((key.name,key.makecell(regf)))

        self.keylist = self.make_key_list_cell(regf, keycells)
        self.valuelist = self.make_value_list_cell(regf, valcells)
        regf.write(mycell,self.encode_header())

        return mycell

class hbin:
    def __init__(self,index):
        self.index = index
        self.used = 32 # Header Size
        self.size = 4096
        self.usable = 4092
        self.block = pad(self.encode_header(),self.size)

    def encode_header(self):
        result = ""
        for type,value in [
            ("4s",     "nibh"),
            (ULONG,    self.index * self.size),
            (ULONG,    self.size),
            (ULONG,    0),
            (ULONG,    0),
            (QUAD,     0),
            (ULONG,    0) ]:
            result = result + struct.pack(type,value)
        return result

    def claim(self,bytes):
        if roundup(self.used + (bytes+4),16) <= self.usable:
            claimed = self.used
            self.used = roundup(self.used + (bytes+4),16)
            return claimed
        else:
            return None

    def write(self,offset,data):
        data = struct.pack(ULONG, ((0xffffffff+1)-roundup(len(data),16)) & 0xffffffff) + \
               data
        self.block = self.block[:offset] + data + self.block[offset+len(data):]

    def export(self,outf):
        self.block = pad(self.block[:self.used] + struct.pack(ULONG,0xffffffff), self.size)
        outf.write(self.block)

class regf:
    def __init__(self,rootkey):
        self.bins = []
        self.alloc_hbin()
        self.blocksize = 4096
        self.rootkey = rootkey
        self.rootkey.flags = self.rootkey.flags | REG_KEY_ROOT_CELL

    def addkey(self,regkey):
        pathparts = regkey.split('\\')
        curkey = self.rootkey
        for i in xrange(1,len(pathparts)):
            pp = pathparts[i]
            if pp == 'CurrentControlSet':
                pp = 'ControlSet001'
            curkey = curkey.navigate(pp)
        return curkey

    def addkey_with_type(self,regkey,flags):
        pathparts = regkey.split('\\')
        curkey = self.rootkey
        for i in xrange(1,len(pathparts)):
            pp = pathparts[i]
            if pp == 'CurrentControlSet':
                pp = 'ControlSet001'
            curkey = curkey.navigate(pp)
        curkey.flags = flags
        return curkey

    def export(self,outf):
        self.keycell = self.rootkey.makecell(self)

        header = ""
        for type,value in [
            ("4s",    "fger"),
            (ULONG,   1), # Seq
            (ULONG,   1),
            (QUAD,    0),
            (ULONG,   1), # Major
            (ULONG,   3), # Minor
            (ULONG,   0), # Type Primary
            (ULONG,   1), # Format
            (ULONG,   self.keycell),# Root Key
            (ULONG,   self.blocksize),# Block size
            (ULONG,   1)]: # Cluster size
            header = header + struct.pack(type,value)
        header = pad(header,0x1fc)

        # Compute checksum
        hwords = struct.unpack(">"+str(len(header)/4)+"I",header)
        checksum = 0
        for i in xrange(0,len(hwords)):
            checksum = checksum ^ hwords[i]
        if checksum == 0xffffffff:
            checksum = 0xfffffffe
        if checksum == 0:
            checksum = 1
        header = header + struct.pack(ULONG,checksum & 0xffffffff)

        # Write
        outf.write(pad(header,self.blocksize))
        for hbin in self.bins:
            hbin.export(outf)

    def alloc_hbin(self):
        self.thebin = hbin(len(self.bins))
        self.bins.append(self.thebin)

    def claim(self,size):
        offset = self.thebin.claim(size)
        if offset is None:
            self.alloc_hbin()
            offset = self.thebin.claim(size)
        return (self.thebin.index << HCELL_BLOCK_SHIFT) | offset

    def write(self,cell,data):
        bindex = (cell & HCELL_BLOCK_MASK) >> HCELL_BLOCK_SHIFT
        boffset = (cell & HCELL_OFFSET_MASK) >> HCELL_OFFSET_SHIFT
        self.bins[bindex].write(boffset,data)

    def claim_write(self,data):
        hcell = self.claim(len(data))
        self.write(hcell,data)
        return hcell
