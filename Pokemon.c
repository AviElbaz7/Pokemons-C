#include "Pokemon.h"
#include "Defs.h"
#include <stdlib.h>


void init_pokemon_type(pokemon_type *type, char *name){
	if(type == NULL){
		return;
	}
	if(name!=NULL){
		type->typeName = malloc(strlen(name) + 1); // Allocate memory for the name
		strcpy(type->typeName, name);
	}
	else{
		return;
	}

	type->numberOfPokes = 0;
	type->size_against_me = 0;
	type->size_against_others = 0;
	type->effective_against_me = NULL;
	type->effective_against_others = NULL;
}

void init_pokemon_bio(pokemon_bio *bio, float height, float weight, int attack) {
    bio->pok_height = height;
    bio->pok_weight = weight;
    bio->pok_attack = attack;
}

// Function to initialize a pokemon
void pokemon_init_pokemon(pokemon *poke, char *name, char *species, pokemon_type *type, pokemon_bio *biology) {
    if (poke == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    poke->pok_name = strdup(name);
    poke->species = strdup(species);
    poke->type = type;
    poke->bio = *biology;

    if (poke->pok_name == NULL || poke->species == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }
}

pokemon_type *allocate_pokemon_types(int n_types) {
    pokemon_type *all_types = (pokemon_type*)malloc(n_types * sizeof(pokemon_type));
    if (all_types == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }
    return all_types;
}

pokemon *allocate_pokemons(int n_pokemons) {
    pokemon *all_pokemons = (pokemon*)malloc(n_pokemons * sizeof(pokemon));
    if (all_pokemons == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }
    return all_pokemons;
}


void addTypeToEffec_against_me(pokemon_type *type, pokemon_type *type_insert){
	type->size_against_me++;
	type->effective_against_me = realloc(type->effective_against_me, type->size_against_me * sizeof(pokemon_type *));
	type->effective_against_me[type->size_against_me - 1] = type_insert;

}

void addTypeToEffec_against_others(pokemon_type *type, pokemon_type *type_insert){
	type->size_against_others++;
	type->effective_against_others = realloc(type->effective_against_others, type->size_against_others * sizeof(pokemon_type *));
	type->effective_against_others[type->size_against_others - 1] = type_insert;

}

void removeTypeToEffec_against_me(pokemon_type *type, pokemon_type *type_insert){
	for(int i=0; i < type->size_against_me; i++){
		if(strcmp(type->effective_against_me[i]->typeName, type_insert->typeName) == 0){
			// maybe need to free elements
			for(int j=i; j<type->size_against_me-1; j++){
				type->effective_against_me[j] = type->effective_against_me[j + 1];
			}
			type->size_against_me--;
			break;
		}
	}
	type->effective_against_me = realloc(type->effective_against_me, type->size_against_me * sizeof(pokemon_type *));;

}

void removeTypeToEffec_against_others(pokemon_type *type, pokemon_type *type_insert){
	for(int i=0; i < type->size_against_others; i++){
		if(strcmp(type->effective_against_others[i]->typeName, type_insert->typeName) == 0){

			for(int j=i; j<type->size_against_others-1; j++){
				type->effective_against_others[j] = type->effective_against_others[j + 1];
			}
		type->size_against_others--;
		break;
		}

	}
	type->effective_against_others = realloc(type->effective_against_others, type->size_against_others * sizeof(pokemon_type *));

}


status print_pokemon(pokemon *poke){
	if(poke->pok_name == NULL){
		return failure;
	}
	printf("%s :\n", poke->pok_name);
	printf("%s, %s Type.\n", poke->species, poke->type->typeName);
	printf("Height: %.2f m    Weight: %.2f kg    Attack: %d\n\n", poke->bio.pok_height, poke->bio.pok_weight, poke->bio.pok_attack);
	return success;
}

status print_poke_type(pokemon_type *type){
	if(type->typeName == NULL){
		return failure;
	}
	printf("Type %s -- %d pokemons\n", type->typeName, type->numberOfPokes);
	if(type->size_against_me > 0){
		printf("\tThese types are super-effective against %s:", type->typeName);
		for(int i=0; i<type->size_against_me; i++){
			if(i == type->size_against_me - 1){
				printf("%s\n", type->effective_against_me[i]->typeName);
			}
			else{
				printf("%s ,", type->effective_against_me[i]->typeName);
			}
		}
	}

	if(type->size_against_others > 0){
		printf("\t%s moves are super-effective against:", type->typeName);
		for(int i=0; i<type->size_against_others; i++){
			if(i == type->size_against_others - 1){
				printf("%s\n", type->effective_against_others[i]->typeName);
			}
			else{
				printf("%s ,", type->effective_against_others[i]->typeName);
			}
		}
	}
	printf("\n");
	return success;
}


void free_pokemon_type(pokemon_type *type) {
    if (type == NULL)
        return;
    if(type->typeName!=NULL){
        free(type->typeName);
        type->typeName = NULL;
    }

    if(type->effective_against_me != NULL){
        free(type->effective_against_me);
        type->effective_against_me = NULL;
    }

    if(type->effective_against_others!=NULL){
        free(type->effective_against_others);
        type->effective_against_others = NULL;
    }

}

void free_pokemon(pokemon *poke) {
    if (poke == NULL)
        return;

    if(poke->pok_name!=NULL){
        free(poke->pok_name);
        poke->pok_name = NULL;
    }

    if(poke->species!=NULL){
        free(poke->species);
        poke->species = NULL;
    }


}




