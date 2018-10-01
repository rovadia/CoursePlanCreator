//Submitter Roei Ovadia
//ID 40885696

#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX 100

struct offpair {
    int quarter;
    char course[20];
};

struct reqstruct {
    char course[20];
};

struct prereqstruct {
    struct reqstruct reqstruct1[MAX];
};

struct output {
    int year;
    int quarter;
    struct reqstruct reqstruct1[MAX];
};

bool req_empty (struct reqstruct *req_array) { //once empty we are done
    int i;
    for (i=0;i< MAX;i++) {
        if (req_array[i].course[0]!='\0') { //if one thing isn't null (hasn't been completed) then not empty
            return false;
        }
    }
    return true;
}

void remove_req (struct reqstruct *req_array, char *course) {
    int i=0;
    for (i=0;i< MAX;i++) {
        if (course[0]!='\0') {
            if (strcmp(req_array[i].course, course) == 0) {
                req_array[i].course[0] = '\0';
            }
        }
    }
}

void remove_offering (struct offpair *offering_array, char *course) {
    int i=0;
    for (i=0;i< MAX;i++) {
        if ( strcmp(offering_array[i].course, course)==0 ) {
            offering_array[i].course[0]='\0';
        }
    }
}

void remove_prereq (struct prereqstruct *prereq_array, char *course) {
    int i=0;
    int j=0;
    for (i=0; i< MAX;i++) {
        for (j=0;j< MAX;j++) {
            if (strcmp(prereq_array[i].reqstruct1[j].course,course)==0) {
                prereq_array[i].reqstruct1[j].course[0]='\0';
            }
        }
    }
}


bool in_req_array(struct reqstruct *req_array, char *course) {
    int i=0;
    for (i=0;i< MAX;i++) {
        if ((strcmp(req_array[i].course,course)==0) ) { //to be required it must exist and not be completed
            return true;
        }
    }
    return false;
}

bool has_prereq(struct prereqstruct *prereq_array, char *course) { //if not ever first in list, has no prereqs. if no course is behind it, has no prereqs
    int i=0;
    int j=0;
    for (i=0;i< MAX;i++) {
        if (strcmp(prereq_array[i].reqstruct1[0].course,course)==0 ) { //if this course is ever first in file
            for (j=1;j< MAX;j++) { //check through all elements after it, j=1 and not 0
                if (prereq_array[i].reqstruct1[j].course[0]!='\0'  ) { //if any elements are not empty (have not been completed yet)
                    return true; //then it has a prereq we haven't done yet, can't take course
                }
            }
        }
    }
    return false;
}

void find_req(struct offpair *offering_array, struct prereqstruct *prereq_array, struct reqstruct *req_array, int quarter, struct reqstruct *out_array) {
    int i=0;
    int j=0;
    for (i=0;i< MAX;i++) { //cycle through offering array
        if (offering_array[i].quarter==quarter) { //if it has the same quarter
            if ( (in_req_array(req_array,offering_array[i].course)) &&  (!(has_prereq(prereq_array,offering_array[i].course)))) { //if it is required AND if it has no prereqs
                strncpy(out_array[j].course, offering_array[i].course, sizeof(out_array[j].course)); //copy this course to first possible slot in our output
                j++; //increment to next output slot
                remove_prereq(prereq_array,offering_array[i].course); //remove it from all instances of prerequisites file
                remove_req(req_array,offering_array[i].course); //remove it from requirements file
                remove_offering(offering_array,offering_array[i].course); //remove it from offerings file
            }
        }
    }
}

void print_schedule (struct output *schedule) {
    int i=0;
    int j=0;
    for (i=0;i< 30;i++) {
        if (schedule[i].reqstruct1[j].course[0]!='\0' && schedule[i].year!=0) {
            printf("%d %d ", schedule[i].year, schedule[i].quarter);
            for (j = 0; j < MAX; j++) {
                if (schedule[i].reqstruct1[j].course[0]!='\0') {
                    printf("%s ", schedule[i].reqstruct1[j]);
                }
            }
            printf("\n");
        }
    }
}

