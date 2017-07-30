#include <fstream>
#include "infgraph.h"

class TimGraph: public InfGraph
{
    public:
        TimGraph(string folder, string graph_file):InfGraph(folder, graph_file ){
        }
        double MgT(int u){
            //static int i=0;
            //i++;
            //TRACE_LINE("mgt", i);
            ASSERT(u>=0);
            ASSERT(u<n);

            return (double)BuildHypergraphNode(u, 0, false, false, 0);
            cout<<"mgt"<<endl;
        }
        double algo2(){
            double lb=1/2.0;
            double c=0;
            while(true){
                int loop= (6 * log(n)  +  6 * log(log(n)/ log(2)) )* 1/lb  ;
                c=0;
                IF_TRACE(int64 now=rdtsc());
                double sumMgTu=0;
                for(int i=0; i<loop; i++){
                    int u=rand()%n;
                    double MgTu=MgT(u);
                    double pu=MgTu/m;
                    sumMgTu+=MgTu;
                    c+=1-pow((1-pu), k);
                }
                c/=loop;
                if(c>lb) break;
                lb /= 2;
            }

            return c * n;
        }
        double KptEstimation()
        {

            Timer t(1, "step1");
            double ept=algo2();
            ept/=2;
            cout<<"KptEstimation"<<endl;
            return ept;
        }
        void RefindKPT(double epsilon, double ept){
            Timer t(2, "step2" );
            ASSERT(ept > 0);
            cout<<"n = "<<n<<", ept = "<<ept<<endl;
            int64 R = (2 + epsilon) * ( n * log(n) ) / ( epsilon * epsilon * ept);
            cout<<"R = "<<R<<endl;
            cout<<"build hypergraph"<<endl;
            BuildHypergraphR(R, false, 0);
//            BuildHypergraphR(10, false, 0);
        }
        double logcnk(int n, int k){
            double ans=0;
            for(int i=n-k+1; i<=n; i++){
                ans+=log(i);
            }
            for(int i=1; i<=k; i++){
                ans-=log(i);
            }
            return ans;
        }
        void NodeSelection(double epsilon, double opt, int isExpon, bool islimit, int size_limit){
            Timer t(3, "step3");
            ASSERT(opt > 0);
            int64 R = (8+2 * epsilon) * ( log(n) + log(2) +  n * logcnk(n, k) ) / ( epsilon * epsilon * opt);
            cout<<"R = "<<R<<endl;
            cout<<"build hypergraph"<<endl;
            BuildHypergraphR(R, islimit, size_limit);
//            BuildHypergraphR(10, islimit, size_limit);
            cout<<"start buildseedset"<<endl;
            BuildSeedSet(isExpon, size_limit);
        }
        void EstimateOPT(double epsilon, int isExpon, bool islimit, int size_limit){
            Timer t(100,"EstimateOPT");

            cout<<"EstimateOPT"<<endl;
            // KPT estimation
            double kpt_star;
            kpt_star=KptEstimation();

            // Refine KPT
            double eps_prime;
            eps_prime=5*pow(pow(epsilon, 2)/(k+1), 1.0/3.0);
            cout<<"refindkpt"<<endl;
            RefindKPT(eps_prime, kpt_star);
            cout<<"buildsedset"<<endl;
            BuildSeedSet(0, 0);
            cout<<"influence hypergraph"<<endl;
            double kpt=InfluenceHyperGraph();
            kpt/=1+eps_prime;
            double kpt_plus = max(kpt, kpt_star);
            cout<<"kpt_plus = "<<kpt_plus<<endl;
            cout<<"node selection"<<endl;
            //Node Selection
            NodeSelection(epsilon, kpt_plus, isExpon, islimit, size_limit);
            disp_mem_usage("");
        }


    /**
 * hyperGT:
 * -------------------------------------------------------
 * seed node | sequence of influenced node (BFS sequence)
 *     2     | 1, 3,
 *     1     | 2, 3
 *     3     | 2,
 * -------------------------------------------------------
 * hyperG:
 * -------------------------------------------------------
 * node ID   | rrsets the node belongs to (sorted)
 *     1     | 1, 2
 *     2     | 1, 2, 3
 *     3     | 1, 2, 3
 * -------------------------------------------------------
 */

