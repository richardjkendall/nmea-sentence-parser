///////////////////////////////
// based on https://github.com/craigpeacock/NMEA-GPS/blob/master/gps.c
///////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct GpsTime {
  int hour;
  int minute;
  int second;
};

int parse_comma_delimited_str(char *string, char **fields, int max_fields) {
	int i = 0;
	fields[i++] = string;

	while ((i < max_fields) && NULL != (string = strchr(string, ','))) {
		*string = '\0';
		fields[i++] = ++string;
	}

	return --i;
}

int nmea_sentence_to_time(char *sentence, struct GpsTime *time) {
  char *field[20];
  
  // parse sentence
  int token_count = parse_comma_delimited_str(sentence, field, 20);
  
  // does sentence have 11 tokens?
  if(token_count == 11) {
    printf("Sentence has 11 tokens\r\n");
    // does the first token match the expected value of $GPRMC
    if(strcmp(field[0], "$GPRMC") == 0) {
      printf("Sentence matches expectations\r\n");
      // get the time field and split it
      char hour[3];
      char min[3];
      char sec[3];
      strncpy(hour, &field[1][0], 2);
      strncpy(min, &field[1][2], 2);
      strncpy(sec, &field[1][4], 2);
      hour[2] = '\0';
      min[2] = '\0';
      sec[2] = '\0';
      printf("Time %s:%s:%s\r\n",hour, min, sec);
      // convert to ints and put into the struct
      int ihour = atoi(hour);
      int imin = atoi(min);
      int isec = atoi(sec);
      // put into struct to return
      struct GpsTime t;
      t.hour = ihour;
      t.minute = imin;
      t.second = isec;
      *time = t;
      return 1;
    } else {
      return 0;
    }
  } else {  
    return 0;
  }
}

int main() {
  char str[] = "$GPRMC,225446,A,4916.45,N,12311.12,W,000.5,054.7,191194,020.3,E*68";
  char *field[20];

  struct GpsTime time;
  int result = nmea_sentence_to_time(str, &time);
  if(result) {
    printf("Time from parsing %d:%d:%d\r\n", time.hour, time.minute, time.second);
  } else {
    printf("No time came back from parsing\r\n");
  }

  return 0;
}