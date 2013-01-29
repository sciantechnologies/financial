#include "StatTracker.h"
#include <iostream>
#include <sstream>

string StatTracker::benchmarkReport(string& name)
{
    stringstream ss;
    ss << name << endl
    << "Average processing time:\t"<< getAvg()<< endl
    << "Minimum processing time:\t" << getMin() << endl
    << "Maximum processing time:\t" << getMax() << endl
    << "Number of updates received:\t" << getUpdates()
    << endl;
    return ss.str();
}


StatTracker::StatTracker(bool iD)
{
    clear();
    isDouble = iD;
    dummy = false;
    lastFound = 0;
	allUpdates = 0;
}

void StatTracker::clear()
{
    min = 0;
    max = 0;
    sum = 0;
    sumsq = 0;
	allUpdates += N;
    N = 0;
    dummyN = 0;
    last = 0;
    dummyStr.clear();
	recur.clear();
}

double StatTracker::getAllUpdates()
{
	if(allUpdates == 0)
		return N;
	return allUpdates;
}

void StatTracker::setStats(double dN, string smin, string smax, string savg, string ssdev)
{
    clear();
    isDouble = false;
    dummy = true;
    dummyStr.push_back(smin);
    dummyStr.push_back(smax);
    dummyStr.push_back(savg);
    dummyStr.push_back(ssdev);
    dummyN = dN;
}

void StatTracker::setStats(double dN, double dmin, double dmax, double davg, double dsdev)
{
    clear();
    isDouble = true;
    dummy = true;
    dummyMin = dmin;
    dummyMax = dmax;
    dummyAvg = davg;
    dummyStd = dsdev;
    dummyN = dN;
}

double StatTracker::getDoubleValue()
{
	switch(type)
	{
	case StatTracker::AVERAGE:
		return getAvg();
		break;
	case StatTracker::MINIMUM:
		return getMin();
		break;
	case StatTracker::MAXIMUM:
		return getMax();
		break;
	case StatTracker::STDDEV:
		return getStdDev();
		break;
	case StatTracker::SAMPLE:
		return getLast();
		break;
	case StatTracker::UPDATES:
		return getUpdates();
		break;
	case StatTracker::LAST:
		return getLastFound();
		break;
	}
	return -1;
}

double StatTracker::getLast()
{
	return last;
}

void StatTracker::newSample(double x)
{
    N++;
    last = x;
    if(x > max || N == 1)
    {
      max = x;
    }
    if(x < min || N == 1)
    {
      min = x;
    }

    sum += x;
    sumsq += x*x;
    lastFound = getStamp();
}

double StatTracker::getAvg()
{
    if(dummy)
      return dummyAvg;
    if(N > 0)
      return (sum / N);
    else
      return 0;
}

double StatTracker::getStdDev()
{
    if(dummy)
      return dummyStd;
    if(N > 0)
    {
      double ssoN = (sum*sum / N);
      if(ssoN >= sumsq)
          return 0;
      else
      {
          double variance = (sumsq - ssoN ) / N;
          return sqrt ( variance );
      }
    }
    else
      return 0;
}

void StatTracker::newSample(string x)
{
    N++;
    if(recur.find(x) == recur.end())
        recur.insert(pair<string,double>(x,1));
    else
        recur.find(x)->second++;
    lastFound = getStamp();
}

double StatTracker::getLastFound()
{
    return getStamp() - lastFound;
}

void StatTracker::setType(string t)
{
	if(t == "max")
		type = StatTracker::MAXIMUM;
	else if(t == "min")
		type = StatTracker::MINIMUM;
	else if(t == "avg")
		type = StatTracker::AVERAGE;
	else if(t == "std")
		type = StatTracker::STDDEV;
	else if(t == "sam")
		type = StatTracker::SAMPLE;
	else if(t == "upd")
		type = StatTracker::UPDATES;
	else if(t == "last")
		type = StatTracker::LAST;
	else
		type = StatTracker::AVERAGE;
}


double StatTracker::getStamp() const
{
    struct timeval stamp_time;
    gettimeofday(&stamp_time, 0);
    double s = stamp_time.tv_sec;
    double u = double(stamp_time.tv_usec) / 1000000;
    return (s + u);
}

void StatTracker::getStrings(vector<string>& vs)
{
    int i;
    if(dummy)
    {
        for(size_t s = 0; s < dummyStr.size(); s++)
            vs.push_back(dummyStr[s]);
        return;
    }
    double gamax = 0;

    map<string,double>::iterator it;
    set<double> gsset;
    // Fill gsset with a set of counts
    for(it = recur.begin(); it != recur.end(); it++)
    {
        gsset.insert(it->second);
    }
    // reverse iterator so it does the biggest one first
    for(set<double>::reverse_iterator rit = gsset.rbegin(); rit != gsset.rend(); rit++)
    {
        for(it = recur.begin(); it != recur.end(); it++)
        {
            if(it->second == (*rit) && vs.size() < 6)
                vs.push_back(it->first);
        }
    }
}