        void outputStat() {

        cout << "get rrsets" << endl;

        vector<int> ustart_num;
        for (int i = 0; i < n; i++) {
            ustart_num.push_back(0);
        }
        for (int i = 0; i < (int) hyperGT.size() - 1; i++) {
            ustart_num[hyperGT[i][0]]++;
        }

        ofstream myfile6;
        myfile6.open("./output/rrset_dif.csv");

        for (int i = 0; i < hyperG.size(); i++) {
            sort(hyperG[i].begin(), hyperG[i].end());
        }
        for (int i = 0; i < rrsetG.size(); i++) {
            sort(rrsetG[i].begin(), rrsetG[i].end());
        }

        for (int i = 0; i < gT.size(); i++) {
            for (int j = 0; j < gT[i].size(); j++) {
                for (int k = 0; k < numVisted[i][j].size(); k++) {
                    sort(numVisted[i][j].begin(), numVisted[i][j].end());
                }
                vector<int> dif1(hyperG[i].size());
                vector<int> dif2(rrsetG[gT[i][j]].size());

                vector<int>::iterator it1;
                vector<int>::iterator it2;
//                    set_difference(hyperG[i].begin(), hyperG[i].end(), hyperG[gT[i][j]].begin(), hyperG[gT[i][j]].end(), dif.begin());
                it1 = set_difference(hyperG[i].begin(), hyperG[i].end(),
                                     hyperG[gT[i][j]].begin(), hyperG[gT[i][j]].end(), dif1.begin());
                it2 = set_difference(rrsetG[gT[i][j]].begin(), rrsetG[gT[i][j]].end(),
                                     numVisted[i][j].begin(), numVisted[i][j].end(), dif2.begin());

                dif1.resize(it1 - dif1.begin());
                dif2.resize(it2 - dif2.begin());
                set<int> s1;
                for (int k = 0; k < int (rrsetG[gT[i][j]].size()); k++)
                    s1.insert(rrsetG[gT[i][j]][k]);
                set<int> s2;
                for (int k = 0; k < int (dif2.size()); k++)
                    s2.insert(dif2[k]);
                cout<<hyperG[gT[i][j]].size()<<", "<<s1.size()<<endl;
                ASSERT(hyperG[gT[i][j]].size() == s1.size());
                myfile6 << gT[i][j] << ", " << i << ","
                        << hyperG[gT[i][j]].size() << ", " << rrsetG[gT[i][j]].size() << ", " << s1.size() << ", "
                        << hyperG[i].size() << ", " << dif2.size() << ","
                        << hyperG[gT[i][j]].size() - s2.size() << ", " << numVisted[i][j].size() << ", "
                        << ustart_num[i] << endl;
            }
        }
    }

    void outputRRset() {
            ofstream myfile;
            myfile.open ("./output/hypergt.csv");
            for(int i = 0; i < (int) hyperGT.size();i++) {
                for(int j =0; j< hyperGT[i].size(); j++){
                    myfile<<hyperGT[i][j]<<", ";
                }
                myfile<<endl;
            }
            myfile.close();

            ofstream myfile9;
            myfile9.open("./output/hyperg.csv");
            for(int i = 0; i < (int) hyperG.size();i++) {
                for(int j =0; j< hyperG[i].size(); j++){
                    myfile9<<hyperG[i][j]<<", ";
                }
                myfile9<<endl;
            }
            myfile9.close();

            ofstream myfile8;
            myfile8.open("./output/rrsetgt.csv");
            for(int i = 0; i < (int) rrsetGT.size();i++) {
                for(int j =0; j< rrsetGT[i].size(); j++){
                    myfile8<<rrsetGT[i][j]<<", ";
                }
                myfile8<<endl;
            }
            myfile8.close();

            ofstream myfile10;
            myfile10.open("./output/rrsetg.csv");
            for(int i = 0; i < (int) rrsetG.size();i++) {
                for(int j =0; j< rrsetG[i].size(); j++){
                    myfile10<<rrsetG[i][j]<<", ";
                }
                myfile10<<endl;
            }
            myfile10.close();
        }
};

