#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
	size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
	return written;
}

float do_something(char val[]) {
        return atof(val);
}

int main(void) {
	CURL *curl;
	CURLcode res;
	struct curl_slist *list = NULL;

	static const char *bodyfilename = "body.out";
	FILE *bodyfile;

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080/api/rest/salinity");
		
		list = curl_slist_append(list, "Content-Type: application/json");
		list = curl_slist_append(list, "x-hasura-admin-secret: mylongsecretkey");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

		bodyfile = fopen(bodyfilename, "wb");

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, bodyfile);

		curl_easy_perform(curl);

		fclose(bodyfile);

		curl_easy_cleanup(curl);

		bodyfile = fopen(bodyfilename, "r");
		char str[255];
		int i = 0;
		while(fscanf(bodyfile, "%s", str) != EOF) {
			char *t1 = strtok(str, ":");
			char *t2 = strtok(NULL, ":");
			char *t3 = strtok(t2, "}");
			if (i!=0) {
				do_something(t3);
			}
			i++;
		}
		fclose(bodyfile);
	}
	return 0;
}
