#include <iostream>
#include <vector>
#include <string>
#include "TCanvas.h"

using namespace std;

struct ClusterDistance
{
	vector<vector<float>> Value;
	vector<float> AverageValue;
	vector<float> Magnitude;
};

struct Cluster
{
	vector<vector<float>> elements;
	//vector<float> distance;
	ClusterDistance distance;

	float midPoint[2];
	float dispersion;
};

vector<vector<float>> Dat2Vec(FILE *datFile);

vector<Cluster> Vec2Cls (vector<vector<float>> Vec);
vector<Cluster> PairGen(vector<Cluster> cls, vector<vector<float>> vec);
vector<Cluster> RemoveDups(vector<Cluster> cls);

ClusterDistance DistCalc (vector<vector<float>> elm, float *midPt, vector<vector<float>> vec);
//vector<float> DistCalc (vector<vector<float>> elm, float *midPt, vector<vector<float>> vec);
float WeightCalc(vector<vector<float>> elm, float *midPt, vector<vector<float>> vec);
bool Comparison (Cluster priCls, Cluster secCls);

void ShowResults(vector<Cluster> cls);
vector<Cluster> clusterVec;
vector<vector<float>> flVec;

vector<string> allArgs;

void ClusterGen(string fileName, int clsSize)
{
	char fChar[1024];
	strcpy(fChar, fileName.c_str());

	FILE *file;
	//file = fopen("5E7/1/en.dat","rt");
	file = fopen(fChar,"rt");
	/*
	FILE *fPos;
	//fPos = fopen("/home/phyxilo/Out/posPythia.dat","rt");
	fPos = fopen("5E7/2/pos.dat","rt");

	FILE *fEn;
	//fEn = fopen("/home/phyxilo/Out/enPythia.dat","rt");
	//fEn = fopen("5E7/2/en.dat","rt");

	FILE *fComb;
	//fEn = fopen("/home/phyxilo/Out/enPythia.dat","rt");
	//fComb = fopen("5E7/1/combined.dat","rt");
	*/
	Cluster clst;

	flVec = Dat2Vec(file);
	clusterVec = Vec2Cls(flVec);

	//--------------------- Debuger ---------------------
	/*
	for (int x = 0; x < clusterVecPos.size(); x++)
	{
		for (int i = 0; i < clusterVecPos[x].elements.size(); i++)
		{
			cout << clusterVecPos[x].elements[i][0] << ", " << clusterVecPos[x].elements[i][1] << endl;
		}
		cout << "dispersion: " << clusterVecPos[x].dispersion << endl;
		cout << "Middle Point X: " << clusterVecPos[x].midPoint[0] << ", Middle Point Y: " << clusterVecPos[x].midPoint[1] << endl;
		cout << "--------------" << endl;
	}
	*/

	//int clsSize = 0;
	//cout << "Enter Cluster Size: "; cin >> clsSize;

	for (int i = 0; i < clsSize-1; i++)
	{
		clusterVec = PairGen(clusterVec, flVec);
	}

	sort(clusterVec.begin(), clusterVec.end(), Comparison);
	clusterVec = RemoveDups(clusterVec);
	ShowResults(clusterVec);
}

vector<vector<float>> Dat2Vec(FILE *datFile)
{
	vector<vector<float>> Vec;
	vector<float> subVec;

	char buf[1024];

	int x = 0;
	int index = 0;

	while (fgets(buf, 1024, datFile))
	{
		float nPar,bPar,posX,posY;

		sscanf(buf,"%f %f %f %f",&nPar,&bPar,&posX,&posY);

		if(posX != 0 || posY != 0)
		{
			index = (int)((nPar - 2) * 2);

			subVec.push_back(nPar); subVec.push_back(bPar); subVec.push_back(posX); subVec.push_back(posY);
			Vec.push_back(subVec);
			subVec.clear();

			x++;
		}
	}
	fclose(datFile);

	return Vec;
}

vector<Cluster> Vec2Cls (vector<vector<float>> Vec)
{
	vector<Cluster> clstVec;
	Cluster cls;
	vector<float> subVec;

	int vecSize = Vec.size();

	for (int x = 0; x < vecSize; x++)
	{
		subVec.push_back(Vec[x][0]); subVec.push_back(Vec[x][1]);

		cls.elements.push_back(subVec);
		cls.midPoint[0] = Vec[x][2]; cls.midPoint[1] = Vec[x][3];
		cls.dispersion = 0;

		clstVec.push_back(cls);

		subVec.clear(); cls.elements.clear();

		//--------------------- Debuger ---------------------
		/*
		cout << cls.elements[0][0] << ", " << cls.elements[0][1] << endl;
		cout << "dispersion: " << cls.dispersion << endl;
		cout << "Middle Point X: " << cls.midPoint[0] << ", Middle Point Y: " << cls.midPoint[1] << endl;
		cout << "--------------" << endl;
		*/
	}

	return clstVec;
}

