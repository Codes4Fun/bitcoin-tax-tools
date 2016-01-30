
var CSV = {}

CSV.parse = function (csvText)
{
	var i;

	function skipWhitespaces(text)
	{
		while (i < text.length && text[i] == ' ') { ++i; }
	}

	function getString(text)
	{
		var string = '';
		while (i < text.length && text[i] != ',' && text != '\t' && text[i] != '\n')
		{
			string += text[i];
			++i;
		}
		return string;
	}

	function getQuoteString(text)
	{
		i++;
		var string = '';
		while (i < text.length && text[i] != '"')
		{
			string += text[i];
			++i;
		}
		i++;
		while (i < text.length && text[i] != ',' && text != '\t' && text[i] != '\n')
		{
			string += text[i];
			++i;
		}
		return string;
	}

	var csv = [];
	var line = [];
	for (i = 0; i < csvText.length; ++i)
	{
		skipWhitespaces(csvText);
		if (csvText[i] == '"')
		{
			//console.log('get quoted string');
			line.push(getQuoteString(csvText));
		}
		else
		{
			//console.log('get string');
			line.push(getString(csvText));
		}
		if (i == csvText.length)
		{
			break;
		}
		if (csvText[i] == '\n')
		{
			//console.log('new line');
			csv.push(line);
			line = [];
		}
	}
	return csv;
}

CSV.stringify = function (csv)
{
	var csvText = '';
	for (var i = 0; i < csv.length; i++)
	{
		csvText += '"' + csv[i].join('","') + '"\n';
	}
	return csvText;
}
