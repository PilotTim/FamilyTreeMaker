// People is a collection of persons and actions that you can
// do to them
class People
{
    public:
        void Marry(class Person *, class Person *);
        void CreateBride(class Person *);
        void CreateGroom();
        void CreateChild();
        void Add(class Person *);
        void Delete(class Person *);
        void Dump();
        void Clean();

        ~People() { Clean(); }

    private:
        std::vector<class Person*> people;
};


