#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Defs.h"
#include "Pokemon.h"


void index_of_type(pokemon_type *all_types, char *name, int n_types, int *index){
	for (int i=0; i<n_types; i++) {
		if(strcmp(all_types[i].typeName, name) == 0 ) {
			*index = i;
			return;
		}
	}
}

void readFile(char *path, pokemon_type *all_types, pokemon *all_pokemons, int n_types, int n_pokemons) {
    FILE *file;
    file = fopen(path, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char buffer[300];
    int line = 1;
    int mode = 0;
    int pok_amount = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {

        int len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            buffer[len - 2] = '\0';
        }

        if (line == 2) {
            char *token;
            token = strtok(buffer, ",");
            int count = 0;
            for (int j = 0; j < n_types; j++) {
                init_pokemon_type(&all_types[count], token);
                token = strtok(NULL, ",");
                count++;
            }
        }
        if (strstr(buffer, "Pokemons") != NULL) {
            mode = 1;
            line++;
            continue;
        }
        if (line > 2) {
            if (mode == 0) {
                char *token;
                char *type;
                char *action;

                // Skip the leading spaces and tabs
                char *start = buffer;
                while (*start == ' ' || *start == '\t') {
                    start++;
                }

                // Extract type
                token = strtok(start, " ");
                type = token;
                int indexx;
                index_of_type(all_types, type, n_types, &indexx);

                token = strtok(NULL, ":");
                action = token;
                int me_or_other = 0;
                if (strcmp(action, "effective-against-me") == 0) {
                    me_or_other = 1;
                }

                token = strtok(NULL, ",");
                while (token != NULL) {
                    int to_add_index;
                    index_of_type(all_types, token, n_types, &to_add_index);

                    if (me_or_other == 0) {
                        addTypeToEffec_against_others(&all_types[indexx], &all_types[to_add_index]);
                    } else {
                        addTypeToEffec_against_me(&all_types[indexx], &all_types[to_add_index]);
                    }
                    token = strtok(NULL, ",");
                }

            } else if (mode == 1) {
                pokemon_bio biology;
                pokemon pokyy;
                char *token = strtok(buffer, ",");
                char *name = token;

                token = strtok(NULL, ",");
                char *species = token;

                token = strtok(NULL, ",");
                float height = atof(token);

                token = strtok(NULL, ",");
                float weight = atof(token);

                token = strtok(NULL, ",");
                int attack = atoi(token);

                token = strtok(NULL, ",");
                char *type_name = token;

                int indextu;
                index_of_type(all_types, type_name, n_types, &indextu);
                init_pokemon_bio(&biology, height, weight, attack);
                pokemon_init_pokemon(&pokyy, name, species, &all_types[indextu], &biology);
                all_types[indextu].numberOfPokes++;

                all_pokemons[pok_amount] = pokyy;
                pok_amount++;
            }
        }

        line++;
    }

    fclose(file);
}

