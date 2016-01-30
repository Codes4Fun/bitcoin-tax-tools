
function RatesData(filename, onReady, onError)
{
	var dataView = null;
	var dataHeader = {};
	var scope = this;
	
	this.getDataView = function ()
	{
		return dataView;
	}
	
	this.getDataHeader = function ()
	{
		return dataHeader;
	}

    var xmlHttp = new XMLHttpRequest();
    xmlHttp.open( "GET", filename);
    xmlHttp.responseType = 'arraybuffer';
	xmlHttp.onreadystatechange = function () {
		if (xmlHttp.readyState === 4) {
			if (xmlHttp.status == 200 || xmlHttp.status == 0)
			{
				dataView = new DataView(xmlHttp.response);
				dataHeader.firstDay = dataView.getUint32(0, true);
				dataHeader.numDays = dataView.getUint32(4, true);
				dataHeader.offset = 4 * dataHeader.numDays + 12;
				dataHeader.entryCount = dataView.getUint32(dataHeader.offset - 4, true);

				onReady();
			}
			else
			{
				onError('failed to load "' + filename + '" with status(' + xmlHttp.status + '): ' + xmlHttp.statusText);
			}
		}
	};
    xmlHttp.send();
    
    this.isReady = function ()
    {
		return dataView != null;
    }

	this.getRate = function (ts)
	{
		var day = Math.trunc(ts / 86400) - dataHeader.firstDay;
		if (day >= dataHeader.numDays)
		{
			alert('day (' + day + ') exceeds our range ('+ dataHeader.numDays +')');
			throw RangeError('day (' + day + ') exceeds our range ('+ dataHeader.numDays +')');
		}
		var entry = dataView.getUint32(8 + day * 4, true);
		if (entry >= dataHeader.entryCount)
		{
			alert('entry (' + entry + ') exceeds our range ('+ dataHeader.entryCount +')');
			throw RangeError('entry (' + entry + ') exceeds our range ('+ dataHeader.entryCount +')');
		}
		var offset = dataHeader.offset + entry*8;
		var rate = dataView.getFloat32(offset + 4, true);
		for (var i = entry; i < dataHeader.entryCount; i++, offset+=8)
		{
			if (dataView.getUint32(offset, true) > ts)
			{
				break;
			}
			rate = dataView.getFloat32(offset + 4, true);
		}
		return rate;
		//return parseFloat(rate.toFixed(6));
	}
}

function Rates(years, onAllReady, onError)
{
	var yearRateData = {}
	this.yearRateData = yearRateData;

	function onReady()
	{
		var allReady = true;
		for (var x in yearRateData)
		{
			if (!yearRateData[x].isReady())
			{
				allReady = false;
				break;
			}
		}
		if (allReady)
		{
			onAllReady();
		}
	}

	for (var i = 0; i < years.length; i++)
	{
		var filename = 'bitcoin.' + years[i] + '.rates';
		yearRateData[years[i]] = new RatesData(filename, onReady, onError);
	}

	this.getRates = function (year, timeStamps)
	{
		var rateData = yearRateData[year];
		if (!rateData) throw RangeError('year ' + year + ' out of range');
		var rates = []
		for (var i = 0; i < timeStamps.length; i++)
		{
			rates.push(rateData.getRate(timeStamps[i]));
		}
		return rates;
	}
}
