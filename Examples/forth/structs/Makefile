TOP        = ../..
SWIG       = $(TOP)/../preinst-swig
OUTPUT     = structs
INTERFACE  = $(OUTPUT).i

.PHONY: $(OUTPUT), clean

$(OUTPUT):
	$(SWIG) -forth -o $(OUTPUT).c $(INTERFACE)
	$(CC) $(OUTPUT).c -o $(OUTPUT)
	./$(OUTPUT) -gforth -gforth

run: $(OUTPUT)
	cat $(OUTPUT)

clean:
	rm -f $(OUTPUT)
	rm -f $(OUTPUT).c
	rm -f $(OUTPUT).fs

