

CXXFLAGS = -g -I/usr/local/include
LIBS = -lssl -lcrypto

objects := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

all:server_demo client_demo

server_demo:server.o
	$(CXX) -o $@ $^ $(LIBS)

client_demo:client.o
	$(CXX) -o $@ $^ $(LIBS)

%.o:%.cpp
	$(CXX) $(CXXFLAGS)  -c -o $@ $^

clean:
	rm $(objects)
	rm server_demo client_demo