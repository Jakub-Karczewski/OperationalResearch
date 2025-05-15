# BO2025
This repository is from the operational research course at AGH, the problem was defined as follows:  
We have packages and vehicles with sizes for each dimension, each vehicle was its own weight limit and each package has its cost and weight.  
Our goal is to fill the vehicles with packages such that any two packages do not overlap and also we have the constraint, that the area below each package has to either be filled with other  packages or it must be floor. We are also not allowed to rotate packages.  
From all solutions we choose the one with the highest cost - sum of the costs from each package that we managed to fit.  
We use genetic algorithm with the greedy heuristic and for the cases in which the summary volume of all vehicles is not great and the number of packages is also low, we can use the brute force, similar to exponential knapsack problem.
