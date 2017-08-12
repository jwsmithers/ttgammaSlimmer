# ttgammaSlimmer
Code to slim ntuples for the ttgamma analysis.

I.e., take CR1's and turns them into SR'1.

# Usage
`source setup.sh`

`mkdir build ; cd build`

`cmake ../`

`make ttgammaSlimmer`

`./ttgammaSlimmer <someinput>.root`


Alternatively, use the batch system (see under `scripts/`). 
Note: The env has to be setup before hand (i.e. `source setup.sh`).

Remember to adjust cuts as needed.
