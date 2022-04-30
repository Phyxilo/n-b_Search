#include <iostream>
#include <cmath>
#include <string>

void drawtext();
Double_t customFit (Double_t *x, Double_t *par);
float FittedM (float sqrtb, float n, float slope);

/*
float posPars [5][2] =
{
	{3.0, 0.8},
	{4.0, 1.6},
	{2.5, 0.6},
	{3.5, 1.0},
	{4.0, 1.4}
};
*/

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

float posBSqrt [5];
float enBSqrt [5];
float posN [5];
float enN [5];

void MCFit()
{

	for (int i = 0; i < 5; i++)
	{
		posBSqrt[i] = sqrt(posPars[i][1]);
		enBSqrt[i] = sqrt(enPars[i][1]);

		posN[i] = posPars[i][0];
		enN[i] = enPars[i][0];
	}

	TCanvas *Canvas= new TCanvas("Canvas","Graph Canvas",20,20,1920,1080);
	Canvas->SetWindowSize(1920, 1080);
	Canvas->SetCanvasSize(192*6, 108*6);
	Canvas->SetGrid();

	TF1 *func = new TF1 ("customFit", "customFit", 2, 5, 2);

	TGraph *pos = new TGraph (5, posN, posBSqrt);

	pos->SetMarkerStyle(20);
	pos->SetMarkerColor(1);
	pos->SetTitle("Position");
	pos->DrawClone("AP");
	pos->Fit("customFit");
	//cout << pos->GetFunction("pol1")->GetParameter(1) << endl;
	/*
	TExec *exPos = new TExec("exPos","drawtext(posPars);");
	pos->GetListOfFunctions()->Add(exPos);
	*/
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

	Canvas->Print( "Out/Fit.pdf(","pdf");

	//float slp = pos->GetFunction("customFit")->GetParameter(1);
	/*
	double sum = 0;
	cout << sqrt(slp) << endl;
	for (int x = 0; x < 5; x++)
	{
		sum += FittedM(posBSqrt[x], posN[x], slp);
		cout << x << ": " << FittedM(posBSqrt[x], posN[x], slp) << endl;
	}
	cout << "Average: " << sum/5 << endl;
	*/

	TGraph *en = new TGraph (5, enN, enBSqrt);

	en->SetMarkerStyle(20);
	en->SetMarkerColor(1);
	en->SetTitle("Energy");
	en->DrawClone("AP");
	en->Fit("customFit");
	//cout << en->GetFunction("pol1")->GetParameter(1) << endl;
	/*
	TExec *exEn = new TExec("exEn","drawtext(enPars);");
	en->GetListOfFunctions()->Add(exEn);
	*/
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

	Canvas->Print( "Out/Fit.pdf","pdf");


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

	Canvas->Print( "Out/Fit.pdf)","pdf");

	//slp = en->GetFunction("customFit")->GetParameter(1);
	//cout << en->GetFunction("pol1")->SetParameter(0) << endl;
	/*
	sum = 0;
	for (int x = 0; x < 5; x++)
	{
		sum += FittedM(enBSqrt[x], enN[x], slp);
		cout << x << ": " << FittedM(enBSqrt[x], enN[x], slp) << endl;
	}
	cout << "Average: " << sum/5 << endl;
	*/
	/*
	TF1 *posFunc;
	posFunc = pos->GetFunction("pol1");
	//cout << "Position: " << FittedM(posFunc->Eval(2), 2, slp) << endl;
	cout << posFunc->Eval(3) << endl;
	TF1 *enFunc;
	enFunc = en->GetFunction("pol1");
	//cout << "Energy: " << FittedM(enFunc->Eval(2), 2, slp) << endl;
	*/
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

/*
void drawtext(float Pars[][])
{
   Int_t i,n;
   Double_t x,y;
   TLatex *l;

   TGraph *g = (TGraph*)gPad->GetListOfPrimitives()->FindObject("Graph");
   n = g->GetN();
   for (i=1; i<n; i++)
	 {
      g->GetPoint(i,x,y);
      l = new TLatex(x,y,Form("{%1.1f, %1.1f}",Pars, Pars));
      l->SetTextSize(0.025);
      l->SetTextFont(42);
      l->SetTextAlign(21);
      l->Paint();
   }
}
*/
