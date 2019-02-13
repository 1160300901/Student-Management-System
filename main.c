#include <stdio.h>
#include "StuProj.h"

int main()
{
    int choice;
    showmenu();
    do
    {
    while(scanf("%d",&choice)!=1)
    {
        printf("Wrong!!Input again:");
        clearall();
    }
    clearall();
    switch(choice)
    {        
        case 1:addstu();break;
        case 2:CacuCourse();break;
		case 3:CacuStuCourse();break;
		case 4:sortstu(cmpdown);printall();break;
		case 5:sortstu(cmpup);printall();break;
		case 6:sortstu(cmpID);printall();break;
		case 7:sortstu(Sortbyname);printall();break;
		case 8:stufindID();break;
        case 9:stufindname();break;
        case 10:analyze();break;
        case 11:printall();break;
		case 12:savetofile();break;
		case 13:getfromfile();break;
        case 0:printf("End of program!\n");exit(0);
        default:printf("Wrong Choice!!\n");break;
    }
    system("pause");
    system("cls");
    showmenu();
    }while(choice!=0);
    return 0;
}
