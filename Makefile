tutorial.exe: tutorial.cpp io.cpp
	g++ -Wall -Werror -pedantic -g --std=c++17 tutorial.cpp -o tutorial.exe

clean :
	rm -rvf *.exe *~ *.out *.dSYM *.stackdump

move15.exe: move15.cpp
	g++ -Wall -Werror -pedantic -g --std=c++17 move15.cpp -o move15.exe

blackjack.exe: blackjack.cpp
	g++ -Wall -Werror -pedantic -g --std=c++17 blackjack.cpp -o blackjack.exe