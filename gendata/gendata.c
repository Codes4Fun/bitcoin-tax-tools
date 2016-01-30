#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include "buffer.h"

#define MAX_STRING 4096


float stof(const char* s){
  float rez = 0, fact = 1;
  if (*s == '-'){
    s++;
    fact = -1;
  };
  int point_seen;
  for (point_seen = 0; *s; s++){
    if (*s == '.'){
      point_seen = 1; 
      continue;
    };
    int d = *s - '0';
    if (d >= 0 && d <= 9){
      if (point_seen) fact /= 10.0f;
      rez = rez * 10.0f + (float)d;
    };
  };
  return rez * fact;
};

void printUsage()
{
	printf("usage: %s <inputfile.csv> <starting TS> <ending TS> <outputname>\n");
}

float parseRate(char * str)
{
	char * szRate = strstr(str, ",") + 1;
	*strstr(szRate, ",") = 0;
	return stof(szRate);
}

int main(int argc, char ** argv)
{
	// gendata inputfile startTS endTS outputfile
	if (argc < 5)
	{
		printUsage();
		return 0;
	}

	uint32_t startTs = atoi(argv[2]);
	uint32_t endTs = atoi(argv[3]);
	if (startTs > endTs)
	{
		printUsage();
	}

	FILE * fin = fopen(argv[1], "r");
	if (!fin)
	{
		printf("unable to open \"%s\" for reading\n", argv[1]);
		return 0;
	}

	FILE * fout = fopen(argv[4], "wb");
	if (!fout)
	{
		printf("unable to open \"%s\" for writing\n", argv[4]);
		return 0;
	}

	char str[MAX_STRING];

	struct Entry
	{
		uint32_t ts;
		float rate;
	} entry, prev_entry;

	struct Buffer buffer;
	new_buffer(&buffer);
	buffer_grow(&buffer, 256 * 1024 * 1024);

	int has_prev = 0;

	while (fgets(str, MAX_STRING, fin))
	{
		uint32_t ts = atoi(str);
		if (ts < startTs)
		{
			prev_entry.ts = ts;
			prev_entry.rate = parseRate(str);
			has_prev = 1;
			continue;
		}
		if (ts > endTs)
		{
			break;
		}
		if (has_prev)
		{
			// we need the entry before our first timestamp, to carry over the rate.
			buffer_append(&buffer, (char*)&prev_entry, sizeof(prev_entry));
			has_prev = 0;
		}
		entry.ts = ts;
		entry.rate = parseRate(str);
		if (entry.rate == 0.0)
		{
			printf("\nERR: rate %f at %d %s\n", entry.rate, entry.ts);
			assert(0);
		}
		buffer_append(&buffer, (char*)&entry, sizeof(entry));
	}

	if (buffer.length == 0)
	{
		printf("no entries found for the time range\n");
		return 0;
	}

	printf("parsed csv, now removing entries with the same timestamp\n");

	// get the last rate of simultanious exchanges.
	// this is causing me grief because I found the order has changed in newer csv files from blockchain.info
	int entry_count = buffer.length / sizeof(struct Entry);
	struct Entry * entries = (struct Entry*) buffer.data;
	struct Buffer buffer2;
	new_buffer(&buffer2);
	int dupts = 0;
	int dupc = 0;
	int peak = 0;
	int unic = 0;
	dupts = entries[0].ts;
	dupc = 1;
	entry.ts = entries[0].ts;
	entry.rate = entries[0].rate;
	int i;
	for (i = 1; i < entry_count; i++)
	{
		if (entries[i].ts == dupts)
		{
			dupc++;
			entry.rate = entries[i].rate; // use the last rate, it will carry over
		}
		else
		{
			if (dupc > peak) peak = dupc;
			// store previous entry
			buffer_append(&buffer2, (char*)&entry, sizeof(entry));
			unic++;
			// setup new entry
			dupts = entries[i].ts;
			dupc = 1;
			entry.ts = entries[i].ts;
			entry.rate = entries[i].rate;
		}
	}
	if (dupc > peak) peak = dupc;
	// store previous entry
	buffer_append(&buffer2, (char*)&entry, sizeof(entry));
	unic++;
	// log
	printf("peak duplicated ts %d\n", peak);
	printf("unique ts %d of %d (%d)\n", unic, entry_count, buffer2.length/sizeof(entry));

	entry_count = buffer2.length / sizeof(struct Entry);
	entries = (struct Entry*)buffer2.data;

	// create LUT (look up table)
	uint32_t firstday = (entries[0].ts/86400);
	uint32_t num_days = (entries[entry_count-1].ts/86400) - (entries[0].ts/86400) + 2;
	int * days = malloc(num_days * 4);
	memset(days, -1, num_days * 4);
	days[0] = 0;
	for (i = 1; i < entry_count; i++)
	{
		// the last trade of the day should be the first one of tomorrow. +1
		uint32_t day_index = entries[i].ts/86400 - firstday + 1;
		assert(day_index < num_days);
		days[day_index] = i;
	}
	// carry forward missing days
	for (i = 1; i < num_days; i++)
	{
		if (days[i] == -1)
		{
			days[i] = days[i-1];
		}
	}
	// save LUT
	fwrite(&firstday, sizeof(firstday), 1, fout);
	fwrite(&num_days, sizeof(num_days), 1, fout);
	fwrite(days, 4, num_days, fout);

	// save entries
	i = buffer2.length/8;
	fwrite(&i, sizeof(i), 1, fout);
	fwrite(entries, sizeof(struct Entry), entry_count, fout);
	delete_buffer(&buffer2);
	delete_buffer(&buffer);

	fclose(fout);
	fclose(fin);

	return 0;
}
