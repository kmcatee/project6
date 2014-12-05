CC = g++
LD = g++
CFLAGS = -c -g -std=c++11 -pedantic-errors -Wall
LFLAGS = -g

OBJS = p6_main.o Model.o View.o 
OBJS += Grid_View.o Map_View.o Local_View.o List_View.o Health_View.o Amount_View.o ETA_View.o
OBJS += Controller.o 
OBJS += Sim_object.o Structure.o Moving_object.o Agent.o
OBJS += Farm.o Town_Hall.o
OBJS += Peasant.o Warrior.o Soldier.o Archer.o Guard.o
OBJS += Agent_factory.o Structure_factory.o
OBJS += Geometry.o Utility.o
PROG = proj6exe

default: $(PROG)

$(PROG): $(OBJS)
	$(LD) $(LFLAGS) $(OBJS) -o $(PROG)

p6_main.o: p6_main.cpp Model.h Controller.h
	$(CC) $(CFLAGS) p6_main.cpp

Model.o: Model.cpp Model.h View.h Sim_object.h Structure.h Agent.h Agent_factory.h Structure_factory.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Model.cpp

View.o: View.cpp View.h Geometry.h Utility.h
	$(CC) $(CFLAGS) View.cpp

Grid_View.o: Grid_View.cpp Grid_View.h View.h
	$(CC) $(CFLAGS) Grid_View.cpp

Map_View.o: Map_View.cpp Map_View.h Grid_View.h View.h
	$(CC) $(CFLAGS) Map_View.cpp

Local_View.o: Local_View.cpp Local_View.h Grid_View.h View.h
	$(CC) $(CFLAGS) Local_View.cpp

List_View.o: List_View.cpp List_View.h View.h
	$(CC) $(CFLAGS) List_View.cpp

Health_View.o: Health_View.cpp Health_View.h List_View.h View.h 
	$(CC) $(CFLAGS) Health_View.cpp

ETA_View.o: ETA_View.cpp ETA_View.h List_View.h View.h 
	$(CC) $(CFLAGS) ETA_View.cpp

Amount_View.o: Amount_View.cpp Amount_View.h List_View.h View.h
	$(CC) $(CFLAGS) Amount_View.cpp

Controller.o: Controller.cpp Controller.h Model.h Map_View.h Local_View.h Health_View.h Amount_View.h ETA_View.h Sim_object.h Structure.h Agent.h Agent_factory.h Structure_factory.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Controller.cpp

Sim_object.o: Sim_object.cpp Sim_object.h Geometry.h
	$(CC) $(CFLAGS) Sim_object.cpp

Structure.o: Structure.cpp Structure.h Model.h Sim_object.h Geometry.h
	$(CC) $(CFLAGS) Structure.cpp

Farm.o: Farm.cpp Farm.h Structure.h Sim_object.h Geometry.h
	$(CC) $(CFLAGS) Farm.cpp

Town_Hall.o: Town_Hall.cpp Town_Hall.h Structure.h Sim_object.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Town_Hall.cpp

Agent.o: Agent.cpp Agent.h Model.h Moving_object.h Sim_object.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Agent.cpp

Peasant.o: Peasant.cpp Peasant.h Agent.h Moving_object.h Sim_object.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Peasant.cpp

Warrior.o: Warrior.cpp Warrior.h Agent.h Sim_object.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Warrior.cpp

Soldier.o: Soldier.cpp Soldier.h Warrior.h Agent.h Sim_object.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Soldier.cpp

Archer.o: Archer.cpp Archer.h Warrior.h Agent.h Sim_object.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Archer.cpp

Guard.o: Guard.cpp Guard.h Warrior.h Agent.h Sim_object.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Guard.cpp

Moving_object.o: Moving_object.cpp Moving_object.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Moving_object.cpp

Agent_factory.o: Agent_factory.cpp Agent_factory.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Agent_factory.cpp

Structure_factory.o: Structure_factory.cpp Structure_factory.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Structure_factory.cpp

Geometry.o: Geometry.cpp Geometry.h
	$(CC) $(CFLAGS) Geometry.cpp

Utility.o: Utility.cpp Utility.h
	$(CC) $(CFLAGS) Utility.cpp


samples:
	./$(PROG) < fight_in.txt > my_fight_out.txt
	diff -q my_fight_out.txt fight_out.txt
	./$(PROG) < states_in.txt > my_states_out.txt
	diff -q my_states_out.txt states_out.txt
	./$(PROG) < views_in.txt > my_views_out.txt
	diff -q my_views_out.txt views_out.txt
	./$(PROG) < workviolence_in.txt > my_workviolence_out.txt
	diff -q my_workviolence_out.txt workviolence_out.txt
	./$(PROG) < new_features_in.txt > my_new_features_out.txt
	diff -q my_new_features_out.txt new_features_out.txt


clean:
	rm -f *.o
real_clean:
	rm -f $(PROG)
	rm -f *.o
