#include <bits/stdc++.h>
using namespace std;


/*-----Global data-----*/

// queue to store the recieved data in order
queue<string> DATA;
// buffer for proccesing input
string buffer = "";

/*----------*/



/*-----Functions Prototypes-----*/

// function to check data validity when processing buffer
bool data_ok();

// function that process buffer
void process_buffer();

// functions that stores the total resistances of a connection depending on connection
double calculate_series();
double calculate_parallel();

// function that calculates the connection
double calculate_connection(vector<double> &resistances , char type);

/*----------*/



int main() {

    // recieving input string
    string input_string;
    getline(cin , input_string);


    // extracting data
    int input_length = input_string.size(); // input length

    for (int idx = 0; idx < input_length; idx++) // iterating on the input string
    {
        if(input_string[idx] == ' '  ||  input_string[idx] == ','){ // load buffer if reached data seperation character
            process_buffer();
        }
        else{
            buffer += input_string[idx]; // adding the data to the buffer
        }
        if(idx == input_length-1){ // load buffer if reached end of input
            process_buffer();
        }
    }


    // calculating total resistance starting the function depending on parent connection type
    double Total_resistance;
    if(DATA.front() == "s"){
        DATA.pop();
        Total_resistance = calculate_series();
    }
    else{
        DATA.pop();
        Total_resistance = calculate_parallel();
    }


    // printing the result
    cout<<"The total resistance = "<<Total_resistance;

    return 0;
}



/*---------------------------Functions Implementation---------------------------*/

bool data_ok(){
    if(buffer.size() == 1){
        buffer.front() = tolower(buffer.front()); // switch input to lower case for easier checking
        char input = buffer.front(); 

         // check if input is a digit
        if(isdigit(input)) return true;

        // check if input is a valid connection type or e
        return (input == 's' || input == 'p' || input == 'e');
    }
    else return true; // if input is not one char then its a number (true)
}


void process_buffer(){
    if( data_ok() ){
        DATA.push(buffer); // store data into the queue
        buffer.clear();    // clear buffer to start loading new data
    }
    else{
        cout<<"Wrong Description"; // if data isnt valid
        exit(0);
    }
}


double calculate_series(){
    vector<double> series_resistances; // vector to hold the resistances of this connection
    while(true){ // looping till the end of the connection
        if(DATA.front() == "s"){
            DATA.pop();
            series_resistances.push_back(calculate_series()); // go calculate the nested series connection and store it
        }
        else if(DATA.front() == "p"){
            DATA.pop();
            series_resistances.push_back(calculate_parallel()); // go calculate the nested parallel connection and store it
        }
        else if(DATA.front() == "e"){
            DATA.pop();
            return calculate_connection(series_resistances , 's'); // calculate the resistances stored for this connection
        }
        else{
            series_resistances.push_back(stod(DATA.front())); // store this resistance value for calculating the total
            DATA.pop();
        }
    }
    
}
double calculate_parallel(){
    vector<double> parallel_resistances; // vector to hold the resistances of this connection
    double resistance = 0;
    while(true){ // looping till the end of the connection
        if(DATA.front() == "s"){
            DATA.pop();
            parallel_resistances.push_back(calculate_series()); // go calculate the nested series connection and store it
        }
        else if(DATA.front() == "p"){
            DATA.pop();
            parallel_resistances.push_back(calculate_parallel()); // go calculate the nested parallel connection and store it
        }
        else if(DATA.front() == "e"){
            DATA.pop();
            return calculate_connection(parallel_resistances , 'p'); // calculate the resistances stored for this connection
        }
        else{
            parallel_resistances.push_back(stod(DATA.front())); // store this resistance value for calculating the total
            DATA.pop();
        }
    }
}

double calculate_connection(vector<double> &resistances , char type){
    double equivalent = 0; // variable to store the equivalent resistance
    if(type == 's'){ // if series computation
        if(resistances.empty()){ // not enough resistances for calculations
            cout<<"Incorrect Input";
            exit(0);
        }
        for(auto &res : resistances)
            equivalent += res; // calculating equivalent resistance
        
        if(equivalent == 0){
            cout<<"Incorrect Input";
            exit(0);
        }
        return equivalent;
    }
    else{ // if parallel computation
        bool zero_flag = false; // flag to check if any zero resistance found
        if(resistances.size() < 2){ // not enough resistances for computation
            cout<<"Incorrect Input";
            exit(0);
        }
        for(auto &res : resistances)
            if(res == 0){
                zero_flag = true;
            }
            else{
                equivalent += 1/res; // calculating equivalent resistance
            }
        if(zero_flag) return 0; // if found zero then Req = 0
        return 1/equivalent;
    }
}

/*----------------------------------------------------------------*/
