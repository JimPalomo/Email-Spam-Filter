/** -----------------------------------------------------------
* This program filters a list of emails based on the  
* conditions within a list. The list is known as the spam 
* list which contains emails addresses that are marked as spam.
* The filtering is done by the use of the provided ourvector class,
* binary searches, and file reading/writing.
* Class: CS 251, Spring 2020
* Author: Jim Palomo
* Date: Jan. 20, 2020
* ----------------------------------------------------------**/ 

// Include standard libraries and the provided ourvector.h file
#include <iostream>
#include <fstream>
#include <string> 
#include "ourvector.h" 

using namespace std;

// List of functions used and are called
void parseEmailAddress(string email, string& username, string& domain);

void remDotCom(string originalAddress, string& newAddress);

void getCurWordUserDom(string& curWord, string& curWordUser, string& curWordDom);

void load(ourvector<string>& spamVect);

void display(ourvector<string>& spamVect);

bool binarySearchUser(ourvector<string>& spamVect, string foundDomain);

bool binarySearch(ourvector<string>& spamVect, string email);

bool check(ourvector<string>& spamVect, string email);

void filter(ourvector<string>& spamVect);

string emailGivenByUser;    // Global variable

int main() {

    // Creating variables
    string response;    // the command that the user inputs
    ourvector<string> spamVect; // creates class vector
    string email;


    // Printing startup message
    cout << "** Welcome to spam filtering app ** \n" << endl;



    // Allows for the main command prompt to reappear
    while (response != "#") {
        cout << "Enter command or # to exit> ";     // main command prompt
        cin >> response;   // get the response for user input for command

        // If-statements to check which command was requested by the user
        if (response == "load") {
            load(spamVect);
        } 
        
        else if (response == "display") {
            display(spamVect);
        }

        else if (response == "check") {
            cin >> email;
            if (check(spamVect, email) == 1) {
                // cout << "This is spam" << endl;
                cout << emailGivenByUser << " is spam" << endl;

            } 

            else {
                cout << emailGivenByUser << " is not spam" << endl;
            }
        }

        else if (response == "filter") {
            filter(spamVect);
        } 

        else if (response != "#") {
            cout << "**invalid command" << endl;
        }


        cout << endl;
    }

    cout << endl;

    return 0;
}

// Function that parses an email address into its domain & username
void parseEmailAddress(string email, string& username, string& domain) {   
   int cut = email.find("@"); 
   
   username = email.substr(0, cut);
   domain = email.substr(cut+1);
   
   // cout << username << endl;  // Debug Flag 5: check parse
   // cout << domain << endl;    // Debug Flag 5: check parse 
}

// Function that removes the .com from a domain or email
void remDotCom(string originalAddress, string& newAddress) {
    int cut = originalAddress.find(".");
    newAddress = originalAddress.substr(0,cut);
}

// Function that splits the username & domain of the current word in binary search [domain.com:user --> domain.com & user are extracted]
void getCurWordUserDom(string& curWord, string& curWordUser, string& curWordDom) {
    int cut = curWord.find(":");
    curWordDom = curWord.substr(0, cut);
    curWordUser = curWord.substr(cut+1);

}

// Function that transfers information from a spam list to a vector
void load(ourvector<string>& spamVect) {

    // Creating variables
    string filename;    // create a filename variable to receive a user response
    string curWord;     // create a variable that determines the current word during the file scan

    cin >> filename;    // getting input for user's file

    spamVect.clear();   // clears imported spam vector (needed to load different spam lists)

    ifstream inFS;  // create an input file stream

    inFS.open(filename);    // open the user's file

    if (!inFS.is_open()) {  // if the file is not found then send a warning message to the user
        cout << "**Error, unable to open '" << filename << "'" << endl;
        return;
    }

    cout << "loading " << "'" << filename  << "'" << endl;  // prints out the file that is loading


    // Gathers each word in the file and adds each word as an element to the vector
    while (inFS >> curWord) {   // gets the current word that is searched for in the spam list and assigns it to curWord variable
        spamVect.push_back(curWord);   // each word from the spam list is added to the spam vector
    }

    cout << "# of spam entries: " << spamVect.size() << endl;   // gets the amount of elements in the list and returns it as the spam entries
    
    inFS.close();   // closes the file & filestream
}

// Traverses through the created spam vector by looping linearly
void display(ourvector<string>& spamVect) {
    for (int i = 0; i < spamVect.size(); i++) { // Loops through each element in the vector and prints that element
        cout << spamVect[i] << endl;    // spamVect[i] is the current vector and for each index, the element is printed out
    }
}

// bool binarySearchUser(ourvector<string>& spamVect, string foundDomain) {
// int l = 0;
//     int r = spamVect.size() - 1;
//     int mid = 0;
//     string username, domain, subDomain, curWord, curWordUser, curWordDom;

//     parseEmailAddress(emailGivenByUser, username, domain);

//     // cout << "Email: " << email << endl;
//     // cout << "Username: " << username << endl;  
//     // cout << "Domain: " << domain << endl;

//     curWord = spamVect[mid];
//     getCurWordUserDom(curWord, curWordUser, curWordDom);

//     // cout << "curWord: " << curWord << endl;
//     // cout << "curWordUser: " << curWordUser << endl;  
//     // cout << "curWordDom: " << curWordDom << endl; 

//     remDotCom(domain, subDomain);

//     // cout << "Domain: " << domain << endl;
//     cout << "Username: " << username << endl;
//     cout << "subDomain: " << subDomain << endl; 
    
//     string domUser = domain + ":" + username;
//     string checkStar = foundDomain + ":" + "*";


//     if (username == "*") {
//         return 1;
//     }

//     if (username == subDomain) {
//         return 0;
//     }


