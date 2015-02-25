#ReadMe
This document explains how to run the individual spatial temporal mobility model from the paper "A Fundamental Model of Individual Mobility to Generate Patterns of Daily Trajectories". The entire modeling process is divided into 13 sub scripts. They are named and introduced by the order they should be run. For each step, the functionality, input and output files, and how to run the code are introduced. We provide two sample input files, *ExtractedStays.txt* and *SampleStayRegion.txt*, so that anyone interested in the model can go through all the steps with the sample data. The code is written in c++ and Matlab. 


##Step1GenerateMoment.cpp
**function**: perform moment calculation for home and other location    <br />
**input file**: SampleStayRegion.txt      <br />
**output file**: MomentCalculation.txt, MeanSDProd.txt      <br />
**output format**:       <br />
MomentCalculation.txt      <br />
each column is a different moment, each row is a cell size granularity,     <br />
row 1 to 9 are for home location moment    <br />
row 10 to 18 are for work location moment    <br />
row 19 to 27 are for other location moment    <br />
MeanSDProd.txt:    <br />
the mean value of the product of the density of home and other location in each cell at each granularity    <br />
**how to run**:    <br />
g++ -O3 Step1GenerateMoment.cpp -o Step1GenerateMoment    <br />
./Step1GenerateMoment > Step1GenerateMoment.txt &    <br />


##Step2FractalAnalysis.m
**function**: Use the moment calculation result to calculate sigma for the lognormal cascade, P for the beta cascade, and correlation rho at each level     <br />
**input file**: MomentCalculation.txt     <br />
**output file**:     <br />
P for the beta cascade: PS, PD    <br />
Sigma for the lognormal cascade: SigmaS, SigmaD    <br />
Correlation coefficient for the demand and supply: rho    <br />


##Step3MomentPlot.m
**function**: Plot the moment measurement    <br />
**input file**: MomentCalculation.txt    <br />
**output file**: None    <br />


##Step4GeneratePoints.m
**function**: generate simulated home and other location using the calibrated parameters from Step 2    <br />
input: parameters calibrated in Step2FractalAnalysis.m    <br />
**output file**: SimuPointHome.txt, SimuPointOther.txt    <br />
**output format**: lat, lon     <br />


##Step5PI.cpp
**function**: calculate the probability to travel outside each tile size    <br />
**input file**: SampleStayRegion.txt    <br />
**output file**: PI.txt    <br />
**output format**: each row is the number of trips within and outside each tile level    <br />
**how to run**:    <br />
g++ -O3 Step5PI.cpp -o Step5PI    <br />
./Step5PI > Step5PI.txt &    <br />


##Step6RankProbability.cpp
**function**: calculate the probability to choose each location rank    <br />
**input file**: ampleStayRegion.txt    <br />
**output file**: RankProbability.txt    <br />
**output format**: an array containing the probability to choose each location rank    <br />
**how to run**:    <br />
g++ -O3 Step6RankProbability.cpp -o Step6RankProbability    <br />
./Step6RankProbability > Step6RankProbability.txt &    <br />


##Step7Pr.cpp
**function**: use either real or simulated location to generate trips    <br />
**input file**: SimuPointHome.txt SimuPointOther.txt; these files are generated in Step4    <br />
**output file**: PrSimu.txt PISimu.txt    <br />
**how to run**:    <br />
g++ -O3 Step7Pr.cpp -o Step7Pr    <br />
./Step7Pr SimuPointHome.txt SimuPointOther.txt PrSimu.txt PISimu.txt > Step7Pr.txt &    <br />


##Step8SpatialPlots.m
**function**: plot P(k), P(r), PI,     <br />
input: results from the three previous steps    <br />
**output file**: None    <br />


##Step9CalculateDT.cpp
**function**: the temporal model, simulate time intervals and visit loctation ID using calibrated parameters    <br />
**input file**: ExtractedStays.txt    <br />
**output file**: DTReal.txt, DTSimu.txt, DailyNReal.txt, DailyNSimu.txt, SimuLocInterpolated.txt, DTParameters.txt    <br />
**output format**:     <br />
DTReal and DTSimu contain real and simulated stay durations; the first column is the original duration in second, the second column is rescaled duration    <br />
DailyNReal and DailyNSimu counts the daily visited number of locations in real and simulated data    <br />
SimuLocInterpolated is the result of location ID using the preferential return mechanism    <br />
DTParameters contains the calibrated parameters for each person    <br />
**how to run**:    <br />
g++ -O3 Step9CalculateDT.cpp -o Step9CalculateDT    <br />
./Step9CalculateDT > Step9CalculateDT.txt &    <br />


##Step10PN.m
**function**: Calculate the daily visit location number from the analytical model    <br />
**input file**: DTParameter.txt PT.txt    <br />
**output file**: pn.mat, pn_non1, which are the daily number of visited location distribution with and without parameter beta1.    <br />


##Step11FK.cpp
**function**: calculate location visit frequency from real data    <br />
**input file**: ExtractedStays.txt    <br />
**output file**: "FK"+ thres_min_str + "To" + thres_max_str + ".txt";     <br />
location visit frequency for user group with total number of locations between thres_min_str and thres_max_str    <br />
**how to run**:    <br />
g++ -O3 Step11FK.cpp -o Step11FK    <br />
./Step11FK 20 30    <br />


##Step12FKSimu.cpp
**function**: calculate location visit frequency from model simulated results    <br />
**input file**: SimuLocInterpolated.txt    <br />
**output file**: "FKSimu"+ thres_min_str + "To" + thres_max_str + ".txt";     <br />
location visit frequency for user group with total number of locations between thres_min_str and thres_max_str    <br />
**how to run**:    <br />
g++ -O3 Step12FKSimu.cpp -o Step12FKSimu    <br />
./Step12FKSimu 20 30 SimuLocInterpolated    <br />


##Step13TemporalPlots.m
**function**: Plot P(N), P(DT), beta1 beta2 distribution, f(k)    <br />
**input file**: results from the previous 4 steps    <br />
**output file**: none    <br />

