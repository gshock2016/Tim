#include "head.h"

//int expoMech(double epsilon, CTDFloatArray* weights)
int expoMech(int sensitivity, double epsilon, vector<int> weights)
{
    int i = 0;
//    int sz = weights->GetSize();
    int sz = weights.size();
    if (sz == 0) {
        ExitMessage("expoMech: Array weights is empty.");
        ASSERT(false);
        return false;
    }

    double r = 0;
//    srand( time(0)); // This will ensure a really randomized number by help of time.
    r = rand();
    r = r/RAND_MAX; // r is a number between 0 and 1
    cout<<"r= "<<r<<endl;

//    double sensitivity = 1;

    // maxWeights is needed for normalizing
//    float maxWeights = 0;
//    for (i = 0; i < sz; ++i ){
////        if (weights->GetAt(i)> maxWeights)
//        if (weights[i] > maxWeights)
//            maxWeights = weights[i];
//    }

    // The sum of all weights.
    long double total = 0;
    for (i = 0; i < sz; ++i )
        total += exp(epsilon * (weights[i])/(200.0));

    cout<<total<<endl;

    int index = 0;
    long double prob = 0;
    long double tProb = 0;
    double sum = 0;

    for (index = 0; index < sz; index++ ){
        cout<<"weights = "<<weights[index]<<endl;
        prob = exp(epsilon * (weights[index])/(200.0));
        tProb += prob;
        sum = tProb/total;
        cout<<prob<<endl;
        if (r <= sum)
            cout<<prob<<endl;
            return index;
    }

    if (index == sz) {
        ExitMessage("expoMech: Out of array size");
        ASSERT(false);
    }

    return false;
}
