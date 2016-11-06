PROGRAM_NAME =signals
CC = clang
CXX = clang++
RM = rm -f

CXXFLAGS = --std=c++11 -g -O3 -pthread -MMD -MP -Wall -pedantic
CPPFLAGS = -I/usr/local/include
LDFLAGS = -L/usr/local/lib
LDLIBS =

INSTALL_DIR = /usr/local/bin

HEADERS = $(wildcard *.hpp)
SRCS = $(wildcard *.cpp)
OBJS = $(subst .cpp,.o,$(SRCS))
DEPS = $(subst .cpp,.d,$(SRCS))

all: $(PROGRAM_NAME)

$(PROGRAM_NAME):$(OBJS)
		$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

-include $(DEPS)
	
clean:
		$(RM) $(OBJS) $(PROGRAM_NAME) $(DEPS)

install:
		cp  $(PROGRAM_NAME) $(INSTALL_DIR)

uninstall:
		$(RM) $(INSTALL_DIR)/$(PROGRAM_NAME)

dist-clean: clean
		$(RM) *~  $(DEPS)
