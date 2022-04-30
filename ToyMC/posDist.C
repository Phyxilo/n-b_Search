#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <string>

using namespace std;

bool Graph = false;

float valSet [49][2] =
{
	{2.0,0.4},
	{2.0,0.6},
	{2.0,0.8},
	{2.0,1.0},
	{2.0,1.2},
	{2.0,1.4},
	{2.0,1.6},
	{2.5,0.4},
	{2.5,0.6},
	{2.5,0.8},
	{2.5,1.0},
	{2.5,1.2},
	{2.5,1.4},
	{2.5,1.6},
	{3.0,0.4},
	{3.0,0.6},
	{3.0,0.8},
	{3.0,1.0},
	{3.0,1.2},
	{3.0,1.4},
	{3.0,1.6},
	{3.5,0.4},
	{3.5,0.6},
	{3.5,0.8},
	{3.5,1.0},
	{3.5,1.2},
	{3.5,1.4},
	{3.5,1.6},
	{4.0,0.4},
	{4.0,0.6},
	{4.0,0.8},
	{4.0,1.0},
	{4.0,1.2},
	{4.0,1.4},
	{4.0,1.6},
	{4.5,0.4},
	{4.5,0.6},
	{4.5,0.8},
	{4.5,1.0},
	{4.5,1.2},
	{4.5,1.4},
	{4.5,1.6},
	{5.0,0.4},
	{5.0,0.6},
	{5.0,0.8},
	{5.0,1.0},
	{5.0,1.2},
	{5.0,1.4},
	{5.0,1.6},
};
/*
float valSet [25][2] =
{
	{2.0,2.0},
	{2.0,3.5},
	{2.0,5.0},
	{2.0,6.5},
	{2.0,8.0},
	{3.5,2.0},
	{3.5,3.5},
	{3.5,5.0},
	{3.5,6.5},
	{3.5,8.0},
	{5.0,2.0},
	{5.0,3.5},
	{5.0,5.0},
	{5.0,6.5},
	{5.0,8.0},
	{6.5,2.0},
	{6.5,3.5},
	{6.5,5.0},
	{6.5,6.5},
	{6.5,8.0},
	{8.0,2.0},
	{8.0,3.5},
	{8.0,5.0},
	{8.0,6.5},
	{8.0,8.0},
};
*/
double binPosRatio[2][49];
double binEnRatio[2][49];

int eventNum = 0;
int arrSize = 0;

class BoxHist {
public:
	double Xmin, Xmax, Ymin, Ymax;
	double DX,DY,X0,Y0,x0,x1,y0,y1;
	vector <double> Xmin_v;
	vector <double> Xmax_v;
	vector <double> Ymin_v;
	vector <double> Ymax_v;
	int  Nbin=0;
	int ID;

// initialization *** area setting ***
	int Init(double xmin, double xmax, double ymin, double ymax, int nbin) {
		Xmin = xmin;
		Xmax = xmax;
		Ymin = ymin;
		Ymax = ymax;
		DX   = 0.5 * (Xmax-Xmin);
		DY   = 0.5 * (Ymax-Ymin);
		X0   = 0.5 * (Xmin+Xmax);
		Y0   = 0.5 * (Ymin+Ymax);

		Nbin = nbin;
		if (nbin<=0) return(-1);

		Xmin_v.clear();
		Xmax_v.clear();
		Ymin_v.clear();
		Ymax_v.clear();
		for (int i=0;i<nbin;i++) {
			x0= X0 -DX * sqrt((double)(i+1)/(double)nbin);
			y0= Y0 -DY * sqrt((double)(i+1)/(double)nbin);
			x1= X0 +DX * sqrt((double)(i+1)/(double)nbin);
			y1= Y0 +DY * sqrt((double)(i+1)/(double)nbin);

			Xmin_v.push_back(x0);
			Ymin_v.push_back(y0);
			Xmax_v.push_back(x1);
			Ymax_v.push_back(y1);
		}
		return(Nbin);
	};

// get AreaId for (x,y)
	int GetAreaID(double x, double y) {
		if (Nbin==0) return(-2);
		if (Xmin_v.size()==0) return(-2);
		int IN=0;
		for (int i=0;i<Nbin;i++) {
			if (x>=Xmin_v[i] && x<=Xmax_v[i] && y>=Ymin_v[i] && y<=Ymax_v[i]) {
				IN = 1;
				ID = i;
				break;
			}
		}
		if (IN==1) return(ID);
		if (IN==0) return(-1);
		else {return 0;}
	};
};

