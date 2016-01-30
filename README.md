# Open Source Bitcoin Tax Tools

These tools are used to help acquire and generate tax documents for BTC holdings.


## privacy

While not necessary, to get the most privacy, it is recommend you run your own insight server node:

https://github.com/bitpay/insight-api

From there you will be able to download transaction data privately.


## installation

You can run it locally using node.js/npm, pull the files from github or download and uncompress them and then from command line:

1. npm install -g grunt-cli
2. npm install
3. grunt

That will launch a local webserver on port 8000.


## usage

The workflow for using these tools consists of:

1) Gather and convert the necessary data. You might have to create your own transaction csv files for your virtual wallets.

tool.00.coinbase.html - conversion tool from coinbase csv files to a processed transaction csv.

tool.00.insight.html - generates links to download json transaction data of bitcoin addresses from insight API based servers.

2) generate input and output csv files. The input.csv shows acquired bitcoins, and output.csv spent bitcoins.

tool.01.generateIO.insight.html - takes transaction csv files and insight API json files and produces BTC input/output csv files.

3) finally you use input.csv and output.csv to generate tax files for the year.

tool.02.report.html - generates tax reports, TXF and a txt file with income/cost/gains information.


## CSV transaction format

You will need to create this for virtual wallets where you don't have a real bitcoin address.

These files have no headers but each column consists of:

"timestamp", "BTC delta", "exchange rate", "transaction id", "source"

where:
 - timestamp - is POSIX time, seconds passed since January 1, 1970.
 - BTC delta - this is how much was gained or lost in BTC in the transaction.
 - exchange rate - this was the exchange rate at the time of exchange. If you sold 2 BTC for 400, this should be 200 (400/2).
 - transaction id - this is the bitcoin transaction id, knowing this allows wallet to wallet transactions to not be counted as a sale/buy.
 - source - this is just for debugging purposes, it lets you know where this transaction came from (a coinbase csv or a bitcoin address, etc).


## Bitcoin exchange rate data

This is bitstamp exchange rates from 2013 to 2015 and is generated using the tool gendata.


## Some History

In 2014 I created my initial tools in python, but not wanting to take a risk of messing something up I payed an online site to do my initial txf documents.
Their tax reports showed I had higher gains so I used their results instead of my own out of fear of underpaying, but later when I looked at the document they produced it made no sense, some values made much higher then they were supposed to and others made much lower, it almost seemed intentionally obfuscated.

In 2015 I ported my tools to javascript and in so doing found a small bug in my python code, as well as found that python had precision issues. But the code was a messy mashup.

Now I have simplified and reviewed the code and am releasing it for others to review and use.

## Tip Me!

[17Mqj42oV1xtMWt16MBPyveZZXKUaJFH1H]

[17Mqj42oV1xtMWt16MBPyveZZXKUaJFH1H]:https://blockchain.info/address/17Mqj42oV1xtMWt16MBPyveZZXKUaJFH1H