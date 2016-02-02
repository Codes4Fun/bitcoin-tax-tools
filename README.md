# Open Source Bitcoin Tax Tools

Use these tools to generate a TXF document that can be imported into tax software.

 - currently only supports FIFO.
 - uses bitstamp exchange rates which can be changed to other exchanges if run locally.

These are not guaranteed to be without risk, or even to give you the lowest tax rate. You should go to a tax professional or [online services](https://en.bitcoin.it/wiki/Tax_compliance#Accounting_and_Tax_Compliance_Software) if you need that, but even if you do that consider using these tools a supplement, not just to double check your taxes but also to double check these tools.

## privacy

These tools do not ever upload your personal data, but they allow you to download data from public servers and that data can be linked to your IP address.

While not necessary to use these tools, if you want the most privacy, it is recommended you run your own insight server node:

> https://github.com/bitpay/insight-api

After you have downloaded all the bitcoin blocks (which can take some time depending on performance of your computer and internet) you will be able to download transaction data privately.

And since you will have created a node you can also help decentralize the network by keeping the node running :)

## workflow

1) Gather transactions.
 - For bitcoin wallets with addresses, generate download links for address transactions (in json) using this [**tool**](tool.00.insight.html).
  - Here it is useful to have a private insight server but public servers are listed in the tool as well.
  - Many wallets have hidden addresses for dealing with change, you should probably review the transactions to look for this and include these addresses.
    + There is an xpub key search [**tool**](tool.00.xpub.insight.html), that will find your hidden/change addresses in wallets. You can export from the Android Bitcoin Wallet via 'Settings'>'Diagnostics'>'Show xpub' which allows you to "Share" to a file.
 - For virtual wallets the tools use their own [transaction csv format](#transaction).
  - If you use coinbase you can convert their transaction csv files with this [**tool**](tool.00.coinbase.html).
  - For other virtual wallets you will need to either
    + Use this [**tool**](tool.00.transaction.csv.html) to manual create these csv files.
    + develop your own automated conversion tool to this [transaction csv format](#transaction).
    + or [request](https://github.com/Codes4Fun/bitcoin-tax-tools/issues) for a conversion tool to be made.

2) Generate input.csv and output.csv files with this [**tool**](tool.01.generateIO.insight.html).
 - input.csv contains all transactions for acquired BTC.
 - output.csv contains all transactions of spent BTC.

3) For a given year generate TXF and compute gains with this [**tool**](tool.02.report.html).
 - taxes.txf a document that can be imported into tax software.
 - taxes.txt a document showing your years income, cost, and gains.
 - taxes.csv alternative format of TXF.

4) Import TXF file into your tax software.
 - TurboTax Online does not support TXF files. If you want to use TurboTax, use TurboTax Premier as it supports investments and will generate the necessary form from the TXF.

## misc tools

Here is a [**csv viewer**](tool.03.csv.viewer.html), use it to examine generated csv files.

Here is a [**balance checker**](tool.03.balance.check.html), use it on grouped json/csv files to see if the balances correspond to wallets.

## <a name="transaction"></a>transaction CSV format

You will need to create this for virtual wallets where you don't have a real bitcoin address.

There is a [tool](tool.00.coinbase.html) for automated conversion of coinbase csv files to this format.

If there are other data formats you think should be automated make it an [issue](https://github.com/Codes4Fun/bitcoin-tax-tools/issues)! Or write code and submit a patch!

There is a [tool](tool.00.transaction.csv.html) to help create/edit these transaction csv files.

These files have no headers but each column consists of:

"timestamp", "BTC delta", "exchange rate", "transaction id", "source"

(Note the necessity of quotes around each column value)

where:
 - **timestamp** - is Unix time, seconds passed since January 1, 1970 UTC.
 - **BTC delta** - this is how much was gained or lost in BTC in the transaction.
 - **exchange rate** - this was the exchange rate at the time of exchange. If you sold 2 BTC for 400, this should be 200 (400/2). If you don't know the rate place double quotes "", and it will pull from the rates binary.
 - **transaction id** - this is the bitcoin transaction id, knowing this allows wallet to wallet transfers to not be counted as a sale/buy (though the fee will be counted).
 - **source** - this is just for debugging purposes, it lets you know where this transaction came from (a coinbase csv or a bitcoin address, etc).

## exchange rate data

The current exchange rates come from bitstamp and cover from 2013 to 2015.

The rate binaries are generated using a natively built tool in the [gendata](https://github.com/Codes4Fun/bitcoin-tax-tools/tree/gh-pages/gendata) directory.

You can use that tool to create rates for other exchanges, the tool has a readme!

To use your own built rates you then need to run it locally and replace the rate files with your own.

## run it locally!

You can run it locally using node.js/npm, use git to clone from github or [download](https://github.com/Codes4Fun/bitcoin-tax-tools/archive/gh-pages.zip) and uncompress them and then from command line:

```
npm install -g grunt-cli
npm install
grunt
```

That will launch a local webserver on port 8000.

Edit Gruntfile.js to change the port.

## some history

 - In 2014 I created my initial tools in python, but not wanting to take a risk of messing something up I payed an online site to do my initial txf documents.
Their tax reports showed I had higher gains so I used their results instead of my own out of fear of underpaying, but later when I looked at the document they produced it made no sense, some values made much higher then they were supposed to and others made much lower, it almost seemed intentionally obfuscated.

 - In 2015 I ported my tools to javascript and in so doing found a small bug in my python code, as well as found that python had precision issues. But the code was a messy mashup.

 - Now (2016) I have simplified and reviewed the code more and releasing it for others to review and use.

## links

source code : https://github.com/Codes4Fun/bitcoin-tax-tools

report bugs, feature requests, etc : https://github.com/Codes4Fun/bitcoin-tax-tools/issues

create bitcoin bounties on those features or bug fixes : https://www.bountysource.com/

tips can go here : [17Mqj42oV1xtMWt16MBPyveZZXKUaJFH1H](https://blockchain.info/address/17Mqj42oV1xtMWt16MBPyveZZXKUaJFH1H)