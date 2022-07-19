#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <string>

using namespace std;

Double_t customFit (Double_t *x, Double_t *par);

/*
float valSet [49][2] =
{
	{2.0,0.4}, {2.0,0.6}, {2.0,0.8}, {2.0,1.0}, {2.0,1.2}, {2.0,1.4}, {2.0,1.6},
	{2.5,0.4}, {2.5,0.6}, {2.5,0.8}, {2.5,1.0}, {2.5,1.2}, {2.5,1.4}, {2.5,1.6},
	{3.0,0.4}, {3.0,0.6}, {3.0,0.8}, {3.0,1.0}, {3.0,1.2}, {3.0,1.4}, {3.0,1.6},
	{3.5,0.4}, {3.5,0.6}, {3.5,0.8}, {3.5,1.0}, {3.5,1.2}, {3.5,1.4}, {3.5,1.6},
	{4.0,0.4}, {4.0,0.6}, {4.0,0.8}, {4.0,1.0}, {4.0,1.2}, {4.0,1.4}, {4.0,1.6},
	{4.5,0.4}, {4.5,0.6}, {4.5,0.8}, {4.5,1.0}, {4.5,1.2}, {4.5,1.4}, {4.5,1.6},
	{5.0,0.4}, {5.0,0.6}, {5.0,0.8}, {5.0,1.0}, {5.0,1.2}, {5.0,1.4}, {5.0,1.6}
};
*/
/*
float valSet [25][2] =
{
	{2.0,2.0}, {2.0,3.5}, {2.0,5.0}, {2.0,6.5}, {2.0,8.0},
	{3.5,2.0}, {3.5,3.5}, {3.5,5.0}, {3.5,6.5}, {3.5,8.0},
	{5.0,2.0}, {5.0,3.5}, {5.0,5.0}, {5.0,6.5}, {5.0,8.0},
	{6.5,2.0}, {6.5,3.5}, {6.5,5.0}, {6.5,6.5}, {6.5,8.0},
	{8.0,2.0}, {8.0,3.5}, {8.0,5.0}, {8.0,6.5}, {8.0,8.0}
};
*/

float valSet [81][2] =
{
	{0.2,0.2}, {0.2,0.4}, {0.2,0.6}, {0.2,0.8}, {0.2,1.0}, {0.2,1.2}, {0.2,1.4}, {0.2,1.6}, {0.2,1.8},
	{0.4,0.2}, {0.4,0.4}, {0.4,0.6}, {0.4,0.8}, {0.4,1.0}, {0.4,1.2}, {0.4,1.4}, {0.4,1.6}, {0.4,1.8},
	{0.6,0.2}, {0.6,0.4}, {0.6,0.6}, {0.6,0.8}, {0.6,1.0}, {0.6,1.2}, {0.6,1.4}, {0.6,1.6}, {0.6,1.8},
	{0.8,0.2}, {0.8,0.4}, {0.8,0.6}, {0.8,0.8}, {0.8,1.0}, {0.8,1.2}, {0.8,1.4}, {0.8,1.6}, {0.8,1.8},
	{1.0,0.2}, {1.0,0.4}, {1.0,0.6}, {1.0,0.8}, {1.0,1.0}, {1.0,1.2}, {1.0,1.4}, {1.0,1.6}, {1.0,1.8},
	{1.2,0.2}, {1.2,0.4}, {1.2,0.6}, {1.2,0.8}, {1.2,1.0}, {1.2,1.2}, {1.2,1.4}, {1.2,1.6}, {1.2,1.8},
	{1.4,0.2}, {1.4,0.4}, {1.4,0.6}, {1.4,0.8}, {1.4,1.0}, {1.4,1.2}, {1.4,1.4}, {1.4,1.6}, {1.4,1.8},
	{1.6,0.2}, {1.6,0.4}, {1.6,0.6}, {1.6,0.8}, {1.6,1.0}, {1.6,1.2}, {1.6,1.4}, {1.6,1.6}, {1.6,1.8},
	{1.8,0.2}, {1.8,0.4}, {1.8,0.6}, {1.8,0.8}, {1.8,1.0}, {1.8,1.2}, {1.8,1.4}, {1.8,1.6}, {1.8,1.8}
};

vector<double> nVec;
vector<double> nPythiaVec;
double nArr[9];
double nPythiaArr[9];
float nPar = 0;
int arrSize = 0, nSize = 0;

