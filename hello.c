/* Hello World program */

#include<stdio.h>

void c(){
}
void d(){
c();
}

void b(){

d();
}
void a(){
b();
}



main()
{
	a();
    printf("Hello World");

}
