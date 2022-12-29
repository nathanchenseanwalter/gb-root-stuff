#include <iostream>
#include <cmath>

double folding(double original, double low_lim, double high_lim) {

    while (original < low_lim) {
        original += high_lim - low_lim;
    }
    while (original > high_lim) {
        original -= high_lim - low_lim;
    }

    return original;
}

void readhist()
{

    int nharmonics = 10;
    int fit_trials = 1000000;
    int coeff_trials = 850000;
    int shift_trials = 850000;
    double sample, newangle, deltaangle;
    
    // Read Histogram
    TFile* hist_file = new TFile("histogram.root", "READ");
    TH1D* hist = (TH1D*)hist_file->Get("double_gaus");

    // Write Shifted Histogram
    // auto An = new TProfile("An", "Sine Coefficient", nharmonics, -1, 10);
    // auto Bn = new TProfile("An", "Cosine Coefficient", nharmonics, -1, 10);
    double An[nharmonics];
    double Bn[nharmonics];
    TH1D* hist2 = new TH1D("hist2", "Shifted Histogram", 100, -M_PI, M_PI);

    double total1, total2;
    for (int n = 1; n < nharmonics + 1; n++) {

        total1 = 0;
        total2 = 0;

        for (int i = 0; i < coeff_trials; i++) {

            sample = folding(hist->GetRandom(), -M_PI, M_PI);

            total1 += sin(n * sample);
            total2 += cos(n * sample);

            // An->Fill(n, 2. / n * sin(n * sample));
            // Bn->Fill(n, 2. / n * cos(n * sample));

        }

        An[n - 1] = total1 / coeff_trials;
        Bn[n - 1] = total2 / coeff_trials;
        cout << "An [" << n << "]: " << An[n - 1] << endl;
        cout << "Bn [" << n << "]: " << Bn[n - 1] << endl;
    }

    for (int i = 0; i < shift_trials; i++) {

        deltaangle = 0;
        sample = folding(hist->GetRandom(), -M_PI, M_PI);
        
        for (int n = 1; n < nharmonics + 1; n++) {
            deltaangle += 2. / n * (-An[n - 1] * cos(n * sample) + Bn[n - 1] * sin(n * sample));
        }
            // deltaangle += An->GetBinContent(n) * cos(n * sample) + Bn->GetBinContent(n) * sin(n * sample);

        newangle = folding(sample + deltaangle, -M_PI, M_PI);
        hist2->Fill(newangle);
    }

    hist2->SetAxisRange(0., 10000., "Y");
    hist2->Draw();
}

void shifting()
{
    readhist();
}