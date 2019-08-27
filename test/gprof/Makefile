CFLAGS += -g -pg

all:test

test: test.o
	$(CXX) $(CFLAGS) -o $@ $^

%.o:%.cpp
	$(CXX) $(CFLAGS) -o $@ -c $<


.PHONY:

clean:
	rm test
	find -name "*.o" | xargs rm