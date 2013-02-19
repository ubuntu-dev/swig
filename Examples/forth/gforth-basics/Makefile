TOP        = ../..
SWIG       = $(TOP)/../preinst-swig
OUTPUT     = example
INTERFACE  = example.i

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

