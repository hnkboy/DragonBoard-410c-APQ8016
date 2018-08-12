#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
	
#include "cJSON.h"
#include "json.h"
typedef struct
{
	int id;
	char firstName[32];
	char lastName[32];
	char email[64];
	int age;
	float height;
}people;

weather today = {0};
//void dofile(char *filename);/* Read a file, parse, render back, etc. */
/*
int main(int argc, char **argv)
{

//	dofile("json_str1.txt");
//	dofile("json_str2.txt");
	dofile("tmp.txt");

	return 0;
}*/

//parse a key-value pair
int cJSON_to_str(char *json_string, char *str_val)
{
	cJSON *root=cJSON_Parse(json_string);
	if (!root)
	{
		printf("Error before: [%s]\n",cJSON_GetErrorPtr());
		return -1;
	}
	else
	{
		cJSON *item=cJSON_GetObjectItem(root,"firstName");
		if(item!=NULL)
		{
			printf("cJSON_GetObjectItem: type=%d, key is %s, value is %s\n",item->type,item->string,item->valuestring);
			memcpy(str_val,item->valuestring,strlen(item->valuestring));
		}
		cJSON_Delete(root);
	}
	return 0;
}

//parse a object to struct
int cJSON_to_struct(char *json_string, people *person)
{
	cJSON *item;
	cJSON *root=cJSON_Parse(json_string);
    cJSON *object;
	if (!root)
	{
		printf("Error before: [%s]\n",cJSON_GetErrorPtr());
		return -1;
	}
	else
	{
		item=cJSON_GetObjectItem(root,"status");
		if(item==NULL)
		{
			printf("Error before: [%s]\n",cJSON_GetErrorPtr());
			cJSON_Delete(root);
			return -1;
		}
		printf("cJSON_GetObjectItem: type=%d, string is %s, valueint=%d\n",item->type,item->string,item->valueint);
		if(item!=NULL)
		{
			today.status = item->valueint;
			printf("today.status = %d\n",today.status);
		}
		if (today.status != 200){
			printf("retuen back\n");
			cJSON_Delete(root);
			return -1;
		}
		object=cJSON_GetObjectItem(root,"data");
		if(object==NULL)
		{
			printf("Error before: [%s]\n",cJSON_GetErrorPtr());
			cJSON_Delete(root);
			return -1;
		}
		printf("cJSON_GetObjectItem: type=%d, key is %s, value is %s\n",object->type,object->string,object->valuestring);

		if(object!=NULL)
		{
			item=cJSON_GetObjectItem(object,"wendu");
			if(item!=NULL)
			{
				printf("cJSON_GetObjectItem: type=%d, string is %s, valuestring=%s\n",item->type,item->string,item->valuestring);
				memcpy(person->firstName,item->valuestring,strlen(item->valuestring));
			}
			today.tmp = (int)strtol(item->valuestring,(char **)NULL,10);
			printf("today.tmp = %d\n",today.tmp);
			item=cJSON_GetObjectItem(object,"shidu");
			if(item!=NULL)
			{
				printf("cJSON_GetObjectItem: type=%d, string is %s, valuestring=%s\n",item->type,item->string,item->valuestring);
				memcpy(person->lastName,item->valuestring,strlen(item->valuestring));
			}

			item=cJSON_GetObjectItem(object,"pm25");
			if(item!=NULL)
			{
				printf("cJSON_GetObjectItem: type=%d, string is %s, valueint=%d\n",item->type,item->string,item->valueint);
			}

			item=cJSON_GetObjectItem(object,"pm10");
			if(item!=NULL)
			{
				printf("cJSON_GetObjectItem: type=%d, string is %s, valueint=%d\n",item->type,item->string,item->valueint);
			}
			else
			{
				printf("cJSON_GetObjectItem: get data failed\n");
			}

		}

		object=cJSON_GetObjectItem(object,"forecast");
		if(object==NULL)
		{
			printf("Error before: [%s]\n",cJSON_GetErrorPtr());
			cJSON_Delete(root);
			return -1;
		}
		printf("cJSON_GetObjectItem: type=%d, key is %s, value is %s\n",object->type,object->string,object->valuestring);

		if(object!=NULL)
		{  
            int size = cJSON_GetArraySize(object);
      
			printf("Get Array Size %d\n",size);
            object = cJSON_GetArrayItem(object,0);
		/*	object=cJSON_GetObjectItem(object,"forecast");
            if(object==NULL)
	    	{
		    	printf("Error before: [%s]\n",cJSON_GetErrorPtr());
		    	cJSON_Delete(root);
		    	return -1;
	     	}
	    	printf("cJSON_GetObjectItem: type=%d, key is %s, value is %s\n",object->type,object->string,object->valuestring);
         */
            if(object!=NULL)
			{	
					item=cJSON_GetObjectItem(object,"high");
					if(item!=NULL)
					{
						printf("cJSON_GetObjectItem: type=%d, string is %s, valuestring=%s\n",item->type,item->string,item->valuestring);
						memcpy(person->firstName,item->valuestring,strlen(item->valuestring));
						memcpy(today.shightemp,item->valuestring,strlen(item->valuestring));
						today.shightemp[strlen(item->valuestring)] = '\0'; 
					}
					item=cJSON_GetObjectItem(object,"low");
					if(item!=NULL)
					{
						printf("cJSON_GetObjectItem: type=%d, string is %s, valuestring=%s\n",item->type,item->string,item->valuestring);
						memcpy(person->firstName,item->valuestring,strlen(item->valuestring));
						memcpy(today.slowtemp,item->valuestring,strlen(item->valuestring));
						today.slowtemp[strlen(item->valuestring)] = '\0'; 
					}
					item=cJSON_GetObjectItem(object,"type");
					if(item!=NULL)
					{
						printf("cJSON_GetObjectItem: type=%d, string is %s, valuestring=%s\n",item->type,item->string,item->valuestring);
						memcpy(person->firstName,item->valuestring,strlen(item->valuestring));
						memcpy(today.stype,item->valuestring,strlen(item->valuestring));
						today.stype[strlen(item->valuestring)] = '\0'; 
					}
					item=cJSON_GetObjectItem(object,"notice");
					if(item!=NULL)
					{
						printf("cJSON_GetObjectItem: type=%d, string is %s, valuestring=%s\n",item->type,item->string,item->valuestring);
						memcpy(person->firstName,item->valuestring,strlen(item->valuestring));
						memcpy(today.snotice,item->valuestring,strlen(item->valuestring));
						today.snotice[strlen(item->valuestring)] = '\0'; 
					}
			}
       }
		cJSON_Delete(root);
	}
	return 0;
}

