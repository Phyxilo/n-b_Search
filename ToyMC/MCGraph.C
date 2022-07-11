#include <iostream>
#include <vector>
#include "TCanvas.h"

int num = 0;
int numSec = 0;

float nPar,bPar,posX,posY;

vector<vector<float>> posVec, enVec, depPar;

void drawtext();

void MCGraph(string posDat, string enDat)
{
	char posDatArr[1024], enDatArr[1024], buf[1024];

	strcpy(posDatArr, posDat.c_str());
	strcpy(enDatArr, enDat.c_str());

	FILE *fPos;
	//fPos = fopen("/home/phyxilo/Out/posPythia.dat","rt");
	//fPos = fopen("5E7/pos.dat","rt");
	fPos = fopen(posDatArr, "rt");

	FILE *fEn;
	//fEn = fopen("/home/phyxilo/Out/enPythia.dat","rt");
	//fEn = fopen("5E7/en.dat","rt");
	fEn = fopen(enDatArr, "rt");

	TCanvas *Canvas= new TCanvas("Canvas","Graph Canvas",20,20,1920*4,1080*4);
	Canvas->SetWindowSize(1920*4, 1080*4);
	Canvas->SetCanvasSize(192*6, 108*6);
	Canvas->SetGrid();

	TMultiGraph *mGraph = new TMultiGraph ();
	//TExec *exPos = new TExec("exPos","drawtext();");

	TTree *posOut = new TTree ("pOut", "Position Ratio");
	TTree *enOut = new TTree ("eOut", "Energy Ratio");

	//TH2F *PosH2D = new TH2F("h_th_eex","#theta .vs. E_ex of Ds",50,0,1,50,0,1);

	int x = 0;

	vector<float> subVec;

	//cout << "---------------------- Position ----------------------" << endl;

	while (fgets(buf, 1024, fPos))
	{
		sscanf(buf,"%f %f %f %f",&nPar,&bPar,&posX,&posY);

		if(posX != 0 || posY != 0)
		{
			//cout << nPar << ", " << bPar << ", " << posX << ", " << posY << endl;
			//PosH2D->Fill(posX, posY);

			subVec.push_back(posX); subVec.push_back(posY);
			posVec.push_back(subVec);
			subVec.clear();

			subVec.push_back(nPar); subVec.push_back(bPar);
			depPar.push_back(subVec);
			subVec.clear();
			
			//posVec[0][x] = posX; posVec[1][x] = posY;
			//depPar[0][x] = nPar; depPar[1][x] = bPar;

			x++;
		}
	}
	fclose(fPos);

	num = posVec.size();
	numSec = posVec[0].size();

	//cout << "---------------------- Energy ----------------------" << endl;

	x = 0;

	while (fgets(buf, 1024, fEn))
	{
		sscanf(buf,"%f %f %f %f",&nPar,&bPar,&posX,&posY);

		if(posX != 0 || posY != 0)
		{
			//cout << nPar << ", " << bPar << ", " << posX << ", " << posY << endl;
			//enVec[0][x] = posX; enVec[1][x] = posY;

			subVec.push_back(posX); subVec.push_back(posY);
			enVec.push_back(subVec);
			subVec.clear();

			x++;
		}
	}
	fclose(fEn);

	float posArr[numSec][num];
	float enArr[numSec][num];

	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < numSec; j++)
		{
			posArr[j][i] = posVec[i][j];
			enArr[j][i] = enVec[i][j];
		}
	}

	//PosH2D->Write("H2D");

	TGraph *posRatio = new TGraph (num, posArr[1], posArr[0]);
	posRatio->SetMarkerStyle(20);
	posRatio->SetMarkerColor(2);
	posRatio->SetTitle("Position");

	TGraph *enRatio = new TGraph (num, enArr[1], enArr[0]);
	enRatio->SetMarkerStyle(20);
	enRatio->SetMarkerColor(4);
	enRatio->SetTitle("Energy");

	mGraph->Add(posRatio);
	mGraph->Add(enRatio);
	mGraph->SetTitle("Combined");
	mGraph->Draw("AP");
	Canvas->Print( "Ratio.pdf(","pdf");

	TExec *exPos = new TExec("exPos","drawtext();");
	posRatio->GetListOfFunctions()->Add(exPos);
	posRatio->Draw("AP");
	Canvas->Print( "Ratio.pdf","pdf");

	TExec *exEn = new TExec("exEn","drawtext();");
	enRatio->GetListOfFunctions()->Add(exEn);
	enRatio->Draw("AP");
	Canvas->Print( "Ratio.pdf)","pdf");


}

void drawtext()
{
   Int_t i,n;
   Double_t x,y;
   TLatex *l;

   float depParArr[numSec][num];

   for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < numSec; j++)
		{
			depParArr[j][i] = depPar[i][j];
		}
	}

   TGraph *g = (TGraph*)gPad->GetListOfPrimitives()->FindObject("Graph");
   n = g->GetN();
   for (i=1; i<n; i++)
	 {
      g->GetPoint(i,x,y);
      l = new TLatex(x,y,Form("{%1.1f, %1.1f}", depParArr[0][i], depParArr[1][i]));
      l->SetTextSize(0.007);
      l->SetTextFont(42);
      l->SetTextAlign(21);
      l->Paint();
   }
}
