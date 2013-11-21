CXX = g++-4.8
RM=rm -f

CPPFLAGS = -g -Wall -std=c++11

SRCS=vm.cpp main.cpp value.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: vm

vm: $(OBJS)
	$(CXX) $(CPPFLAGS) -o vm $(OBJS)

vm.o: instr.h value.h vm.h $(SRCS)

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) vm
