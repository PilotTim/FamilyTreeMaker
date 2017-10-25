#include <vector>
#include <deque>
#include <string>

#include "date_class.h"
#include "person.h"
#include "people.h"

void People::Marry(class Person *p1, class Person *p2)
{
}

void People::Add(class Person *person)
{
    people.push_back(person);
}

void People::Dump()
{
    for (std::vector<class Person *>::iterator it=people.begin(); 
            it != people.end(); ++it)
    {
        (*it)->dump();
    }
    printf("0 HEAD\n");
    printf("1 CHAR ASCII\n");
    printf("1 SOUR FFTM\n");
    printf("1 GEDC\n");
    printf("2 VERS 5.5\n");
    printf("2 FORM Lineage-Linked\n");
    printf("1 SUBM @Tim@\n");
    printf("0 @Tim@ SUBM\n");
    printf("1 NAME /Tim/\n");

    for (std::vector<class Person *>::iterator it=people.begin(); 
            it != people.end(); ++it)
    {
        class Person *peep=(*it);
        peep->dumpGEDCOM();
    }

    printf("0 TRLR\n");
}

void People::Clean()
{
    for (std::vector<class Person *>::iterator it=people.begin(); 
            it != people.end(); ++it)
    {
        class Person *peep = *it;
        
        delete (*it);
    }
}

