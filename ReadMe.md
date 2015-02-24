#ReadMe
This document explains **how to run** the individual spatial temporal mobility model. The entire process is divided into 13 steps. They are names and introduced by the order they should be run. For each step, the **function**ality, input and **output file**s, and **how to run** the code are introduced. We provide two sample **input file**s, *ExtractedStays.txt* and *SampleStayRegion.txt*, so that anyone interested in the model can go through all the steps with the sample data. The code is written in c++ and Matlab. 


##Step1GenerateMoment.cpp
**function**: do moment calculation for home and other location
**input file**: SampleStayRegion.txt
**output file**: MomentCalculation.txt, MeanSDProd.txt
**output format**: 
MomentCalculation.txt
each column is a different moment, each row is a cell size granularity, 
row 1 to 9 are for home location moment
row 10 to 18 are for work location moment
row 19 to 27 are for other location moment
MeanSDProd.txt:
the mean value of the product of the density of home and other location in each cell at each granularity
**how to run**:
g++ -O3 Step1GenerateMoment.cpp -o Step1GenerateMoment
./Step1GenerateMoment > Step1GenerateMoment.txt &


##Step2FractalAnalysis.m
**function**: Use the moment calculation result to calculate sigma for the lognormal cascade, P for the beta cascade, and correlation rho at each level
**input file**: MomentCalculation.txt
**output file**: 
P for the beta cascade: PS, PD
Sigma for the lognormal cascade: SigmaS, SigmaD
Correlation coefficient for the demand and supply: rho


##Step3MomentPlot.m
**function**: Plot the moment measurement
**input file**: MomentCalculation.txt
**output file**: None


##Step4GeneratePoints.m
**function**: generate simulated home and other location using the calibrated parameters from Step 2
input: parameters calibrated in Step2FractalAnalysis.m
**output file**: SimuPointHome.txt, SimuPointOther.txt
**output format**: lat, lon 


##Step5PI.cpp
**function**: calculate the probability to travel outside each tile size
**input file**: SampleStayRegion.txt
**output file**: PI.txt
**output format**: each row is the number of trips within and outside each tile level
**how to run**:
g++ -O3 Step5PI.cpp -o Step5PI
./Step5PI > Step5PI.txt &


##Step6RankProbability.cpp
**function**: calculate the probability to choose each location rank
**input file**: ampleStayRegion.txt
**output file**: RankProbability.txt
**output format**: an array containing the probability to choose each location rank
how to use:
g++ -O3 Step6RankProbability.cpp -o Step6RankProbability
./Step6RankProbability > Step6RankProbability.txt &


##Step7Pr.cpp
**function**: use either real or simulated location to generate trips
**input file**: SimuPointHome.txt SimuPointOther.txt; these files are generated in Step4
**output file**: PrSimu.txt PISimu.txt
how to use:
g++ -O3 Step7Pr.cpp -o Step7Pr
./Step7Pr SimuPointHome.txt SimuPointOther.txt PrSimu.txt PISimu.txt > Step7Pr.txt &


##Step8SpatialPlots.m
**function**: plot P(k), P(r), PI, 
input: results from the three previous steps
**output file**: None


##Step9CalculateDT.cpp
**function**: the temporal model, simulate time intervals and visit loctation ID using calibrated parameters
**input file**: ExtractedStays.txt
**output file**: DTReal.txt, DTSimu.txt, DailyNReal.txt, DailyNSimu.txt, SimuLocInterpolated.txt, DTParameters.txt
**output format**: 
DTReal and DTSimu contain real and simulated stay durations; the first column is the original duration in second, the second column is rescaled duration
DailyNReal and DailyNSimu counts the daily visited number of locations in real and simulated data
SimuLocInterpolated is the result of location ID using the preferential return mechanism
DTParameters contains the calibrated parameters for each person
how to use:
g++ -O3 Step9CalculateDT.cpp -o Step9CalculateDT
./Step9CalculateDT > Step9CalculateDT.txt &


##Step10PN.m
**function**: Calculate the daily visit location number from the analytical model
**input file**: DTParameter.txt PT.txt
**output file**: pn.mat, pn_non1, which are the daily number of visited location distribution with and without parameter beta1.


##Step11FK.cpp
**function**: calculate location visit frequency from real data
**input file**: ExtractedStays.txt
**output file**: "FK"+ thres_min_str + "To" + thres_max_str + ".txt"; 
location visit frequency for user group with total number of locations between thres_min_str and thres_max_str
how to use:
g++ -O3 Step11FK.cpp -o Step11FK
./Step11FK 20 30


##Step12FKSimu.cpp
**function**: calculate location visit frequency from model simulated results
**input file**: SimuLocInterpolated.txt
**output file**: "FKSimu"+ thres_min_str + "To" + thres_max_str + ".txt"; 
location visit frequency for user group with total number of locations between thres_min_str and thres_max_str
how to use:
g++ -O3 Step12FKSimu.cpp -o Step12FKSimu
./Step12FKSimu 20 30 SimuLocInterpolated


##Step13TemporalPlots.m
**function**: Plot P(N), P(DT), beta1 beta2 distribution, f(k)
**input file**: results from the previous 4 steps
**output file**: none

