#include <iostream>
#include <cmath>

void createhisto()
{
    TFile *f = new TFile("histogram.root","UPDATE");
    TH1D *histo = new TH1D("double_gaus", "20 Numbers", 500, -10, 10);

    int trials = 1000000;
    for (int i = 0; i < trials * 2; i++)
        histo -> Fill(gRandom -> Gaus(0,1));
    for (int i = 0; i < trials; i++)
        histo -> Fill(gRandom -> Gaus(2,3));

    f -> Write();
    f -> Close();
}

void openhisto()
{
    int trials = 1000000;
    TFile *hist_file = new TFile("histogram.root", "READ");
    TH1D *hist = (TH1D*)hist_file -> Get("double_gaus");

    gStyle -> SetOptFit(1);
    TF1  *func = new TF1("fitfunc","gaus(0) + gaus(3)",-10, 10);

    int params [] = {trials * 2, 0, 1, trials, 2, 3};

    for (int i = 0; i < 6; i++)
        func -> SetParameter(i,params[i]);

    hist -> Fit("fitfunc");
    TF1 *vals = hist -> GetFunction("gaus");
    //hist_file -> Close();
}

void inputoutput()
{
    createhisto();
    openhisto();
}