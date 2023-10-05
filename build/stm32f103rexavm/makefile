CC = gcc
CCFLAGS = -O3
LDFLAGS = -O3
AR = ar
ARFLAGS = cr
RM = rm -rf
OBJ = \
  compile.o\
  dict.o\
  diss.o\
  lookupl.o\
  stack.o\
  vm.o\
  vmio.o\
  vmsys.o\
  vmloop.o\
  vmwords.o
%.o: %.c
	$(CC) $(CCFLAGS) -o $@ -c $<
vm.a:$(OBJ)
	$(AR) $(ARFLAGS) $@ $^
clean:
	$(RM) *.o *.a
