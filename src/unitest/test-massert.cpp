#include <string>
#include <stdio.h>
#include "common/massert.h"

std::string test(const std::string &str){
	massert_retval(!str.empty(), "null");
}

int test(int a){
	massert_retval(0 == 0, 0);
	return 0;
}

int main(){
	test(1);
	test("");
	test(0);
	test("abc");
}
