CXX = clang++
RM=rm -f

CPPFLAGS = -g -Wall -std=c++11

SRCS=vm.cpp main.cpp value.cpp builder.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: vm

vm: $(OBJS)
	$(CXX) $(CPPFLAGS) -o vm $(OBJS)

variant_test: variant_test.o
	$(CXX) $(CPPFLAGS) -o variant_test variant_test.o

variant.h: mpl.h

value.h: variant.h

value.o: value.cpp value.h

vm.o: vm.cpp vm.h op_codes.h

main.o: main.cpp op_codes.h vm.h builder.h

clean:
	$(RM) $(OBJS) variant_test.o

dist-clean: clean
	$(RM) vm
