#include "utils.h"

void* (* Abilities[4])(void* x) = {RotateMatrix, DecodeString, InvertArray, KillPlayer};

char *fromEnumtoString(PlayerRoles playerRole)
{
	char *str = calloc(MAX_LEN_STR_ATR, sizeof(char));
	switch(playerRole)
	{
		case Rotator:
			str = strcpy(str, "Rotator");
			break;
		case Decoder:
			str = strcpy(str, "Decoder");
			break;
		case Invertor:
			str = strcpy(str, "Invertor");
			break;
		case Impostor:
			str = strcpy(str, "Impostor");
			break;
	}
	return str;
}
void itoa(int a, char res[])
{
	//int to array
	int len = 0, i = 0;
	if(a == 0)
	{
		res[0] = '0';
		res[1] = '\0';
		return ;
	}

	while(a!=0)
	{
		res[len] = a%10 + '0';
		a/=10;
		len++;
	}
	res[len]= '\0';

	for(i = 0; i<len/2; i++)
	{
		char aux = res[i];
		res[i] = res[len - i - 1];
		res[len - i - 1] = aux;
	}
}
// Task 1
void *RotateMatrix(void *input)
{
	// TODO
	int i, j, n = *(int *)input;
	char *string;

	string = malloc(n * n * n * sizeof(char));

	for(i=0; i < n; i++){
		for (j=0; j < n; j++)
		{
			char I[10], J[10];

			itoa(n-j, I);
			itoa(i+1, J);
			strcat(string, I);
			strcat(string, J);

			strcat(string, " \0");

		}
		if( i!= n-1)
			strcat(string, "\n\0");
		else
			strcat(string, "\0");
	}

	return string;
}

// Task 2
void *DecodeString(void *input)
{
	// TODO
	int sum = 0, len = 0;
	char *c, *res, *string;
	res = malloc(8 * sizeof(char));

	string = (char *)input;
	c = strtok(string, "_");
	while(c != NULL)
	{
		sum+= atoi(c);
		c = strtok(NULL, "_");
	} //s-a obtinut suma intr-o variabila de tip int

	int inv = 0;
	while(sum!=0)
	{
		inv = inv*10 + sum%10;
		sum/=10;
	}
	while(inv!=0)
	{
		res[len] = inv%10 + '0';
		inv/=10;
		len++;
	}
	res[len]= '\0';

	return res;
}

//Task 3
void *InvertArray(void *input)
{
	// TODO

	int *vec, i, n;
	char *res;

	vec = (int *)input;

	n = vec[0];
	res = malloc(sizeof(char) * 2 * (n+1) );

	if( n % 2 == 1){

	for(i = 1; i<= n/2; i++)
		{
			int aux = vec[i];
			vec[i] = vec[n - i + 1];
			vec[n - i + 1] = aux;
		}
	}
	else{

	for(i = 1; i<= n; i+=2)
	{
		int aux = vec[i];
		vec[i] = vec[i + 1];
		vec[i + 1] = aux;
	}
}
	for(i = 1; i<= n; i++)
	{
		char c[10];
		itoa(vec[i], c);

		strcat(res, c);
		strcat(res, " \0");
	}
	strcat(res, "\0");
	return res;
}

//Task 4
Player *allocPlayer()
{
	// TODO
	Player *P = (Player *)calloc(1, sizeof(Player));

	P->name = calloc(MAX_LEN_STR_ATR, sizeof(char));
	P->hat = calloc(MAX_LEN_STR_ATR, sizeof(char));
	P->color = calloc(MAX_LEN_STR_ATR, sizeof(char));

	P->alive = 1;

	return P;
}

//Task 4
Game *allocGame()
{
	// TODO

	Game *G = (Game *)calloc(1, sizeof(Game));

	G->name = calloc(MAX_LEN_STR_ATR, sizeof(char));
	return G;
}

//Task 5
Player *ReadPlayer(FILE *inputFile)
{
	// TODO
	Player *player = allocPlayer();
	fscanf(inputFile, "%s", player->name);
	fscanf(inputFile, "%s", player->color);
	fscanf(inputFile, "%s", player->hat);
	fscanf(inputFile, "%d", &player->numberOfLocations);

	char buffer[100];
	int i;
	player->locations = (Location*)calloc(player->numberOfLocations, sizeof(Location));
	for(i = 0; i < player->numberOfLocations; i++)
	{
		fscanf(inputFile, "%s", buffer); // buffer =(x,y)
										// se extrage x si y din string
		char *X = buffer + 1, *Y = strchr(buffer, ',') + 1;
		char *p = strchr(buffer, ')');
		*p = '\0';
		p = strchr(buffer, ',');
		*p = '\0';


		player->locations[i].x = atoi(X);
		player->locations[i].y = atoi(Y);
	}
	player->indexOfLocation = 0;

	char S[20];

	fscanf(inputFile, "%s", S); //player role

	if(strcmp(S, "Invertor") == 0)
	{
		player->playerRole = Invertor;
	}
	if(strcmp(S, "Rotator") == 0)
	{
		player->playerRole = Rotator;
	}
	if(strcmp(S, "Decoder") == 0)
	{
		player->playerRole = Decoder;
	}
	if(strcmp(S, "Impostor") == 0)
	{
		player->playerRole = Impostor;
	}

	return player;
}

