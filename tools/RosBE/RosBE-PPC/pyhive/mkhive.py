import os
import os.path
import sys
import hbin
import reginf

regpaths = {
    "default":     "\Registry\User\.DEFAULT",
    "sam":         "\Registry\Machine\SAM",
    "security":    "\Registry\Machine\SECURITY",
    "software":    "\Registry\Machine\SOFTWARE",
    "system":      "\Registry\Machine\SYSTEM"
    }

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print sys.argv[0],'[inf-dir]','[hive-dir]'
        sys.exit(1)

    infdir = sys.argv[1]
    hivedir = sys.argv[2]

    ri = reginf.reginf()
        
    for file in ["hivesys.inf","hivecls.inf","hivesft.inf","hivedef.inf"]:
        lines = open(os.path.join(infdir,file)).read().split('\n')
        curline = 0
        while curline < len(lines):
            regline,curline = reginf.nextline(lines,curline)
            if regline != '':
                ri.addentry(regline)

    for key in regpaths:
        outf = open(os.path.join(hivedir,key),"wb")

        # Make the absolute root
        rookey = hbin.regkey('','',0)
        roothive = hbin.regf(rookey)

        # Make a hive where we want it
        thehive = hbin.regf(roothive.addkey(regpaths[key]))

        # Import values into the hive (some may not be children of the target
        # hive, but that's fine
        for vline in ri.content:
            if vline.iskey():
                key = roothive.addkey_with_type(vline.keyname(),vline.flags())
            elif not vline.empty():
                key = roothive.addkey(vline.keyname())
                key.addvalue(hbin.regval(vline.valuename(),vline.flags(),vline.bindata()))

        # Now write our hive
        thehive.export(outf)
        outf.close()

    
