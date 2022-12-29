#include <iostream>
#include <cmath>
using namespace std;

void test() {

    double x[100];
    TRandom3* rand = new TRandom3();
    rand->SetSeed();
    TProfile *p_v2 = new TProfile("p_v2","p_v2", 1, 0.5, 1.5, 0,2*M_PI);

    for (int i = 0; i < 100; i++){
        x[i] = rand->Uniform(2*M_PI); //phi
        cout << x[i] << ",";
    }

    for (int i = 0; i < 100; i++){
        p_v2->Fill(1, x[i]);  
    }

    p_v2->Draw();

}