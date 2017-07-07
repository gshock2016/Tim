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
//            BuildHypergraphR(10, islimit, size_limit);
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

            int nodeID;

            vector<int> infValues;
            for(int i =0; i < n; i++){
                infValues.push_back(0);
            }

//            ofstream myfile;
//            myfile.open ("./output/hypergt.csv");
//
//            for(int i = 0; i < (int) hyperGT.size();i++) {
//                for(int j =0; j< hyperGT[i].size(); j++){
//                    myfile<<hyperGT[i][j]<<", ";
//                }
//                myfile<<endl;
//            }
//            myfile.close();
//
//            ofstream myfile8;
//            myfile8.open("./output/hypergt_dup.csv");
//            for(int i = 0; i < (int) rrsetT.size();i++) {
//                for(int j =0; j< rrsetT[i].size(); j++){
//                    myfile8<<rrsetT[i][j]<<", ";
//                }
//                myfile8<<endl;
//            }
//            myfile8.close();

            ofstream myfile9;
            myfile9.open("./output/hyperg.csv");
            for(int i = 0; i < (int) hyperG.size();i++) {
                for(int j =0; j< hyperG[i].size(); j++){
                    myfile9<<hyperG[i][j]<<", ";
                }
                myfile9<<endl;
            }

            ofstream myfile10;
            myfile10.open("./output/hyperg_dup.csv");
            for(int i = 0; i < (int) rrset_true.size();i++) {
                for(int j =0; j< rrset_true[i].size(); j++){
                    myfile10<<rrset_true[i][j]<<", ";
                }
                myfile10<<endl;
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

            ofstream myfile6;
            myfile6.open("./output/rrset_dif.csv");

            for(int i = 0; i< hyperG.size(); i++){
                sort(hyperG[i].begin(), hyperG[i].end());
            }
            for(int i = 0; i< rrset_true.size(); i++){
                sort(rrset_true[i].begin(), rrset_true[i].end());
            }

            for(int i =0; i < gT.size();i++){
                for(int j =0; j< gT[i].size();j++){
                    vector<int> dif1(hyperG[gT[i][j]].size());
                    vector<int> dif2(rrset_true[gT[i][j]].size());

                    vector<int>::iterator it1;
                    vector<int>::iterator it2;
//                    set_difference(hyperG[i].begin(), hyperG[i].end(), hyperG[gT[i][j]].begin(), hyperG[gT[i][j]].end(), dif.begin());
                    it1 = set_difference(hyperG[gT[i][j]].begin(), hyperG[gT[i][j]].end(), hyperG[i].begin(), hyperG[i].end(), dif1.begin());
                    it2 = set_difference(rrset_true[gT[i][j]].begin(), rrset_true[gT[i][j]].end(), hyperG[i].begin(), hyperG[i].end(), dif2.begin());
//                    for(int x = 0; x < hyperG[i].size(); x++){
//                        myfile6<<hyperG[i][x]<<", ";
//                    }
//                    myfile6<<endl;
//                    for(int x = 0; x < hyperG[gT[i][j]].size(); x++){
//                        myfile6<<hyperG[gT[i][j]][x]<<", ";
//                    }
//                    myfile6<<endl;
//                    for(int x = 0; x < dif.size(); x++){
//                        myfile6<<dif[x]<<", ";
//                    }
//                    myfile6<<endl;
                    dif1.resize(it1 - dif1.begin());
                    dif2.resize(it2 - dif2.begin());
                    set<int> s;
                    unsigned size = dif2.size();
//                    for( unsigned i = 0; i < size; ++i )
//                        s.insert( dif2[i] );
                    for( unsigned k = 0; k < rrset_true[gT[i][j]].size(); ++k )
                        s.insert(rrset_true[gT[i][j]][k]);

                    myfile6<<gT[i][j]<< ", "<<i<<","
                           <<hyperG[gT[i][j]].size()<<", "<<rrset_true[gT[i][j]].size()<<", "<< hyperG[i].size()<<", "
                           <<(dif1.size())<<", "<<(dif2.size())<<", "<<s.size()<<", "
                           <<numVisted[i][j]<<endl;
                }
            }

            myfile6.close();

            vector<set<int>> v_rrsets;
            for(int i=0; i< (int) hyperG.size();i++){
                infValues[i] = hyperG[i].size();
                set<int> tmpset(hyperG[i].begin(), hyperG[i].end());
                v_rrsets.push_back(tmpset);
            }

            ofstream myfile7;
            myfile7.open("./output/ustart.csv");
            vector<int> ustart_num;
            for (int i = 0; i < n;i++){
                ustart_num.push_back(0);
            }

            for (int i = 0; i < (int) hyperGT.size() - 1;i++){
                ustart_num[hyperGT[i][0]]++;

                int rrsetID = i;
//                nodeID = hyperGT[i][0];
//                myfile <<nodeID<<",";
//                infAdjList[nodeID].push_back(rrsetID);
                for(int j = 0; j < (int) hyperGT[i].size(); j++){
                    nodeID = hyperGT[i][j];
//                    myfile7<<hyperGT[i][j]<<endl;
//                    myfile <<nodeID<<",";
//                    cout<<"hello"<<endl;
                    infAdjList[nodeID].push_back(rrsetID);
//                    infmatrix[nodeID][rrsetID]=1;
//                    infValues[nodeID] = infValues[nodeID] + 1;
                }
//                myfile << "\n";
//                cout<<endl;
            }



            cout<<hyperGT.size()<<endl;
            for (int i = 0; i < (int) ustart_num.size();i++){
                myfile7<<i<<", "<<ustart_num[i]<<endl;

            }

            myfile7.close();

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

//            int first[] = {5,10,15,20,25};
//            int second[] = {50,40,30,20,10};
//            vector<int> a, b ;
//            for(int i =0; i< 5; i++){
//                a.push_back(first[i]);
//                b.push_back(second[i]);
//            }
//            std::vector<int> v(10);                      // 0  0  0  0  0  0  0  0  0  0
//            std::vector<int>::iterator it;
//
//            std::sort (a.begin(),a.end());     //  5 10 15 20 25
//            std::sort (b.begin(),b.end());   // 10 20 30 40 50
//
//            it=std::set_difference (a.begin(), a.end(), b.begin(), b.end(), v.begin());
//            //  5 15 25  0  0  0  0  0  0  0
//            v.resize(it-v.begin());
//            std::cout << "The difference has " << (v.size()) << " elements:\n";
//            for (it=v.begin(); it!=v.end(); ++it)
//                std::cout << ' ' << *it;
//            std::cout << '\n';
//            myfile3.close();
//            myfile4.close();
    }

};