//     while (r >= l) {    
//         mid = l + ((r - l) / 2);

//         curWord = spamVect[mid];
//         getCurWordUserDom(curWord, curWordUser, curWordDom);
//         cout << "Domain: " << domain << endl;
//         cout << "curWordDom: " << curWordDom << endl;

//         cout << "DomUser: " << domUser << endl;
//         cout << "curWord: " << curWord << endl;

//         cout << "Username: " << username << endl;
//         cout << "curWordUser: " << curWordUser << endl;
        
//             if (checkStar == curWord) {
//                 return 1;
//             }
        
//             if(domUser != checkStar) {

//             if (domUser == curWord) {
//                 return 1;
//             }

//             else if (checkStar > curWord) {
//                 l = mid + 1;
//             }

//             else if (checkStar < curWord) {
//                 r = mid - 1;
//             }

//             else {
//                 return 1;
//             }


//         }


//     }

//     return 0;
// }

bool binarySearch(ourvector<string>& spamVect, string email) {
    int l = 0;
    int r = spamVect.size() - 1;
    int mid = 0;
    string username, domain, subDomain, curWord, curWordUser, curWordDom;

    // cout << "Email: " <<  email << endl;

    emailGivenByUser = email; 

    // if spamVect size is 0 no elements so return 0;
    if ((spamVect.size()) == 0) {
        return 0;
    }

    // // if not a proper email then exit


    // checks to see if the user enters a proper @ symbol else not spam since not an email
    size_t found = email.find("@");

    if (found == string::npos) {
    //     cout << "Found!" << endl;
    // } 

    // else {
        return 0;
    }



    parseEmailAddress(email, username, domain);

    // cout << "Email: " << email << endl;
    // cout << "Username: " << username << endl;  
    // cout << "Domain: " << domain << endl;

    curWord = spamVect[mid];
    getCurWordUserDom(curWord, curWordUser, curWordDom);

    // cout << "curWord: " << curWord << endl;
    // cout << "curWordUser: " << curWordUser << endl;  
    // cout << "curWordDom: " << curWordDom << endl; 

    remDotCom(domain, subDomain);

    // cout << "Domain: " << domain << endl;
    // cout << "Username: " << username << endl;
    // cout << "subDomain: " << subDomain << endl; 
    
    string domUser = domain + ":" + username;
    string checkStar = domain + ":" + "*";


    if (username == subDomain) {
        return 0;
    }


    // while (r >= l) {    
    //     mid = l + ((r - l) / 2);

    //     curWord = spamVect[mid];
    //     getCurWordUserDom(curWord, curWordUser, curWordDom);
    //     // cout << "Domain: " << domain << endl;
    //     // cout << "curWordDom: " << curWordDom << endl;

    //     // cout <<  "DomUser: " << domUser << endl;
    //     // cout << "curWord: " << curWord << endl;

    //     // cout << "Username: " << username << endl;
    //     // cout << "curWordUser: " << curWordUser << endl;
        
    //     if (domain == curWordDom) {

    //         // second binary search to check usernames & asterisk exists
    //         if (binarySearchUser(spamVect, domain) == 1) {
    //             return 1;
    //         }
    //         return 0;
    //     }

    //     else if (domain > curWordDom) {
    //         l = mid + 1;
    //     }

    //     else if (domain < curWordDom) {
    //         r = mid - 1;
    //     }

    //     else {
    //         return 1;
    //     }
    // }

    while (l <= r) {
        mid = (r + l) / 2;
        curWord = spamVect[mid];
        // cout << "Domain: " << domain << endl;
        // cout << "curWordDom: " << curWordDom << endl;

        // cout <<  "DomUser: " << domUser << endl;
        // cout << "curWord: " << curWord << endl;

        // cout << "Username: " << username << endl;
        // cout << "curWordUser: " << curWordUser << endl;

        if (checkStar == curWord) {
            return 1;
        }

        if (domUser > curWord) {
            l = mid + 1;
        } 

        else if (domUser < curWord) {
            r = mid - 1;
        }

        else {
            return 1;
        }
    }

    return 0;

}

bool check(ourvector<string>& spamVect, string email) {
    // cout <<  binarySearch(spamVect, email) << endl;

    return binarySearch(spamVect, email);
} 

// Function opens up a file of emails that the user requested and traverses through the list and checks for spams. All non-spams are transferred to a new file by using ifstream & ofstream
void filter(ourvector<string>& spamVect) {
    string fileIn;
    string fileOut;
    
    cin >> fileIn; // gets user's input for the email file
    cin >> fileOut; // gets the user's input for the filtered file (no spam) they want to create 

    ifstream inFS(fileIn); // creates input file stream for user's email list

    // Checks to see if the user's file opened, else send a warning message
    if (!inFS.is_open()) {
        cout << "**Error, unable to open '" << fileIn << "'" << endl;
        return;
    }

    ofstream outFS(fileOut);    // creates an output file stream for the user's selected output file

    int emailsProcessed = 0;    // counter for email processes
    int nonSpam = 0;    // counter for spam process
    int emailID = 0;    // the ID assigned to the email in email list
    string email;   // email being searched through in input file stream of email text files
    string note;    // note being searched through in input file stream of email text files

    while (!inFS.eof()) {
        inFS >> emailID;
        inFS >> email;
        getline(inFS, note);

        if(!inFS.fail()) {
            if (check(spamVect, email) == 0) {
                outFS << emailID << " " << email << note << endl;
                
                nonSpam++;
            }

            emailsProcessed++;

        }
    }

    cout << "# emails processed: " << emailsProcessed << endl;
    cout << "# non-spam emails: " << nonSpam << endl;
}

// 1) Check to see if the domain name matches from spamlist
// 2) check to see the asterisk
// 3) if no asterisk found, then return not spam unless it matches word to word