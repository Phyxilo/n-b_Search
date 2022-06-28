#include <vector>
#include <TRandom3.h>

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



struct mom {
	double px;
	double py;
	double pz;
};

double XF(TRandom3 *gen, double n) {
	double m=1./(n+1.);
	double r= gen->Rndm();
	r = std::pow(r,m);
	double s= gen->Rndm();
	if (s>0.5) return(1.-r);
	else return(r-1.);
};

namespace MCVar
{
	double binInt[100];
	double binEn[100];
}

void tau_neutrino_generator(double E_beam, double n_inp, double b_inp, int n_sample);

double* ToyMC(double energy, double n, double b, int nSamp) {

	tau_neutrino_generator(energy, n, b, nSamp);

	return MCVar::binInt;
	/*
	// n = 2
	tau_neutrino_generator(400,2.0,0.4);
	tau_neutrino_generator(400,2.0,0.6);
	tau_neutrino_generator(400,2.0,0.8);
	tau_neutrino_generator(400,2.0,1.0);
	tau_neutrino_generator(400,2.0,1.2);
	tau_neutrino_generator(400,2.0,1.4);
	tau_neutrino_generator(400,2.0,1.6);

	// n = 2.5
	tau_neutrino_generator(400,2.5,0.4);
	tau_neutrino_generator(400,2.5,0.6);
	tau_neutrino_generator(400,2.5,0.8);
	tau_neutrino_generator(400,2.5,1.0);
	tau_neutrino_generator(400,2.5,1.2);
	tau_neutrino_generator(400,2.5,1.4);
	tau_neutrino_generator(400,2.5,1.6);

	// n = 3
	tau_neutrino_generator(400,3.0,0.4);
	tau_neutrino_generator(400,3.0,0.6);
	tau_neutrino_generator(400,3.0,0.8);
	tau_neutrino_generator(400,3.0,1.0);
	tau_neutrino_generator(400,3.0,1.2);
	tau_neutrino_generator(400,3.0,1.4);
	tau_neutrino_generator(400,3.0,1.6);

	// n = 3.5
	tau_neutrino_generator(400,3.5,0.4);
	tau_neutrino_generator(400,3.5,0.6);
	tau_neutrino_generator(400,3.5,0.8);
	tau_neutrino_generator(400,3.5,1.0);
	tau_neutrino_generator(400,3.5,1.2);
	tau_neutrino_generator(400,3.5,1.4);
	tau_neutrino_generator(400,3.5,1.6);

	// n = 4
	tau_neutrino_generator(400,4.0,0.4);
	tau_neutrino_generator(400,4.0,0.6);
	tau_neutrino_generator(400,4.0,0.8);
	tau_neutrino_generator(400,4.0,1.0);
	tau_neutrino_generator(400,4.0,1.2);
	tau_neutrino_generator(400,4.0,1.4);
	tau_neutrino_generator(400,4.0,1.6);

	// n = 4.5
	tau_neutrino_generator(400,4.5,0.4);
	tau_neutrino_generator(400,4.5,0.6);
	tau_neutrino_generator(400,4.5,0.8);
	tau_neutrino_generator(400,4.5,1.0);
	tau_neutrino_generator(400,4.5,1.2);
	tau_neutrino_generator(400,4.5,1.4);
	tau_neutrino_generator(400,4.5,1.6);

	// n = 5
	tau_neutrino_generator(400,5.0,0.4);
	tau_neutrino_generator(400,5.0,0.6);
	tau_neutrino_generator(400,5.0,0.8);
	tau_neutrino_generator(400,5.0,1.0);
	tau_neutrino_generator(400,5.0,1.2);
	tau_neutrino_generator(400,5.0,1.4);
	tau_neutrino_generator(400,5.0,1.6);
	*/

};


