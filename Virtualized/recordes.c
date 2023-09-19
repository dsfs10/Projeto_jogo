#include "raylib.h"

void Recordes(int qtd_rec,double *tempo,int destaque){
    ClearBackground(BLACK);
    DrawText(TextFormat("Recordes:"), 120, 80, 40, WHITE);
    DrawText(TextFormat("Voltar"), 990, 600, 40, WHITE);
    if(GetMouseX() >= 990 && GetMouseX() <= 1130 && GetMouseY() >= 600 && GetMouseY() <= 640) {
      DrawText("Voltar", 990, 600, 40, BLUE);
      }
    for(int i=0;i<qtd_rec;i++){
        int coluna = i/18;
        int linha = i%18;
        if(tempo[i]>=3600){
            int h= (tempo[i]/3600);
            int resto =(int) tempo[i]%3600;
            int m=resto/60;
            resto = resto%60;
            if(i==destaque) DrawText(TextFormat("%d. %d:%02d:%02d",i+1,h,m,resto), 200*coluna+100, (100)+30*(linha+1), 20, RED);
            else DrawText(TextFormat("%d. %d:%02d:%02d",i+1,h,m,resto), 200*coluna+100, (100)+30*(linha+1), 20, WHITE);
        }else{
            int resto =(int)tempo[i]%3600;
            int m=resto/60;
            resto = resto%60;
            if(i==destaque) DrawText(TextFormat("%d. %02d:%02d",i+1,m,resto), 200*coluna+100, (100)+30*(linha+1), 20, RED);
            else DrawText(TextFormat("%d. %02d:%02d",i+1,m,resto), 200*coluna+100, (100)+30*(linha+1), 20, WHITE);
        }
    }
}


int lfbubbleSort(double *vetor, int tamanho) {
    int i, j;
    double temp;
	int destaque = tamanho-1;
    for (i = 0; i < tamanho - 1; i++) {
        for (j = 0; j < tamanho - i - 1; j++) {
            if (vetor[j] > vetor[j + 1]) {
                // Troca os elementos se estiverem fora de ordem
                temp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temp;
				if((j+1)==destaque)destaque--;  //devolve o destaque, que eh o recorde do jogo atual
            }
        }
    }
	return destaque;
}