void dataOut(int n);

void posDist(int Dist)
{
	Float_t unitMult = 1;

	Float_t neuPosX;
	Float_t neuPosY;
	Float_t neuPosZ;
	Float_t neuEn;
	Float_t intPosX;
	Float_t intPosY;
	Float_t intPosZ;

	Float_t neuNormPosX;
	Float_t neuNormPosY;
	Float_t neuNormPosZ;

	int idArea;

	arrSize = sizeof(valSet)/sizeof(valSet[0]);

	char  p_name[64];

	TCanvas *Canvas= new TCanvas("Canvas","Graph Canvas",20,20,1920*4,1080*4);
	Canvas->SetWindowSize(1920*4, 1080*4);
	Canvas->SetCanvasSize(192*6, 108*6);
	Canvas->SetGrid();

	Canvas->Print( "posDist.pdf[","pdf");

	for (int i = 0; i < arrSize; i++ )
	{
		eventNum = 0;

		double posArea0 = 0, posArea1 = 0, posArea2 = 0;
		double enArea0 = 0, enArea1 = 0, enArea2 = 0;

		char title[64];
		sprintf( title,"[n = %.1f, b = %.1f] T_X vs T_Y (@ %.1f m.)", valSet[i][0], valSet[i][1], (float) (Dist));

		TH2F *posHist  = new TH2F("Histogram",title,100,-0.009826,0.009826,100,-0.009826,0.009826);
		//TH2F *posHist = new TH2F("Histogram",title,100,-0.4,0.4,100,-0.4,0.4);

		sprintf( p_name,"Input/%.1f_%.1f_NeutDir.dat", valSet[i][0], valSet[i][1]);

		ifstream inputTauDirFile(p_name);

		TGraph *posNeuGraph = new TGraph ( 1000 );
		//TGraph *posNeuGraphInt = new TGraph ( 1000 );

		double theta_box1  = 0.4/Dist;					// outer  40cm/40.71m
		double theta_box0  = -1. * theta_box1;
		double theta_box11 = sqrt(2./3.)* theta_box1; 		// 2nd inner
		double theta_box10 = -1. * theta_box11;
		double theta_box21 = sqrt(1./3.)* theta_box1;			// most inner
		double theta_box20 = -1. * theta_box21;

		BoxHist *BH = new BoxHist;
		BH->Init(theta_box0,theta_box1,theta_box0,theta_box1,3);

		string line;
		int x = 0;

		while (getline(inputTauDirFile, line))
		{
			neuPosX = unitMult*stof(line.substr(0, line.find(" ")))/1000;
			neuPosY = unitMult*stof(line.substr(13, line.find(" ")))/1000;
			neuPosZ = unitMult*stof(line.substr(26, line.find(" ")))/1000;
			neuEn = unitMult*stof(line.substr(39, line.find(" ")));
			intPosX = unitMult*stof(line.substr(52, line.find(" ")))/1000;
			intPosY = unitMult*stof(line.substr(65, line.find(" ")))/1000;
			intPosZ = unitMult*stof(line.substr(78, line.find(" ")))/1000;

			if (neuPosZ != 0)
			{

				if (Dist > intPosZ)
				{
					neuNormPosX = ((neuPosX / neuPosZ) * (Dist-intPosZ) + intPosX)/Dist;
					neuNormPosY = ((neuPosY / neuPosZ) * (Dist-intPosZ) + intPosY)/Dist;

					eventNum++;
				}

				//neuNormPosZ = ( neuPosZ / neuPosZ ) * Dist;
				/*
				if (Dist > intPosZ)
				{
					neuNormPosX = (neuPosX / neuPosZ);
					neuNormPosY = (neuPosY / neuPosZ);

					eventNum++;
				}
				*/
				if (neuNormPosX <= 0.4 && neuNormPosX >= -0.4 && neuNormPosY <= 0.4 && neuNormPosY >= -0.4)
				{
					if (Graph)
					{
						//posNeuGraphInt->SetPoint(x, neuPosX, neuPosY);
						posNeuGraph->SetPoint(x, neuNormPosX, neuNormPosY); //TGRAPH
					}


					posHist->Fill(neuNormPosX, neuNormPosY);

					idArea = BH->GetAreaID(neuNormPosX, neuNormPosY);
					if (idArea == 0){ posArea0++; enArea0 += neuEn;}
					else if (idArea == 1){posArea1++; enArea1 += neuEn;}
					else if (idArea == 2){posArea2++; enArea2 += neuEn;}

				}

				x++;
			}
		}

		binPosRatio[0][i] = posArea1/posArea0;
		binPosRatio[1][i] = posArea2/posArea0;

		binEnRatio[0][i] = enArea1/enArea0;
		binEnRatio[1][i] = enArea2/enArea0;

		cout << "n = " << valSet[i][0] << ", b = " << valSet[i][1] << endl;
		cout << binPosRatio[0][i] << ", " << binPosRatio[1][i] << endl;

		if (Graph)
		{
			char title[64];
			sprintf( title,"[n = %.1f, b = %.1f] T_X vs T_Y (@ %.1f m.)", valSet[i][0], valSet[i][1], (float) (Dist));

			posNeuGraph->Draw("AP");
			posNeuGraph->SetTitle(title);

			char mean[64], rms[64], evntNm[64];
			sprintf( mean, "Mean  X: %.4f, Y: %.4f", posNeuGraph->GetMean(1), posNeuGraph->GetMean(2));
			sprintf( rms, "RMS  X: %.4f, Y: %.4f", posNeuGraph->GetRMS(1), posNeuGraph->GetRMS(2));
			sprintf( evntNm, "Event Number: %d", eventNum);

			TLegend *Leg = new TLegend (0.1, 0.8, 0.32, 0.9);
			Leg->AddEntry((TObject*)0, mean, "");
			Leg->AddEntry((TObject*)0, rms, "");
			Leg->AddEntry((TObject*)0, evntNm, "");
			Leg->Draw();

			Canvas->Print( "posDist.pdf","pdf");

			cout << mean << endl;
			cout << rms << endl;
		}

		if (!Graph)
		{
			posHist->SetMinimum(0.0);
			posHist->SetFillColor(5);
			posHist->Draw("coltz");

			Canvas->Print( "posDist.pdf","pdf");

			posHist->Delete();
		}

		//posNeuGraphInt->Draw("AP");
		//posNeuGraphInt->SetTitle("Delta X vs Delta Y (@ Vertex)");
		//Canvas->Print( "posDist.pdf)","pdf");

		inputTauDirFile.close();
	}
	Canvas->Print( "posDist.pdf]","pdf");

	dataOut(arrSize);
}

void dataOut(int n)
{
	FILE *fpo, *fen;

	fpo = fopen("/home/phyxilo/Out/posPythia.dat","wt");
	fen = fopen("/home/phyxilo/Out/enPythia.dat","wt");

	for (int j = 0; j < n; j++)
	{
		fprintf(fpo,"%1.1f %1.1f %4.8f %4.8f\n",valSet[j][0], valSet[j][1], binPosRatio[0][j], binPosRatio[1][j]);
		fprintf(fen,"%1.1f %1.1f %4.8f %4.8f\n",valSet[j][0], valSet[j][1], binEnRatio[0][j], binEnRatio[1][j]);
	}

	fclose(fpo);
	fclose(fen);
}