// Task 5
Game *ReadGame(FILE *inputFile)
{
	// TODO

	Game *game = allocGame();

	fscanf(inputFile, "%s", game->name);
	fscanf(inputFile, "%d", &game->killRange);
	fscanf(inputFile, "%d", &game->numberOfCrewmates);

	int i;
	game->crewmates = (Player*)calloc(game->numberOfCrewmates, sizeof(Player));

	for(i = 0; i < game->numberOfCrewmates; i++)
	{
		game->crewmates[i] = allocPlayer();
		game->crewmates[i] = ReadPlayer(inputFile);
	}
	game->impostor = allocPlayer();
	game->impostor = ReadPlayer(inputFile);

	return game;
}

// Task 6
void WritePlayer(Player *player, FILE *outputFile)
{
	// TODO

	char *rol;
	rol = fromEnumtoString(player->playerRole);

	fprintf(outputFile, "Player %s with color %s, hat %s and role %s has entered the game.\n"
		, player->name, player->color, player->hat, rol);

	fprintf(outputFile, "Player's locations:");
	for(int i = 0; i < player->numberOfLocations; i++)
		fprintf(outputFile, " (%d,%d)", player->locations[i].x, player->locations[i].y);

	fprintf(outputFile, "\n");

	return;
}

// Task 6
void WriteGame(Game *game, FILE *outputFile)
{
	// TODO

	fprintf(outputFile, "Game %s has just started!\n", game->name);
	fprintf(outputFile, "\tGame options:\nKill Range: %d\n", game->killRange);
	fprintf(outputFile, "Number of crewmates: %d\n", game->numberOfCrewmates);
	fprintf(outputFile, "\n\tCrewmates:\n");

	for(int i = 0; i < game->numberOfCrewmates; i++)
		WritePlayer(game->crewmates[i], outputFile);


	fprintf(outputFile, "\n\tImpostor:\n");
	WritePlayer(game->impostor, outputFile);
	return;
}

//Task 7
void *KillPlayer(void *input)
{
	// TODO

	Game *game = (Game*)(input);

	int cycleIMP = ( game->impostor->indexOfLocation + 1) % game->impostor->numberOfLocations - 1;
	if(cycleIMP == -1)
			cycleIMP = game->impostor->numberOfLocations - 1;

	int dis, minimDIS = __INT_MAX__, indice = -1;

	for(int i = 0; i < game->numberOfCrewmates; i++)
		if(game->crewmates[i]->alive == 1)
		{
			int cycle = ( game->crewmates[i]->indexOfLocation + 1) % game->crewmates[i]->numberOfLocations - 1;
			if(cycle == -1)
				cycle = game->crewmates[i]->numberOfLocations - 1;

			int a1 = game->impostor->locations[cycleIMP].x;
			int a2 = game->impostor->locations[cycleIMP].y;
			int b1 = game->crewmates[i]->locations[cycle].x;
			int b2 = game->crewmates[i]->locations[cycle].y;
			dis = fabs(a1 - b1) + fabs(a2 - b2);

			if(minimDIS >= dis)
				{
					minimDIS = dis;
					indice = i;
				}
		}

	char *output = (char*)calloc(50 ,sizeof(char));

	if(game->killRange <= minimDIS)
	{
		sprintf(output, "Impostor %s couldn't kill anybody.", game->impostor->name);
	}
	else
	{
		sprintf(output, "Impostor %s has just killed crewmate %s from distance %d.", 
			game->impostor->name, game->crewmates[indice]->name, minimDIS);

		game->crewmates[indice]->alive = 0;
	}
	
	return output;
}

// Task 8
void CalcuateNextCycleOfGame(Game *game, FILE *outputFile, void **inputMatrix)
{
	// TODO
	int cycle;

	for(int i = 0; i < game->numberOfCrewmates; i++)
		{
			cycle = ( game->crewmates[i]->indexOfLocation + 1) % game->crewmates[i]->numberOfLocations ;

			if(game->crewmates[i]->alive == 0)
			{
				fprintf(outputFile, "Crewmate %s is dead.\n", game->crewmates[i]->name);
			}
		else
			{
				fprintf(outputFile, "Crewmate %s went to location (%d,%d).\n", 
				game->crewmates[i]->name, game->crewmates[i]->locations[cycle].x, game->crewmates[i]->locations[cycle].y);

				fprintf(outputFile, "Crewmate %s's output:\n", game->crewmates[i]->name);


				char *res = (char*)Abilities[game->crewmates[i]->playerRole](inputMatrix[i]);
				fprintf(outputFile, "%s\n", res);
				game->crewmates[i]->indexOfLocation ++;
			}
		}
		//Impostor
		cycle = ( game->impostor->indexOfLocation + 1) % game->impostor->numberOfLocations ;
		fprintf(outputFile, "Impostor %s went to location (%d,%d).\n", 
		game->impostor->name, game->impostor->locations[cycle].x, game->impostor->locations[cycle].y);

		fprintf(outputFile, "Impostor %s's output:\n", game->impostor->name);


		char *res =(char*)Abilities[game->impostor->playerRole](game);

		fprintf(outputFile, "%s\n", res);
		game->impostor->indexOfLocation ++;

	return;
}

// Task 9
void FreePlayer(Player *player)
{
	// TODO
	return;
}

// Task 9
void FreeGame(Game *game)
{
	// TODO
	return;
}