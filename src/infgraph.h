#include <ctime> // Needed for the true randomization
#include "expon.h"
#include "graph.h"

class InfGraph:public Graph
{
    public:
        vector<vector<int>> hyperG;
        vector<vector<int>> rrsetG;

        vector<vector<int>> rrsetGT;
        vector<vector<int>> hyperGT;

        vector<vector<vector<int>>> numVisted;

        //return the number of edges visited
        int64 hyperId = 0;
        deque<int> q;
        set<int> seedSet;

        mt19937 gen;
        uniform_real_distribution<> disR = uniform_real_distribution<>(0, 1);
        uniform_int_distribution<> disI = uniform_int_distribution<>(1, n);


        enum ProbModel {TR, WC, TR001};
        ProbModel probModel;

        InfGraph(string folder, string graph_file):Graph(folder, graph_file){

            random_device rd;  //Will be used to obtain a seed for the random number engine
            gen.seed(rd()); //Standard mersenne_twister_engine seeded with rd()

//            numVisted = gT;
//            for(int i=0; i<n; i++){
//                for(int j=0; j< numVisted[i].size(); j++){
//                    numVisted[i][j] = 0;
//                }
//            }
        }

        void BuildHypergraphR(int64 R, bool limit, int size_limit){

            hyperId=R;

            hyperG.clear();
            for(int i=0; i<n; i++) {
                hyperG.push_back(vector<int>());
            }

            hyperGT.clear();
            while((int)hyperGT.size() <= R) {
                hyperGT.push_back(vector<int>());
            }

            rrsetG.clear();
            while((int)rrsetG.size() <= n) {
                rrsetG.push_back(vector<int>());
            }

            rrsetGT.clear();
            while((int)rrsetGT.size() <= R) {
                rrsetGT.push_back(vector<int>());
            }

            numVisted.clear();
            for(int i = 0; i < (int) gT.size(); i++){
                vector<vector<int>> tmp;
                for(int j = 0; j <(int) gT[i].size(); j++){
                    tmp.push_back(vector<int>());
//                    cout<<i<<j<<endl;
                }
                numVisted.push_back(tmp);
            }

            cout<<"numVisted.size()"<<numVisted.size()<<endl;
//            cout<<"start"<<endl;

            if(limit == true){
                for(int i=0; i<R; i++){
                    BuildHypergraphNode(disI(gen), i, true, true, size_limit);
                }
            }

            if(limit == false){
                for(int i=0; i<R; i++){
                    BuildHypergraphNode(disI(gen), i, true, false, size_limit);
                }
            }

//            int totAddedElement=0;
//            for(int i=0; i<R; i++){
//                for(int t:hyperGT[i])
//                {
//                    hyperG[t].push_back(i);
//                    //hyperG.addElement(t, i);
//                    totAddedElement++;
//                }
//            }

//            cout<<"add hyperG"<<endl;
//            cout<<"rrset_true= "<<rrset_true.size()<<endl;
//            cout<<"rrsetT= "<<rrsetT.size()<<endl;

//            for(int i=0; i<R; i++){
////                cout<<"rrsett = "<<rrsetT[i].size()<<endl;
//                for(int t:rrsetT[i]) {
//                    rrset_true[t].push_back(i);
//                }
//            }

            ASSERT(hyperId == R);

            cout<<"finish buildHypergraphR"<<endl;
        }

