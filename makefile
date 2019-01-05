# generic makefile June 2016
CC = g++
CC_FLAGS = -Wall -g -std=c++11 `pkg-config --cflags opencv`
LN_FLAGS = `pkg-config --libs opencv`

# file names
EXEC = main
SOURCES = $(wildcard *.cc)
OBJECTS = $(SOURCES:.cc=.o)

# main target
all: $(OBJECTS)
	$(CC) $(OBJECTS) $(CC_FLAGS) $(LN_FLAGS) -o $(EXEC)

-include $(OBJECTS:.o:.d)

# obtain object files
%.o: %.cc
	$(CC) -c $(CC_FLAGS) $< -o $@
	$(CC) -M $(CC_FLAGS) $*.cc > $*.d

# note: automatic variables
# $< contains the first dependency file name
# $@ contains the target file name

.PHONY: clean
# clean out the directory but keep the executables with command clean
clean: 
	rm -f $(EXEC) *~ *% *# .#* *.o *.d
