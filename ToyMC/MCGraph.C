#include <iostream>
#include "TCanvas.h"

float nPar,bPar,posX,posY;

float posVec [2][49];
float enVec [2][49];
float depPar [2][49];

void drawtext();

void MCGraph()
{
	int num = sizeof(posVec[0])/sizeof(posVec[0][0]);

	char buf[1024];
	FILE *fPos;
	//fPos = fopen("/home/phyxilo/Out/posPythia.dat","rt");
	fPos = fopen("5E7/pos.dat","rt");
	FILE *fEn;
	//fEn = fopen("/home/phyxilo/Out/enPythia.dat","rt");
	fEn = fopen("5E7/en.dat","rt");

	TCanvas *Canvas= new TCanvas("Canvas","Graph Canvas",20,20,1920*4,1080*4);
	Canvas->SetWindowSize(1920*4, 1080*4);
	Canvas->SetCanvasSize(192*6, 108*6);
	Canvas->SetGrid();

	TMultiGraph *mGraph = new TMultiGraph ();
	//TExec *exPos = new TExec("exPos","drawtext();");

	TFile f ("RatioPythia.root", "recreate");
	TTree *posOut = new TTree ("pOut", "Position Ratio");
	TTree *enOut = new TTree ("eOut", "Energy Ratio");

	TH2F *PosH2D = new TH2F("h_th_eex","#theta .vs. E_ex of Ds",50,0,1,50,0,1);

	int x = 0;
	int index = 0;

	cout << "---------------------- Position ----------------------" << endl;

	while (fgets(buf, 1024, fPos))
	{
		sscanf(buf,"%f %f %f %f",&nPar,&bPar,&posX,&posY);

		if(posX != 0 || posY != 0)
		{
			index = (int)((nPar - 2) * 2);

			cout << index << ", " << nPar << ", " << bPar << ", " << posX << ", " << posY << endl;
			//PosH2D->Fill(posX, posY);
			posVec[0][x] = posX; posVec[1][x] = posY;
			depPar[0][x] = nPar; depPar[1][x] = bPar;

			x++;
		}
	}
	fclose(fPos);

	cout << "---------------------- Energy ----------------------" << endl;

	x = 0;

	while (fgets(buf, 1024, fEn))
	{
		sscanf(buf,"%f %f %f %f",&nPar,&bPar,&posX,&posY);

		if(posX != 0 || posY != 0)
		{
			index = (int)((nPar - 2) * 2);

			cout << index << ", " << nPar << ", " << bPar << ", " << posX << ", " << posY << endl;
			enVec[0][x] = posX; enVec[1][x] = posY;

			x++;
		}
	}
	fclose(fEn);
	//f.cd();
	//mGraph->Write("PosGraph");
	PosH2D->Write("H2D");
	/*
	TGraph *posRatio = new TGraph (num, posVec[1], posVec[0]);
	posRatio->SetMarkerStyle(21);
	posRatio->SetMarkerColor(4);
	posRatio->SetTitle("Position");
	*/

	//posRatio->GetListOfFunctions()->Add(exPos);
	//mGraph->Draw("AP");

	//Canvas->Print( "Out/Test.pdf(","pdf");
	TGraph *posRatio = new TGraph (num, posVec[1], posVec[0]);
	posRatio->SetMarkerStyle(20);
	posRatio->SetMarkerColor(2);
	posRatio->SetTitle("Position");

	TGraph *enRatio = new TGraph (num, enVec[1], enVec[0]);
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

   TGraph *g = (TGraph*)gPad->GetListOfPrimitives()->FindObject("Graph");
   n = g->GetN();
   for (i=1; i<n; i++)
	 {
      g->GetPoint(i,x,y);
      l = new TLatex(x,y,Form("{%1.1f, %1.1f}",depPar[0][i], depPar[1][i]));
      l->SetTextSize(0.007);
      l->SetTextFont(42);
      l->SetTextAlign(21);
      l->Paint();
   }
}
