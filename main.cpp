/** ------------------------------------------------------------------
* This program filters a list of emails based on the conditions of a 
* spam list. The list contains emails which are marked as spams.
* The filtering is done by the use of the provided ourvector class,
* binary searches, and file reading/writing. Other commands: loads
* in a spam list, display the spamlist, check if an email is a spam,
* then filter the list and create a new output file of non-spam emails. 
* Class: CS 251, Spring 2020
* Author: Jim Palomo
* Date: Jan. 20, 2020
* ------------------------------------------------------------------**/ 

#include <iostream>
#include <fstream>
#include <string> 
#include "ourvector.h" 

using namespace std;

/* Function calls showcasing the functions used (descriptions for functions are under main) */
void parseEmailAddress(string email, string& username, string& domain);

void parseEmailAddressForStar(string email, string& username, string& domain);

void load(ourvector<string>& spamVect);

void display(ourvector<string>& spamVect);

bool binarySearch(ourvector<string>& spamVect, string email);

bool binarySearchForStar(ourvector<string>& spamVect, string email);

bool check(ourvector<string>& spamVect, string email);

void filter(ourvector<string>& spamVect);

int main() {
    string response;                                        // the command that the user inputs
    string email;                                           // email requested from user to be checked (for check function)
    ourvector<string> spamVect;                             // creates a vector based on the ourvector class to represent the spamVector

    cout << "** Welcome to spam filtering app ** \n" << endl;

    while (response != "#") {                               // allows for command prompt to be repeated
        cout << "Enter command or # to exit> ";             // main command prompt
        cin >> response;                                    // get the response for user input for command

        if (response == "load") {                           // if-statements to check which command was requested by the user & then functions are called accordingly
            load(spamVect);
        } 
        
        else if (response == "display") {
            display(spamVect);
        }

        else if (response == "check") {
            cin >> email;
            if (check(spamVect, email) == 1) {              // if the check function found the email requested then return as spam else non-spam
                cout << email << " is spam" << endl;

            } 

            else {
                cout << email << " is not spam" << endl;
            }
        }

        else if (response == "filter") {
            filter(spamVect);
        } 

        else if (response != "#") {                         // if no user response is associated to a command then send invalid command warning
            cout << "**invalid command" << endl;
        }


        cout << endl;
    }

    cout << endl;

    return 0;
}

/*
Purpose: function that parses an email address into its separate domain & username (must be in form aaa@abc.com)
Parameters: email (the email is being parsed), username (the username that is going to be updated), domain (the domain that is going to be updated)
Return Value: the function returns the new parsed username and domain through pass by reference 
*/
void parseEmailAddress(string email, string& username, string& domain) {   
   int cut = email.find("@"); 
   
   username = email.substr(0, cut);     // gets all before characters before @
   domain = email.substr(cut+1);        // gets all characters after @
}

/*
Purpose: function that parses an email address into its separate domain & username (must be in form aaa@abc.com)
Parameters: email (the email is being parsed), username (the username that is going to be updated to a "*"), domain (the domain that is going to be updated)
Return Value: the function returns the new parsed username ("*") and domain through pass by reference 
*/
void parseEmailAddressForStar(string email, string& username, string& domain) {   
   int cut = email.find("@"); 
   
   username = "*";                      // change username to "*"
   domain = email.substr(cut+1);        // gets all characters after @
}

/*
Purpose: function transfers emails (in the form domain.com:username) from a spamlist.txt file to the spam vector created in main. This is done by traversing the .txt file and adding its elements to a vector using .push_back()
Parameters: spamVect (the spam vector created in main which is used to store the spam emails)
Return Value: returns the updated spam vector containing spam from the spamlist by pass by reference which reduces the number of elements accessed
*/
void load(ourvector<string>& spamVect) {

    string filename;                                                    // create a filename variable to receive a user response
                            
    string curWord;                                                     // create a variable that determines the current word during the file scan
                        
    cin >> filename;                                                    // getting input for user's file
                        
    spamVect.clear();                                                   // clears imported spam vector (needed to load different spam lists)
                        
    ifstream inFS;                                                      // create an input file stream
                        
    inFS.open(filename);                                                // open the user's file
                        
    if (!inFS.is_open()) {                                              // if the file is not found then send a warning message to the user
        cout << "**Error, unable to open '" << filename << "'" << endl;
        return;
    }

    cout << "loading " << "'" << filename  << "'" << endl;      // prints out the file that is loading


    while (inFS >> curWord) {                                   // traverses the spamlist.txt file and adds each email from the list to the spamVector
        spamVect.push_back(curWord);                            // each word from the spam list is added to the spam vector
    }

    cout << "# of spam entries: " << spamVect.size() << endl;   // gets the amount of elements in the list and returns it as the spam entries
    
    inFS.close();                                               // closes the file & filestream
}

