elfpe: argparse.cpp argparse.h header.cpp header.h \
	objectfile.cpp objectfile.h util.cpp util.h pedef.h \
	elfpe.cpp reloc.cpp coff.cpp exports.cpp imports.cpp
	g++ -g -o $@ $^ -lelf

clean:
	rm -rf *.o elfpe
