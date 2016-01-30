## gendata

This utility makes faster and more compact binary files from the csv files located here:
https://api.bitcoincharts.com/v1/csv/


It removes duplicate entries (trades that happend in the same second) to save space.


to build:
gcc -o gendata gendata.c buffer.c


example:
$ ./gendata .bitstampUSD.2014.csv 1325404800 1357027200 bitcoin.2012.rates
$ ./gendata .bitstampUSD.2014.csv 1357027200 1388563200 bitcoin.2013.rates
$ ./gendata .bitstampUSD.2014.csv 1388563200 1420099200 bitcoin.2014.rates
$ ./gendata .bitstampUSD.2015.csv 1420099200 1451635200 bitcoin.2015.rates


