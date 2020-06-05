#include "stdio.h"

#define LOG_TYPE1(format, ...) do{ 		\
		printf(format, __VA_ARGS__); 	\
										\
} while(0)

#define LOG_TYPE2(format, args...) do{ 	\
		printf(format, args); 			\
										\
} while(0)


#define LOG_TYPE3(format, ...) do{ 		\
		printf(format, ##__VA_ARGS__); 	\
										\
} while(0)


#define LOG_TYPE4(format, args...) do{ 	\
		printf(format, ##args); 		\
										\
} while(0)


#define LOG(x) printf("LOG "#x" %d \n", x);

int value = 10;

int main()
{

	printf("hello world. \n");

	//LOG_TYPE1("hello %d \n");	error
	LOG_TYPE1("hello %d \n", 1);

	//LOG_TYPE2("hello \n"); error
	LOG_TYPE2("hello %d \n", 2);

	LOG_TYPE3("hello 3\n");
	LOG_TYPE3("hello %d\n", 3);

	LOG_TYPE4("hello 4\n");
	LOG_TYPE4("hello %d\n", 4);


	LOG(10);
	LOG(value);

	return 0;
}