vector<Cluster> PairGen(vector<Cluster> cls, vector<vector<float>> vec)
{
	
	int vecSize = cls.size();
	int clstCount = 0;

	for (int x = 0; x < vecSize; x++)
	{
		vector<float> selVec;
		float secSel[2];

		float dist = 10;
		float priX, priY, secX, secY;
		priX = cls[x].midPoint[0]; priY = cls[x].midPoint[1];
		
		for (int y = 0; y < vecSize; y++)
		{
			vector<float> secVec;
			secVec.push_back(cls[y].elements[0][0]);
			secVec.push_back(cls[y].elements[0][1]);

			bool cond = find(cls[x].elements.begin(), cls[x].elements.end(), secVec) != cls[x].elements.end();
			
			if (!cond)
			{
				secX = vec[y][2]; secY = vec[y][3];
				float mag = sqrt((priX-secX)*(priX-secX) + (priY-secY)*(priY-secY));
				
				//cout << mag << endl;
				//cout << vec[y][0] << ", " << vec[y][1] << endl;

				if (dist > mag)
				{
					dist = mag;
					clstCount = cls[x].elements.size();

					//cls[x].midPoint[0] = (priX + secX)/2;
					//cls[x].midPoint[1] = (priY + secY)/2;

					cls[x].midPoint[0] = priX + (secX-priX)/(clstCount+1);
					cls[x].midPoint[1] = priY + (secY-priY)/(clstCount+1);

					secSel[0] = cls[y].elements[0][0]; secSel[1] = cls[y].elements[0][1];

				}
			}
		}
		
		selVec.push_back(secSel[0]); selVec.push_back(secSel[1]);

		cls[x].elements.push_back(selVec);
		cls[x].dispersion = WeightCalc(cls[x].elements, cls[x].midPoint, vec);
		cls[x].distance = DistCalc(cls[x].elements, cls[x].midPoint, vec);

		
		//--------------------- Debuger ---------------------
		/*
		cout << "Cluster Size: " << cls[x].elements.size() << endl;
		for (int i = 0; i < cls[x].elements.size(); i++)
		{
			cout << cls[x].elements[i][0] << ", " << cls[x].elements[i][1] << endl;
		}

		cout << "Min Distance: " << dist << endl;
		cout << "dispersion: " << cls[x].dispersion << endl;
		cout << "Middle Point X: " << cls[x].midPoint[0] << ", Middle Point Y: " << cls[x].midPoint[1] << endl;
		cout << "--------------" << endl;
		*/
	}

	return cls;
}
/*
vector<float> DistCalc (vector<vector<float>> elm, float *midPt, vector<vector<float>> vec)
{
	vector<float> distVec;

	vector<vector<float>> fullVec;
	vector<float> parVec;

	for (int i = 0; i < vec.size(); i++)
	{
		parVec.push_back(vec[i][0]); parVec.push_back(vec[i][1]);
		fullVec.push_back(parVec);

		parVec.clear();
	}

	int index = 0;
	float posX, posY, dist;
	int vecSize = elm.size();

	for (int i = 0; i < vecSize; i++)
	{
		vector<float> selVec;
		selVec.push_back(elm[i][0]);
		selVec.push_back(elm[i][1]);

		auto it = find(fullVec.begin(), fullVec.end(), selVec);
		if (it != fullVec.end())
		{
			index = it - fullVec.begin();

			posX = vec[index][2]; posY = vec[index][3];
			dist = sqrt((posX - midPt[0])*(posX - midPt[0]) + (posY - midPt[1])*(posY - midPt[1]));

			distVec.push_back(dist);
		}
	}

	return distVec;
}
*/
ClusterDistance DistCalc (vector<vector<float>> elm, float *midPt, vector<vector<float>> vec)
{
	ClusterDistance clsDist;
	vector<float> distVec;

	vector<vector<float>> fullVec;
	vector<float> parVec;

	float distSumX = 0;
	float distSumY = 0;

	for (int i = 0; i < vec.size(); i++)
	{
		parVec.push_back(vec[i][0]); parVec.push_back(vec[i][1]);
		fullVec.push_back(parVec);

		parVec.clear();
	}

	int index = 0;
	float posX, posY, dist;
	int vecSize = elm.size();

	for (int i = 0; i < vecSize; i++)
	{
		vector<float> selVec;
		selVec.push_back(elm[i][0]);
		selVec.push_back(elm[i][1]);

		auto it = find(fullVec.begin(), fullVec.end(), selVec);
		if (it != fullVec.end())
		{
			index = it - fullVec.begin();

			posX = vec[index][2]; posY = vec[index][3];
			distVec.push_back(posX - midPt[0]); distVec.push_back(posY - midPt[1]);
			dist = sqrt((posX - midPt[0])*(posX - midPt[0]) + (posY - midPt[1])*(posY - midPt[1]));
			distSumX += abs(posX - midPt[0]); distSumY += abs(posY - midPt[1]);

			clsDist.Magnitude.push_back(dist);
			clsDist.Value.push_back(distVec);
			distVec.clear();
		}
	}

	clsDist.AverageValue.push_back(distSumX/vecSize);
	clsDist.AverageValue.push_back(distSumY/vecSize);

	return clsDist;
}

