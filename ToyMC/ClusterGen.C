#include <iostream>
#include <vector>
#include "TCanvas.h"

using namespace std;

struct Cluster
{
	vector<vector<float>> elements;
	
	float midPoint[2];
	float weight;
};

vector<vector<float>> Dat2Vec(FILE *datFile);

vector<Cluster> Vec2Cls (vector<vector<float>> Vec);
vector<Cluster> PairGen(vector<Cluster> cls, vector<vector<float>> vec);
vector<Cluster> RemoveDups(vector<Cluster> cls);

float WeightCalc(vector<vector<float>> elm, float *midPt, vector<vector<float>> vec);
bool Comparison (Cluster priCls, Cluster secCls);

void ShowResults(vector<Cluster> cls);

vector<Cluster> clusterVecPos;
vector<Cluster> clusterVecEn;
vector<vector<float>> posVec;
vector<vector<float>> enVec;

void ClusterGen()
{
	char buf[1024];
	FILE *fPos;
	//fPos = fopen("/home/phyxilo/Out/posPythia.dat","rt");
	fPos = fopen("5E7/pos.dat","rt");

	FILE *fEn;
	//fEn = fopen("/home/phyxilo/Out/enPythia.dat","rt");
	fEn = fopen("5E7/en.dat","rt");

	Cluster clst;

	posVec = Dat2Vec(fPos);
	enVec = Dat2Vec(fEn);

	clusterVecPos = Vec2Cls(posVec);
	clusterVecEn = Vec2Cls(enVec);

	//--------------------- Debuger ---------------------
	/*
	for (int x = 0; x < clusterVecPos.size(); x++)
	{
		for (int i = 0; i < clusterVecPos[x].elements.size(); i++)
		{
			cout << clusterVecPos[x].elements[i][0] << ", " << clusterVecPos[x].elements[i][1] << endl;
		}
		cout << "Weight: " << clusterVecPos[x].weight << endl;
		cout << "Middle Point X: " << clusterVecPos[x].midPoint[0] << ", Middle Point Y: " << clusterVecPos[x].midPoint[1] << endl;
		cout << "--------------" << endl;
	}
	*/

	int clsSize = 0;
	cout << "Enter Cluster Size: "; cin >> clsSize;

	for (int i = 0; i < clsSize-1; i++)
	{
		//clusterVecPos = PairGen(clusterVecPos, posVec);
		clusterVecEn = PairGen(clusterVecEn, enVec);
	}

	//sort(clusterVecPos.begin(), clusterVecPos.end(), Comparison);
	sort(clusterVecEn.begin(), clusterVecEn.end(), Comparison);

	//clusterVecPos = RemoveDups(clusterVecPos);
	clusterVecEn = RemoveDups(clusterVecEn);
	
	//ShowResults(clusterVecPos);
	ShowResults(clusterVecEn);
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
		cls.weight = 0;

		clstVec.push_back(cls);

		subVec.clear(); cls.elements.clear();

		//--------------------- Debuger ---------------------
		/*
		cout << cls.elements[0][0] << ", " << cls.elements[0][1] << endl;
		cout << "Weight: " << cls.weight << endl;
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
		cls[x].weight = WeightCalc(cls[x].elements, cls[x].midPoint, vec);

		
		//--------------------- Debuger ---------------------
		/*
		cout << "Cluster Size: " << cls[x].elements.size() << endl;
		for (int i = 0; i < cls[x].elements.size(); i++)
		{
			cout << cls[x].elements[i][0] << ", " << cls[x].elements[i][1] << endl;
		}

		cout << "Min Distance: " << dist << endl;
		cout << "Weight: " << cls[x].weight << endl;
		cout << "Middle Point X: " << cls[x].midPoint[0] << ", Middle Point Y: " << cls[x].midPoint[1] << endl;
		cout << "--------------" << endl;
		*/
	}

	return cls;
}

float WeightCalc(vector<vector<float>> elm, float *midPt, vector<vector<float>> vec)
{
	double weight = 0;

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

			weight = ceil (dist/vecSize * 10000000) / 10000000;

			//cout << index << endl;
		}
	}

	return weight;
}

bool Comparison (Cluster priCls, Cluster secCls)
{
	if (priCls.weight > secCls.weight) { return true; }
	else { return false; }
}

void ShowResults(vector<Cluster> cls)
{
	int clstCount = cls.size();

	for (int x = 0; x < clstCount; x++)
	{
		cout << "\nCluster Rank: " << clstCount - x << endl;
		cout << "Cluster Size: " << clstCount << endl;

		cout << "*********Cluster Elements*********" << endl;
		cout.precision(2);
		for (int i = 0; i < cls[x].elements.size(); i++)
		{
			cout << i << ". (" << cls[x].elements[i][0] << ", " << cls[x].elements[i][1] << ")" << endl;
		}
		cout.precision(6);

		cout << "*********Cluster Properties*********" << endl;
		cout << "Weight: " << cls[x].weight << endl;
		cout << "Middle Point: (" << cls[x].midPoint[0] << ", " << cls[x].midPoint[1] << ")" << endl;
	}
}


vector<Cluster> RemoveDups(vector<Cluster> cls)
{
	vector<Cluster> selCls;

	for (int x = 0; x < cls.size(); x++)
	{
		vector<float> selWeight;

		for (int y = 0; y < selCls.size(); y++) { selWeight.push_back(selCls[y].weight); }

		if (find(selWeight.begin(), selWeight.end(), cls[x].weight) == selWeight.end())
		{
			selCls.push_back(cls[x]);
		}
	}

	return selCls;
}