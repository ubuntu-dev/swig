TOP        = ../..
SWIG       = $(TOP)/../preinst-swig
OUTPUT     = example
INTERFACE  = example.i
OPTIONS		= -forth -no-sectioncomments -stackcomments

$(OUTPUT).fs: $(OUTPUT).fsx
	./$(OUTPUT).fsx -gforth > $(OUTPUT).fs

$(OUTPUT).fsx: $(OUTPUT).fsi
	$(CC) -o $(OUTPUT).fsx $(OUTPUT)-fsi.c

$(OUTPUT).fsi:
	$(SWIG) $(OPTIONS) -o $(OUTPUT)-fsi.c $(INTERFACE)

run: $(OUTPUT).gforth.fs $(OUTPUT).swiftforth.fs $(OUTPUT).vfx.fs $(OUTPUT).fsi.fs
	cat $(OUTPUT).gforth.fs $(OUTPUT).swiftforth.fs $(OUTPUT).vfx.fs $(OUTPUT).fsi.fs

run-gforth: $(OUTPUT).gforth.fs
	cat $(OUTPUT).gforth.fs

.PHONY: clean

clean:
	rm -f $(OUTPUT).fsi
	rm -f $(OUTPUT).fs
	rm -f $(OUTPUT).fso
	rm -f $(OUTPUT).fsx
