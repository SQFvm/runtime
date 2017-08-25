COMPILER = gcc -std=c99
ARGS =  -g -rdynamic
CARGS = -c
LIBS = -lm -ldl

prog: basetype.c Entry.c SQF.c SQF_inst.c SQF_parse.c SQF_types.c string_map.c string_op.c textrange.c
	$(COMPILER) $(ARGS) $(CARGS) basetype.c
	$(COMPILER) $(ARGS) $(CARGS) Entry.c
	$(COMPILER) $(ARGS) $(CARGS) SQF.c
	$(COMPILER) $(ARGS) $(CARGS) SQF_inst.c
	$(COMPILER) $(ARGS) $(CARGS) SQF_parse.c
	$(COMPILER) $(ARGS) $(CARGS) SQF_types.c
	$(COMPILER) $(ARGS) $(CARGS) string_map.c
	$(COMPILER) $(ARGS) $(CARGS) string_op.c
	$(COMPILER) $(ARGS) $(CARGS) textrange.c
	$(COMPILER) $(ARGS) -o prog basetype.o Entry.o SQF.o SQF_inst.o SQF_parse.o SQF_types.o string_map.o string_op.o textrange.o $(LIBS)

clean:
	rm prog
	rm -f *.so
	rm -f *.o
	rm -f *.bak *~

remake: clean prog

lib:
	$(eval ARGS = -Wall -fpic)
	$(COMPILER) $(ARGS) $(CARGS) basetype.c
	$(COMPILER) $(ARGS) $(CARGS) Entry.c
	$(COMPILER) $(ARGS) $(CARGS) SQF.c
	$(COMPILER) $(ARGS) $(CARGS) SQF_inst.c
	$(COMPILER) $(ARGS) $(CARGS) SQF_parse.c
	$(COMPILER) $(ARGS) $(CARGS) SQF_types.c
	$(COMPILER) $(ARGS) $(CARGS) string_map.c
	$(COMPILER) $(ARGS) $(CARGS) string_op.c
	$(COMPILER) $(ARGS) $(CARGS) textrange.c
	$(COMPILER) $(ARGS) -shared -o libsqfvm.so basetype.o Entry.o SQF.o SQF_inst.o SQF_parse.o SQF_types.o string_map.o string_op.o textrange.o $(LIBS)
