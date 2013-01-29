#include "BenchMark.h

BenchMark::BenchMark(string n)
{
	st = new StatTracker(true);
	name = n;
}

BenchMark::~BenchMark()
{
	delete st;
	delete &t_start;
	delete &t_finish;
}

void BenchMark::start()
{
	gettimeofday(&t_start, NULL);
}

void BenchMark::finish()
{
	gettimeofday(&t_finish, NULL);
	diff = t_finish.tv_usec - t_start.tv_usec;
	if(diff < 0)
		diff = t_finish.tv_usec + (1000000 - t_start.tv_usec);
	st->newSample(diff);
}

string BenchMark::report()
{
	return st->benchmarkReport(name);
}

void BenchMark::reset()
{
	st->clear();
}