/*
Purpose: traverses and prints out the current emails from the spamlist through the created spam vector by looping linearly
Parameters: spamVect (the spam vector created in main which is used to store the spam emails)
Return Value: returns the spam vector and uses pass by reference to limit the number of elements accessed
*/
void display(ourvector<string>& spamVect) {
    for (int i = 0; i < spamVect.size(); i++) {     // loops through each element in the vector and prints that element
        cout << spamVect[i] << endl;                // spamVect[i] is the current vector and for each index, the element is printed out
    }
}

/*
Purpose: function used to search through the list of spam contained in the spam vector for spam emails. If the user's email matches current email that the search is on then return as spam else non-spam.
Parameters: spamVect (the spam vector contains the spam emails from the spam list; needed for traversing to locate spam emails with the user's provided email that they want to check), email (user's email request to be checked)  
Return Value: returns the spam vector and uses pass by reference to limit the number of accesses.  Also returning a boolean value which is used in check to determine if the email is a spam or not
*/
bool binarySearch(ourvector<string>& spamVect, string email) {
    int l = 0;                          // represents left and is used for the binary search (same as low)
    int r = spamVect.size() - 1;        // represents right and is used for the binary search (same as high)
    int mid = 0;                        // represents the middle which is used for splitting & identifying words in the binary search
    string username;                    // username from the email that the user gives
    string domain;                      // domain from the email that user gives
    string curWord;                     // the current word used for comparison with the user's email inside the binary search
    string domUser;                     // variable representing domain.com:username, used for binary search

    if ((spamVect.size()) == 0) {       // used to make sure that if there is no spamlist loaded then any email that the user checks is not a spam 
        return 0;
    }

    size_t found = email.find("@");     // used for the conditional below to check to see if the user enters a proper email address (e.g. someone@email.com)

    if (found == string::npos) {        // if the character, in this case, @ is not found then return any response back to user (e.g. pooja will be non-spam since there is no @domain.com)
        return 0;
    }

    parseEmailAddress(email, username, domain);     // calls parse function to parse emails into username and domain separately
    
    domUser = domain + ":" + username;              // concatenates the domain and username into domain:username which is used in the binary search for comparison

    while (l <= r) {                                // binary search starts here (uses a while loop and does not stop until l <= r condition is broken)
        mid = l + ((r - l) / 2);                    // assigns the midpoint for binary search (not using [(l + r) / 2] to not encounter overloading errors; covered in lecture)
        
        curWord = spamVect[mid];                    // assigns the current word, used for comparison, to the midpoint

        if (domUser == curWord) {                   // checks to see if the current word contains a star and if the midpoint also, returns spam (searches for the star first)
            return 1;
        }

        if (domUser > curWord) {                    // if the domain.com:username does not match the current's word domain.com:username then readjust & redo binary search until found
            l = mid + 1;
        } 

        else if (domUser < curWord) {               // if the domain.com:username does not match the current's word domain.com:username then readjust & redo binary search until found
            r = mid - 1;
        }
    }

    return 0;                                       // if user's email is not found on the list then return non-spam

}

