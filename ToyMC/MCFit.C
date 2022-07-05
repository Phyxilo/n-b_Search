#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

void drawtext();
Double_t customFit (Double_t *x, Double_t *par);
float FittedM (float sqrtb, float n, float slope);
vector<vector<vector<float>>> DataRead(string fName);

vector<vector<vector<float>>> posClusterVec, enClusterVec;
vector<vector<float>> posParsVec, enParsVec;
vector<float> posBSqrt, enBSqrt, posN, enN;

/*
float posPars [4][2] =
{
	{3.0, 0.6},
	{4.0, 1.0},
	{3.5, 0.8},
	{4.5, 1.4}
};

float posPars [4][2] =
{
	{3.0, 0.8},
	{2.5, 0.6},
	{3.5, 1.0},
	{4.0, 1.4}
};

float enPars [5][2] =
{
	{2.5, 0.6},
	{3.0, 0.8},
	{3.5, 1.0},
	{4.0, 1.2},
	{4.5, 1.6}
};

float enPars [4][2] =
{
	{5.0, 1.6},
	{4.0, 1.0},
	{3.5, 0.8},
	{4.5, 1.2}
};
*/

void MCFit()
{
	posClusterVec = DataRead("ClusterOutPos.txt");
	enClusterVec = DataRead("ClusterOutEn.txt");

	/*
	for (int x = 0; x < posClusterVec.size(); x++)
	{
		for (int y = 0; y < posClusterVec[x].size(); y++)
		{
			for (int z = 0; z < posClusterVec[x][y].size(); z++)
			{
				cout << posClusterVec[x][y][z] << endl;
			}
		}
	}
	*/

	int posIn = 1, enIn = 1, posRank, enRank;

	/*
	cout << "Position Rank: ";
	cin << posIn;

	cout << "/nEnergy Rank: ";
	cin << enIn;
	*/

	posRank = posClusterVec.size()-(posIn);
	enRank = enClusterVec.size()-(enIn);

	posParsVec = posClusterVec[posRank];
	enParsVec = enClusterVec[enRank];

	const int posNum = posParsVec.size();
	const int enNum = enParsVec.size();

	float posBSqrt [posNum];
	float enBSqrt [posNum];
	float posN [enNum];
	float enN [enNum];

	float errArrX[4] = {0.1, 0.4, 0.2, 0.3};
	float errArrY[4] = {0.1, 0.1, 0.1, 0.1};

	for (int i = 0; i < posNum; i++ )
	{
		posBSqrt[i] = sqrt(posParsVec[i][1]);
		posN[i] = posParsVec[i][0];
	}

	for (int i = 0; i < enNum; i++ )
	{
		enBSqrt[i] = sqrt(enParsVec[i][1]);
		enN[i] = enParsVec[i][0];
	}

	TCanvas *Canvas= new TCanvas("Canvas","Graph Canvas",20,20,1920,1080);
	Canvas->SetWindowSize(1920, 1080);
	Canvas->SetCanvasSize(192*6, 108*6);
	Canvas->SetGrid();

	TF1 *func = new TF1 ("customFit", "customFit", 2, 4, 2);

	//TGraph *pos = new TGraphErrors (posNum, posN, posBSqrt, errArrX, errArrY);
	TGraph *pos = new TGraph (posNum, posN, posBSqrt);

	pos->SetMarkerStyle(20);
	pos->SetMarkerColor(1);
	pos->SetTitle("Position");
	pos->DrawClone("AP");
	pos->Fit("customFit");

	pos->Draw("AP");

	string posPar0Entry = "Parameter 0 = ";
	string posPar1Entry = "Parameter 1 = ";

	posPar0Entry += to_string(pos->GetFunction("customFit")->GetParameter(0));
	posPar1Entry += to_string(pos->GetFunction("customFit")->GetParameter(1));

	cout << posPar0Entry << endl;
	cout << posPar1Entry << endl;

	TLegend *posLeg = new TLegend (0.1, 0.7, 0.48, 0.9);
	posLeg->SetHeader("Fit Parameters", "C");
	posLeg->AddEntry((TObject*)0, posPar0Entry.c_str(), "");
	posLeg->AddEntry((TObject*)0, posPar1Entry.c_str(), "");
	posLeg->Draw();

	Canvas->Print( "5E7Fit.pdf(","pdf");

	//TGraph *en = new TGraphErrors (enNum, enN, enBSqrt, errArrX, errArrY);
	TGraph *en = new TGraph (enNum, enN, enBSqrt);

	en->SetMarkerStyle(20);
	en->SetMarkerColor(1);
	en->SetTitle("Energy");
	en->DrawClone("AP");
	en->Fit("customFit");

	en->Draw("AP");

	string enPar0Entry = "Parameter 0 = ";
	string enPar1Entry = "Parameter 1 = ";

	enPar0Entry += to_string(en->GetFunction("customFit")->GetParameter(0));
	enPar1Entry += to_string(en->GetFunction("customFit")->GetParameter(1));

	cout << enPar0Entry << endl;
	cout << enPar1Entry << endl;

	TLegend *enLeg = new TLegend (0.1, 0.7, 0.48, 0.9);
	enLeg->SetHeader("Fit Parameters", "C");
	enLeg->AddEntry((TObject*)0, enPar0Entry.c_str(), "");
	enLeg->AddEntry((TObject*)0, enPar1Entry.c_str(), "");
	enLeg->Draw();

	Canvas->Print( "5E7Fit.pdf","pdf");


	TMultiGraph *mGraph = new TMultiGraph ();

	pos->GetListOfFunctions()->Remove(pos->GetFunction("customFit"));
	en->GetListOfFunctions()->Remove(en->GetFunction("customFit"));

	pos->SetMarkerStyle(20);
	en->SetMarkerStyle(20);

	pos->SetMarkerColorAlpha(kRed, 0.6);
	en->SetMarkerColorAlpha(kBlue, 0.6);

	mGraph->Add(pos);
	mGraph->Add(en);
	mGraph->SetTitle("Combined");
	mGraph->Draw("AP");

	TLegend *combinedLeg = new TLegend (0.1, 0.7, 0.48, 0.9);
	combinedLeg->AddEntry(pos, "Position" , "p");
	combinedLeg->AddEntry(en, "Energy" , "p");
	combinedLeg->Draw();

	Canvas->Print( "5E7Fit.pdf)","pdf");
}

