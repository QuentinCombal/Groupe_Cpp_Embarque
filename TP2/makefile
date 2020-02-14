CC = g++                          # le compilateur à utiliser
CFLAGS =                          # les options du compilateur
LDFLAGS =                         # les options pour l'éditeur de liens
SRC = main_interpolation.cpp      # les fichiers sources
PROG = interpol_lineaire          # nom de l'exécutable
OBJS = $(SRC:.cpp=.o)             # les .o qui en découlent
.SUFFIXES: .cpp .o                # lien entre les suffixes

all: $(PROG)

# étapes de compilation et d'édition de liens
# $@ la cible $^ toutes les dépendances
$(PROG): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

main_interpolation.o: linearInt.hpp
# le lien entre .o et .c

# $< dernière dépendance
%.o: %.cpp
	$(CC) $(CFLAGS) -c $<
