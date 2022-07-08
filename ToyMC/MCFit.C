#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

struct Cluster
{
	vector<vector<vector<float>>> elements;
	vector<vector<vector<float>>> distance;
	vector<vector<float>> midPoint;
	vector<vector<float>> distAvg;
	vector<float> disp;
};

void drawtext();
Double_t customFit (Double_t *x, Double_t *par);
float FittedM (float sqrtb, float n, float slope);
Cluster DataRead(string fName);

//vector<vector<vector<float>>> posClusterVec, enClusterVec;
Cluster posCluster, enCluster;
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

void MCFit(string posDat, string enDat, int Rank)
{
	//posCluster = DataRead("ClusterOutPos.txt");
	//enCluster = DataRead("ClusterOutEn.txt");

	posCluster = DataRead(posDat);
	enCluster = DataRead(enDat);

	/*
	for (int i = 0; i < posCluster.elements.size(); i++)
	{
		cout << "Elements" << endl;

		for (int j = 0; j < posCluster.elements[i].size(); j++)
		{
			for (int k = 0; k < posCluster.elements[i][j].size(); k++)
			{
				cout << posCluster.elements[i][j][k] << endl;
			}
		}

		cout << endl;
	}
	*/
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

	int posIn = Rank, enIn = Rank, posRank, enRank;

	/*
	cout << "Position Rank: ";
	cin << posIn;

	cout << "/nEnergy Rank: ";
	cin << enIn;
	*/

	posRank = posCluster.elements.size()-(posIn);
	enRank = enCluster.elements.size()-(enIn);

	posParsVec = posCluster.elements[posRank];
	enParsVec = enCluster.elements[enRank];

	const int posNum = posParsVec.size();
	const int enNum = enParsVec.size();

	double posBSqrt [posNum];
	double enBSqrt [posNum];
	double posN [enNum];
	double enN [enNum];

	double posErrArrX[posNum];
	double posErrArrY[posNum];
	double enErrArrX[enNum];
	double enErrArrY[enNum];

	for (int i = 0; i < posNum; i++ )
	{
		posBSqrt[i] = sqrt(posParsVec[i][1]);
		posN[i] = posParsVec[i][0];

		posErrArrX[i] = abs(posCluster.distance[posRank][i][0] - posCluster.distAvg[posRank][0]);
		posErrArrY[i] = sqrt(abs(posCluster.distance[posRank][i][1] - posCluster.distAvg[posRank][1]));

		//cout << "X: " << posCluster.distance[posRank][i][0] - posCluster.distAvg[posRank][0] << ", Y: " << posCluster.distance[posRank][i][1] - posCluster.distAvg[posRank][1] << endl;
	}

	for (int i = 0; i < enNum; i++ )
	{
		enBSqrt[i] = sqrt(enParsVec[i][1]);
		enN[i] = enParsVec[i][0];

		enErrArrX[i] = abs(enCluster.distance[enRank][i][0] - enCluster.distAvg[enRank][0]);
		enErrArrY[i] = sqrt(abs(enCluster.distance[enRank][i][1] - enCluster.distAvg[enRank][1]));
	}

	/*
	for (int i = 0; i < enCluster.distance[enRank].size(); i++)
	{
		cout << "X: " << enErrArrX[i] << ", Y: " << enErrArrY[i] << endl;
	}
	*/

	TCanvas *Canvas= new TCanvas("Canvas","Graph Canvas",20,20,1920,1080);
	Canvas->SetWindowSize(1920, 1080);
	Canvas->SetCanvasSize(192*6, 108*6);
	Canvas->SetGrid();

	TF1 *func = new TF1 ("customFit", "customFit", 2, 4, 2);

	TGraphErrors *pos = new TGraphErrors (posNum, posN, posBSqrt, posErrArrX, posErrArrY);
	//TGraph *pos = new TGraph (posNum, posN, posBSqrt);

	pos->SetMarkerStyle(20);
	pos->SetMarkerColor(1);
	pos->SetTitle("Position");
	pos->DrawClone("AP");
	pos->Fit("customFit");

	pos->Draw("AP");

	string posPar0Entry = "Parameter 0 = ";
	string posPar1Entry = "Parameter 1 = ";
	string posChi2Entry = "Chisquare = ";

	TF1 *posFunc = pos->GetFunction("customFit");

	posPar0Entry += to_string(posFunc->GetParameter(0));
	posPar1Entry += to_string(posFunc->GetParameter(1));
	posChi2Entry += to_string(posFunc->GetChisquare());

	cout << posFunc->GetChisquare() << endl;

	cout << posPar0Entry << endl;
	cout << posPar1Entry << endl;
	cout << posChi2Entry << endl;

	TLegend *posLeg = new TLegend (0.1, 0.7, 0.48, 0.9);
	posLeg->SetHeader("Fit Parameters", "C");
	posLeg->AddEntry((TObject*)0, posPar0Entry.c_str(), "");
	posLeg->AddEntry((TObject*)0, posPar1Entry.c_str(), "");
	posLeg->AddEntry((TObject*)0, posChi2Entry.c_str(), "");
	posLeg->Draw();

	Canvas->Print( "ClusterFit.pdf(","pdf");
	
	TGraphErrors *en = new TGraphErrors (enNum, enN, enBSqrt, enErrArrX, enErrArrY);
	//TGraph *en = new TGraph (enNum, enN, enBSqrt);

	en->SetMarkerStyle(20);
	en->SetMarkerColor(1);
	en->SetTitle("Energy");
	en->DrawClone("AP");
	en->Fit("customFit");

	en->Draw("AP");

	string enPar0Entry = "Parameter 0 = ";
	string enPar1Entry = "Parameter 1 = ";
	string enChi2Entry = "Chisquare = ";

	TF1 *enFunc = en->GetFunction("customFit");

	enPar0Entry += to_string(enFunc->GetParameter(0));
	enPar1Entry += to_string(enFunc->GetParameter(1));
	enChi2Entry += to_string(enFunc->GetChisquare());

	cout << enPar0Entry << endl;
	cout << enPar1Entry << endl;
	cout << enChi2Entry << endl;

	TLegend *enLeg = new TLegend (0.1, 0.7, 0.48, 0.90);
	enLeg->SetHeader("Fit Parameters", "C");
	enLeg->AddEntry((TObject*)0, enPar0Entry.c_str(), "");
	enLeg->AddEntry((TObject*)0, enPar1Entry.c_str(), "");
	enLeg->AddEntry((TObject*)0, enChi2Entry.c_str(), "");
	enLeg->Draw();

	Canvas->Print( "ClusterFit.pdf","pdf");


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

	TLegend *combinedLeg = new TLegend (0.1, 0.75, 0.3, 0.9);
	combinedLeg->AddEntry(pos, "Position" , "p");
	combinedLeg->AddEntry(en, "Energy" , "p");
	combinedLeg->Draw();

	Canvas->Print( "ClusterFit.pdf)","pdf");
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

Cluster DataRead(string fName)
{
	Cluster clst;
	vector<vector<float>> Vec;
	vector<float> subVec;

	fstream f;
	f.open(fName);

	if (f.is_open())
	{
		string line;

		bool isElement = false, isDistance = false;
 		
		while(getline(f, line))
		{
			
			if (line == "*********Cluster Elements*********"){isElement = true;}
			if (line == "*********Cluster Properties*********"){isElement = false; clst.elements.push_back(Vec); Vec.clear();}

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
			
			if (line == "----------Distance Values----------"){isDistance = true;}
			if (line == "----------------------------------"){isDistance = false; clst.distance.push_back(Vec); Vec.clear();}

			if (isDistance && line.size() > 1 && line.find(",") != string::npos)
			{	
				line.erase(line.begin(), line.begin()+4);
				line.erase(line.end()-1, line.end());

				char lineArr[line.length()+1];
				strcpy(lineArr, line.c_str());

				char *token;
				token = strtok(lineArr, ",");

				while (token != NULL)
				{
					subVec.push_back(stof(token));
					token = strtok(NULL, ", ");
					//cout << token << endl;
				}
				Vec.push_back(subVec);
				subVec.clear();
			}
			
			string wgStr = "Dispersion: ";
			string dsStr = "Average: ";
			
			if (line.find(wgStr) != string::npos) { line.erase(line.begin(), line.begin() + wgStr.size()); clst.disp.push_back(stof(line));}
			/*
			if (line.find(dsStr) != string::npos) 
			{
				line.erase(line.begin(), line.begin() + dsStr.size());
				line.erase(line.begin(), line.begin() + 1);
				line.erase(line.end() - 1, line.end());

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

				clst.distance.push_back(subVec);
				subVec.clear();
			}
			*/

			if (line.find(dsStr) != string::npos) 
			{
				line.erase(line.begin(), line.begin() + dsStr.size());
				line.erase(line.begin(), line.begin() + 1);
				line.erase(line.end() - 1, line.end());

				char lineArr[line.length()+1];
				strcpy(lineArr, line.c_str());

				char *token;
				token = strtok(lineArr, ",");

				while (token != NULL)
				{
					//cout << cnt << ". " << token << endl;

					subVec.push_back(stof(token));
					token = strtok(NULL, ", ");
				}

				clst.distAvg.push_back(subVec);
				subVec.clear();
			}
   	}
   }
	f.close();

	return clst;
}