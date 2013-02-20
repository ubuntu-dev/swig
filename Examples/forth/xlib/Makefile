TOP        = ../..
SWIG       = $(TOP)/../preinst-swig
OUTPUT     = xlib
INTERFACE  = xlib.i
OPTIONS	   = -forth -no-sectioncomments -stackcomments -includeall -I/usr/include -I/usr/include/linux -I/usr/include/x86_64-linux-gnu/

.PHONY: $(OUTPUT), clean

$(OUTPUT):
	$(SWIG) $(OPTIONS) -o $(OUTPUT).c $(INTERFACE)
	$(CC) $(OUTPUT).c -o $(OUTPUT)
	./$(OUTPUT) -gforth -gforth

run: $(OUTPUT)
	cat $(OUTPUT)

clean:
	rm -f $(OUTPUT)
	rm -f $(OUTPUT).c
	rm -f $(OUTPUT).fs

