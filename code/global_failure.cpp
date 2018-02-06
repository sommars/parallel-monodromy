#include "global_failure.h"

//------------------------------------------------------------------------------
double FindGlobalFailurePoint(HomotopyTestSetup Setup, double Precision, bool Verbose)
{
	// For now, I'm hardcoding in a number of trials, 10
	// Furthermore, I'm defining global failure to happen if the graph fails
	// to find all solutions 5 times or more.

	throw invalid_argument("This function is not currently supported.");	
	
	Setup.ThreadCount = 1;
	double ProbabilityOfFailure = .5;

	double Floor = 0;
	double Ceiling = 1;
	while (true)
	{
		AverageHomotopyData Data = AverageTests(&Setup, false);
		double NewProb;
		if (Data.ExistsCompleteNode > 0.5)
		{
			NewProb = (ProbabilityOfFailure + Ceiling) / 2;
			Floor = ProbabilityOfFailure;
		} else
		{
			NewProb = (ProbabilityOfFailure + Floor) / 2;
			Ceiling = ProbabilityOfFailure;
		}
		if ((abs(NewProb - ProbabilityOfFailure)) < Precision)
			break;
		ProbabilityOfFailure = NewProb;
		if (Verbose)
			cout << "Current probability of failure:" << ProbabilityOfFailure << endl;
	};
	
	return ProbabilityOfFailure;
};

