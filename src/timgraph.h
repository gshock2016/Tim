#include <fstream>

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
            return ept;
        }
        void RefindKPT(double epsilon, double ept){
            Timer t(2, "step2" );
            ASSERT(ept > 0);
            int64 R = (2 + epsilon) * ( n * log(n) ) / ( epsilon * epsilon * ept);
            BuildHypergraphR(R, false, 0);
//            BuildHypergraphR(10);
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

            cout<<"build hypergraph"<<endl;
            BuildHypergraphR(R, islimit, size_limit);
//            BuildHypergraphR(10);
            cout<<"R = "<<R<<endl;
            BuildSeedSet(isExpon, size_limit);
        }
        void EstimateOPT(double epsilon, int isExpon, bool islimit, int size_limit){
            Timer t(100,"EstimateOPT");


            // KPT estimation
            double kpt_star;
            kpt_star=KptEstimation();

            // Refine KPT
            double eps_prime;
            eps_prime=5*pow(sqr(epsilon)/(k+1), 1.0/3.0);
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

        void getRRsets(int threshold){

            vector<set<int>> v_rrsets;



//            int64 R = (8+2 * epsilon) * ( log(n) + log(2) +  n * logcnk(n, k) ) / ( epsilon * epsilon * opt);
//            BuildHypergraphR(R);
//
//            cout<<"R= "<<R<<endl;
            cout<<"get rrsets"<<endl;

/*
 * RRset ID start from 0
 * Output inlfuence matrix, infadjlist, rrsets
 * Example:
 * RRset:
 * -------------------------------------------------------
 * seed node | sequence of influenced node (BFS sequence)
 *     2     | 1, 3,
 *     1     | 2, 3
 *     3     | 2,
 * -------------------------------------------------------
 * InfValue:
 * -------------------------------------------------------
 * node ID   | influence value (number of rrsets it belongs to)
 *     1     | 2
 *     2     | 3
 *     3     | 3
 * -------------------------------------------------------
 * InfAdjList:
 * -------------------------------------------------------
 * node ID   | rrsets the node belongs to (sorted)
 *     1     | 1, 2
 *     2     | 1, 2, 3
 *     3     | 1, 2, 3
 * -------------------------------------------------------
 * InfMatrix
 * -------------------------------------------------------
 * node ID   | if node i belong to rrset j, then (i, j) = 1, otherwise (i,j)=0
 *     1     | 1, 1, 0
 *     2     | 1, 1, 1
 *     3     | 1, 1, 1
 * -------------------------------------------------------
 */

            ofstream myfile;
            myfile.open ("./output/rrset.csv");
//            myfile.close();
            int nodeID;

            vector<int> infValues;
            for(int i =0; i < n; i++){
                infValues.push_back(0);
            }

            ofstream myfile5;
            myfile5.open("./output/numVisted.csv");

            for(int i=0; i< numVisted.size(); i++){
                for(int j = 0; j < numVisted[i].size(); j++){
//                    myfile5 << i <<", "<< gT[i][j]<< ":"<< numVisted[i][j];
                    myfile5<<i << ", "<<gT[i][j]<<","<<numVisted[i][j];
                    myfile5<<"\n";
                }
            }
            myfile5.close();

            for(int i=0; i< (int) hyperG.size();i++){
                infValues[i] = hyperG[i].size();
                set<int> tmpset(hyperG[i].begin(), hyperG[i].end());
                v_rrsets.push_back(tmpset);
                set_difference(v_rrsets[i], v_rrsets[i+1]);
            }

            for (int i = 0; i < (int) hyperGT.size();i++){
                int rrsetID = i;
//                nodeID = hyperGT[i][0];
//                myfile <<nodeID<<",";
//                infAdjList[nodeID].push_back(rrsetID);
                for(int j = 0; j < (int) hyperGT[i].size(); j++){
                    nodeID = hyperGT[i][j];
                    myfile <<nodeID<<",";
//                    cout<<"hello"<<endl;
                    infAdjList[nodeID].push_back(rrsetID);
//                    infmatrix[nodeID][rrsetID]=1;
//                    infValues[nodeID] = infValues[nodeID] + 1;
                }
                myfile << "\n";
//                cout<<endl;
            }
            myfile.close();

            ofstream myfile2;
            myfile2.open("./output/infvalue.csv");
//            ofstream myfile3;
//            myfile3.open("./output/infmatrix.csv");
//            ofstream myfile4;
//            myfile4.open("./output/infadjlist.csv");

//            cout<<"writing files"<<endl;
            for (int i = 0; i < n; i++){
//                myfile2<<i<<","<<infValues[i]<< "\n";
                if(infValues[i] > threshold) {
//                    myfile2<<i<<","<<n * infValues[i] /hyperId<< "\n";
                    myfile2<<i<<","<<infValues[i]<< "\n";
                    myfile2<<i<<","<<v_rrsets[i].size()<< "\n";
//                    for (int j = 0; j < (int) infmatrix[i].size(); j++) {
//                        myfile3 << infmatrix[i][j] << ",";
//                    }

/********************************* infmatrix ******************/
//                    int preRRid = 0;
//                    int curRRid = 0;
//                    int R = hyperGT.size()-1;
////                    cout<<"R = "<<R<<endl;
//                    for (int j = 0; j < infAdjList[i].size(); j++) {
////                        cout<<(int) g.infAdjList[i].size()<<endl;
//                        myfile4 << infAdjList[i][j] << ",";
//                        curRRid = infAdjList[i][j];
//                        for(k = 0 ; k < curRRid - preRRid; k++){
//                            myfile3 << 0 <<",";
//                        }
//                        myfile3 << 1 <<",";
//                        preRRid = curRRid+1;
////                        cout<<"predid = "<<preRRid<<endl;
//                    }
//                    if(preRRid < R){
//                        curRRid = R;
//                        for(k = 0; k < curRRid - preRRid; k++){
//                            myfile3 << 0 <<",";
//                        }
//                    }
//                    myfile3 << "\n";
/********************************* infmatrix ******************/

//                    myfile4 << "\n";
                }

            }
            myfile2.close();
//            myfile3.close();
//            myfile4.close();
    }

};