float FittedM (float sqrtb, float n, float slope)
{
	return log(sqrtb/slope) / log(n);
}

Double_t customFit (Double_t *x, Double_t *par)
{
	//Double_t arg = 0;
	return par[0]*pow(x[0], par[1]);

}

vector<vector<vector<float>>> DataRead(string fName)
{
	vector<vector<vector<float>>> clsVec;
	vector<vector<float>> Vec;
	vector<float> subVec;

	fstream f;
	f.open(fName);

	if (f.is_open())
	{
		string line;

		bool isElement = false;
 		
		while(getline(f, line))
		{
			
			if (line == "*********Cluster Elements*********"){isElement = true;}
			if (line == "*********Cluster Properties*********"){isElement = false; clsVec.push_back(Vec); Vec.clear();}

			if (isElement && line.size() > 1 && line.find(",") != string::npos)
			{
				line.erase(line.begin(), line.begin()+4);
				line.erase(line.end()-1, line.end());

				char lineArr[line.length()+1];
				strcpy(lineArr, line.c_str());

				char *token;
				token = strtok(lineArr, ",");

				int cnt = 0;

				while (token != NULL)
				{
					//cout << cnt << ". " << token << endl;

					subVec.push_back(stof(token));
					token = strtok(NULL, ", ");

					cnt++;
				}
				Vec.push_back(subVec);
				subVec.clear();
			}

			//cout << line << "\n";
			
		}
		
   }
	f.close();

	return clsVec;
}