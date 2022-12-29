#include <iostream>
#include <cmath>
using namespace std;

double do_Psi(int nprt, int starting, double *x, double w) {
    double X_n = 0, Y_n = 0;
    for (int i = starting; i < starting + nprt; i++){
        X_n += w*cos(2*x[i]);
        Y_n += w*sin(2*x[i]);
    }
    return atan2(Y_n,X_n) / 2;
}

void subevent() {

    //inputs
    int nevnt = 10000;
    int nprt = 100; //will double, use half of wanted
    double v2_input = 0.05; //5%
    double w = 1; //weight

    //parameters
    double Psi_RP, PsiA, PsiB; //reaction plane, event planes
    double x_temp, y_temp; //temp values when populating
    double *x = new double(nprt * 2); //phi

    //output
    double v2, v2_err;

    //ROOT variables
    TRandom3* random = new TRandom3();
    random->SetSeed();
    TProfile *v2_obs = new TProfile("v2_obs", "v2_obs",1,0.5,1.5,-1,1);
    TProfile* epr = new TProfile("epr", "epr",1,0.5,1.5,-1,1);

    //evaluate v2
    for (int k = 0; k < nevnt; k++){ //event

        //pick random reaction plane
        Psi_RP = random->Uniform(2*M_PI);
        
        //populate x with random
        for (int j = 0; j < nprt * 2; j++){
            do{
                x_temp = random->Uniform(2*M_PI);
                y_temp = random->Uniform(1+2*v2_input);
            } while (y_temp >= 1 + 2*v2_input*cos(2*x_temp-2*Psi_RP));
            x[j] = x_temp; //phi
        }

        //sub-event Psi
        PsiA = do_Psi(nprt, 0, x, w);
        PsiB = do_Psi(nprt, nprt, x, w);

        //sub-event v2_obs (doesn't include autocorrelation)
        for (int i = 0; i < nprt; i++){
            v2_obs->Fill(1,cos(2*x[i] - 2*PsiB));
        }
        for (int i = nprt; i < nprt * 2; i++){
            v2_obs->Fill(1,cos(2*x[i] - 2*PsiA));
        }

        //get event plane resolution
        epr->Fill(1,cos(2*PsiA - 2*PsiB));

    }
    
    //output
    v2 = v2_obs->GetBinContent(1) / sqrt(epr->GetBinContent(1));
    v2_err = sqrt(v2_obs->GetBinError(1)*v2_obs->GetBinError(1)
            + epr->GetBinError(1)*epr->GetBinError(1)) / 2;

    cout << "v2_obs:\t" << v2_obs->GetBinContent(1) << "\t" << v2_obs->GetBinError(1) << endl;
    cout << "epr:\t" << sqrt(epr->GetBinContent(1)) << "\t" << epr->GetBinError(1) << endl;
    cout << "v2:\t" << v2 << "\t" << v2_err << endl;
    
    delete x;
    gDirectory->Delete("v2_obs");
    gDirectory->Delete("epr");
    gDirectory->Delete("random");

}