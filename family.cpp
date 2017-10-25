#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdio>
#include <getopt.h>
#include <string>
#include <iostream>
#include <fstream>
#include <deque>
#include <vector>
#include <time.h>

#include "date_class.h"
#include "person.h"
#include "people.h"

std::vector<std::string> firstNames;
std::vector<std::string> lastNames;

People ListOfPeople;

void fillNames()
{
    char line[256];
    char firstname[64];
    char lastname[64];
    std::ifstream input("randomNames.csv");

    // skip header line
    input.getline(line, 256);

    while( !input.eof() )
    {
        // read in whole line up to '\n'
        input.getline(line, 256);

        // scan and parse first two CSV's and ignore the rest
        sscanf(line, "%[^,],%[^,]", firstname, lastname);

        // add these to the database
        firstNames.push_back(firstname);
        lastNames.push_back(lastname);
    }
    std::cout << "Read " << firstNames.size() << " names" << std::endl;
}

std::string GetRandomGivenName()
{
    return( firstNames[rand() % firstNames.size()] );
}

std::string GetRandomSurname()
{
    return( lastNames[rand() % lastNames.size()] );
}

int main(int argc, char **argv)
{
    Date startDate = "1/1/1000";
    Date endDate = "12/31/1100";
    Date birthDate = "8/15/1050";
    std::string firstName = "John";
    std::string lastName = "Doe";
    Person::gender_t gender = Person::Male;
    int c;

    int random_flag = 0;

    fillNames();

    while ( 1 )
    {
        static struct option long_opts[] = 
        {
            {"start", required_argument, 0, 's'},
            {"end", required_argument, 0, 'e'},
            {"dob", required_argument, 0, 'd'},
            {"firstname", required_argument, 0, 'f'},
            {"lastname", required_argument, 0, 'l'},
            {"random", no_argument, &random_flag, 1},
            {0, 0, 0, 0}
        };

        int opt_idx = 0;

        c = getopt_long( argc, argv, "rf:l:d:e:s:", long_opts, &opt_idx);
        if ( c == -1 )
            break;

        switch (c)
        {
            case 'f':
                firstName = optarg;
                break;
            case 'l':
                lastName = optarg;
                break;
            case 'e':
                endDate = optarg;
                break;
            case 's':
                startDate = optarg;
                break;
            case 'd':
                birthDate = optarg;
                break;
            default:
                break;
        }
    }

    timespec mytime;
    clock_gettime(CLOCK_MONOTONIC, &mytime);
    srand(mytime.tv_nsec);

    if (random_flag)
    {
        lastName = GetRandomSurname();
        firstName = GetRandomGivenName();

        // TODO randomize gender and death
        if (rand() % 100 > 50) {
            gender = Person::Female; 
        }
        startDate = "1/1/1000";
        startDate += rand() % 365*1000;
        birthDate = startDate + rand() % 365*100;
    }

    if ( endDate <= startDate )
    {
        endDate = startDate;
        endDate.AddYears(100);
    }
    if ( birthDate < startDate )
    {
        startDate = birthDate;
    }
    if ( endDate < birthDate )
    {
        endDate = birthDate;
        endDate.AddYears(100);
    }
    printf("%s Generating a %sfamily history for:\n", argv[0], random_flag?"random ":"");
    printf("\t%s %s\n", firstName.c_str(), lastName.c_str());
    printf("\tBorn %s\n", birthDate.formatDate());
    printf("\tFrom %s through ", startDate.formatDate());
    printf("%s\n", endDate.formatDate());

    // Originator aka Person 0
    Person *personOne = new Person(lastName, firstName, gender, birthDate);
    personOne->CreateFamilyHistory(startDate, endDate);

#if 0
    // create a bride
    personOne->CreateSpouse(GetRandomSurname(),GetRandomGivenName());

    // create a child
    personOne->CreateChild(GetRandomGivenName(), 1);
#endif
    ListOfPeople.Dump();
#if 0
    ListOfPeople.Add(myPerson);

    // Spouse
    Person *mySpouse = new Person(GetRandomSurname(),GetRandomGivenName(),Person::Female);
    ListOfPeople.Add(mySpouse);

    mySpouse->AdjustBirthDate(Person::Peer, myPerson);
    myPerson->Marry(mySpouse, 0l);

    mySpouse = new Person(GetRandomSurname(),GetRandomGivenName(),Person::Female);
    ListOfPeople.Add(mySpouse);

    myPerson->Marry(mySpouse, 100l);

    //myPerson->MakeARandomSpouseFor();
#endif
        /*
    std::cout << myPerson.GivenName() << " " << myPerson.Surname() << std::endl;
    if ( myPerson.Gender() == Male )
        std::cout << "Male" << std::endl;
    else
        std::cout << "Female" << std::endl;
*/
    return 0;
}

