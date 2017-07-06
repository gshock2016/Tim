#include <ctime> // Needed for the true randomization
#include "expon.h"

class InfGraph:public Graph
{
    public:
        vector<vector<int>> hyperG;

        vector<vector<int>> rrset_true;
        //VV hyperG;
        vector<vector<int>> hyperGT;

        vector<vector<int>> rrsetT;

//        vector<vector<int>> infmatrix;
        vector<vector<int>> infAdjList;

        vector<vector<int>> numVisted;

        InfGraph(string folder, string graph_file):Graph(folder, graph_file){
            hyperG.clear();
            for(int i=0; i<n; i++)
                hyperG.push_back(vector<int>());
            for(int i=0; i<n; i++)
                rrset_true.push_back(vector<int>());
            for(int i=0; i<n; i++)
                hyperG.push_back(vector<int>());
//            for(int i=0; i<12; i++)
//                sfmt_init_gen_rand(&sfmtSeed, i+1234);
            srand( time(0)); // This will ensure a really randomized number by help of time.

            int xRan=rand()%15+1; // Randomizing the number between 1-15.
//        sfmt_init_gen_rand(&sfmtSeed , 95082);
            sfmt_init_gen_rand(&sfmtSeed , xRan);

            numVisted = gT;
            for(int i=0; i<n; i++){
                for(int j=0; j< numVisted[i].size(); j++){
                    numVisted[i][j] = 0;
                }
            }
        }

        enum ProbModel {TR, WC, TR001};
        ProbModel probModel;

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

            rrsetT.clear();

            while((int)rrsetT.size() <= R) {
                rrsetT.push_back(vector<int>());
            }

            cout<<"size: "<<rrsetT.size()<<endl;

            for (int i = 0; i < n; i++){
//                infmatrix.push_back(vector<int>());
                infAdjList.push_back(vector<int>());
//-------------------------intialize infmatrix -------------------------------
//                for(int j =0; j < R; j++)
//                    infmatrix[i].push_back(0);
            }

            if(limit == true){
                for(int i=0; i<R; i++){
                    BuildHypergraphNode(sfmt_genrand_uint32(&sfmtSeed)%n, i, true, true, size_limit);

                }
            }

            if(limit == false){
                for(int i=0; i<R; i++){
                    BuildHypergraphNode(sfmt_genrand_uint32(&sfmtSeed)%n, i, true, false, size_limit);
                }
            }


            int totAddedElement=0;
            for(int i=0; i<R; i++){
                for(int t:hyperGT[i])
                {
                    //hyperG is the same as infadjlist
                    hyperG[t].push_back(i);
                    //hyperG.addElement(t, i);
                    totAddedElement++;
                }
            }

            for(int i=0; i<R; i++){
                for(int t:rrsetT[i])
                {
                    rrset_true[t].push_back(i);
                }
            }

            ASSERT(hyperId == R);
        }

        int BuildHypergraphNode(int uStart, int hyperiiid, bool addHyperEdge, bool limit, int size_limit){
            int n_visit_edge=1;
            if(addHyperEdge)
            {
                ASSERT((int)hyperGT.size() > hyperiiid);
                hyperGT[hyperiiid].push_back(uStart);
                rrsetT[hyperiiid].push_back(uStart);
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
            while(!q.empty()) {
                int expand=q.front();
                q.pop_front();
                if(influModel==IC){
                    int i=expand;
                    for(int j=0; j<(int)gT[i].size(); j++){
                        //int u=expand;
                        int v=gT[i][j];
/*************************************visted edges***************************************************/
                        n_visit_edge++;

                        double randDouble=double(sfmt_genrand_uint32(&sfmtSeed))/double(RAND_MAX)/2;
                        if(randDouble > probT[i][j])
                            continue;
                        if(limit == true && numVisted[i][j] > size_limit)
                            continue;
                        if(visit[v]){
                            if(addHyperEdge) {
                                ASSERT((int) rrsetT.size() > hyperiiid);
                                rrsetT[hyperiiid].push_back(v);
                            }
                            continue;
                        }
                        if(!visit[v])
                        {
                            ASSERT(n_visit_mark < n);
                            visit_mark[n_visit_mark++]=v;
                            visit[v]=true;
                            numVisted[i][j]++;
                        }
                        q.push_back(v);
                        //#pragma omp  critical 
                        //if(0)
                        if(addHyperEdge)
                        {
                            //hyperG[v].push_back(hyperiiid);
                            ASSERT((int)hyperGT.size() > hyperiiid);
                            hyperGT[hyperiiid].push_back(v);
                            rrsetT[hyperiiid].push_back(v);
                        }
                    }
                }
                else if(influModel==LT){
                    if(gT[expand].size()==0)
                        continue;
                    ASSERT(gT[expand].size()>0);
                    n_visit_edge+=gT[expand].size();
                    double randDouble=double(sfmt_genrand_uint32(&sfmtSeed))/double(RAND_MAX)/2;
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
            return n_visit_edge;
        }


    //return the number of edges visited
        int64 hyperId = 0;
        deque<int> q;
        sfmt_t sfmtSeed;
        set<int> seedSet;

        void BuildSeedSet(int isExpon, int size_limit) {
            vector< int > degree;
            vector< int> visit_local(hyperGT.size());
            //sort(ALL(degree));
            //reverse(ALL(degree));
            seedSet.clear();
            for(int i=0; i<n; i++)
            {
                degree.push_back( hyperG[i].size() );
                //degree.push_back( hyperG.size(i) );
            }
            ASSERT(k > 0);
            ASSERT(k < (int)degree.size());
            for(int i=0; i<k; i++){
                int id = 0;

                if (isExpon == 1) {
                    id = expoMech(size_limit, 0.1, degree);
                    cout<<"expon_"<<i<<" = "<<id<<","<<"degree = "<< hyperG[id].size()<<endl;
                }
                if(isExpon == 0){
                    //Returns an iterator pointing to the element with the largest value in the range [first,last).
                    auto t=max_element(degree.begin(), degree.end());
                    // id is the index of the node with largest degree
                    id = t-degree.begin();
                    cout<<"greedy_"<<i<<" = "<<id<<","<<"degree = "<< hyperG[id].size()<<endl;
                }
                seedSet.insert(id);
                degree[id]=0;
                // t is the index of rrset
                for(int t:hyperG[id]){
                    if(!visit_local[t]){
                        visit_local[t]=true;
                        for(int item:hyperGT[t]){
                            degree[item]--;
                        }
                    }
                }
            }
        }
        double InfluenceHyperGraph(){

            set<int> s;
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



