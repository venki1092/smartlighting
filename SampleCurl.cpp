#include <stdio.h>
#include <iostream>
#include <string>
#include <curl/curl.h> //your directory may be different
#include "Json_Parser.h"
#include <string.h>
#include <stdlib.h>

using namespace std;

string data; //will hold the url's contents
int PublishMessage(char *ipaddress, char *topicname , char * messagecontent , int msgconlen);
size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up)
{ //callback must have this declaration
    //buf is a pointer to the data that curl has for us
    //size*nmemb is the size of the buffer

    for (int c = 0; c<size*nmemb; c++)
    {
        data.push_back(buf[c]);
    }
    return size*nmemb; //tell curl how many bytes we handled
}
void parse_json(char *json, char * retVal, char * property)
{
  struct json_token *arr, *tok;
  arr = parse_json2(json, strlen(json));

  tok = find_json_token(arr, "properties");
  sprintf(retVal,"%.*s", tok->len, tok->ptr);
  arr = parse_json2(retVal, strlen(retVal));
  
  tok = find_json_token(arr, "soilmask");
  sprintf(retVal,"%.*s", tok->len, tok->ptr);
  
  if(strcmp(retVal , "soil") == 0){
	tok = find_json_token(arr, property);
	sprintf(retVal,"%.*s", tok->len, tok->ptr);
  }
  else {
	   sprintf(retVal," The area has soil mask of water \r\n");
  }
  free(arr);
}
void get_ph_val(char *json, char*phVal){
	parse_json(json,phVal,(char*)"PHIHOX");
}
void get_sndppt_val(char *json, char *sndPptVal){
	parse_json(json,sndPptVal,(char*)"SNDPPT");
}
void get_sltppt_val(char *json, char *sltPptVal){
	parse_json(json,sltPptVal,(char*)"SLTPPT");
}
void get_crfvol_val(char *json, char* cfrVal){
	parse_json(json,cfrVal,(char*)"CRFVOL");
}
void get_taxgwrb_val(char *json, char* clyVal){
	parse_json(json,clyVal,(char*)"TAXGWRBMajor");
}
void get_taxusda_val(char *json, char* tVal){
	parse_json(json,tVal,(char*)"TAXGOUSDAMajor");
}	
int main()
{
    CURL* curl; //our curl object
	
    curl_global_init(CURL_GLOBAL_ALL); //pretty obvious
    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, "http://rest.soilgrids.org/query?lon=5.39&lat=51.57");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //tell curl to output its progress

    curl_easy_perform(curl);   
	char * soildata = (char*) &data[0u];
    /*char * ph_val = (char*)malloc(2000);
    char * cfr_val = (char*)malloc(2000);
    char * slt_ppt = (char*)malloc(2000);
    char * snd_ppt = (char*)malloc(2000);
    char * cly_ppt = (char*)malloc(2000);
    char * cec = (char*)malloc(2000);
    char * orcdrg = (char*)malloc(2000);
    char * bld = (char*)malloc(2000);
    char * TaxGWRBMajor = (char*) malloc(2000);
    char * TaxGWRBMinor = (char*) malloc(2000);*/
    char * parseddata = (char*)malloc(strlen(soildata));
    
    cout << endl << "*************" << endl <<soildata << endl;
    get_ph_val(soildata,parseddata);
    printf("pH value is %s \r\n",parseddata);
    
    get_sltppt_val(soildata,parseddata);
    printf("slt_ppt value is %s \r\n",parseddata);

    get_crfvol_val(soildata,parseddata);
    printf("cfrvol value is %s \r\n",parseddata);
    
    get_sndppt_val(soildata,parseddata);
    printf("sndppt value is %s \r\n",parseddata);
    
    get_taxgwrb_val(soildata,parseddata);
    printf("get_taxgwrb_val value is %s \r\n",parseddata);
    
    get_taxusda_val(soildata,parseddata);
    printf("get_taxusda_val value is %s \r\n",parseddata);
    
    
    //printf("\r\n%s %d \r\n",ph_val,strlen(data1));
    PublishMessage(" 54.186.77.119", "/a/b" , parseddata , strlen(parseddata));
    
    cin.get();
	
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}