//parse a struct array
int cJSON_to_struct_array(char *text, people worker[])
{
	cJSON *json,*arrayItem,*item,*object;
	int i;

	json=cJSON_Parse(text);
	if (!json)
	{
		printf("Error before: [%s]\n",cJSON_GetErrorPtr());
	}
	else
	{
		arrayItem=cJSON_GetObjectItem(json,"data");
		if(arrayItem!=NULL)
		{
			int size=cJSON_GetArraySize(arrayItem);
			printf("cJSON_GetArraySize: size=%d\n",size);

			for(i=0;i<size;i++)
			{
				printf("i=%d\n",i);
				object=cJSON_GetArrayItem(arrayItem,i);

				item=cJSON_GetObjectItem(object,"date");
				if(item!=NULL)
				{
					printf("cJSON_GetObjectItem: type=%d, string is %s\n",item->type,item->string);
					memcpy(worker[i].firstName,item->valuestring,strlen(item->valuestring));
				}

				item=cJSON_GetObjectItem(object,"pm10");
				if(item!=NULL)
				{
					printf("cJSON_GetObjectItem: type=%d, string is %s, valuestring=%s\n",item->type,item->string,item->valuestring);
				}

				item=cJSON_GetObjectItem(object,"pm25");
				if(item!=NULL)
				{
					printf("cJSON_GetObjectItem: type=%d, string is %s, valuestring=%s\n",item->type,item->string,item->valuestring);
				}

				item=cJSON_GetObjectItem(object,"shidu");
				if(item!=NULL)
				{
					printf("cJSON_GetObjectItem: type=%d, string is %s, valueint=%d\n",item->type,item->string,item->valueint);
				}
				else
				{
					printf("cJSON_GetObjectItem: get age failed\n");
				}

				item=cJSON_GetObjectItem(object,"ganmao");
				if(item!=NULL)
				{
					printf("cJSON_GetObjectItem: type=%d, string is %s, value=%f\n",item->type,item->string,item->valuedouble);
					worker[i].height=item->valuedouble;
				}
			}
		}

		for(i=0;i<3;i++)
		{
			printf("i=%d, firstName=%s,lastName=%s,email=%s,age=%d,height=%f\n",
					i,
					worker[i].firstName,
					worker[i].lastName,
					worker[i].email,
					worker[i].age,
					worker[i].height);
		}

		cJSON_Delete(json);
	}
	return 0;
}

// Read a file, parse, render back, etc.
#if 1
int dofile(char *filename,weather *pweather)
{
	FILE *f;
	int len;
	char *data;
	
	f=fopen(filename,"rb");
	fseek(f,0,SEEK_END);
	len=ftell(f);
	fseek(f,0,SEEK_SET);
	data=(char*)malloc(len+1);
	fread(data,1,len,f);
	fclose(f);
	
	printf("read file %s complete, len=%d.\n",filename,len);
//	char str_name[40];
//	int ret = cJSON_to_str(data, str_name);

	people person;
    int ret = cJSON_to_struct(data, &person);
    *pweather = today;
	printf("success json.\n");
	//people worker[3]={{0}};
	//cJSON_to_struct_array(data, worker);

	free(data);
	return 0;
}
#endif