void xfDist()
{
	Float_t neuDirX;
	Float_t neuDirY;
	Float_t neuDirZ;
	Float_t neuEn;

	arrSize = sizeof(valSet)/sizeof(valSet[0]);

	char  p_name[64];

	TCanvas *Canvas= new TCanvas("Canvas","Graph Canvas",20,20,1920*4,1080*4);

	Canvas->SetWindowSize(1920*4, 1080*4);
	Canvas->SetCanvasSize(192*6, 108*6);
	Canvas->SetGrid();
	Canvas->Print( "xfDist.pdf[","pdf");

	for (int i = 0; i < arrSize; i++ )
	{
		if (nPar != valSet[i][0])
		{
			nPar = valSet[i][0];
			nVec.push_back(nPar);
			nSize = 1;
		}
		else
		{
			nSize++;
		}
	}

	float nTot = 0;

	for (int i = 0; i < arrSize; i++ )
	{
		char title[64];
		sprintf( title,"[n = %.1f, b = %.1f] T_X vs T_Y", valSet[i][0], valSet[i][1]);

		TH1F *xfDistHist = new TH1F("Histogram", title, 30, -1, 1);

		TF1 *func = new TF1 ("cFit", customFit, -1, 1, 2);

		sprintf( p_name,"Input/%.1f_%.1f_NeutDir.dat", valSet[i][0], valSet[i][1]);

		ifstream inputTauDirFile(p_name);
		
		string line;
		int x = 0;

		while (getline(inputTauDirFile, line))
		{
			neuDirX = stof(line.substr(0, line.find(" ")));
			neuDirY = stof(line.substr(13, line.find(" ")));
			neuDirZ = stof(line.substr(26, line.find(" ")));
			neuEn = stof(line.substr(39, line.find(" ")));

			//cout << 2*neuDirZ/400 << endl;

			double xf = neuDirZ/400;

			if(xf < 1 && xf > -1)
			{
				xfDistHist->Fill(xf);
				xfDistHist->Fill(-xf);
			}
		}

		cout << "n = " << valSet[i][0] << ", b = " << valSet[i][1] << endl;
		func->SetParameter(0, 8);
		func->SetParameter(1, 350);
		//func->SetParNames("n");
		func->SetParLimits(0, 3, 8);

		Canvas->Print( "xfDist.pdf","pdf");

		ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(100);

		xfDistHist->SetXTitle("XF");
		xfDistHist->SetYTitle("Entries");
		//xfDistHist->Scale(1/xfDistHist->GetEntries());
		//xfDistHist->DrawClone();
		xfDistHist->Fit("cFit");
		xfDistHist->Draw();
		//func->Draw();

		string par0Entry = "Parameter 0 = ";
		string par1Entry = "Parameter 1 = ";

		TF1 *fitFunc = (TF1*)xfDistHist->GetListOfFunctions()->FindObject("cFit");
		double n = fitFunc->GetParameter(0);
		//cout << n << endl;

		//nArr[i] = n;
		//nPythiaArr[i] = valSet[i][0];

		for (int j = 0; j < nVec.size(); j++)
		{
			if (valSet[i][0] == nVec[j])
			{
				nTot += n;
			}

		}

		if ((i+1)%nSize == 0 && i != 0)
		{
			nPythiaVec.push_back(nTot/nSize);
			nTot = 0;
		}

		copy(nVec.begin(), nVec.end(), nArr);
		copy(nPythiaVec.begin(), nPythiaVec.end(), nPythiaArr);

		par0Entry += to_string(n);
		par1Entry += to_string(n);

		TLegend *posLeg = new TLegend (0.1, 0.7, 0.40, 0.9);
		posLeg->SetHeader("Fit Parameters", "C");
		posLeg->AddEntry((TObject*)0, par0Entry.c_str(), "");
		posLeg->AddEntry((TObject*)0, par1Entry.c_str(), "");
		posLeg->Draw();

		inputTauDirFile.close();
	}
	Canvas->Print( "xfDist.pdf]","pdf");

	TCanvas *nCanvas= new TCanvas("Canvas","Graph Canvas",20,20,1920*4,1080*4);
	TGraph *nComp = new TGraph (9, nArr, nPythiaArr);

	//nComp->SetMarkerStyle(20);
	//nComp->SetMarkerColor(1);
	nComp->Fit("pol1");
	nComp->SetTitle("Pythia vs Calculated n Parameters");
	nComp->Draw();

	nCanvas->Print( "NComp.pdf","pdf");
}

Double_t customFit (Double_t *x, Double_t *par)
{
	Float_t xAbs = TMath::Abs(x[0]);
	//return par[0]*pow(x[0], par[1]);
	return par[1]*pow(1-xAbs, par[0]);
}