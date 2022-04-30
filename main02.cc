// main01.cc is a part of the PYTHIA event generator.
// Copyright (C) 2008 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL version 2, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// This is a simple test program. It fits on one slide in a talk.
// It studies the charged multiplicity distribution at the LHC.


#include <stdlib.h>
#include <time.h>

#include "Pythia8/Event.h"
#include "Pythia8/ParticleData.h"
#include "Pythia8/Pythia.h"
#include <math.h>
using namespace Pythia8;


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

float Dot (float x1,float x2,float y1,float y2,float z1,float z2)
{
	float dot;

	dot = x1*x2 + y1*y2 + z1*z2;

	return dot;
}

float Magnitude (float x,float y,float z)
{
	float Mag;

	Mag = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

	return Mag;
}

float FindAngle (float x1,float x2,float y1,float y2,float z1,float z2, bool rad)
{
	float Output;
	float radToDeg;

	if (rad == true) {radToDeg = 1;}
	else {radToDeg = (180/3.14f);}
	Output = radToDeg*acos(Dot (x1, x2, y1, y2, z1, z2)/(Magnitude(x1, y1, z1)*Magnitude(x2, y2, z2)));

	if (Output == NAN){return 0.0f;}
	else{return Output;}
}

int main(int argc, char *argv[]) {

	if( argc < 2 ){
		fprintf(stderr,"Usage : main01m runcard [decaymodedata2]\n");
		exit(0);
	}

	int n = 0;
	n = atoi(argv[2]);

	cout << n << endl;

	srand((unsigned) time(NULL));

	// Generator. Process selection. LHC initialization. Histogram.
	Pythia pythia("../xmldoc");

	//
  	// Particle decay initialize first
	//
	// pythia.particleData.listFF("decaymode.dat");
	/*
	if( argc == 3 ){
		pythia.particleData.readFF(argv[2]);
		pythia.particleData.checkTable();
	}
	*/
	//
	// Read runcard override default
	//
	pythia.settings.addMode("User:DsCharge",1,true,true,-1,1);	// Ds charge (default 1)
	pythia.settings.addWord("User:OutFile","result.dat");		// Output file (default result.dat)

	pythia.readFile( argv[1] );					// Read runcard

	int nEvent   = pythia.mode("Main:numberOfEvents");
	int DsCharge = pythia.mode("User:DsCharge");
	string OutFile = pythia.word("User:OutFile");

	int idA    = pythia.mode("Beams:idA");
	int idB    = pythia.mode("Beams:idB");
	double eA  = pythia.parm("Beams:eA");
	double eB  = pythia.parm("Beams:eB");

	char setStrN[64], setStrB[64], fName[64];

	sprintf (fName, "%.1f_%.1f_NeutDir.dat", valSet[n][0], valSet[n][1]);

	//sprintf( setStrB,"BeamRemnants:valencePowerUinP = %.1f", valSet[n][1]);
	//sprintf( setStrN,"BeamRemnants:valencePowerDinP = %.1f", valSet[n][0]);

	sprintf( setStrB,"Diffraction:MBRbeta0 = %.1f", valSet[n][1]);
	sprintf( setStrN,"Diffraction:MBRsigma0 = %.1f", valSet[n][0]);

	//cout << "\r " << ((float)n / (float)arrSize)*100 << "% Completed" << endl;	//Debugger
	cout << setStrN << ", " << setStrB << endl;	//Debugger

	pythia.settings.readString(setStrB);
	pythia.settings.readString(setStrN);

	//
	// Set Ds decay mode
	//
	ParticleDataEntry *pds = pythia.particleData.particleDataEntryPtr(431);
	for(int i=0;i<pds->sizeChannels();i++){ //pds->channels.size()
		DecayChannel& c = pds->channel(i); //decay[i]

		if( c.contains(-15,16) && c.multiplicity() == 2 ){	// Ds -> tau decay
			if( DsCharge == 1 ){ c.onMode(2); }else{ c.onMode(3); }
		}else{
			if( DsCharge == 1 ){ c.onMode(3); }else{ c.onMode(2); }
		}
	}

	FILE *fTauNeutDir;
	fTauNeutDir = fopen(fName, "wt");
	//fTauNeutDir = fopen("signalTauNeutDir.dat", "wt");

	//
	// Initialize
	//
	pythia.init(idA, idB, eA, eB);

	float progress = 0.0;
	int barWidth = 70;

	// Begin event loop. Generate event. Skip if error. List first one.
	for (int iEvent = 0; iEvent < nEvent; iEvent+=0) {
		if (!pythia.next()) continue;
		//if (iEvent < 1) {pythia.info.list(); pythia.event.list();} //Debugger

		// loop over particle
		int nCharged = 0;
		int idDs  = -1;
		int idTau = -1;
		int idCharm  = -1;
		std::vector<int> ds_dau;	ds_dau.clear();
		std::vector<int> tau_dau;	tau_dau.clear();
		std::vector<int> charm_dau;	charm_dau.clear();
		for (int i = 0; i < pythia.event.size(); ++i) {
			if( pythia.event[i].id() == 431*DsCharge ){	// Tag Ds-
				idDs = i;
				ds_dau = pythia.event.daughterList(idDs);
				for(int j=0;j<ds_dau.size();j++){
					if( pythia.event[ds_dau[j]].id() == -15*DsCharge ){ idTau = ds_dau[j]; }
				}
				for(int j=idTau+1;j<pythia.event.size();j++){
					if( pythia.event.isAncestor(j,idTau) && pythia.event[j].isFinal() ){
						tau_dau.push_back( j );
					}
				}
			}

			if( pythia.event[i].id() ==  -411*DsCharge
			 || pythia.event[i].id() ==  -421*DsCharge
			 || pythia.event[i].id() ==  -431*DsCharge
			 || pythia.event[i].id() == -4122*DsCharge
			 || pythia.event[i].id() == -4132*DsCharge
			 || pythia.event[i].id() == -4232*DsCharge ){	// Tag pair charm decay
				idCharm = i;
				for(int j=idCharm+1;j<pythia.event.size();j++){
					if( pythia.event.isAncestor(j,idCharm) && pythia.event[j].isFinal() ){
						charm_dau.push_back( j );
					}
				}
			}
		}

		// Output
		if( idDs != -1 ){
			if( idTau != -1 && idCharm != -1 ){
				//cout << setStrN << ", " << setStrB << endl;	//Debugger
				//fprintf(stderr,"%d %d %d %d\n",iEvent,ds_dau.size(),tau_dau.size(),charm_dau.size());	//Debugger

				progress += 1/(float)nEvent;

				cout << "[";
				int pos = barWidth * progress;
				for (int i = 0; i < barWidth; ++i) {
						if (i < pos) cout << "=";
						else if (i == pos) cout << ">";
						else cout << " ";
				}
				cout << "] " << int(progress * 100.0) << " %\r";
				cout.flush();

				Particle *p,*d;

				p = &pythia.event[idDs];

				for(int j=0;j<ds_dau.size();j++)
				{
					d = &pythia.event[ds_dau[j]];

					//float parMag = Magnitude(p->px(), p->py(), p->pz());
					float chlMag = Magnitude(d->px(), d->py(), d->pz());

					if ((d->id() == -16 || d->id() == 16) && chlMag != 0)
					{
						//fprintf(fTauNeut,"%-12f %-12f %-12f ", p->xDec(), p->yDec(), p->zDec());
						//fprintf(fTauNeut,"%-12f %-12f %-12f %-12f %-12f\n", d->xDec(), d->yDec(), d->zDec(), parMag, FindAngle(p->px(), d->px(), p->py(), d->py(), p->pz(), d->pz(), true));
						//fprintf(fTauNeutDir, "%-12f %-12f %-12f %-12f %-12f %-12f\n", p->px()/parMag, p->py()/parMag, p->pz()/parMag, d->px()/chlMag, d->py()/chlMag, d->pz()/chlMag);
						fprintf(fTauNeutDir, "%-12f %-12f %-12f %-12f %-12f %-12f %-12f\n", d->px()/chlMag, d->py()/chlMag, d->pz()/chlMag, d->e(), d->xDec(), d->yDec(), d->zDec());
					}

				}

				p = &pythia.event[idTau];

				for(int j=0;j<tau_dau.size();j++)
				{
					d = &pythia.event[tau_dau[j]];

					//float parMag = Magnitude(p->px(), p->py(), p->pz());
					float chlMag = Magnitude(d->px(), d->py(), d->pz());

					if((d->id() == 16 || d->id() == -16) && chlMag != 0)
					{
						//fprintf(fTauNeutDir, "%-12f %-12f %-12f %-12f %-12f %-12f\n", p->px()/parMag, p->py()/parMag, p->pz()/parMag, d->px()/chlMag, d->py()/chlMag, d->pz()/chlMag);
						//fprintf(fTauNeutDir, "%-12f %-12f %-12f %-12f\n", d->px()/parMag, d->py()/parMag, d->pz()/parMag, d->e());
						fprintf(fTauNeutDir, "%-12f %-12f %-12f %-12f %-12f %-12f %-12f\n", d->px()/chlMag, d->py()/chlMag, d->pz()/chlMag, d->e(), d->xDec(), d->yDec(), d->zDec());
					}

				}

				iEvent++;
			}
			else{
				//pythia.event.list();	//Debugger
			}
		}

	}  // End of event loop. Statistics. Histogram. Done.
	cout << "\n" << endl;

	return 0;
}
