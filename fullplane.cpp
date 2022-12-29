#include <iostream>
#include <cmath>
using namespace std;

double do_Psi(int nprt, int skip, double *x) {
    double X_n = 0, Y_n = 0;
    for (int i = 0; i < nprt; i++){
        if (i == skip) continue;
        X_n += cos(2*x[i]);
        Y_n += sin(2*x[i]);
    }
    return atan2(Y_n,X_n) / 2;
}

void fullplane() {

    //inputs
    int nevnt = 5000;
    int nprt = 200;
    double v2_input = 0.05; //5%

    //parameters
    double Psi_r, Psi_i, Psi; //reaction plane, event planes
    double x_temp, y_temp; //temp values when populating
    double sub_res, epr, vm, chi1, chi2;
    double *x = new double(nprt); //phi

    //output
    double v2;

    //ROOT variables
    TRandom3* random = new TRandom3();
    random->SetSeed();
    TProfile *v2_obs = new TProfile("v2_obs", "v2_obs",1,0.5,1.5,-1,1);
    TProfile* sub_avg = new TProfile("sub_avg", "sub_avg",1,0.5,1.5,-1,1);

    //evaluate v2
    for (int k = 0; k < nevnt; k++){ //event

        //pick random reaction plane
        Psi_r = random->Uniform(2*M_PI);
        
        //populate x with random
        for (int j = 0; j < nprt; j++){
            do{
                x_temp = random->Uniform(2*M_PI);
                y_temp = random->Uniform(1+2*v2_input);
            } while (y_temp >= 1 + 2*v2_input*cos(2*x_temp-2*Psi_r));
            x[j] = x_temp; //phi
        }

        for (int i = 0; i < nprt; i++){
            Psi_i = do_Psi(nprt, i, x);
            v2_obs->Fill(1,cos(2*x[i]-2*Psi_i));
            Psi = atan2(sin(2*x[i]),cos(2*x[i])) / 2;
            sub_avg->Fill(1,cos(2*Psi_i-2*Psi));
        }
    }

    sub_res = sqrt(sub_avg->GetBinContent(1));
    // vm = sqrt(4/M_PI) * sqrt(sub_res/2);
    // chi1 = sqrt(4) * vm;
    // chi2 = sqrt(2) * chi1;
    // epr = sqrt(M_PI/8) * chi2;
    epr = 2 * sqrt(sub_res/2);
    v2 = v2_obs->GetBinContent(1) / epr;

    cout << "v2_obs:\t" << v2_obs->GetBinContent(1) << endl;
    cout << "subres:\t" << sub_res << endl;
    cout << "vm:\t" << vm << endl;
    cout << "chi1:\t" << chi1 << endl;
    cout << "epr:\t" << epr << endl;
    cout << "v2:\t" << v2 << endl;
}