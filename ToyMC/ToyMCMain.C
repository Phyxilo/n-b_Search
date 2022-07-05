#include <iostream>
#include <vector>
#include "TClass.h"
#include "ToyMC.C"
#include "LUTable.h"

using namespace MCVar;

vector<vector<double>> binPosRatio, binEnRatio;

void drawtext();
void dataOut(int n);

vector<vector<float>> luTable = Table2;

void ToyMCMain()
{
	int n = luTable.size();

	for (int i = 0; i < n; i++)
	{
		ToyMC(luTable[i][0], luTable[i][1], luTable[i][2], 10000);

		vector<double> posVec;
		posVec.push_back(binInt[1]/binInt[0]); posVec.push_back(binInt[2]/binInt[0]);
		binPosRatio.push_back(posVec);
		posVec.clear();
		
		cout << "Pos -> Middle/Center: " << binInt[1]/binInt[0] << ", Pos -> Edge/Center: " << binInt[2]/binInt[0] << endl;

		vector<double> enVec;
		enVec.push_back(binEn[1]/binEn[0]); enVec.push_back(binEn[2]/binEn[0]);
		binEnRatio.push_back(enVec);
		enVec.clear();

		cout << "En -> Middle/Center: " << binInt[1]/binInt[0] << ", En -> Edge/Center: " << binInt[2]/binInt[0] << endl;
		
	}
	TCanvas *Canvas= new TCanvas("Canvas","Graph Canvas",20,20,1920,1080);
	
	double posRatioArr[2][luTable.size()];
	double enRatioArr[2][luTable.size()];

	for (int i = 0; i < luTable.size(); i++)
	{
		posRatioArr[0][i] = binPosRatio[i][0]; posRatioArr[1][i] = binPosRatio[i][1];
		enRatioArr[0][i] = binEnRatio[i][0]; enRatioArr[1][i] = binEnRatio[i][1];
	}
	
	TGraph *posRatio = new TGraph (n, posRatioArr[1], posRatioArr[0]);
	posRatio->SetMarkerStyle(21);
	posRatio->SetMarkerColor(4);
	posRatio->SetTitle("Position");

	TExec *exPos = new TExec("exPos","drawtext();");
	posRatio->GetListOfFunctions()->Add(exPos);
	posRatio->Draw("AP");

	Canvas->Print( "Out/MCRatio.pdf(","pdf");

	TGraph *enRatio = new TGraph (n, enRatioArr[1], enRatioArr[0]);
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
      l = new TLatex(x,y+0.01,Form("{%1.1f, %1.1f}",luTable[i][1], luTable[i][2]));
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
		fprintf(fpo,"%1.1f %1.1f %4.4f %4.4f\n",luTable[j][1], luTable[j][2], binPosRatio[j][0], binPosRatio[j][1]);
		fprintf(fen,"%1.1f %1.1f %4.4f %4.4f\n",luTable[j][1], luTable[j][2], binEnRatio[j][0], binEnRatio[j][1]);
	}
	fclose(fpo);
	fclose(fen);
}
