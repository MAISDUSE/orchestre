/*----------------------------------------------------------

             semaphores de Dijkstra
             
 Attention : ne pas affecter les semaphores a SEM_UNDO
             car il y a risque que des operations soient defaites
             alors qu'il y a encore un processus utilisateur
---------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>

//================= LOCAL

static int tabSemId;

static void erreurFin(const char* msg)
  {
  perror(msg);
  exit(1);
  }
  
static void appelSem(short unsigned int numSem, short int op)
  {
  struct sembuf sempar;
  const unsigned int nbop = 1;
  
  sempar.sem_num = numSem;      
  sempar.sem_op = op;
  sempar.sem_flg = 0;  // ne pas mettre a SEM_UNDO !!
  if (semop(tabSemId, &sempar, nbop) == -1) erreurFin("appelSem");
  }
  
//================== EXPORTE

/*----------------- primitives ---------------------------*/

void P(short unsigned  numSem)  {appelSem(numSem,-1);}
void V(short unsigned  numSem)  {appelSem(numSem,1);}
  
/*----------------- creation -------------------------------
             pvinit : tableau d'entiers contenant les valeurs
                      initiales, init a 0 si NULL          */

void initSem(short unsigned  nbSem,char* argv0,int* pvinit)
  {
  int i;
  key_t cle = ftok(argv0, '0');
  tabSemId  = semget(cle, nbSem, IPC_CREAT | 0600);
  if (tabSemId == -1) erreurFin("initSem : semget");
  for (i=0;i<nbSem;i++)
     if (pvinit == NULL) {
        if (semctl(tabSemId, i, SETVAL, 0) == -1)
           erreurFin("initSem : semctl 1");
        }
     else 
        if (semctl(tabSemId, i, SETVAL, pvinit[i]) == -1)
           erreurFin("initSem : semctl 2");
  }

/*-------------- destruction (car objets globaux persistents)*/
void libereSem() 
  {
  if (semctl(tabSemId, IPC_RMID, 0))
     erreurFin("libereSem");
  }
  
  
