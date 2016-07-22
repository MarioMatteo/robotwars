#Semplice makefile per compilare programmi Aria composti da un unico file
#residenti in qualunque directory

PROGRAM_NAME=main
VERSION=1.0

#ALL_FILES = *.cpp *.h Makefile TODO CHANGELOG COPYING *.map
ALL_FILES = *.cpp Makefile 

#Eventuale percorso se i sorgenti non sono in questa directory
#ARIA_PREFIX=/home/cassinis/programmi_di_prova/src


ARIA_INCLUDE_PATH=/usr/local/Aria/include
ARIA_LINK_PATH=/usr/local/Aria/lib

CXX=g++
CXXINC=-Iinclude -I$(ARIA_INCLUDE_PATH) -I/usr/include
CXXLINK=-Llib -L$(ARIA_PREFIX)/$(ARIA_LINK_PATH) -lAria -lpthread -ldl -lrt
BARECXXFLAGS= -g -Wall -D_REENTRANT
CXXFLAGS+=$(BARECXXFLAGS) -fno-exceptions

all:	$(PROGRAM_NAME)

$(PROGRAM_NAME): $(PROGRAM_NAME).o 
	$(CXX) $(CXXFLAGS) $(CXXINC) $(PROGRAM_NAME).o -o $@ $(CXXLINK) 

$(PROGRAM_NAME).o: $(PROGRAM_NAME).cpp
	$(CXX) -c $(CXXFLAGS) $(CXXINC) $< -o $@

clean:
	rm -rf *~
	rm -rf *.o
	rm -rf $(PROGRAM_NAME)

install:	all
	cp $(PROGRAM_NAME) /usr/bin/$(PROGRAM_NAME)

dist:	clean
	mkdir $(PROGRAM_NAME)
	cp $(ALL_FILES) $(PROGRAM_NAME)
	tar czvf ../$(PROGRAM_NAME)-V$(VERSION).tgz $(PROGRAM_NAME)
	rm -rf $(PROGRAM_NAME)
# All done!

