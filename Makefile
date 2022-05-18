

Parent:Parent.c
	gcc   -o Parent Parent.c -lm -L Soldier.c

Soldier:Soldier.c 
	gcc -o Soldier Soldier.c 

clean:
	rm -f *.o Parent Soldier



 