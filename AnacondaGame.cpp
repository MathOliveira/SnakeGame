#include <conio.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>

#define COLUNAS 30
#define LINHAS 20
#define SHOWTEXTS 1
#define TIME 100
#define VIDAS 3

/**
* @autor Matheus Sant"Anna de Oliveira <https://github.com/MathOliveira>*
*/

struct nodo{
	struct nodo *ant;
	int x;
	int y;
	struct nodo *prox;
} corpo;

struct nodo *inicio;
struct nodo *fim;
int tam = -2;
int melhor = 0;
int vidas = VIDAS;

/*-------------------------------------------------------------------*/
//função vai para posição solicitada da tela
void mgotoxy(int x,int y){	
	COORD p={x,y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),p);
}

/*-------------------------------------------------------------------*/
//função para montar tela 
void montarTela(){
	int x;
    //esquerda 
    for(x=0;x<=LINHAS+1;x++){   
		mgotoxy(0,x);
        printf("%c",35);
    }
    //direita 
    for(x=0;x<=LINHAS+1;x++){   
		mgotoxy(COLUNAS+1,x);
        printf("%c",35);
    }
    //superior 
    for(x=0;x<=COLUNAS+1;x++){
		mgotoxy(x,0);
        printf("%c",35);
    }
    //inferior 
    for(x=0;x<=COLUNAS+1;x++){
		mgotoxy(x,LINHAS+1);
        printf("%c",35);
    } 
    //inicializa semente do rand()
	srand((unsigned)time(NULL)); 
	//
	if (SHOWTEXTS==1){
		mgotoxy(COLUNAS+5,2);	
	    printf("ANACONDA GAME");	
		mgotoxy(COLUNAS+5,14);
		printf("Shortcuts");
		mgotoxy(COLUNAS+6,16);
		putch(24);	
		mgotoxy(COLUNAS+5,17);
		putch(27);	
		mgotoxy(COLUNAS+6,17);
		putch(25);	
		mgotoxy(COLUNAS+7,17);
		putch(26);	
		mgotoxy(COLUNAS+9,17);
		printf("To move ");
		mgotoxy(COLUNAS+5,18);
		printf("+   Increase speed");
		mgotoxy(COLUNAS+5,19);
		printf("-   Decrease speed");
		mgotoxy(COLUNAS+5,20);
		printf("ESC Exit the game");
	}else{
		//se mostrar texto estiver em 1 jogador tera apenas uma vida
		vidas=1;	
	}	
}

/*-------------------------------------------------------------------*/
// função para atualizar painel
void atualizaPainel(){
	if (SHOWTEXTS==1){
		mgotoxy(COLUNAS+5,6);		
		printf("Humans killed:	%d  ",tam);
		mgotoxy(COLUNAS+5,8);
		printf("Your best score:	%d  ",melhor);
		mgotoxy(COLUNAS+5,4);		
		printf("Lives:");
		int c;
		for (c=1;c<=VIDAS;c++){
			mgotoxy(COLUNAS+25+c,4);	
			if (c<=vidas){
				putch(3);
			}else{
				putch(0);
			}
		}
	}
}

/*-------------------------------------------------------------------*/
// função para consultar valor na lista (corpo)
int consulta(int x, int y){
  int resp = 0;  
  struct nodo *atual = inicio;
  while((atual != NULL)&&(resp == 0)){
	 if((atual->x == x)&&(atual->y == y)){
        resp = 1;
     }
	 atual = atual->prox;
  }
  return resp;
}

/*-------------------------------------------------------------------*/
// função para inserção no inicio da lista (corpo)
void insere(int x, int y){
	struct nodo *pnovo;
	pnovo=(struct nodo*)malloc(sizeof(corpo));
	pnovo->x=x;
	pnovo->y=y;
	pnovo->prox=NULL;  
	if(inicio == NULL){
		inicio=pnovo;
		fim=pnovo;
	}else{
		inicio->ant=pnovo;
		pnovo->prox=inicio;
		inicio=pnovo;   
	}	
	mgotoxy(x,y);
	//corpo cobra
	putch(15);
	tam++;	
	if(tam>melhor){
		melhor=tam;
	}
	atualizaPainel();
}

