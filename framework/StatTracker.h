#ifndef STATTRACKER_H
#define STATTRACKER_H

#include <math.h>
#include <map>
#include <vector>
#include <set>
#include <string>
#ifdef WIN32
#include <common/Synchronization.h>
#else
#include <sys/time.h>
#endif
using namespace std;

class StatTracker
{
public:
	enum Type{AVERAGE, MINIMUM, MAXIMUM, STDDEV, SAMPLE, UPDATES, LAST};
    StatTracker(bool iD = true);
    StatTracker(double dN, string smin, string smax, string savg, string ssdev)
        { setStats(dN, smin, smax, savg, ssdev); }

    StatTracker(double dN, double dmin, double dmax, double davg, double dsdev)
        { setStats(dN, dmin, dmax, davg, dsdev); }
	string benchmarkReport(string& name);
    void setStats(double dN, string smin, string smax, string savg, string ssdev);
    void setStats(double dN, double dmin, double dmax, double davg, double dsdev);
    StatTracker();
    ~StatTracker(){
    };
    void newSample(double x);
    void newSample(string x);
    double getAvg();
    double getMin() { if(dummy) return dummyMin; else return min; }
    double getMax() { if(dummy) return dummyMax; else return max; }
    double getStdDev();
	double getDoubleValue();
    void getStrings(vector<string>& vs);
    bool isDouble;
    double getUpdates(){ if(dummy) return dummyN; else return N; }
	double getAllUpdates();
    void clear();
    bool dummy;
    double getN(){ return N; }
    double getLast();
    double getLastFound();
    double getStamp() const;
	void setType(string t);
private:
	double allUpdates;
    double lastFound;
    double min;
    double last;
    double max;
    double sum;
    double sumsq;
    double N;
    double dummyMin;
    double dummyMax;
    double dummyAvg;
    double dummyStd;
    double dummyN;
	StatTracker::Type type;
    map<string, double> recur;
    vector<string> dummyStr;
};

#endif