float WeightCalc(vector<vector<float>> elm, float *midPt, vector<vector<float>> vec)
{
	double dispersion = 0;

	vector<vector<float>> fullVec;
	vector<float> parVec;

	for (int i = 0; i < vec.size(); i++)
	{
		parVec.push_back(vec[i][0]); parVec.push_back(vec[i][1]);
		fullVec.push_back(parVec);

		parVec.clear();
	}

	int index = 0;
	float posX, posY, dist;
	int vecSize = elm.size();

	for (int i = 0; i < vecSize; i++)
	{
		vector<float> selVec;
		selVec.push_back(elm[i][0]);
		selVec.push_back(elm[i][1]);

		auto it = find(fullVec.begin(), fullVec.end(), selVec);
		if (it != fullVec.end())
		{
			index = it - fullVec.begin();

			posX = vec[index][2]; posY = vec[index][3];
			dist += sqrt((posX - midPt[0])*(posX - midPt[0]) + (posY - midPt[1])*(posY - midPt[1]));

			dispersion = ceil (dist/vecSize * 10000000) / 10000000;

			//cout << index << endl;
		}
	}

	return dispersion;
}

bool Comparison (Cluster priCls, Cluster secCls)
{
	if (priCls.dispersion > secCls.dispersion) { return true; }
	else { return false; }
}

void ShowResults(vector<Cluster> cls)
{

	ofstream out = ofstream("ClusterOut.txt", ios::out);

	int clstCount = cls.size();
	int clstSize = cls[0].elements.size();

	for (int x = 0; x < clstCount; x++)
	{
		out << "\nCluster Rank: " << clstCount - x  << "/" << clstCount << endl;
		out << "Cluster Size: " << clstSize << endl;

		out << "*********Cluster Elements*********" << endl;
		
		for (int i = 0; i < clstSize; i++)
		{
			out << fixed << setprecision(1) << i << ". (" << cls[x].elements[i][0] << ", " << cls[x].elements[i][1] << ")" << endl;
		}
		out.precision(6);

		out << "*********Cluster Properties*********" << endl;

		out << "Dispersion: " << cls[x].dispersion << endl;
		out << "Middle Point: (" << cls[x].midPoint[0] << ", " << cls[x].midPoint[1] << ")" << endl;

		out << "*********(Mid Point - Element) Distance*********" << endl;

		out << "----------Distance Values----------" << endl;
		for (int i = 0; i < clstSize; i++) 
		{
			out << i << ". (" << cls[x].distance.Value[i][0] << ", " << cls[x].distance.Value[i][1] << ")" << endl;
		}

		/*
		out << "Distance Magnitude: (";
		for (int i = 0; i < clstSize; i++) { out << cls[x].distance.Magnitude[i]; if (i < clstSize - 1){ out << ", "; } }
		out << ")" << endl;
		*/
		out << "----------------------------------" << endl;

		out << "Average: (" << cls[x].distance.AverageValue[0] << ", " << cls[x].distance.AverageValue[1] << ")" << endl;
	}
}


vector<Cluster> RemoveDups(vector<Cluster> cls)
{
	vector<Cluster> selCls;

	for (int x = 0; x < cls.size(); x++)
	{
		vector<float> selWeight;

		for (int y = 0; y < selCls.size(); y++) { selWeight.push_back(selCls[y].dispersion); }

		if (find(selWeight.begin(), selWeight.end(), cls[x].dispersion) == selWeight.end())
		{
			selCls.push_back(cls[x]);
		}
	}

	return selCls;
}