TESTS = 00_downcast 00_multiple-inheritance 01_test 02_test \
        collect_test hash_test

BGEN      = ../../bin/bgen
CXX       = libtool --mode=link g++
INCLUDES  = -I/usr/src/googletest/googletest/include -I../../
OBJS      = ../../libjj.la
LIBS      = -lgtest -lgtest_main -pthread
CXXFLAGS  = -g -o a.out -Wall $(INCLUDES)

.cpp.o:
	$(CXX) -g -c -Wall $INCLUDES $<

.PHONY: test

all:

check: $(TESTS)

clean:
	rm -f *.b *.o a.out core tmp*

install:
	# do nothing

00_downcast:
	$(CXX) $(CXXFLAGS) $@.cpp $(OBJS) $(LIBS) && ./a.out
00_multiple-inheritance:
	$(CXX) $(CXXFLAGS) $@.cpp $(OBJS) $(LIBS) && ./a.out
01_test:
	$(BGEN) $@.cpp >$@.b
	$(CXX) $(CXXFLAGS) $@.cpp $(OBJS) $(LIBS) && ./a.out
02_test:
	$(BGEN) $@.cpp >$@.b
	$(CXX) $(CXXFLAGS) $@.cpp $(OBJS) $(LIBS) && ./a.out
collect_test:
	$(BGEN) $@.cpp >$@.b
	$(CXX) $(CXXFLAGS) $@.cpp $(OBJS) $(LIBS) && ./a.out
hash_test:
	$(BGEN) $@.cpp >$@.b
	$(CXX) $(CXXFLAGS) $@.cpp $(OBJS) $(LIBS) && ./a.out $(GTEST_OPT)
