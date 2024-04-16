#ifndef POKEMON_H_
#define POKEMON_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Defs.h"

typedef struct pokemon_type pokemon_type;

struct pokemon_type {
	char *typeName;
	int numberOfPokes;
	pokemon_type **effective_against_others;
	int size_against_me;
	int size_against_others;
	pokemon_type **effective_against_me;
};

typedef struct{
	float pok_height;
	float pok_weight;
	int pok_attack;
}pokemon_bio;

typedef struct{
	char *pok_name;
	char *species;
	pokemon_type *type;
	pokemon_bio bio;
}pokemon;

void init_pokemon_type(pokemon_type *type, char *name);
void pokemon_init_pokemon(pokemon *poke, char *name, char *species, pokemon_type *type, pokemon_bio *bio);
void init_pokemon_bio(pokemon_bio *bio, float height, float weight, int attack);
void addTypeToEffec_against_me(pokemon_type *type, pokemon_type *type_insert);
void addTypeToEffec_against_others(pokemon_type *type, pokemon_type *type_insert);
void removeTypeToEffec_against_me(pokemon_type *type, pokemon_type *type_insert);
void removeTypeToEffec_against_others(pokemon_type *type, pokemon_type *type_insert);
void free_pokemon_type(pokemon_type *type);
void free_pokemon_bio(pokemon_bio *bio);
void free_pokemon(pokemon *poke);
pokemon *allocate_pokemons(int n_pokemons);
pokemon_type *allocate_pokemon_types(int n_types);

status print_pokemon(pokemon *poke);
status print_poke_type(pokemon_type *type);



#endif /* POKEMON_H_ */
