# gendata

This utility makes compact binary files from the csv files located here:

> https://api.bitcoincharts.com/v1/csv/

It is optimized for quickly acquiring rates for a given timestamp.


## to build:
```
gcc -o gendata gendata.c buffer.c
```

## example generating bitstamp rate files:
```
$ ./gendata .bitstampUSD.2014.csv 1325404800 1357027200 bitcoin.2012.rates
$ ./gendata .bitstampUSD.2014.csv 1357027200 1388563200 bitcoin.2013.rates
$ ./gendata .bitstampUSD.2014.csv 1388563200 1420099200 bitcoin.2014.rates
$ ./gendata .bitstampUSD.2015.csv 1420099200 1451635200 bitcoin.2015.rates
```