/*
Purpose: function used to search through the list of spam contained in the spam vector for spam emails. The function looks for emails with a star ("*") and checks if the domain+star (domain.com:star) is in the list.
Parameters: spamVect (the spam vector contains the spam emails from the spam list; needed for traversing to locate spam emails with the user's provided email that they want to check), email (user's email request to be checked)  
Return Value: returns the spam vector and uses pass by reference to limit the number of accesses. Also returning a boolean value which is used in check to determine if the email is a spam or not
*/
bool binarySearchForStar(ourvector<string>& spamVect, string email) {
    int l = 0;                          // represents left and is used for the binary search (same as low)
    int r = spamVect.size() - 1;        // represents right and is used for the binary search (same as high)
    int mid = 0;                        // represents the middle which is used for splitting & identifying words in the binary search
    string username;                    // username from the email that the user gives
    string domain;                      // domain from the email that user gives
    string curWord;                     // the current word used for comparison with the user's email inside the binary search
    string domUser;                     // variable representing domain.com:username, used for binary search

    if ((spamVect.size()) == 0) {       // used to make sure that if there is no spamlist loaded then any email that the user checks is not a spam 
        return 0;
    }

    size_t found = email.find("@");     // used for the conditional below to check to see if the user enters a proper email address (e.g. someone@email.com)

    if (found == string::npos) {        // if the character, in this case, @ is not found then return no spam response back to user (e.g. pooja will be non-spam since there is no @domain.com)
        return 0;
    }

    parseEmailAddressForStar(email, username, domain);     // calls parse function to parse emails into username and domain separately
    
    domUser = domain + ":" + username;                      // concatenates the domain and username into domain:username which is used in the binary search for comparison

    while (l <= r) {                                        // binary search starts here (uses a while loop and does not stop until l <= r condition is broken)
        mid = l + ((r - l) / 2);                            // assigns the midpoint for binary search (not using [(l + r) / 2] to not encounter overloading errors; covered in lecture)

        curWord = spamVect[mid];                            // assigns the current word, used for comparison, to the midpoint

        if (domUser == curWord) {                           // checks to see if the current word contains a star and if the midpoint also, returns spam (searches for the star first)
            return 1;       
        }       

        if (domUser > curWord) {                            // if the domain.com:username does not match the current's word domain.com:username then readjust & redo binary search until found
            l = mid + 1;        
        }       

        else if (domUser < curWord) {                       // if the domain.com:username does not match the current's word domain.com:username then readjust & redo binary search until found
            r = mid - 1;        
        }       
    }       

    return 0;                                               // if user's email is not found on the list then return non-spam

}

/*
Purpose: function that returns the binary search's boolean value which is used to determine if an email is a spam or not. This is done by checking the binary searches conducted and seeing if both say it is a spam or not.
Parameters: spamVect (the spam vector containing the spam emails from the spamlist.txt file), email (the email that the user requested to be checked)
Return Value: returns a boolean value to check if the email is a spam or not [0 = not spam; 1 = spam] & the spam vector with pass by reference to limit elements accessed
*/
bool check(ourvector<string>& spamVect, string email) {
    bool check1 = binarySearch(spamVect, email);            // first binary search looks for domain + user and sees if the spam vector contains it
    bool check2 = binarySearchForStar(spamVect, email);     // the second binary search looks for the domain + * and sees if the spam vector contains it
    
    if ((check1 || check2) == 1) {                          // if one binary searches find a match then return spam 
        return 1;
    } 
    
    else if ((check1 && check2) == 0) {                     // else if they both are not returning matches, then the email being checked is not a match
        return 0;
    }
} 

/*
Purpose: function that filters the emails from a text file and checks to see if those emails are spams. Uses the check function to compare emails with the spam vector containing the spam emails. Uses file reading and writing to transfer non-spam emails
Parameters: spamVect (spam vector containing the spam emails from the spamlist.txt file)
Return Value: the filter returns an output file that the user decides to name containing non-spam emails by filteration & spam vector by pass by reference
*/
void filter(ourvector<string>& spamVect) {
    string fileIn;              // the email list being checked
    string fileOut;             // the new output file that the user wants to use
    string email;               // email being searched through in input file stream of email text files
    string note;                // note that is provided after the email that is being searched through in input file stream of email text files
    int processedEmails = 0;    // counter for email processes
    int nonSpamEmails = 0;      // counter for non spam emails
    int emailIDNumber = 0;      // the ID assigned to the email in email list (number provided before email)
    
    cin >> fileIn;              // gets user's input for the email file
    cin >> fileOut;             // gets the user's input for the filtered file (no-spam) that they want to create 

    ifstream inFS(fileIn);      // creates input file stream for user's email list

    if (!inFS.is_open()) {      // checks to see if the user's file opened properly or is open at all, else send a warning message that the file was not opened
        cout << "**Error, unable to open '" << fileIn << "'" << endl;
        return;
    }

    ofstream outFS(fileOut);    // creates an output file stream for the user's selected output file

    while (!inFS.eof()) {       // traverses the email until the end of file and obtains the emailIDNumber, email name, and the note provided based on the email
        inFS >> emailIDNumber;
        inFS >> email;
        getline(inFS, note);

        if(!inFS.fail()) {      // if the file stream does not fail meaning no error occurred then write email information (ID number, address, and note) to output file 
            if (check(spamVect, email) == 0) {
                outFS << emailIDNumber << " " << email << note << endl;
                
                nonSpamEmails++;
            }

            processedEmails++;

        }
    }

    cout << "# emails processed: " << processedEmails << endl;
    cout << "# non-spam emails: " << nonSpamEmails << endl;
}