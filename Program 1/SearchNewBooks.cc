#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<algorithm>
#include <chrono> // NOLINT (build/c++11)
using namespace std;

//Declare class Book and functions
class Book  {
    int ISBN;
    string type;
    public: Book();
    void setISBN(int is);
    void setType(string type);
    int getISBN();
    string getType();
};

Book::Book()  {
    ISBN = 0;
    type = "";
}
void Book::setISBN(int i)
{
    ISBN = i;
}
void Book::setType(string t)
{
    type=t;
}
int Book::getISBN()
{
    return ISBN;
}
string Book::getType()
{
    return type;
}

ostream &operator<<(ostream &out, vector<Book> vec);
bool loadToVector(string filename, vector<Book> &vec);
bool binarySearch(vector<Book> vec, int search);
bool recursiveBinarySearch(vector<Book> vec, int low, int high, int x);
bool linearSearch(vector<Book> vec, int search);
bool compare(Book i, Book j);


int main(int argc, char **argv) {
    //Declare two vectors for new books and requested books
    vector<Book> newBooks, requestedBooks;

    string filename1, filename2;
    if (argc < 4)
    {
        cout << "Error: ./SearchNewBooks newbooks.dat request.dat" << endl;
        return -1;
    }
    //Assigned two filename
    filename1 = argv[1];
    filename2 = argv[2];
    char choice;
    if (loadToVector(filename1, newBooks) == false)
        return -1;
    if (loadToVector(filename2, requestedBooks) == false)
        return -1;
    
    cout << "Choice of search method ([l]inear, [b]inary, [r]ecursiveBinary) " << endl;
    cin >> choice;
    //Writing file
    ofstream out;
    out.open(argv[3]);
    if(!out){
        cout << "Not able to open file for writing \n" << endl;
        return -1;
    }
    int count = 0;
    auto start=chrono::high_resolution_clock::now();
    double elapsed_us;
    auto end = chrono::high_resolution_clock::now();
    switch (choice) {
        
        //Binary Search
        case 'b':
            //Sort before calling binary search function
            sort(newBooks.begin(), newBooks.end(), compare);
            for (int i = 0; i < requestedBooks.size(); i++) {
                if (binarySearch(newBooks, requestedBooks[i].getISBN()) == true) {
                    count++;
                }
                
            }
            end=chrono::high_resolution_clock::now();
            out<<count<<endl;
            elapsed_us = chrono::duration<double,micro>(end-start).count();
            cout<<"CPU time: "<<elapsed_us<<"microseconds"<<endl;
        break;
        //Linear Search
        case 'l':
            for (int i = 0; i < requestedBooks.size(); i++) {
                if (linearSearch(newBooks, requestedBooks[i].getISBN()) == true) {
                    count++;
                }
                
            }
            end=chrono::high_resolution_clock::now();
            out<<count<<endl;
            elapsed_us = chrono::duration<double,micro>(end-start).count();
            cout<<"CPU time: "<<elapsed_us<<"microseconds"<<endl;
        break;
        //Recursive Binary Search
        case 'r':
            //Sort before doing recursive binary search
            sort(newBooks.begin(), newBooks.end(), compare);
            for(int i=0; i<requestedBooks.size(); i++){
                if (binarySearch(newBooks, requestedBooks[i].getISBN()) == true) {
                    count++;
                }
                  
            }
            end=chrono::high_resolution_clock::now();
            out<<count<<endl;
            elapsed_us = chrono::duration<double,micro>(end-start).count();
            cout<<"CPU time: "<<elapsed_us<<"microseconds"<<endl;
        break;

        default:
            cout << "Incorrect choice " << endl;
    }

}

ostream &operator<<(ostream &out, vector<Book> vec) {
    for (int i = 0; i < vec.size(); i++)  {
        out << "ISBN: " << vec[i].getISBN() << endl;
        out << "Type: " << vec[i].getType() << endl;
    }
    return out;
}

//Open file and load the data to vector Book
bool loadToVector(string filename, vector<Book> &vec) {
    ifstream in;

    in.open(filename);
    if (!in)  {
        cout << "Can not open " << filename << endl;
        return false;
    }

    string str;

    while (!in.eof()) {
        Book *b;
        b = new Book;
        while(getline(in, str, ',')){
            b->setISBN(stoi(str));
            getline(in, str);
            b->setType(str);
            vec.push_back(*b);
        }
    }
}

//Compare ISBN function
bool compare(Book i, Book j) { 
    return i.getISBN() < j.getISBN(); 
}

//Binary Search function
bool binarySearch(vector<Book> vec,int search) {
    int high, low, mid;
    high = vec.size() - 1;
    low = 0;

    while (low <= high) {
        mid = (high + low) / 2;
        if (search == vec[mid].getISBN())
            return true;
        else if (search < vec[mid].getISBN()) {
            high = mid - 1;
        }
        else
            low = mid + 1;
    }
    return false; //search not found
}

//Recursive Binary Search function
bool recursiveBinarySearch(vector<Book> vec, int low, int high, int x) {
    high = vec.size() -1;
    low = 0;

    if (high >= low) {

        int mid = low+ (high - low) / 2;
 
        
        if (vec[mid].getISBN() == x)
            return true;
 
        
        if (vec[mid].getISBN() > x)
            return recursiveBinarySearch(vec, low, mid - 1, x);
 
        
        return recursiveBinarySearch(vec, mid + 1, high, x);
    }
 
    
    return false;
}
//Linear Search function
bool linearSearch(vector<Book> vec,int search) {
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i].getISBN() == search) {
        return true;
        }
    }
    return false;
}