void tau_neutrino_generator(double E_beam, double n_inp, double b_inp, int n_sample) {

	TH1F *h_xf      = new TH1F("h_xf" ,"XF of Ds"  ,100, -1, 1);
	TH1F *h_ptx     = new TH1F("h_ptx","Pt_x of Ds",100, -3, 3);
	TH1F *h_pty     = new TH1F("h_pty","Pt_y of Ds",100, -3, 3);
	TH1F *h_pt2     = new TH1F("h_pt2","Pt^{2} of Ds",100, 0, 6);
	TH1F *h_pz      = new TH1F("h_pz","Pz of Ds",100, -10, 10);
	TH1F *h_ax      = new TH1F("h_ax","ax of Ds",100, -0.2, 0.2);
	TH1F *h_ay      = new TH1F("h_ay","ay of Ds",100, -0.2, 0.2);
	TH1F *h_eex     = new TH1F("h_eex","Energy of Ds",100,0,400);

	TH1F *h_th     = new TH1F("h_th","Emission angle #theta of Ds",100,  0.0, 0.2);
	TH1F *h_th_tau = new TH1F("h_th_tau","Emission angle #theta of #tau",100,  0.0, 0.2);
	TH1F *h_th_n1  = new TH1F("h_th_n1","Emission angle #theta of #nu1",100,  0.0, 0.2);
	TH1F *h_th_n2  = new TH1F("h_th_n2","Emission angle #theta of #nu2",100,  0.0, 0.2);
	TH1F *h_th_nal = new TH1F("h_th_nal","Emission angle #theta of #nu all",100,  0.0, 0.2);
// ** in the acceptance circle **
	TH1F *h_th_n1a = new TH1F("h_th_n1a","Emission angle #theta in 0.005 #nu1",10,  0.0, 0.005);
	TH1F *h_th_n2a = new TH1F("h_th_n2a","Emission angle #theta in 0.005 #nu2",10,  0.0, 0.005);
	TH1F *h_th_naal = new TH1F("h_th_naal","Emission angle #theta in 0.005 #nu",10,  0.0, 0.005);

	TH2F *h_th_eex  = new TH2F("h_th_eex","#theta .vs. E_ex of Ds",100,0,0.2,100,0,400);
	TH2F *h_Ds_ang  = new TH2F("h_th_Ds_ang","#thetax .vs. #thetay of Ds",100,-0.2,0.2,100,-0.2,0.2);
	TH2F *h_tau_ang = new TH2F("h_th_tau_ang","#thetax .vs. #thetay of #tau",100,-0.2,0.2,100,-0.2,0.2);
	TH2F *h_neu1ang = new TH2F("h_th_neu1ang","#thetax .vs. #thetay of #nu1",100,-0.2,0.2,100,-0.2,0.2);
	TH2F *h_neu2ang = new TH2F("h_th_neu2ang","#thetax .vs. #thetay of #nu2",100,-0.2,0.2,100,-0.2,0.2);

// ** in the acceptance box **
	TH2F *h_neu1ain = new TH2F("h_th_neu1ain","#thetax .vs. #thetay of #nu1 in SHiP em",100,-0.009826,0.009826,100,-0.009826,0.009826);
	TH2F *h_neu2ain = new TH2F("h_th_neu2ain","#thetax .vs. #thetay of #nu2 in SHiP em",100,-0.009826,0.009826,100,-0.009826,0.009826);
	TH2F *h_neuaain = new TH2F("h_th_neuaain","#thetax .vs. #thetay of #nu in SHiP em",100,-0.009826,0.009826,100,-0.009826,0.009826);

	TH1F *h_e_n1   = new TH1F("h_e_n1","Energy of #nu1 in FD",100,0,400);
	TH1F *h_e_n2   = new TH1F("h_e_n2","Energy of #nu2 in FD",100,0,400);
	TH1F *h_e_nal  = new TH1F("h_e_nal","Energy of #nu in FD",100,0,400);

	TH1F *h_ke_n1   = new TH1F("h_ke_n1","K*Energy of #nu1 in FD",100,0,400);
	TH1F *h_ke_n2   = new TH1F("h_ke_n2","K*Energy of #nu2 in FD",100,0,400);
	TH1F *h_ke_nal  = new TH1F("h_ke_nal","K*Energy of #nu in FD",100,0,400);

// *** interacted ***

	TH1F *h_ei_n1   = new TH1F("h_e_n1","Energy of #nu1 in FD",100,0,400);
	TH1F *h_ei_n2   = new TH1F("h_e_n2","Energy of #nu2 in FD",100,0,400);
	TH1F *h_ei_nal  = new TH1F("h_e_nal","Energy of #nu in FD",100,0,400);
	TH1F *h_areai   = new TH1F("h_areai","Area ID 1 0.5x0.5 0 outer in FD",3,-0.5,2.5);

	TH1F *h_areai2  = new TH1F("h_areai2","Area ID 1 0.5x0.5 0 outer in FD",100,-0.5,99.5);
	TH1F *EnArea  = new TH1F("EnArea","Area ID 1 0.5x0.5 0 outer in FD",100,-0.5,99.5);

	TH2F *h_neu1iain = new TH2F("h_th_neu1iain","#thetax .vs. #thetay of #nu1 int in SHiP em",100,-0.009826,0.009826,100,-0.009826,0.009826);
	TH2F *h_neu2iain = new TH2F("h_th_neu2iain","#thetax .vs. #thetay of #nu2 int in SHiP em",100,-0.009826,0.009826,100,-0.009826,0.009826);
	TH2F *h_neuaiain = new TH2F("h_th_neuaiain","#thetax .vs. #thetay of #nu  int in SHiP em",100,-0.009826,0.009826,100,-0.009826,0.009826);

	TH2F *h_neuaiain0 = new TH2F("h_th_neuaiain0","#thetax .vs. #thetay of #nu  int in SHiP em area0",100,-0.009826,0.009826,100,-0.009826,0.009826);
	TH2F *h_neuaiain1 = new TH2F("h_th_neuaiain1","#thetax .vs. #thetay of #nu  int in SHiP em area1",100,-0.009826,0.009826,100,-0.009826,0.009826);
	TH2F *h_neuaiain2 = new TH2F("h_th_neuaiain2","#thetax .vs. #thetay of #nu  int in SHiP em area2",100,-0.009826,0.009826,100,-0.009826,0.009826);

	TH2F *neuEn = new TH2F("neuEn","Energy of each neutrino in SHiP em",100,-0.009826,0.009826,100,-0.009826,0.009826);
//
// Input variables
//
// beam proton Energy
	double Ene;    // SPS-> 400 GeV ;
	Ene = E_beam;

// (1-XF)^n_index
    double n_index; // in case 400 GeV -> 4.5;
    n_index = n_inp;

	double Pt = sqrt(1./(2.*b_inp));	// Pt sigma for projection case of b=b_inp -> exp(-bPt^2)

//
	double m  = 0.938;	// Nucleon mass GeV/C2
	double mc = 1.968;	// Ds Mass GeV/c2
	double S  = sqrt(2*m*Ene); // Center of Mass System Energy
	double p  = sqrt((S/2)*(S/2)-mc*mc); 	// Momentum at Center of Mass System
	double gamma = Ene/S;	// Gamma factor for Center of Mass System
	double beta  = sqrt(1.-1./(gamma*gamma));

//
	double xf,ptx,pty,pt2;
	TRandom3 *gen= new TRandom3();
	gen->SetSeed();

// *** 100GeV tau decay nu_tau momentum DATA by GEANT4  1 line = 1 nu_tau from 100 GeV tau decay  ***

	char buf[1024];
	FILE *fp;
	fp = fopen("/home/phyxilo/root/macros/ToyMC/nu_tau_mom.100GeV_tau_decay.dat","rt");
	std::vector<double> px_v,py_v,pz_v;
	int sim_size;
	int ev,n_dau;
	double npx,npy,npz,s_ene;

	while (fgets(buf, 1024, fp)) {
		sscanf(buf,"%d %d %lf %lf %lf %lf",&ev,&n_dau,&s_ene,&npx,&npy,&npz);
//		fprintf(stdout,"%d\n",ev);
		px_v.push_back(npx);
		py_v.push_back(npy);
		pz_v.push_back(npz);
	}
	fclose(fp);
	sim_size=px_v.size();
	fprintf(stdout,"tau decay simulation data size=%d\n",sim_size);

// *** K-factor sigma(tau_nu)/sigma(muon) reading ***

	fp = fopen("/home/phyxilo/root/macros/ToyMC/Kfactor.dat","rt");
	int it=0;
	double Ke[2500],Ktau[2500], Kant[2500];		// 0 to 249.9 GeV for iterator 0 to 2499
	while (fgets(buf, 1024, fp)) {
		sscanf(buf,"%lf %lf %lf",&Ke[it], &Ktau[it],&Kant[it]);
		it +=1;
	}
	fclose(fp);



// variables parameters ***
// Ds->tau + n_tau
	double gamma1;// Ds's gamma factor
	double beta1;
	double p1= 0.182;					// Decay Pt GeV (Ds->tau +n_tau)
	double cs1;
	double sn1;
	double ph1;
//	tau (-neutrino) momentum in CMS
	double p1z;
	double p1x;
	double p1y;
//
	double m_tau = 1.777; 	// tau mas GeV/c2
	double e1_tau;
	double p1x_ex_tau;
	double p1y_ex_tau;
	double p1z_ex_tau;
	double e1_ex_tau;
	double e1_neu;
	double p1x_ex_neu;
	double p1y_ex_neu;
	double p1z_ex_neu;
	double e1_ex_neu;
	double ax1_tau;
	double ay1_tau;
	double th_tau;
	double ax1_neu;
	double ay1_neu;
	double th_n1;
// tau -> x+n_tau
	double gamma2;		// Tau's gamma factor
	double beta2;
	double p2x_ex_neu;
	double p2y_ex_neu;
	double p2z_ex_neu;
	double e2_ex_neu;
	double ax2_neu;
	double ay2_neu;
	double th_n2;

//
	double kv;
	double KE,r_KE;
	int it_e, id_area, id_area2;

// SHiP fiducial setting ***

	double theta_box1  = 0.0098256;					// outer  40cm/40.71m
	double theta_box0  = -1. * theta_box1;
	double theta_box11 = sqrt(2./3.)* theta_box1; 		// 2nd inner
	double theta_box10 = -1. * theta_box11;
	double theta_box21 = sqrt(1./3.)* theta_box1;			// most inner
	double theta_box20 = -1. * theta_box21;

	BoxHist *BH = new BoxHist;
	BH->Init(theta_box0,theta_box1,theta_box0,theta_box1,3);

//
// Ds particle genaration by the differential cross section
// d^2 N / dXF / dPt^2 = (1-XF)^n * exp(-b * Pt^2)
//

//	int n_sample=100000;
	int sim_data;			// For iterator of G4 simulated tau neutrino
	double inv_n = 1.0/(double)n_sample;
	int step = 10000;
	double inv_step = 1.0/(double)step;
	for (int i=0;i<n_sample;i++) {
		if (i-step*int(inv_step*i)==0) {
			fprintf(stdout,"%8d /%8d = %5.1lf%%\r",i,n_sample,100.*i*inv_n);
		}
		xf  = XF(gen, n_index);

// generated Ds momentum
		double pz  = p*xf;		// momentum for z direction

		ptx = Pt * gen->Gaus();
		pty = Pt * gen->Gaus();
//

		pt2 = ptx*ptx + pty*pty;
		double e   = sqrt(mc*mc+pt2+pz*pz); // Ds energy in CMS

// Lorentz transformation to experimental system

		double pz_ex = gamma * (beta*e + pz);
		double e_ex  = gamma * (e      + beta*pz);

// generated Ds track slope
		double ax    = ptx / pz_ex;
		double ay    = pty / pz_ex;
		double th    = sqrt(ax*ax + ay*ay);

//
// *** generate Ds->tau + n_tau ****
//
		gamma1 = e_ex /mc;			// Ds's gamma factor
		beta1 = sqrt(1.-1./(gamma1*gamma1));

//		double p1= 0.182;					// Decay Pt GeV (Ds->tau +n_tau)
		cs1 = 2.*(gen->Rndm()-0.5);	// cos(theta) in Ds CMS

		sn1 = sqrt(1.-cs1*cs1);
		if (gen->Rndm()<0.5) sn1*=-1;

		double ph1 = 3.1415*(gen->Rndm());	// phi

//		tau (-neutrino) momentum in CMS
		p1z = p1 * cs1;
		p1x = p1 * sn1 *cos(ph1);
		p1y = p1 * sn1 *sin(ph1);

//
//	*** Lorentz transformation by Ds gamma factor ***
//
//		tau
//		double m_tau = 1.777; 	// tau mas GeV/c2

		e1_tau= sqrt(m_tau*m_tau +p1*p1);
		p1x_ex_tau = p1x;
		p1y_ex_tau = p1y;
		p1z_ex_tau = gamma1 * (beta1 * e1_tau + p1z);
		e1_ex_tau = gamma1 *(e1_tau + beta1 *p1z);

//		neutrino
		e1_neu= sqrt(p1*p1);
		p1x_ex_neu = -p1x;
		p1y_ex_neu = -p1y;
		p1z_ex_neu = gamma1 * (beta1 * e1_neu  -p1z);
		e1_ex_neu  = gamma1 * (e1_neu + beta1* -p1z);

		ax1_tau = p1x_ex_tau / p1z_ex_tau;
		ay1_tau = p1y_ex_tau / p1z_ex_tau;
		double th_tau = sqrt((ax1_tau + ax)*(ax1_tau + ax) + (ay1_tau + ay) * (ay1_tau +ay));

		ax1_neu = p1x_ex_neu / p1z_ex_neu;
		ay1_neu = p1y_ex_neu / p1z_ex_neu;
		double th_n1   = sqrt((ax1_neu + ax)*(ax1_neu + ax) + (ay1_neu + ay) * (ay1_neu +ay));

//
// *** genarate tau decay neutrinos ***
//
		gamma2 = e1_ex_tau / m_tau;			// Tau's gamma factor
		beta2  = sqrt(1.-1./(gamma2*gamma2));

		sim_data = i % sim_size;		//������ ���܂�
//		fprintf(stdout,"%d\n",sim_data);
		p2x_ex_neu= px_v[sim_data];
		p2y_ex_neu= py_v[sim_data];
		p2z_ex_neu= pz_v[sim_data] *e1_ex_tau / 100.;		// normalized to energy=e1_ex_tau
		e2_ex_neu  = sqrt(p2x_ex_neu * p2x_ex_neu + p2y_ex_neu * p2y_ex_neu + p2z_ex_neu * p2z_ex_neu);

		ax2_neu = p2x_ex_neu / p2z_ex_neu;
		ay2_neu = p2y_ex_neu / p2z_ex_neu;
		double th_n2   = sqrt((ax2_neu + ax + ax1_tau) * (ax2_neu + ax + ax1_tau) + (ay2_neu + ay + ay1_tau) * (ay2_neu + ay + ay1_tau));


		double AX1_neu = ax + ax1_neu ;
		double AY1_neu = ay + ay1_neu ;
		double AX2_neu = ax + ax1_tau + ax2_neu ;
		double AY2_neu = ay + ay1_tau + ay2_neu ;
// Filling into Histgrams Ds
		h_xf->Fill(xf);
		h_ptx->Fill(ptx);
		h_pty->Fill(pty);
		h_pt2->Fill(pt2);
		h_pz->Fill(pz);
		h_ax->Fill(ax);
		h_ay->Fill(ay);
		h_th->Fill(th);
		h_th_tau->Fill(th_tau);
		h_th_n1->Fill(th_n1);
		h_th_n2->Fill(th_n2);
		if (th_n1<0.005) {
			h_th_n1a->Fill(th_n1);
			h_th_naal->Fill(th_n1);
		}
		if (th_n2<0.005) {
			h_th_n2a->Fill(th_n2);
			h_th_naal->Fill(th_n2);
		}
		h_eex->Fill(e_ex);
		h_th_eex->Fill(th,e_ex);

		h_Ds_ang->Fill(ax,ay);
		h_tau_ang->Fill(ax + ax1_tau , ay + ay1_tau);
		h_neu1ang->Fill(AX1_neu , AY1_neu);
		h_neu2ang->Fill(AX2_neu , AY2_neu);


// in SHiP emulsion area
/*
		fprintf(stdout,"%lf %lf\n",theta_box1,theta_box0);
		fprintf(stdout,"%lf %lf\n",theta_box11,theta_box10);
		fprintf(stdout,"%lf %lf\n",theta_box21,theta_box20);
*/
//		double kv;
//		double KE,r_KE;
//		int it_e, id_area, id_area2;
		if (AX1_neu>= theta_box0 && AX1_neu<=theta_box1 && AY1_neu>= theta_box0 && AY1_neu<= theta_box1) {
			h_neu1ain->Fill(AX1_neu , AY1_neu);
			h_neuaain->Fill(AX1_neu , AY1_neu);
			h_e_n1->Fill(e1_ex_neu);
			h_e_nal->Fill(e1_ex_neu);

			it_e = int(e1_ex_neu*10+0.5);
			if (it_e>2499) it_e=2499;
			kv= 0.5*(Ktau[it_e]+Kant[it_e]);
			h_ke_n1->Fill(kv*e1_ex_neu);
			h_ke_nal->Fill(kv*e1_ex_neu);

			r_KE= Ene * gen->Rndm();
			KE= kv* e1_ex_neu;
			if (r_KE<=KE) {				//  **** interacted ****
				h_neu1iain->Fill(AX1_neu , AY1_neu);
				h_neuaiain->Fill(AX1_neu , AY1_neu);
				h_ei_n1->Fill(e1_ex_neu);
				h_ei_nal->Fill(e1_ex_neu);

				neuEn->Fill(AX1_neu , AY1_neu, e1_ex_neu); //Energy2d Hist

				id_area=0;
				if (AX1_neu>= theta_box10 && AX1_neu<=theta_box11 && AY1_neu>= theta_box10 && AY1_neu<= theta_box11) {
					id_area=1;
					if (AX1_neu>= theta_box20 && AX1_neu<=theta_box21 && AY1_neu>= theta_box20 && AY1_neu<= theta_box21) {
						id_area=2;
					}
				}
				id_area = 2 -id_area;
				id_area2 =BH->GetAreaID(AX1_neu,AY1_neu);

				if (id_area==0)		h_neuaiain0->Fill(AX1_neu , AY1_neu);
				else if (id_area==1)	h_neuaiain1->Fill(AX1_neu , AY1_neu);
				else if (id_area==2)	h_neuaiain2->Fill(AX1_neu , AY1_neu);
				h_areai->Fill(id_area);
				h_areai2->Fill(id_area2);
				EnArea->Fill(id_area2, e1_ex_neu); //Area Hist

			}

		}
		if (AX2_neu>= theta_box0 && AX2_neu <=theta_box1 && AY2_neu>=theta_box0 && AY2_neu <=theta_box1) {
			h_neu2ain->Fill(AX2_neu , AY2_neu);
			h_neuaain->Fill(AX2_neu , AY2_neu);
			h_e_n2->Fill(e2_ex_neu);
			h_e_nal->Fill(e2_ex_neu);

			it_e = int(e2_ex_neu*10+0.5);
			if (it_e>2499) it_e=2499;
			kv= 0.5*(Ktau[it_e]+Kant[it_e]);
			h_ke_n2->Fill(kv*e2_ex_neu);
			h_ke_nal->Fill(kv*e2_ex_neu);

			r_KE= Ene * gen->Rndm();
			KE= kv* e2_ex_neu;
			if (r_KE<=KE) {				// **** interacted ****
				h_neu2iain->Fill(AX2_neu , AY2_neu);
				h_neuaiain->Fill(AX2_neu , AY2_neu);
				h_ei_n2->Fill(e2_ex_neu);
				h_ei_nal->Fill(e2_ex_neu);

				neuEn->Fill(AX2_neu , AY2_neu, e2_ex_neu); //Energy2d Hist
				id_area=0;
				if (AX2_neu>= theta_box10 && AX2_neu <=theta_box11 && AY2_neu>=theta_box10 && AY2_neu <=theta_box11) {
					id_area=1;
					if (AX2_neu>= theta_box20 && AX2_neu <=theta_box21 && AY2_neu>=theta_box20 && AY2_neu <=theta_box21) {
						id_area=2;
					}
				}
				id_area  = 2 -id_area;
				id_area2 =BH->GetAreaID(AX2_neu,AY2_neu);

				if (id_area==0) h_neuaiain0->Fill(AX2_neu , AY2_neu);
				else if (id_area==1) h_neuaiain1->Fill(AX2_neu , AY2_neu);
				else if (id_area==2) h_neuaiain2->Fill(AX2_neu , AY2_neu);
				h_areai->Fill(id_area);
				h_areai2->Fill(id_area2);
				EnArea->Fill(id_area2, e2_ex_neu); //Area Hist
			}

		}

	}




// Drawings

	TCanvas *c1= new TCanvas("c1","Test Canvas",20,20,800,800);
	char  p_name[64], p_file[64];
	sprintf( p_name,"Out/tau_neutrino_gen_E%03.0lf_n%2.1lf_b%3.1lf.test.pdf",E_beam,n_inp,b_inp);
	sprintf( p_file,"%s[",p_name);
	c1->Print( p_file,"pdf");

	sprintf( p_file,"%s",p_name);

//
// 1st page
//
	c1->Divide(2,2);

//	gStyle->SetNdivisions(504);
//	gStyle->SetPadGridY(1);

	c1->cd(1);
//	gStyle->SetOptStat(0001111);
	gStyle->SetStatH(0.1);
	gStyle->SetStatW(0.2);
	gStyle->SetStatFontSize(0.05);
// define here the fitting function (1-|XF|)^n
	TF1 *f = new TF1("f","[0]*pow((1.-sqrt(x*x)),[1])",-1,1);
	float P0 = (float)n_sample*0.1;
	float P1 = (float)n_inp;
	f->SetParameter(P0,P1);
	h_xf->SetXTitle("XF");
	h_xf->SetYTitle("Entries");
	h_xf->SetMinimum(0.0);
	h_xf->SetFillColor(5);		// Yellow
	h_xf->Draw();
	h_xf->Fit("f");		// Fit here to see n

	c1->cd(2);
	h_ptx->SetXTitle("Pt_x (GeV/c)");
	h_ptx->SetYTitle("Entries");
	h_ptx->SetMinimum(0.0);
	h_ptx->SetFillColor(5);		// Yellow
	h_ptx->Draw();
//	h_ptx->Fit("gaus");

	c1->cd(3);
//	gPad->SetLogy();
    gStyle->SetOptFit(1111);
	h_pt2->SetXTitle("Pt^{2} (GeV^{2}/c^{2})");
	h_pt2->SetYTitle("Entries");
	h_pt2->SetMinimum(0.0);
	h_pt2->SetFillColor(5);		// Yellow
	h_pt2->Draw();
	h_pt2->Fit("expo");			// To see b value

	c1->cd(4);
	h_pz->SetXTitle("CMS Pz (GeV/c)");
	h_pz->SetYTitle("Entries");
	h_pz->SetMinimum(0.0);
	h_pz->SetFillColor(5);		// Yellow
	h_pz->Draw();

	c1->Print(p_file,"pdf");

// 2nd page
	c1->Clear();
	c1->Divide(2,2);
	c1->Update();

	gStyle->SetPalette(1);

	c1->cd(1);
	h_ax->SetXTitle("ax (rad)");
	h_ax->SetYTitle("Entries");
	h_ax->SetMinimum(0.0);
	h_ax->SetFillColor(5);		// Yellow
	h_ax->Draw();

	c1->cd(2);
	h_th->SetXTitle("#theta (rad)");
	h_th->SetYTitle("Entries");
	h_th->SetMinimum(0.0);
	h_th->SetFillColor(5);		// Yellow
	h_th->Draw();

	c1->cd(3);
	h_eex->SetXTitle("Eex (GeV)");
	h_eex->SetYTitle("Entries");
	h_eex->SetMinimum(0.0);
	h_eex->SetFillColor(5);		// Yellow
	h_eex->Draw();

	c1->cd(4);
	h_th_eex->SetXTitle("Emission angle #theta (rad)");
	h_th_eex->SetYTitle("Eex (GeV)");
	h_th_eex->SetMinimum(0.0);
	h_th_eex->SetFillColor(5);		// Yellow
	h_th_eex->Draw("coltz");


	c1->Print(p_file,"pdf");

// 3rd page  Ds->tau + neu_tau

	c1->Clear();
	c1->Divide(2,2);
	c1->Update();

	c1->cd(1);
	h_Ds_ang->SetXTitle("#thetax");
	h_Ds_ang->SetYTitle("#thetay");
	h_Ds_ang->SetFillColor(5);		// Yellow
	h_Ds_ang->Draw("coltz");

	c1->cd(2);
	h_tau_ang->SetXTitle("#thetax");
	h_tau_ang->SetYTitle("#thetay");
	h_tau_ang->SetFillColor(5);		// Yellow
	h_tau_ang->Draw("coltz");

	c1->cd(3);
	h_neu1ang->SetXTitle("#thetax");
	h_neu1ang->SetYTitle("#thetay");
	h_neu1ang->SetFillColor(5);		// Yellow
	h_neu1ang->Draw("coltz");

	c1->cd(4);
	h_neu2ang->SetXTitle("#thetax");
	h_neu2ang->SetYTitle("#thetay");
	h_neu2ang->SetFillColor(5);		// Yellow
	h_neu2ang->Draw("coltz");

	c1->Print(p_file,"pdf");


// 4th page  Ds->tau + neu_tau

	c1->Clear();
	c1->Divide(2,3);
	c1->Update();

	c1->cd(1);
	h_th->SetXTitle("#theta (rad)");
	h_th->SetYTitle("Entries");
	h_th->SetMinimum(0.0);
	h_th->SetFillColor(5);		// Yellow
	h_th->Draw();

	c1->cd(2);
	h_th_tau->SetXTitle("#theta (rad)");
	h_th_tau->SetYTitle("Entries");
	h_th_tau->SetMinimum(0.0);
	h_th_tau->SetFillColor(5);		// Yellow
	h_th_tau->Draw();

	c1->cd(3);
	h_th_n1->SetXTitle("#theta (rad)");
	h_th_n1->SetYTitle("Entries");
	h_th_n1->SetMinimum(0.0);
	h_th_n1->SetFillColor(5);		// Yellow
	h_th_n1->Draw();

	c1->cd(4);
	h_th_n2->SetXTitle("#theta (rad)");
	h_th_n2->SetYTitle("Entries");
	h_th_n2->SetMinimum(0.0);
	h_th_n2->SetFillColor(5);		// Yellow
	h_th_n2->Draw();

	c1->cd(5);
	h_th_n1a->SetXTitle("#theta (rad)");
	h_th_n1a->SetYTitle("Entries");
	h_th_n1a->SetMinimum(0.0);
	h_th_n1a->SetFillColor(5);		// Yellow
	h_th_n1a->Draw();

	c1->cd(6);
	h_th_n2a->SetXTitle("#theta (rad)");
	h_th_n2a->SetYTitle("Entries");
	h_th_n2a->SetMinimum(0.0);
	h_th_n2a->SetFillColor(5);		// Yellow
	h_th_n2a->Draw();

	c1->Print(p_file,"pdf");

// 5th page  Ds->tau + neu_tau

	c1->Clear();
	c1->Divide(2,3);
	c1->Update();

	c1->cd(1);
	h_neu1ain->SetXTitle("#thetax");
	h_neu1ain->SetYTitle("#thetay");
	h_neu1ain->SetFillColor(5);		// Yellow
	h_neu1ain->Draw("coltz");

	c1->cd(2);
	h_neu2ain->SetXTitle("#thetax");
	h_neu2ain->SetYTitle("#thetay");
	h_neu2ain->SetFillColor(5);		// Yellow
	h_neu2ain->Draw("coltz");

	c1->cd(3);
	h_e_n1->SetXTitle("Energy");
	h_e_n1->SetYTitle("Entries");
	h_e_n1->SetMinimum(0.0);
	h_e_n1->SetFillColor(5);		// Yellow
	h_e_n1->Draw();

	c1->cd(4);
	h_e_n2->SetXTitle("Energy");
	h_e_n2->SetYTitle("Entries");
	h_e_n2->SetMinimum(0.0);
	h_e_n2->SetFillColor(5);		// Yellow
	h_e_n2->Draw();

	c1->cd(5);
	h_neuaain->SetXTitle("#thetax");
	h_neuaain->SetYTitle("#thetay");
	h_neuaain->SetFillColor(5);		// Yellow
	h_neuaain->Draw("coltz");

	c1->cd(6);
	h_e_nal->SetXTitle("Energy");
	h_e_nal->SetYTitle("Entries");
	h_e_nal->SetMinimum(0.0);
	h_e_nal->SetFillColor(5);		// Yellow
	h_e_nal->Draw();

	c1->Print(p_file,"pdf");



// 6th page  Ds->tau + neu_tau

	c1->Clear();
	c1->Divide(2,3);
	c1->Update();

	c1->cd(1);
	h_neu1ain->SetXTitle("#thetax");
	h_neu1ain->SetYTitle("#thetay");
	h_neu1ain->SetFillColor(5);		// Yellow
	h_neu1ain->Draw("coltz");

	c1->cd(2);
	h_neu2ain->SetXTitle("#thetax");
	h_neu2ain->SetYTitle("#thetay");
	h_neu2ain->SetFillColor(5);		// Yellow
	h_neu2ain->Draw("coltz");

	c1->cd(3);
	h_ke_n1->SetXTitle("Energy");
	h_ke_n1->SetYTitle("Entries");
	h_ke_n1->SetMinimum(0.0);
	h_ke_n1->SetFillColor(5);		// Yellow
	h_ke_n1->Draw();

	c1->cd(4);
	h_ke_n2->SetXTitle("Energy");
	h_ke_n2->SetYTitle("Entries");
	h_ke_n2->SetMinimum(0.0);
	h_ke_n2->SetFillColor(5);		// Yellow
	h_ke_n2->Draw();

	c1->cd(5);
	h_neuaain->SetXTitle("#thetax");
	h_neuaain->SetYTitle("#thetay");
	h_neuaain->SetFillColor(5);		// Yellow
	h_neuaain->Draw("coltz");

	c1->cd(6);
	h_ke_nal->SetXTitle("Energy");
	h_ke_nal->SetYTitle("Entries");
	h_ke_nal->SetMinimum(0.0);
	h_ke_nal->SetFillColor(5);		// Yellow
	h_ke_nal->Draw();

/*
	c1->cd(3);
	h_th_naal->SetXTitle("#theta (rad)");
	h_th_naal->SetYTitle("Entries");
	h_th_naal->SetMinimum(0.0);
	h_th_naal->SetFillColor(5);		// Yellow
	h_th_naal->Draw();
*/

	c1->Print(p_file,"pdf");


// 7th page  Ds->tau + neu_tau			interacted

	c1->Clear();
	c1->Divide(2,3);
	c1->Update();

	c1->cd(1);
	h_neu1iain->SetXTitle("#thetax");
	h_neu1iain->SetYTitle("#thetay");
	h_neu1iain->SetFillColor(5);		// Yellow
	h_neu1iain->Draw("coltz");

	c1->cd(2);
	h_neu2iain->SetXTitle("#thetax");
	h_neu2iain->SetYTitle("#thetay");
	h_neu2iain->SetFillColor(5);		// Yellow
	h_neu2iain->Draw("coltz");

	c1->cd(3);
	h_ei_n1->SetXTitle("Energy");
	h_ei_n1->SetYTitle("Entries");
	h_ei_n1->SetMinimum(0.0);
	h_ei_n1->SetFillColor(5);		// Yellow
	h_ei_n1->Draw();

	c1->cd(4);
	h_ei_n2->SetXTitle("Energy");
	h_ei_n2->SetYTitle("Entries");
	h_ei_n2->SetMinimum(0.0);
	h_ei_n2->SetFillColor(5);		// Yellow
	h_ei_n2->Draw();

	c1->cd(5);
	h_neuaiain->SetXTitle("#thetax");
	h_neuaiain->SetYTitle("#thetay");
	h_neuaiain->SetFillColor(5);		// Yellow
	h_neuaiain->Draw("coltz");

	c1->cd(6);
	h_ei_nal->SetXTitle("Energy");
	h_ei_nal->SetYTitle("Entries");
	h_ei_nal->SetMinimum(0.0);
	h_ei_nal->SetFillColor(5);		// Yellow
	h_ei_nal->Draw();

	c1->Print(p_file,"pdf");


// 8th page  Ds->tau + neu_tau			interacted

	c1->Clear();
	c1->Divide(2,3);
	c1->Update();

	c1->cd(1);
	h_neuaiain->SetXTitle("#thetax");
	h_neuaiain->SetYTitle("#thetay");
	h_neuaiain->SetFillColor(5);		// Yellow
	h_neuaiain->Draw("coltz");

	c1->cd(2);
	h_neuaiain0->SetXTitle("#thetax");
	h_neuaiain0->SetYTitle("#thetay");
	h_neuaiain0->SetFillColor(5);		// Yellow
	h_neuaiain0->Draw("coltz");

	c1->cd(3);
	h_neuaiain1->SetXTitle("#thetax");
	h_neuaiain1->SetYTitle("#thetay");
	h_neuaiain1->SetFillColor(5);		// Yellow
	h_neuaiain1->Draw("coltz");

	c1->cd(4);
	h_neuaiain2->SetXTitle("#thetax");
	h_neuaiain2->SetYTitle("#thetay");
	h_neuaiain2->SetFillColor(5);		// Yellow
	h_neuaiain2->Draw("coltz");

	c1->cd(5);
	h_areai->SetXTitle("Area-ID");
	h_areai->SetYTitle("Entries");
	h_areai->SetMinimum(0.0);
	h_areai->SetFillColor(5);		// Yellow
	h_areai->Draw();

	c1->cd(6);
	h_areai2->SetXTitle("Area-ID");
	h_areai2->SetYTitle("Entries");
	h_areai2->SetMinimum(0.0);
	h_areai2->SetFillColor(5);		// Yellow
	h_areai2->Draw();

	//9th page
	c1->Clear();
	c1->Update();

	//neuEn->SetXTitle("#thetax");
	//neuEn->SetYTitle("#thetay");
	neuEn->SetFillColor(5);		// Yellow
	neuEn->Draw("coltz");


	FILE *fpo;
	char f_name[64];
	sprintf( f_name,"Out/tau_neutrino_gen_E%03.0lf_n%2.1lf_b%3.1lf.pos.dat",E_beam,n_inp,b_inp);
	fpo= fopen(f_name,"wt");

	int NbA= h_areai2->GetXaxis()->GetNbins();
	int N;
	int EN;
	for (int j=1;j<NbA+1;j++) {
		N= h_areai2->GetBinContent(j);
		EN = EnArea->GetBinContent(j);
//		fprintf(fpo,"%3d %10d\n",j,N);
		fprintf(fpo,"%10d\n",N);
		MCVar::binInt[j-1] = N;
		MCVar::binEn[j-1] = EN;
	}
	fclose(fpo);

	c1->Print(p_file,"pdf");


//
//      Closing picture file
//
	sprintf( p_file,"%s]",p_name);
	c1->Print(p_file,"pdf");

	/*
	int NbA = h_areai2->GetXaxis()->GetNbins();
	int N;
	for (int j = 1; j < NbA+1; j++)
	{
		N = h_areai2->GetBinContent(j);
		MCVar::binInt[j-1] = N;
	}
	*/

	return;
}
