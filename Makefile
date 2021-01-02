# Makefile

CPP      = g++

LIBS     = 
INCS     = 
BINSER	 = server
BINCLI	 = client
CFLAGS   = -lstdc++
RM       = rm -f

.PHONY: all all-before all-after clean clean-custom

all: clean all-before $(BINSER) $(BINCLI) all-after

clean: clean-custom
	${RM} servermain.o $(BINSER) client.o $(BINCLI)

$(BINSER):
	$(CPP) -o server calcLib.c servermain.cpp $(LIBS) $(CFLAGS)

calcLib.o: calcLib.cpp
	$(CPP) -c calcLib.c -o calcLib.o $(CFLAGS)
	
servermain.o: servermain.cpp
	$(CPP) -c servermain.cpp -o servermain.o $(CFLAGS)

$(BINCLI):
	$(CPP) -o $(BINCLI) clientmain.cpp $(LIBS) $(CFLAGS)

client.o: clientmain.cpp
	$(CPP) -c clientmain.cpp -o client.o $(CFLAGS)