        int BuildHypergraphNode(int uStart, int hyperiiid, bool addHyperEdge, bool limit, int size_limit){
            int n_visit_edge=1;
//            cout<<"ustart = "<<uStart<<endl;

            if(addHyperEdge)
            {
                ASSERT((int)hyperGT.size() > hyperiiid);
                hyperGT[hyperiiid].push_back(uStart);
                rrsetGT[hyperiiid].push_back(uStart);
            }

            int n_visit_mark=0;
            //for(int i=0; i<12; i++) ASSERT((int)visit[i].size()==n);
            //for(int i=0; i<12; i++) ASSERT((int)visit_mark[i].size()==n);
            //hyperiiid ++;
            q.clear();
            q.push_back(uStart);
            ASSERT(n_visit_mark < n);

            visit_mark[n_visit_mark++]=uStart;
            visit[uStart]=true;

            cout<<hyperiiid<<endl;

            while(!q.empty()) {
                int expand=q.front();
                q.pop_front();
                if(influModel==IC){
                    int i=expand;

                    for(int j=0; j<(int)gT[i].size(); j++){
                        //int u=expand;
                        int v=gT[i][j];
                        n_visit_edge++;
//                        printf("v = %i\n", v);
                        double randDouble = disR(gen);
//                        cout<<"randDouble = "<<randDouble<<", prob = "<<probT[i][j]<<endl;
                        if(randDouble > probT[i][j])
                            continue;
                        if(addHyperEdge && limit == true && numVisted[i][j].size() > size_limit)
                            continue;
                        if(visit[v]){
                            if(addHyperEdge){
                                ASSERT((int) rrsetGT.size() > hyperiiid);
                                rrsetGT[hyperiiid].push_back(v);
                                ASSERT((int)rrsetG.size() > v);
                                rrsetG[v].push_back(hyperiiid);
                            }
                            continue;
                        }
                        if(!visit[v]){
                            ASSERT(n_visit_mark < visit_mark.size());
                            visit_mark[n_visit_mark++]=v;
//                            printf("v = %i, visit,size = %i", v, int(visit.size()));
                            ASSERT(v < int(visit.size()));
                            visit[v]=true;
                        }
                        q.push_back(v);
                        //#pragma omp  critical 
                        //if(0)
                        if(addHyperEdge){
                            ASSERT((int)hyperG.size() > v);
                            ASSERT((int)rrsetG.size() > v);
                            hyperG[v].push_back(hyperiiid);
                            rrsetG[v].push_back(hyperiiid);
                            ASSERT((int)hyperGT.size() > hyperiiid);
                            ASSERT((int)rrsetGT.size() > hyperiiid);
                            hyperGT[hyperiiid].push_back(v);
                            rrsetGT[hyperiiid].push_back(v);
                            numVisted[i][j].push_back(hyperiiid);
                        }
                    }
                }
                else if(influModel==LT){
                    if(gT[expand].size()==0)
                        continue;
                    ASSERT(gT[expand].size()>0);
                    n_visit_edge+=gT[expand].size();
                    double randDouble = disR(gen);
                    for(int i=0; i<(int)gT[expand].size(); i++){
                        ASSERT( i< (int)probT[expand].size());
                        randDouble -= probT[expand][i];
                        if(randDouble>0)
                            continue;
                        //int u=expand;
                        int v=gT[expand][i];

                        if(visit[v])
                            break;
                        if(!visit[v])
                        {
                            visit_mark[n_visit_mark++]=v;
                            visit[v]=true;
                        }
                        q.push_back(v);
                        if(addHyperEdge)
                        {
                            ASSERT((int)hyperGT.size() > hyperiiid);
                            hyperGT[hyperiiid].push_back(v);
                        }
                        break;
                    }
                }
                else
                    ASSERT(false);
            }
            for(int i=0; i<n_visit_mark; i++)
                visit[visit_mark[i]]=false;
//            cout<<"uStart = "<<uStart<<", n_visit_edge = "<<n_visit_edge<<endl;
            return n_visit_edge;
        }

        void BuildSeedSet(int isExpon, int size_limit) {
            vector< int > degree;
            vector< int> visit_local(hyperGT.size());
            //sort(ALL(degree));
            //reverse(ALL(degree));
            seedSet.clear();
            cout<<"degree: "<<degree.size()<<endl;
            for(int i=0; i<n; i++)
            {
                degree.push_back( hyperG[i].size() );

//                degree.push_back( n * hyperG[i].size()/hyperId );

            }
            ASSERT(k > 0);
            ASSERT(k < (int)degree.size());
            for(int i=0; i<k; i++){
                int id = 0;

                if (isExpon == 1) {
                    double r = disR(gen);
                    id = expoMech(r, size_limit, 0.5, degree);
//                    cout<<"expon_"<<i<<" = "<<id<<","<<"degree = "<< degree[id]<<endl;
                }
                if(isExpon == 0){
                    //Returns an iterator pointing to the element with the largest value in the range [first,last).
                    auto t=max_element(degree.begin(), degree.end());
                    // id is the index of the node with largest degree
                    id = t-degree.begin();
                    cout<<"greedy_"<<i<<" = "<<id<<","<<"degree = "<< degree[id]<<endl;
                }
                seedSet.insert(id);
                cout<<i<<" : "<<id<<","<<"degree = "<< degree[id]<<endl;

                // t is the index of rrset
                for(int t:hyperG[id]){
                    if(!visit_local[t]){
                        visit_local[t]=true;
                        for(int item:hyperGT[t]){
                            degree[item]--;
                        }
                    }
                }
                degree[id]=0;
            }
        }
        double InfluenceHyperGraph(){

            set<int> s;
            cout<<"seedset:" <<seedSet.size()<<endl;
            for(auto t:seedSet){
                for(auto tt:hyperG[t]){
                //for(int index=hyperG.head[t]; index!=-1; index=hyperG.next[index]){
                    //int tt=hyperG.data[index];
                    s.insert(tt);
                }
            }
            double inf=(double)n*s.size()/hyperId;
            return inf;
        }

};