/*-------------------------------------------------------------------*/
// função para excluir um valor do fim da lista (corpo)
void excluiFim(){
	int resp = 0;  
	struct nodo *atual=fim;
	if(inicio != NULL){		
		if(inicio==fim){
		    inicio=NULL;
			fim=NULL;          
		}else{
		    fim=fim->ant;
		    fim->prox=NULL;
		}
		//limpa fim
		mgotoxy(atual->x,atual->y);
		putch(' ');
		tam--;	
		free(atual);
	}
}

/*-------------------------------------------------------------------*/
// função para limpar lista (corpo)
void limpaLista(){
	while(inicio != NULL){
		excluiFim();
	}
}

/*-------------------------------------------------------------------*/
//função para gerar valor randomico 
int gerarValorRandomico(int max){
	return (rand()%max)+1;
}	

/*-------------------------------------------------------------------*/
//função para gerar nova comida 
void novaComida(int *cx, int *cy, int x, int y){
	*cx=gerarValorRandomico(COLUNAS);
	*cy=gerarValorRandomico(LINHAS);
	//verifica se gerou na posição atual
	if ((*cx==x)&&(*cy==y)){
		novaComida(cx,cy,x,y);		
	}else{		
		//verifica se gerou no corpo da cobra	
		if (consulta(*cx,*cy)==1){
			novaComida(cx,cy,x,y);
		}else{		
			mgotoxy(*cx,*cy);
			putch(12);			
		}
	}
}

/*-------------------------------------------------------------------*/
//principal
main()
{   
	int count,x,y,direcao,pausa,cx,cy;
	montarTela();  	
	unsigned char c;	
	bool erro,comeu;
	//percorre vidas
	while ((vidas>0)&&(c!=27)){//enquando restarem vidas
		direcao=0;
		pausa=TIME;
		erro=false;
		comeu=false;
		x=1;
		y=1;		
		c=0;	
		count=0;	
		insere(x,y);
		novaComida(&cx,&cy,x,y);		
		//loop
		while (erro==false){//enquanto não morrer				
			//direção
			switch(direcao){
				case 0: x++;//direita
				break;
				case 1: y++;//baixo
				break;
				case 2: x--;//esquerda
				break;
				case 3: y--;//cima
				break;
			}
			//verifica se bateu nos limites do quadro
			if ((x==0)||(y==0)||(x==COLUNAS+1)||(y==LINHAS+1)){
				erro=true;
			}
			//verifica se bateu no corpo da cobra	
			if (consulta(x,y)==1){
				erro=true;
			}	
			//tira uma vida se morreu
			if (erro==true){
				vidas--;
			}else			
			//verifica se comeu a comida
			if ((x==cx)&&(y==cy)){
				novaComida(&cx,&cy,x,y);
				comeu=true;			
			}
			//se não morreu
			if (erro==false){
				//adiciona na posicão a cobra
				insere(x,y);
				Sleep(pausa);
				//verifica tecla lida				
				if (kbhit()){
				 	//le duas vezes se for usado teclas especiais
				    c=getch(); 
					if (c==224){
						c=getch();
					}
					if      (c=='-') pausa+=20;
					else if (c=='+') pausa-=20;
					else if (c== 27) erro=true;
					else if ((c==75)&&(direcao!=0)) direcao=2;//esquerda	   	
					else if ((c==72)&&(direcao!=1)) direcao=3;//cima	   	
					else if ((c==77)&&(direcao!=2)) direcao=0;//direita	   	
					else if ((c==80)&&(direcao!=3)) direcao=1;//baixo
				}
				//se comeu não elemina rabo para aumentar o tamanho da cobra
				if (comeu==true){
					comeu=false;				
				}else{
					excluiFim();
				}			
			}		
		}
		//limpa para novo jogo		
		limpaLista();
		mgotoxy(cx,cy);
		putch(0);
		tam=-2;		
		Sleep(1500);
	}
	//fim do jogo   
	tam=0;
	atualizaPainel();
	if (SHOWTEXTS==1){ 
		mgotoxy((COLUNAS/2)-5,LINHAS/2);		
		printf("GAME OVER!");
	}
    getch();
}
