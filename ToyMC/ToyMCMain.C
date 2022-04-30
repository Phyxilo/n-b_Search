#include <iostream>
#include "TClass.h"
#include "ToyMC.h"

using namespace MCVar;

double binPosRatio[2][49];
double binEnRatio[2][49];

double MCComb[49][3] =
{
	{400,2.0,0.4},
	{400,2.0,0.6},
	{400,2.0,0.8},
	{400,2.0,1.0},
	{400,2.0,1.2},
	{400,2.0,1.4},
	{400,2.0,1.6},
	{400,2.5,0.4},
	{400,2.5,0.6},
	{400,2.5,0.8},
	{400,2.5,1.0},
	{400,2.5,1.2},
	{400,2.5,1.4},
	{400,2.5,1.6},
	{400,3.0,0.4},
	{400,3.0,0.6},
	{400,3.0,0.8},
	{400,3.0,1.0},
	{400,3.0,1.2},
	{400,3.0,1.4},
	{400,3.0,1.6},
	{400,3.5,0.4},
	{400,3.5,0.6},
	{400,3.5,0.8},
	{400,3.5,1.0},
	{400,3.5,1.2},
	{400,3.5,1.4},
	{400,3.5,1.6},
	{400,4.0,0.4},
	{400,4.0,0.6},
	{400,4.0,0.8},
	{400,4.0,1.0},
	{400,4.0,1.2},
	{400,4.0,1.4},
	{400,4.0,1.6},
	{400,4.5,0.4},
	{400,4.5,0.6},
	{400,4.5,0.8},
	{400,4.5,1.0},
	{400,4.5,1.2},
	{400,4.5,1.4},
	{400,4.5,1.6},
	{400,5.0,0.4},
	{400,5.0,0.6},
	{400,5.0,0.8},
	{400,5.0,1.0},
	{400,5.0,1.2},
	{400,5.0,1.4},
	{400,5.0,1.6},
};
void drawtext();
void dataOut(int n);

int n = 49;
void ToyMCMain()
{
	for (int i = 0; i < n; i++)
	{
		//cout << ToyMC (MCComb[i][0], MCComb[i][1], MCComb[i][2])[4] << endl;
		ToyMC(MCComb[i][0], MCComb[i][1], MCComb[i][2], 4000000);

		binPosRatio[0][i] = binInt[1]/binInt[0];
		binPosRatio[1][i] = binInt[2]/binInt[0];
		cout << "Pos -> Middle/Center: " << binInt[1]/binInt[0] << ", Pos -> Edge/Center: " << binInt[2]/binInt[0] << endl;

		binEnRatio[0][i] = binEn[1]/binEn[0];
		binEnRatio[1][i] = binEn[2]/binEn[0];
		cout << "En -> Middle/Center: " << binInt[1]/binInt[0] << ", En -> Edge/Center: " << binInt[2]/binInt[0] << endl;
	}
	TCanvas *Canvas= new TCanvas("Canvas","Graph Canvas",20,20,1920,1080);

	TGraph *posRatio = new TGraph (n, binPosRatio[1], binPosRatio[0]);
	posRatio->SetMarkerStyle(21);
	posRatio->SetMarkerColor(4);
	posRatio->SetTitle("Position");

	TExec *exPos = new TExec("exPos","drawtext();");
	posRatio->GetListOfFunctions()->Add(exPos);
	posRatio->Draw("AP");

	Canvas->Print( "Out/MCRatio.pdf(","pdf");

	TGraph *enRatio = new TGraph (n, binEnRatio[1], binEnRatio[0]);
	enRatio->SetMarkerStyle(20);
	enRatio->SetMarkerColor(2);
	enRatio->SetTitle("Energy");

	TExec *exEn = new TExec("exEn","drawtext();");
	enRatio->GetListOfFunctions()->Add(exEn);
	enRatio->Draw("AP");

	Canvas->Print( "Out/MCRatio.pdf)","pdf");

	dataOut(n);
}

void drawtext()
{
   Int_t i,n;
   Double_t x,y;
   TLatex *l;

   TGraph *g = (TGraph*)gPad->GetListOfPrimitives()->FindObject("Graph");
   n = g->GetN();
   for (i=1; i<n; i++)
	 {
      g->GetPoint(i,x,y);
      l = new TLatex(x,y+0.01,Form("{%1.1f, %1.1f}",MCComb[i][1], MCComb[i][2]));
      l->SetTextSize(0.025);
      l->SetTextFont(42);
      l->SetTextAlign(21);
      l->Paint();
   }
}

void dataOut(int n)
{
	FILE *fpo, *fen;

	fpo = fopen("Out/pos.dat","wt");
	fen = fopen("Out/en.dat","wt");

	for (int j = 0; j < n; j++)
	{
		fprintf(fpo,"%1.1f %1.1f %4.4f %4.4f\n",MCComb[j][1], MCComb[j][2], binPosRatio[0][j], binPosRatio[1][j]);
		fprintf(fen,"%1.1f %1.1f %4.4f %4.4f\n",MCComb[j][1], MCComb[j][2], binEnRatio[0][j], binEnRatio[1][j]);
	}
	fclose(fpo);
	fclose(fen);
}
