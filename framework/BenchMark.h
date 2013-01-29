#ifndef BENCH_MARK_H
#define BENCH_MARK_H

#include <framework/StatTracker.h>
#include <sys/time.h>

#ifdef __BENCHMARK

#define BENCHMARK_INIT(BMNAME, BMID) BenchMark* BMNAME = new BenchMark(BMID);
#define BENCHMARK_START(BMNAME) BMNAME->start();
#define BENCHMARK_FINISH(BMNAME) BMNAME->finish();
#define BENCHMARK_GET_REPORT(BMNAME) BMNAME->report();
#define BENCHMARK_REPORT(BMNAME) cout << BMNAME->report();
#define BENCHMARK_RESET(BMNAME) BMNAME->reset();
#define BENCHMARK_CONCLUDE(BMNAME) \
	BMNAME->finish(); \
	cout << BMNAME->report(); \
	BMNAME->reset();

#else

#define BENCHMARK_INIT(BMNAME, BMID)
#define BENCHMARK_START(BMNAME)
#define BENCHMARK_FINISH(BMNAME)
#define BENCHMARK_REPORT(BMNAME)
#define BENCHMARK_RESET(BMNAME)
#define BENCHMARK_CONCLUDE(BMNAME)

#endif

class BenchMark
{
public:
	BenchMark(string n);
	~BenchMark();
	void start();
	void finish();
	string report();
	void reset();
private:
    struct timeval t_start;
    struct timeval t_finish;
    StatTracker* st;
	double diff;
	string name;
};
#endif