int main(int argc, char *argv[]){

	if(argc != 4){
		return 1;
	}

	int n_types = atoi(argv[1]);
	int n_pokemons = atoi(argv[2]);

	char *path = argv[3];

	pokemon_type *all_types = allocate_pokemon_types(n_types);
	pokemon *all_pokemons = allocate_pokemons(n_pokemons);
	readFile(path, all_types, all_pokemons, n_types, n_pokemons);
	int terminated = 0;
	int input;
	while(terminated == 0){
		printf("Please choose one of the following numbers:\n");
		printf("1 : Print all Pokemons\n");
		printf("2 : Print all Pokemons types\n");
		printf("3 : Add type to effective against me list\n");
		printf("4 : Add type to effective against others list\n");
		printf("5 : Remove type from effective against me list\n");
		printf("6 : Remove type from effective against others list\n");
		printf("7 : Print Pokemon by name\n");
		printf("8 : Print Pokemons by type\n");
		printf("9 : Exit\n");
        if (scanf("%d", &input) != 1 || input < 1 || input > 9) {
            while (getchar() != '\n');
            printf("Please choose a valid number.\n");
        }
        else {
            switch(input) {
                case 1:
                    for(int i=0; i<n_pokemons; i++){
                    	print_pokemon(&all_pokemons[i]);
                    }
                    break;

                case 2:
                    for(int i=0; i<n_types; i++){
                    	print_poke_type(&all_types[i]);
                    }
                    break;
                case 3:
                    printf("Please enter type name:\n");
                    char buffer[300];
                    char buffer2[300];
                    if (scanf("%s",buffer) == 1) {
                        int index = -1;
                        int index2 = -1;
                        index_of_type(all_types, buffer, n_types, &index);
                        if(index > -1){
                        	printf("Please enter type name to add to %s effective against me list:\n", all_types[index].typeName);
                        	scanf("%s", buffer2);
                        	index_of_type(all_types, buffer2, n_types, &index2);
                        	if(index2 > -1){
                        		int exists_already=0;
                        		for (int k=0; k<all_types[index2].size_against_me; k++){
                        			if (strcmp(all_types[index2].typeName, all_types[index].effective_against_me[k]->typeName)==0){
                        				exists_already=1;
                        				break;
                        			}
                        		}
                        		if(exists_already == 0){
                            		addTypeToEffec_against_me(&all_types[index], &all_types[index2]);
                            		print_poke_type(&all_types[index]);
                        		}
                        		else{
                        			printf("This type already exist in the list.\n");
                        		}
                        	}
                        	else{
                        		printf("Type name doesn't exist.\n");
                        	}
                        }
                        else{
                        	printf("Type name doesn't exist.\n");
                        }
                    }
                    else {
                        printf("Invalid input. Please enter a string.\n");
                    }
                    break;
                case 4:
                    printf("Please enter type name:\n");
                    char buffer3[300];
                    char buffer4[300];
                    if (scanf("%s",buffer3) == 1) {
                        int index = -1;
                        int index2 = -1;
                        index_of_type(all_types, buffer3, n_types, &index);
                        if(index > -1){
                        	printf("Please enter type name to add to %s effective against others list:\n", all_types[index].typeName);
                        	scanf("%s", buffer4);
                        	index_of_type(all_types, buffer4, n_types, &index2);
                        	if(index2 > -1){
                        		int exists_already = 0;

                        		for (int k = 0; k < all_types[index].size_against_others; k++) {
                        		    if (strcmp(all_types[index2].typeName, all_types[index].effective_against_others[k]->typeName) == 0) {
                        		        exists_already = 1;
                        		        break;
                        		    }
                        		}

                        		if(exists_already == 0){
                            		addTypeToEffec_against_others(&all_types[index], &all_types[index2]);
                            		print_poke_type(&all_types[index]);
                        		}
                        		else{
                        			printf("This type already exist in the list.\n");
                        		}
                        	}
                        	else{
                        		printf("Type name doesn't exist.\n");
                        	}
                        }
                        else{
                        	printf("Type name doesn't exist.\n");
                        }
                    }
                    else {
                        printf("Invalid input. Please enter a string.\n");
                    }
                    break;
                case 5:
                    printf("Please enter type name:\n");
                    char buffer5[300];
                    char buffer6[300];
                    if (scanf("%s",buffer5) == 1) {
                        int index = -1;
                        int index2 = -1;
                        index_of_type(all_types, buffer5, n_types, &index);
                        if(index > -1){
                        	printf("Please enter type name to remove from %s effective against me list:\n", all_types[index].typeName);
                        	scanf("%s", buffer6);
                        	index_of_type(all_types, buffer6, n_types, &index2);
                        	if(index2 > -1){
                        		int exists_already = 0;

                        		for (int k = 0; k < all_types[index].size_against_me; k++) {
                        		    if (strcmp(all_types[index2].typeName, all_types[index].effective_against_me[k]->typeName) == 0) {
                        		        exists_already = 1;
                        		        break;
                        		    }
                        		}

                        		if(exists_already == 0){
                        			printf("Type name doesn't exist in the list.\n");
                        		}
                        		else{

                        			removeTypeToEffec_against_me(&all_types[index], &all_types[index2]);
                        			print_poke_type(&all_types[index]);
                        		}
                        	}
                        	else{
                        		printf("Type name doesn't exist in the list.\n");
                        	}
                        }
                        else{
                        	printf("Type name doesn't exist.\n");
                        }
                    }
                    else {
                        printf("Invalid input. Please enter a string.\n");
                    }
                    break;
                case 6:
                    printf("Please enter type name:\n");
                    char buffer7[300];
                    char buffer8[300];
                    if (scanf("%s",buffer7) == 1) {
                        int index = -1;
                        int index2 = -1;
                        index_of_type(all_types, buffer7, n_types, &index);
                        if(index > -1){
                        	printf("Please enter type name to remove from %s effective against others list:\n", all_types[index].typeName);
                        	scanf("%s", buffer8);
                        	index_of_type(all_types, buffer8, n_types, &index2);
                        	if(index2 > -1){
                        		int exists_already = 0;

                        		for (int k = 0; k < all_types[index].size_against_others; k++) {
                        		    if (strcmp(all_types[index2].typeName, all_types[index].effective_against_others[k]->typeName) == 0) {
                        		        exists_already = 1;
                        		        break;
                        		    }
                        		}

                        		if(exists_already == 0){
                        			printf("Type name doesn't exist in the list.\n");
                        		}
                        		else{

                        			removeTypeToEffec_against_others(&all_types[index], &all_types[index2]);
                        			print_poke_type(&all_types[index]);
                        		}
                        	}
                        	else{
                        		printf("Type name doesn't exist in the list.\n");
                        	}
                        }
                        else{
                        	printf("Type name doesn't exist.\n");
                        }
                    }
                    else {
                        printf("Invalid input. Please enter a string.\n");
                    }
                    // Handle option 6
                    break;
                case 7:
                	printf("Please enter Pokemon name:\n");
                	char buffer9[300];
                	scanf("%s", buffer9);
                	int jj = 0;
                	for(int i=0; i<n_pokemons; i++){
                		if(strcmp(all_pokemons[i].pok_name, buffer9)==0){
                			print_pokemon(&all_pokemons[i]);
                			jj = 1;
                			break;
                		}
                	}
                	if(jj==0){
                		printf("The Pokemon doesn't exist.\n");
                	}
                    break;
                case 8:
                   	printf("Please enter type name:\n");
					char buffer10[300];
					scanf("%s", buffer10);
					int kk = 0;
					int foundPokemon = 0;
					for(int i=0; i<n_types; i++){
						if(strcmp(all_types[i].typeName, buffer10)==0){
							for(int j=0; j<n_pokemons; j++){
								if(strcmp(all_pokemons[j].type->typeName, buffer10)==0){
				                    if(foundPokemon==0) {
				                        printf("There are %d Pokemons with this type:\n", all_types[i].numberOfPokes);
				                        foundPokemon = 1; // Set the flag to true once a Pokemon is found
				                    }
									print_pokemon(&all_pokemons[j]);
								}
							}
							if(all_types[i].numberOfPokes == 0){
								printf("There are no Pokemons with this type.\n");
							}

							kk = 1;
							break;
						}
					}
					if(kk==0){
						printf("Type name doesn't exist.\n");
					}
                    break;
                case 9:
                    terminated = 1;
                    break;
                default:
                    printf("Invalid option. Please choose a valid option.\n");
                    break;
            }
        }
	}

    for(int i = 0; i < n_types; i++) {
        if(&all_types[i] != NULL) {
            free_pokemon_type(&all_types[i]);
        }
    }

    if(all_types != NULL){
    	free(all_types);
    	all_types = NULL;
    }



    for(int i = 0; i < n_pokemons; i++) {
        if(&all_pokemons[i] != NULL) {
            free_pokemon(&all_pokemons[i]);
        }
    }

    if(all_pokemons != NULL){
    	free(all_pokemons);
    	all_pokemons = NULL;
    }

    printf("All the memory cleaned and the program is safely closed.\n");
	return 0;
}



