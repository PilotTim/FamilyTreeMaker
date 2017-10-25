class Person
{
    public:
        enum gender_t
        {
            Random,
            Male,
            Female
        };

        enum race_t
        {
            White,
            Black,
            Latin,
            Asian,
            Islander
        };

        enum reference_t
        {
            Child,
            Peer,
            Parent
        };

        Person(std::string Surname="Doe", 
                std::string GivenName="John", 
                gender_t Gender=Random,
                Date dob="1/1/1000");

        ~Person();

        void CreateFamilyHistory(Date startDate, Date endDate);

        /// Debug
        void dump( void );
        void dumpGEDCOM( void );

    private:
        class Marriage
        {
            public:
                Date dateOfMarriage;
                Date dateOfDivorce;
                class Person *bride;
                class Person *groom;
                std::deque<class Person *> children;
        };

        class Person* createSpouse(std::string, std::string);
        class Person* createChild(std::string, int);

        // Names
        std::string surname;
        std::string maidenName;
        std::string givenName;

        // characteristics
        gender_t gender;
        race_t race;
        unsigned int id;

        // dates
        Date dateOfBirth;
        Date dateOfDeath;

        // Relationships
        class Person *Father;
        class Person *Mother;

        std::deque<class Marriage *> myMarriages;
        // 
}; 
