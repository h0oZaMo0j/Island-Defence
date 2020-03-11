SOURCES=islandDefence.cpp boat.cpp defence.cpp helpers.cpp island.cpp missile.cpp wave.cpp
HEADERS=islandDefence.h boat.h defence.h global.h helpers.h island.h missile.h wave.h
OBJS=islandDefence.o boat.o defence.o helpers.o island.o missile.o wave.o
PROGRAM=island_defence
FLAGS= -std=c++11
LDFLAGS = -lGL -lGLU -lglut  


island_defence: $(OBJS)
	g++ $(FLAGS) $(LDFLAGS) -o $(PROGRAM) $(OBJS)

islandDefence.o: islandDefence.cpp islandDefence.h boat.h island.h missile.h defence.h global.h wave.h
	g++ $(FLAGS) $(LDFLAGS) -c -o $@ $<

boat.o: boat.cpp boat.h helpers.h wave.h global.h missile.h defence.h island.h
	g++ $(FLAGS) $(LDFLAGS) -c -o $@ $<

defence.o: defence.cpp defence.h helpers.h wave.h global.h island.h
	g++ $(FLAGS) $(LDFLAGS) -c -o $@ $<

helpers.o: helpers.cpp helpers.h
	g++ $(FLAGS) $(LDFLAGS) -c -o $@ $<

island.o: island.cpp island.h helpers.h missile.h defence.h
	g++ $(FLAGS) $(LDFLAGS) -c -o $@ $<

missile.o: missile.cpp missile.h helpers.h wave.h global.h boat.h
	g++ $(FLAGS) $(LDFLAGS) -c -o $@ $<

wave.o: wave.cpp wave.h helpers.h global.h
	g++ $(FLAGS) $(LDFLAGS) -c -o $@ $<

clean:
	rm $(PROGRAM) $(OBJS)

archive:
	zip $(USER)-a1 $(SOURCES) $(HEADERS) Makefile readme.txt