void create_schedule (struct offpair *offering_array, struct prereqstruct *prereq_array, struct reqstruct *req_array) {
    struct output schedule[30]={NULL};
    int index=0;
    int year=1;
    while (!req_empty(req_array)) {
        //use function to find all courses that are in fall and all of its prereqs are filled and it's required, assign array of course arrays to this
        find_req(offering_array, prereq_array, req_array, 1,schedule[index].reqstruct1);
        schedule[index].quarter=1; //fall
        schedule[index].year=(year);
        index++; //each quarter is a new index

        find_req(offering_array, prereq_array, req_array, 2,schedule[index].reqstruct1);
        schedule[index].quarter=2; //winter
        schedule[index].year=(year);
        index++;
//        for (int xx=0;xx<2;xx++) {
//            for (int y=0; y<6;y++) {
//                printf("preReq array is %s ", prereq_array[xx].reqstruct1[y].course);
//            }
//            printf("\n");
//        }
//        printf("\n");
//        for (int x=0;x<8;x++) {
//            printf("req array is %s", req_array[x].course);
//        }
//        printf("\n");
//        for (int x=0;x<8;x++) {
//            printf("offering array is %s", offering_array[x].course);
//        }
//        printf("\n");

        find_req(offering_array, prereq_array, req_array, 3,schedule[index].reqstruct1);
        schedule[index].quarter=3; //spring
        schedule[index].year=(year);
        index++;
        year++; //move on to next year
    }
    print_schedule(schedule);
}



int main(int argc, char *argv[]) {
//    FILE *offering = fopen(argv[1], "r");
//    FILE *prereq = fopen(argv[2], "r");
//    FILE *req = fopen(argv[3], "r");

    FILE *offering = fopen("C:\\Users\\rovad\\Desktop\\ICS53\\offering.txt", "r");
    FILE *prereq = fopen("C:\\Users\\rovad\\Desktop\\ICS53\\prerequisites.txt", "r");
    FILE *req = fopen("C:\\Users\\rovad\\Desktop\\ICS53\\requirements.txt", "r");

    int offering_index=0;
    struct offpair offering_array [MAX]={NULL}; //array of structs
    char *c1;
    char *token1;
    char temp1[100];
    c1=fgets(temp1,100,offering);
    while (c1!=NULL) { //while file is open
        token1=strtok(temp1, " ");
        strncpy ( offering_array[offering_index].course, token1, sizeof(offering_array[offering_index].course) ); //before space is course
        token1=strtok(NULL, " ");
        offering_array[offering_index].quarter=atoi(token1); //after space is quarter
        c1=fgets(temp1,100,offering);
        offering_index++;
    }
    fclose(offering);

    struct reqstruct req_array[MAX]={NULL};
    int req_index=0;
    char *c3;
    char temp3[100];
    char *token3;
    c3=fgets(temp3,100,req);
    while (c3!=NULL) {
        token3 = strtok(temp3, " ");
        while (token3 != NULL) {
            strncpy(req_array[req_index].course, token3, sizeof(offering_array[req_index].course));
            token3 = strtok(NULL, " ");
            req_index++;
        }
        c3=fgets(temp3,100,req);
    }

    struct prereqstruct prereq_array [MAX]={NULL};
    int prereq_index=0;
    int inner_index=0;
    int len=0;
    char *c2;
    char *token2;
    char temp2[100];
    c2=fgets(temp2,100,prereq);
    while (c2!=NULL) {
        token2=strtok(temp2, " ");
        while (token2!=NULL) {

            strncpy( prereq_array[prereq_index].reqstruct1[inner_index].course, token2, sizeof(prereq_array[prereq_index].reqstruct1[inner_index].course));
            token2=strtok(NULL, " ");
            if (prereq_array[prereq_index].reqstruct1[inner_index].course[strlen(prereq_array[prereq_index].reqstruct1[inner_index].course)-1]=='\n') { //remove newline if it exists
                (prereq_array[prereq_index].reqstruct1[inner_index].course[strlen(prereq_array[prereq_index].reqstruct1[inner_index].course)-2])='\0';
            }
            if (!(in_req_array(req_array,prereq_array[prereq_index].reqstruct1[inner_index].course))) { //if there is prereq that is not already in req array
                if (in_req_array(req_array,prereq_array[prereq_index].reqstruct1[0].course )) { //and its parent course is required (in req array)
                    if (prereq_array[prereq_index].reqstruct1[inner_index].course[0]!='\0') { //and it's not null
                        strncpy(req_array[req_index].course, prereq_array[prereq_index].reqstruct1[inner_index].course,  //copy it to the req array
                                sizeof(req_array[req_index].course));
                        req_index++;
                    }
                }
            }
            inner_index++;

        }
        c2=fgets(temp2,100,prereq);
        prereq_index++;
        inner_index=0;
    }

    create_schedule(offering_array, prereq_array, req_array);
    fclose(req);
    fclose(prereq);





//    printf(argv[0]);
//    printf(argv[1]);
//    printf(argv[2]);
//    printf(argv[3]);
    return 0;
}