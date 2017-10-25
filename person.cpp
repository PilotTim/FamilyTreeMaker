#include <vector>
#include <deque>
#include <string>

#include "date_class.h"
#include "person.h"
#include "people.h"

extern People ListOfPeople;

std::string GetRandomGivenName(void);
std::string GetRandomSurname(void);

Date cutoffDate;
unsigned int masterid = 0;

long bellCurve(int max, int tightness)
{
    long x = 0;
    for (int i=0; i< 4*tightness; i++)
        x += (rand() % max);
    x /= 2*(tightness);
    return x;
}

Person::Person(std::string Surname, std::string GivenName, 
        gender_t Gender, Date dob) :
    surname(Surname), maidenName(Surname.c_str()), givenName(GivenName),
    gender(Gender), race(White), 
    dateOfBirth(dob),
    Father(NULL), Mother(NULL)
{
    if ( gender == Random )
    {
        // TODO: Make gender selection odds selectable
        // 50/50 gender selection
        if (rand() % 100 > 50) {
            //givenName="Jane";
            gender = Female; 
        }
        else
            gender = Male;
    }

    if ( gender == Male )
        dateOfDeath = dateOfBirth + bellCurve(76 * 385, 8);
    else
        dateOfDeath = dateOfBirth + bellCurve(83 * 385, 8);

    id = masterid++;

    // debug
    //dump();

    ListOfPeople.Add(this);
}

Person::~Person()
{
    for (unsigned int i = 0; i<myMarriages.size();++i)
    {
        class Marriage *curMarriage = myMarriages[i];
        class Person *spouse;

        if ( gender == Male )
            spouse = curMarriage->bride;
        else
            spouse = curMarriage->groom;

        if ( spouse == NULL )
            delete curMarriage;
        else
        {
            if ( gender == Male )
                curMarriage->groom = NULL;
            else
                curMarriage->bride = NULL;
        }
    }
}

void Person::CreateFamilyHistory(Date startDate, Date endDate)
{
    // work towards end date

    cutoffDate = endDate;

    // assume marriage
    // TODO: Maybe don't assume marriage
    createSpouse(GetRandomSurname(), GetRandomGivenName());
    int numberOfChildren = (rand() % 600)/100;
    while ( numberOfChildren-- )
    {
        class Person *child = createChild(GetRandomGivenName(), myMarriages.size());
        if ( child )
            child->CreateFamilyHistory(startDate, endDate);
    }
}

class Person *Person::createSpouse(std::string Surname, std::string GivenName)
{
    gender_t spouseGender = Female;
    if ( gender == Female )
        spouseGender = Male;

    class Person *spouse = new Person(Surname, GivenName, spouseGender);
    long lifespan = spouse->dateOfDeath - spouse->dateOfBirth;

    Marriage *thisMarriage = new Marriage;

    // set birthday to generally make females younger than their
    // male spouse
    if ( spouseGender == Female )
    {
        spouse->dateOfBirth = dateOfBirth - (1*365) + bellCurve(6*365,3);
        spouse->surname = surname;
        thisMarriage->bride = spouse;
        thisMarriage->groom = this;
    }
    else
    {
        spouse->dateOfBirth = dateOfBirth + (4*365) - bellCurve(8*365,3);
        thisMarriage->bride = this;
        thisMarriage->groom = spouse;
    }

    // adjust death based on prior lifespan
    spouse->dateOfDeath = spouse->dateOfBirth+lifespan;

    thisMarriage->dateOfMarriage = dateOfBirth + (16 * 365) + bellCurve(8*365,3);

    myMarriages.push_front(thisMarriage);
    spouse->myMarriages.push_front(thisMarriage);

    return spouse;
}

class Person *Person::createChild(std::string GivenName, int marriage)
{
    // allow mariage to be counted from 1
    marriage--;

    Marriage *curMarriage = myMarriages[marriage];
    int numberOfChildrenAlready = curMarriage->children.size();

    Date dob;
    if ( numberOfChildrenAlready == 0)
        dob = curMarriage->dateOfMarriage;
    else
    {
        dob = curMarriage->children[numberOfChildrenAlready-1]->dateOfBirth;
    }
    dob += (259+rand()%(293-259));
    dob.AddMonths(rand()%18);

    class Person *child = NULL;

    if ( dob <= cutoffDate )
    {
        child = new Person(surname, GivenName, Random, dob);

        if ( gender == Male )
        {
            child->Father = this;
            child->Mother = curMarriage->bride;
        }
        else
        {
            child->Mother = this;
            child->Father = curMarriage->groom;
        }

        curMarriage->children.push_back(child);
    }
    return child;
}

void Person::dump()
{
    printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    std::cout << givenName << " " << surname;

    if ( gender == Male )
        std::cout << " Male";
    else
    {
        if (myMarriages.size())
            std::cout << " nee " << maidenName;
        std::cout << " Female";
    }

    std::cout << " " << id;
    std::cout << std::endl;
    std::cout << "Born " << dateOfBirth.formatDate(Date::FULL) << std::endl;
    std::cout << "Died " << dateOfDeath.formatDate(Date::FULL) << std::endl;

    Date age = dateOfDeath-dateOfBirth;
    std::cout << "Age " << age.formatDate(Date::AGE) << std::endl;

    if ( Father )
        std::cout << "Father " << Father->givenName << " " << Father->surname << std::endl;
    if ( Mother )
        std::cout << "Mother " << Mother->givenName << " " << Mother->surname << std::endl;

    for (unsigned int i = 0; i<myMarriages.size();++i)
    {
        class Marriage *curMarriage = myMarriages[i];
        class Person *spouse;

        if ( gender == Male )
            spouse = curMarriage->bride;
        else
            spouse = curMarriage->groom;

        Date ageAtMarriage = curMarriage->dateOfMarriage - dateOfBirth;
        std::cout << "Married to " << 
            spouse->givenName << 
            " " << 
            spouse->surname;
        if (gender == Male)
        {
            std::cout << " nee " << 
                spouse->maidenName;
        }
        std::cout << " on " << curMarriage->dateOfMarriage;
        std::cout << " Age " << ageAtMarriage.formatDate(Date::AGE) <<
            std::endl;
        for (unsigned int j = 0; j<curMarriage->children.size();++j)
        {
            std::cout << "Child " << j << " "<< curMarriage->children[j]->givenName << std::endl;
        }
    }
}

void Person::dumpGEDCOM()
{
    printf("0 @%d@ INDI\n", id);
    printf("1 NAME /%s/%s/\n", givenName.c_str(), surname.c_str());
    printf("1 BIRT\n");
    printf("2 DATE %s\n", dateOfBirth.formatDate(Date::EUROPEAN));
    printf("1 DEAT\n");
    printf("2 DATE %s\n", dateOfDeath.formatDate(Date::EUROPEAN));
}

