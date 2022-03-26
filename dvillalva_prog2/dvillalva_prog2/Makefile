# Complete this rule
dvillalva_prog2: dvillalva_prime.o dvillalva_prog2.c
	gcc -pthread dvillalva_prime.o dvillalva_prog2.c -o dvillalva_prog2

# Replace with correct file names
dvillalva_prime.o: dvillalva_prime.c dvillalva_prime.h
	gcc -c dvillalva_prime.c

clean:
	rm *.o; rm dvillalva_prog2
	rm *.txt
	rm *.dat
