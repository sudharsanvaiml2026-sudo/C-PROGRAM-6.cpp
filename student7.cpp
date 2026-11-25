#include <iostream>
#include <memory>
#include <string>
using namespace std;

// Resource class (name is given by the user)
class Resource
{
private:
    string name;

public:
    Resource(string n) : name(n)
    {
        cout << "Resource '" << name << "' acquired" << endl;
    }

    ~Resource()
    {
        cout << "Resource '" << name << "' released" << endl;
    }

    void use()
    {
        cout << "Resource '" << name << "' is being used" << endl;
    }
};

// Manual Smart Pointer using RAII
class MySmartPointer
{
private:
    Resource* ptr;

public:
    MySmartPointer(Resource* p) : ptr(p) {}

    ~MySmartPointer()
    {
        delete ptr;
        cout << "Manual smart pointer destroyed" << endl;
    }

    Resource* operator->()
    {
        return ptr;
    }

    // Disable copying to avoid double deletion
    MySmartPointer(const MySmartPointer&) = delete;
    MySmartPointer& operator=(const MySmartPointer&) = delete;
};

int main()
{
    string resName;
    int owners;

    cout << "Enter resource name: ";
    cin >> resName;

    cout << "Enter number of shared owners for shared_ptr (>=1): ";
    while (!(cin >> owners) || owners < 1)
    {
        cout << "Enter a number >= 1: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }

    cout << "\n----- Manual RAII Smart Pointer -----" << endl;
    {
        MySmartPointer smartPtr(new Resource(resName));
        smartPtr->use();
    }

    cout << "\n----- unique_ptr -----" << endl;
    {
        unique_ptr<Resource> uniquePtr = make_unique<Resource>(resName);
        uniquePtr->use();
    }

    cout << "\n----- shared_ptr -----" << endl;
    {
        shared_ptr<Resource> first = make_shared<Resource>(resName);
        shared_ptr<Resource> others[100];

        for (int i = 1; i < owners && i < 100; i++)
            others[i] = first;   // copy -> reference count increases

        first->use();
        cout << "Shared Pointer Count: " << first.use_count() << endl;
    }

    cout << "\nProgram completed successfully." << endl;
    return 0;
}