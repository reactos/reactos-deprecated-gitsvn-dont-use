#!/usr/bin/env python

import sys
import struct

def swapnls(nlsbuf):
    result = ""
    nls_file_header = struct.unpack("<8H", nlsbuf[:16])
    result = result + struct.pack(">8H", *nls_file_header) + nlsbuf[16:26]
    header_size = 2 * nls_file_header[0]
    narrow_size = struct.unpack("<H",nlsbuf[header_size:header_size+2])[0]
    start = header_size

    # Ansi table
    for i in xrange(0,narrow_size):
        result = result + struct.pack(">H",*struct.unpack("<H",nlsbuf[start:start+2]))
        start = start + 2

    # Optional OEM table
    nextlen = struct.unpack("<H",nlsbuf[start:start+2])[0]
    result = result + struct.pack(">H",nextlen)
    start = start + 2

    if nextlen != 0:
        for i in xrange(0,256):
            result = result + struct.pack(">H",*struct.unpack("<H",nlsbuf[start:start+2]))
            start = start + 2
    else:
        result = result + struct.pack(">H",nextlen)

    dbcs_ranges = struct.unpack("<H",nlsbuf[start:start+2])[0]
    if (dbcs_ranges & 0x8000) != 0:
        for i in xrange(start/2,len(nlsbuf)/2):
            result = result + struct.pack(">H",*struct.unpack("<H",nlsbuf[start:start+2]))
            start = start + 2

    result = result + nlsbuf[len(result):]

    return result

if __name__ == '__main__':
    nlsfile = open(sys.argv[1],'rb').read()
    open(sys.argv[2],'wb').write(swapnls(nlsfile))

