//#define HEAD_TRACE
#define HEAD_INFO

#define HEAD_INFO
//#define HEAD_TRACE
#include "head.h"
#include "memoryusage.h"
#include "timgraph.h"
#include <algorithm>


void readGraphwProb(string rawGraph, string folder, string graph_file){
    vector<int> inDeg;
    vector<vector<int>> gT;

    FILE * fin= fopen(rawGraph.c_str(), "r");
    cout<<rawGraph<<endl;
    ofstream myfile2;
    ofstream myfile1;
    myfile1.open((folder+"attribute.txt").c_str());
    myfile2.open((graph_file).c_str());
    int readCnt=0;

    set<int> nodes;

    random_device rd;  //Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    uniform_real_distribution<> dis(0, 1);

    int a, b;
    char buffer[100];

    while(fgets(buffer, 100, fin)){
//        double p = 0;
//        sscanf(buffer, "%i %i %lf", &a, &b, &p);
        sscanf(buffer, "%i %i", &a, &b);
        nodes.insert(a);
        nodes.insert(b);
        readCnt ++;
    }
    int m = readCnt;
    int n = max(int(nodes.size()), *nodes.rbegin())+1;
//    int n = *nodes.rbegin();
    myfile1<<"n="<<n<<endl;
    myfile1<<"m="<<m<<endl;
    myfile1.close();

    FOR(i, n) {
        inDeg.push_back(0);
        gT.push_back(vector<int>());
    }

    rewind(fin);
    while(fgets(buffer, 100, fin)){
        sscanf(buffer, "%i %i", &a, &b);
        gT[b].push_back(a);
        inDeg[b]++;
    }

    for(int i = 0; i < gT.size(); i++){
        ASSERT(gT[i].size() == inDeg[i]);
        for(int j: gT[i]){
//            double p = 0;
//            p = dis(gen);
            myfile2<<j<<" "<<i<<" "<<1.0/inDeg[i]<<endl;
        }
    }
    myfile2.close();
    fclose(fin);
}

void run(TimGraph & m, string dataset, int k, double epsilon, string model, int isExpon , bool islimit,
         int size_limit, int output){
    cout << "dataste:" << dataset << " k:" << k << " epsilon:"<< epsilon <<   " model:" << model << endl;
    m.k=k;
    if(model=="IC")
        m.setInfuModel(InfGraph::IC);
    else if(model=="LT")
        m.setInfuModel(InfGraph::LT);
    else
        ASSERT(false);

    cout<<"Finish Read Graph, Start Influecne Maximization"<<endl;
    m.EstimateOPT(epsilon, isExpon, islimit, size_limit);
    cout<<"Time used: " << Timer::timeUsed[100]/TIMES_PER_SEC << "s" <<endl;

    cout<<"Selected k SeedSet: ";
    for(auto item:m.seedSet)
        cout<< item << " ";
    cout<<endl;
    cout<<"Estimated Influence: " << m.InfluenceHyperGraph() << endl;
    Counter::show();
    if(output) {
        m.outputRRset();
//        m.outputStat();
    }
}
void parseArg(int argn, char ** argv)
{
    string dataset="";
    string rawdata = "";
    double epsilon=0;
    string model="";
    int k=0;
    int isExpon =0;
    int size_limit = 0;
    bool islimit =false;
    int output = 0;

    for(int i=0; i<argn; i++)
    {
        if(argv[i]==string("-rawdata")) rawdata=string(argv[i+1]);
        if(argv[i]==string("-dataset")) dataset=string(argv[i+1])+"/";
        if(argv[i]==string("-epsilon")) epsilon=atof(argv[i+1]);
        if(argv[i]==string("-k")) k=atoi(argv[i+1]);
        if(argv[i]==string("-expon")) isExpon = atoi(argv[i+1]);
        if(argv[i]==string("-sizelimit")) {
            islimit = true;
            size_limit = atoi(argv[i + 1]);
        }
        if(argv[i]==string("-model")) {
            if(argv[i+1]==string("LT"))
            {
                model=argv[i+1];
            }
            else if(argv[i+1]==string("IC"))
            {
                model=argv[i+1];
            }
            else
                ExitMessage("model should be IC or LT");
        }
        if(argv[i]==string("-output")) output = atoi(argv[i+1]);
    }
    if (dataset=="")
        ExitMessage("argument dataset missing");
    if(rawdata == "") {
        if (k == 0)
            ExitMessage("argument k missing");
        if (epsilon == 0)
            ExitMessage("argument epsilon missing");
    }
    if (model == "")
        ExitMessage("argument model missing");

    string graph_file;
    if(model=="IC")
        graph_file=dataset + "graph_ic.inf";
    else if(model=="LT")
        graph_file=dataset + "graph_lt.inf";

    if(rawdata == "") {
        TimGraph m(dataset, graph_file);
        run(m, dataset, k,  epsilon, model, isExpon, islimit, size_limit, output);
    } else{
        cout<<"raw graph"<<endl;
        readGraphwProb(rawdata, dataset, graph_file);
    }

}


int main(int argn, char ** argv)
{
    OutputInfo info(argn, argv);
    parseArg( argn, argv );
}






