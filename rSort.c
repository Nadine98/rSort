#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Word_Lenght 101// für 100 Wörter + 1 für das Nullzeichen
#define Array_Size 100 // Initialisierung der Array mit 100 Speicherplätzen




//----------------------------------------------Funktionsdeklaration-------------------------------------------------------------------------
static void error(const char *);
static char **createArray();
static int compare(const void *, const void *);
static void freeArray(char **, int);



int main()
{

    //Anzahl der Element im Array - Initialisiert erst mit 100 Plätzen
    int arraySize =Array_Size;

    // Anazahl der eingelesen Wörter
    int numberOfWords =0;

    // Die Anzahl der eingelesenen Buchstaben eines Wortes
    int lengthOfWord=0;

    // Array, in dem die Wörter eingespeichert werden
    char **wordArray=createArray();



//------------------------------------------- Input einlesen -----------------------------------

   char c; // Zum Einlesen jedes Zeichens

    while((c=fgetc(stdin))!=EOF){


    // Array hat begrenzte Speicherplätze --> Array erweitern
        if ( numberOfWords+1 == arraySize){
            arraySize = arraySize *2 ;

            wordArray= (char **) realloc(wordArray, (arraySize * sizeof(wordArray)));
            if(wordArray==NULL)
                error("realloc");

            for(int i=numberOfWords; i < arraySize -1;i++){
                wordArray[i]=(char*) calloc(Word_Lenght, sizeof(wordArray[i]));

                if(wordArray[i]==NULL)
                    error("calloc");
            }
         }




// Hier werden aus dem stdin- Kanal die Wörter eingelesen


            //Fall 1) Einlesen der Zeichen in das Array

            if(lengthOfWord < Word_Lenght && c!=10){
                wordArray[numberOfWords][lengthOfWord]=c;
                lengthOfWord++;
            }

            // Fall 2) Wortlänge ist kleiner 100 Buchstaben
            else if ( (lengthOfWord >0 && lengthOfWord< Word_Lenght)&& c==10){
                wordArray[numberOfWords][lengthOfWord]=0;
                numberOfWords++;
                lengthOfWord=0;
            }
            
            // Fall 3) Zeile ist leer, also wird diese ignoriert 
            else if(lengthOfWord==0 && c==10){
				continue;
			}

            //Fall 4) Wort mit mehr als 100 Zeichen, werden gekürzt
            else if(lengthOfWord>Word_Lenght && c==10){

                // Setze an das 101.Zeichen das Nullzeichen, zum Beenden des Strings
                wordArray[numberOfWords][Word_Lenght-1]=0;
                numberOfWords++;
                lengthOfWord=0;
            }

            //Fall 5) Wortlänge größer als 100 (noch nicht \n erreicht), Wörter nach dem 100.Buchstaben ignorieren
            else{
                // alles ab dem 101 Buchstaben
                if(lengthOfWord==100){
                    if (fprintf(stderr, "Zeile ist zu lang\n") < 0){
                        error("fprintf");
                    }

                }

                continue;
        }
    }

//-----------------------------------------------------------------------------------------------------

   // Array wird sortiert
    qsort(wordArray,numberOfWords,sizeof(char*),compare);


    for(int i=0 ; i<numberOfWords; i++){

        if(printf("%s\n", wordArray[i])<0)
            error("Printf");
     }
     


     // Allokierten Speicher freigeben
     freeArray(wordArray,arraySize);



}



//----------------------------------------------Funktionsdefiniton-------------------------------------------------------------------------

static void freeArray(char **array, int arraySize){

    for(int i =0; i< arraySize ; i++){
        free(array[i]);
    }

    free(array);
}


static int compare(const void *a, const void *b){

	char **c = (char **) a;
	char **d = (char **) b;
	int x = strcmp(*c, *d);
	(x >= 0) ? ((x>0) ? (x  = -1) : (x = 0)) : (x = 1);
	return x;
}



static char **createArray(){


    // Array mit 100 Einträgen für die Wörter erzeugen --> das 101 Zeichen ist für das Null-Zeichen
    char **words= (char **) malloc(Array_Size * sizeof(words));

    if(words == NULL)
        error("Malloc");


    for(int i=0; i<Array_Size; i++){
        words[i]=(char *) calloc(Word_Lenght,sizeof(words[i]));
        if(words[i]==NULL)
            error("Calloc");
    }

    return words;

}



static void error(const char *msg){
    perror(msg);
    exit(EXIT_FAILURE);

}
