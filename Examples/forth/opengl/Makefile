TOP        = ../..
SWIG       = $(TOP)/../preinst-swig
TARGETS    = opengl.fs sdl.fs glext.fs
DEMO       = demo.fs
OPTIONS    = -forth -no-sectioncomments -stackcomments -I/usr/include -I/usr/lib/gcc/i486-linux-gnu/4.1/include/ -I/usr/include/SDL

all: $(TARGETS)

# execute compiled file
%.fs: %.fsx
	./$< -gforth > $@

# compile fsi-file
%.fsx: %-fsi.c
	$(CC) -o $@ $<

# use swig to create the fsi file
%-fsi.c: 
	$(SWIG) $(OPTIONS) -o $@ $(patsubst %-fsi.c, %.i, $@)

run: $(DEMO)
	gforth $(DEMO)

.PHONY: clean

clean:
	rm -f $(TARGETS)
	rm -f $(patsubst %.fs, %-fsi.c, $(TARGETS))
	rm -f $(patsubst %.fs, %.fsx, $(TARGETS))

