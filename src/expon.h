#include "head.h"
#include "math.h"

//int expoMech(double epsilon, CTDFloatArray* weights)
int expoMech(double r, int sensitivity, double epsilon, vector<int> weights)
{
//    int i = 0;
//    int sz = weights->GetSize();
    int sz = weights.size();
    if (sz == 0) {
        ExitMessage("expoMech: Array weights is empty.");
        ASSERT(false);
        return false;
    }
    cout<<"***************************"<<endl;

    cout<<"r= "<<r<<endl;

    double sense = 300;

    // maxWeights is needed for normalizing
    int maxWeights = 0;
    for (int i = 0; i < sz; i++ ){
//        if (weights->GetAt(i)> maxWeights)
        if (weights[i] > maxWeights)
            maxWeights = weights[i];
    }

    cout<<"maxWeights = "<<maxWeights<<endl;

    // The sum of all weights.
    long double total = 0;
    cout<<"total: "<<total<<endl;
    for (int i = 0; i < sz; i++ ) {
        if(weights[i] > 0) {
            cout << "weights[i]:" << weights[i]<< endl;
            cout << "weights[i] -  maxWeights:" << weights[i] - maxWeights << endl;
            cout << "exp(...): " << exp(epsilon * (weights[i] - maxWeights) / 2.0 * sense) << endl;
//            total = total + exp(epsilon * (weights[i] - maxWeights) / (2 * sense));
            total = total + exp(epsilon * (weights[i]) / (2 * sense));
            cout << "total: " << total << endl;
        }
    }

    int index = 0;
    long double prob = 0;
    long double tProb = 0;
    long double sum = 0;

    for (index = 0; index < sz; index++ ){
        if(weights[index] > 0) {
            cout << index << "_weight = " << weights[index] << endl;
//            prob = exp(epsilon * (weights[index] - maxWeights) / (2 * sense));
            prob = exp(epsilon * (weights[index]) / (2 * sense));
            cout << "prob: " << prob << endl;
            tProb += prob;
            cout << "tprob: " << tProb << endl;
            sum = tProb / total;
            cout << "r: " << r << ", sum: " << sum << endl;
            if (r <= sum) {
                cout << "prob: " << prob << endl;
                return index;
            }
        }
    }

    if (index == sz) {
        ExitMessage("expoMech: Out of array size");
        ASSERT(false);
    }

    return false;
}
