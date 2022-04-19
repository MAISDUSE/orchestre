extern void erreurFin(const char* msg);
extern void initSem(short unsigned  nbSem,char* nomFich,int* pvinit);
extern void libereSem();
extern void P(short unsigned  numSem);
extern void V(short unsigned  numSem);
