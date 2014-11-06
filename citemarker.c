#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int searchCR(char *line, char **mypubs, int pubcnt);

int main(int argc, char **argv){
  char line[5000];
  char field[10];
  int records=0;
  int markedpub=0;
  int markedcite=0;
  FILE *cite, *pubs;
  char **mypubs;
  int i;
  int pubcnt=0;
  int prevfound=0;
  char prevti[5000];
  int citeret;
  int wasau;
  
  prevti[0]=0;

  if (argc!=3){
    fprintf(stderr, "Citation Marker for the Annual Faculty Survey.\n\nUsage: %s <citations.txt> <mypubs.txt>\n\n", argv[0]);
    fprintf(stderr, "\t<citations.txt>: List of citations downloaded as a text file from WoS.\n");
    fprintf(stderr, "\t<mypubs.txt>   : List of own papers. See mypubs.txt in the git repo as an example.\n\n");
    fprintf(stderr, "This program will halt when it finds a citation that doesn't match to at least one publication in the mypubs.txt file. If this happens. find the missing publication, add it to mypubs.txt file, and rerun.\n");
    return -1;
  }

  cite = fopen(argv[1], "r");
  pubs = fopen(argv[2], "r");

  while(1){
    fgets(line, 5000, pubs);
    if (feof(pubs)) break;
    pubcnt++;
  }

  mypubs = (char **) malloc (sizeof(char *) * pubcnt);
  for (i=0;i<pubcnt;i++)
    mypubs[i] = (char *) malloc (sizeof(char) * 5000);

  rewind(pubs);
  
  pubcnt=0;
  while(1){
    fgets(line, 5000, pubs);
    
    if (feof(pubs)) break;
    line[strlen(line)-1] = 0;
    strcpy(mypubs[pubcnt++], line);
  }

  fprintf(stderr, "%d pubs loaded.\n", pubcnt);


  field[0]=0;

  i=0;
  wasau=0;

  while(1){
    
    fgets(line, 5000, cite);
    if (feof(cite)) break;
    
    line[strlen(line)-1] = 0;
    
    //    fprintf(stdout, "_________LINE_______ %s ___FIELD___%s\n", line, field);
    //getchar();
      
    if (!isspace(line[0])){
      field[0]=line[0];
      field[1]=line[1];
      field[2]=0;
      if (strcmp(field, "AU")) { if (wasau) printf("\n"); wasau=0;}

      //fprintf(stdout, "field : %s\n", field);
    }
    
    
    if (!strcmp(field, "FN") || !strcmp(field, "VR"))
      continue;
    
    if (!strcmp(field, "WC") || !strcmp(field, "SC"))
      continue;
    
    if (!strcmp(field, "PA") || !strcmp(field, "JI") || !strcmp(field, "ID"))
      continue;
    
    
    if (!strcmp(field, "AB") || !strcmp(field, "EM") || !strcmp(field, "FU")  || !strcmp(field, "FX"))
      continue;

    if (!strcmp(field, "LA") || !strcmp(field, "J9") || !strcmp(field, "DE")  || !strcmp(field, "RI"))
      continue;
    
    if (!strcmp(field, "Z9") || !strcmp(field, "RP")  || !strcmp(field, "C1") || !strcmp(field, "AF"))
      continue;
    

    if (!strcmp(field, "CR")){
      
      //fprintf(stdout, "_________CR________ %s\n", line);
      //getchar();

      if (line[0]!=' '){
	citeret = searchCR(line+3, mypubs, pubcnt);
	if (citeret == 1) {
	  prevfound = 1;
	  printf("CR >>>> %s <<<<\n", line+3);
	  markedcite++;
	}
	else
	  printf("CR %s\n", line+3);
	continue;
      }
   
      else{
	//printf("search this...\n");
	citeret = searchCR(line, mypubs, pubcnt);
	if (citeret == 1) {
	  prevfound = 1;
	  printf(">>>> %s <<<<\n", line);
	  markedcite++;
	}
	else
	  printf("%s\n", line);
	continue;
      }

    }

    if (!strcmp(field, "AU")){
      wasau=1;
      if (line[0]!=' ')
	printf("%s; ", line);
      else
	printf("%s; ", line+3);
      continue;
    }
    
    if (!strcmp(field, "TI"))
      strcpy(prevti, line);
    
    if (!strcmp(field, "PT")){
      records++;
      
      if (records!=1 && prevfound == 0){
	fprintf(stderr, "******\n");
	fprintf(stderr, "Cite not found for TI: %s\n", prevti);
	fprintf(stderr, "****** pubcnt: %d record: %d *****\n", pubcnt, records-1);
	return 0;
      }
      prevfound = 0;
    }

    if (strcmp(field, "CR"))
      printf("%s\n", line);
	
    /*
    if (line[0]==0){

      if (line[0]!='C' || line[1]!='R')
	printf("%s\n", line);
      
      else if (!strcmp(field, "CR")){  
      //(line[0]!='C' || line[1]!='R'){
	printf("search this...\n");
	citeret = searchCR(line, mypubs, pubcnt);
	if (citeret == 1) {
	  prevfound = 1;
	  printf(">>>> %s <<<<\n", line);
	}
	else
	  printf("%s\n", line);
	//      printf("%s\n", line);

    }

    }*/

  }
  fprintf (stderr,"Done with %d records and total %d citations.\n", records, markedcite);
}


int searchCR(char *line, char **mypubs, int pubcnt){
  int i;
  int j;
  int k;
  char cline[5000];
  
  //  printf("search %s\n", line);
  //getchar();

  j=0;i=0;
  while (line[i] == ' ') i++;

  for (k=i;k<strlen(line);k++){
    cline[j++]=line[k];
  }
  cline[j]=0;

  for (i=0;i<pubcnt;i++)
    if (!strcmp(cline, mypubs[i]))
      return 1;

  return 0;
}
