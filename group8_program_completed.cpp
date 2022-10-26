#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <ctime>
using namespace std;


// Prepared By Group 8 Student:
// HO KO EE, WONG JUN YUAN, LIAW YU YII, LEE WOON XUN 


//Admin user data
string admin_name = "admin";
string password = "testing123";
bool login_status = false;

int top_ticket_id = 00;

//structure / Records
struct Subway{
	int id;
	string name, nearby_attraction;
	double distance_prev, cost_prev, distance_next, cost_next;
	int duration_prev, duration_next;
	//doubly linked list, next station, prev station
	Subway *next, *prev; 
}*head, *tail, *curr;

struct Transaction{ //singly linked list
	string ticket_id, depart_station, destination_station, departure_time;
	string cus_name, cus_ic;
	double price, distances;
	int quantity; 
	
	Transaction *next;
	
}*trans_head, *trans_tail, *trans_curr;


//creating new Nodes for each structure
struct Subway *createNewSubNode(int id, string name, double distance_prev, double cost_prev, int duration_prev, double distance_next, double cost_next, int duration_next,string nearby_attraction){
	struct Subway *temp;
	temp = new struct Subway;
	temp->id = id;
	temp->name = name;
	temp->distance_prev = distance_prev;
	temp->cost_prev = cost_prev;
	temp->duration_prev = duration_prev;
	temp->distance_next = distance_next;
	temp->cost_next = cost_next;
	temp->duration_next = duration_next;
	temp->nearby_attraction = nearby_attraction;
	temp->next = NULL;
	temp->prev = NULL;
	
	return temp;
}

struct Transaction *createNewTransNode(string ticket_id, string depart, string destination, string depart_time, string cus_name, string cus_ic, double price, int quantity, double distances){
	struct Transaction *temp;
	temp = new struct Transaction;
	temp->ticket_id = ticket_id;
	temp->depart_station = depart;
	temp->destination_station = destination;
	temp->departure_time = depart_time;
	temp->cus_ic = cus_ic;
	temp->cus_name = cus_name;
	temp->price = price;
	temp->quantity = quantity;
	temp->distances = distances;
	temp->next = NULL;
	
	return temp; 
};

void insertFront_trans(string ticket_id, string depart, string destination, string depart_time, string cus_name, string cus_ic, double price, int quantity, double distances){
	
	Transaction *newnode = createNewTransNode(ticket_id, depart, destination, depart_time, cus_name, cus_ic, price, quantity, distances);
	cout<<depart_time;
	if(trans_head == NULL){
		// if there is nothing in the list
		trans_head = newnode;
		trans_tail = newnode;
	} else {
		newnode->next = trans_head;
		trans_head = newnode;	
	}
	
	
}

bool check_station(string stat);
void display_routes_adm(string depart, string destination, tm *ltm);

//function for sellecting user type
int identify_user_type(){
	int user_type; //1 for normal cus , 2 for admin user
	cout<<endl<<endl<<"\tWelcome to Subway Ticket Booking System"<<endl
		<<"----------------------------------------------------------"<<endl
		<<"Please Select Your Identity"<<endl
		<<"Enter 1 for normal customer"<<endl
		<<"Enter 2 for admin user"<<endl
		<<"Enter 3 to Exit Program"<<endl
		<<"Enter your option here: ";
	
	
	while(true){ 
		if(cin>>user_type){
			if(user_type == 1 || user_type ==2 || user_type == 3){
				break;		
			}
		} else{
			cin.clear();
			cin.ignore();
		}
		cout<<"Invalid Input, Please Try Again"<<endl
			<<"Enter Your option here: ";
	}
	return user_type;
}

//function to insert static subway to the linked list
void insertAtEnd_subways(Subway *node){
	if(head == NULL){
		head = node;
		tail = node;
	} else{
		//insert to the end using tail
		node->prev = tail;
		tail->next = node;
		tail = node;	
	}
}

//functions to load the system at start
void load_system(){
	cout<<"Booting System . . . "<<endl<<endl;
	//setting up the default setting of the system, eg: subways details, static data, etc
	string station_name[8] = {"Titiwangsa", "PWTC", "Sultan Ismail", "MajlisJamek", "Plaza Rakyat", "Hang Tuah", "Pudu", "Chan Sow Lin"};
	//data to previus station
	double distance_prev[8] = {0, 4, 8,8,6,10,5,5}; //km
	double cost_prev[8]= {0, 0.4, 0.8, 0.8, 0.6, 1.0, 0.5, 0.5};
	int duration_prev[8] = {0, 3, 7, 7, 5, 9, 4, 4};//minutes
	//data to next station
	double distance_next[8] = {4,8,8,6,10,5,5,0}; //km
	double cost_next[8] = {0.4, 0.8, 0.8, 0.6, 1.0, 0.5, 0.5, 0};
	int duration_next[8] = {3,7,7,5,9,4,4,0}; //minutes
	string nearby_attraction[8] = {"National Arts", "Petrosains", "MinNature", "Suria KLCC", "St. Anthony Church", "Bert's Garden Seafood Restaurant", "Waka Waka", "Central Mart"};
	
	//While loop to create each node and forming linked list
	
	int i = 0;
	while(i<8){
		//creating new node
		Subway *newnode = createNewSubNode(i+1, station_name[i], distance_prev[i], cost_prev[i], duration_prev[i], distance_next[i], cost_next[i], duration_next[i], nearby_attraction[i]);
		//inserting each subway to the end
		insertAtEnd_subways(newnode);
		i++;
	}
	
	
	return;
}

//displaying main menu

void display_mainmenu_cus(){
	cout<<endl<<endl<<"\t\t\t Main Menu"<<endl
		<<"-------------------------------------------------------"<<endl
		<<"1) Routes Details"<<endl
		<<"2) Purchase Ticket"<<endl
		<<"3) Purchase History"<<endl
		<<"4) Delete Ticket"<<endl
		<<"5) Log Out"<<endl;
}

void view_routes_cus(){
	cout<<endl<<endl<<"\t\t\t Routes"<<endl
		<<"----------------------------------------------------------------------------------------"<<endl;
	
	curr = head;
	
	while(curr->next){
		cout<<curr->name<<"  -->  ";
		curr = curr->next;
	}
	//last station
	cout<<curr->name<<endl<<endl;
	
	curr = tail;
	
	while(curr->prev){
		cout<<curr->name<<"  -->  ";
		curr = curr->prev;
	}
	
	cout<<curr->name<<endl<<endl;
}

//function to display the routes, use the search function after this.
void display_routes(){
	
	cout<<endl<<endl<<"\t\t\t Routes"<<endl
		<<"----------------------------------------------------------------------------------------"<<endl;
	
	curr = head;
	
	while(curr->next){
		cout<<curr->name<<"  -->  ";
		curr = curr->next;
	}
	//last station
	cout<<curr->name<<endl<<endl;
	
	curr = tail;
	
	while(curr->prev){
		cout<<curr->name<<"  -->  ";
		curr = curr->prev;
	}
	
	cout<<curr->name<<endl<<endl;
	
	int opt; 
	cout<<endl<<endl<<"\t\t Operations"<<endl
		<<"-----------------------------------------------------"<<endl
		<<"1) Search For Station Details "<<endl
		<<"2) Search Routes information (price, duration, etc) "<<endl
		<<"3) Back to Main Menu"<<endl
		<<"Enter Option [1-3]: ";
	
	//checking input 
	while(true){
		if(cin >> opt){
			if(opt == 1){
				// call search station function or write what ever code for searching station details
				
				
				string search_station;
			
				cin.ignore();
				while(true){
					cout<<"Please Enter Subway Station Name: ";
					getline(cin, search_station);
					
					if(check_station(search_station)){
						break;
					} else {
						cout<<"No such Station! Please Try Agian!"<<endl;
					}
				}
				
				//displaying sub if subway exist
				cout<<endl<<"\t\t Station:  "<<search_station<<endl
					<<"-------------------------------------------------"<<endl
					<<"Station ID: "<<curr->id<<endl<<endl
					<<"Titiwangsa to Chan Sow Lin line"<<endl<<endl
					<<"Previous Station: "<<curr->prev->name<<endl
					<<"Distance to Previous Station: "<<curr->distance_prev<<" km"<<endl
					<<"Duration to Previous Station: "<<curr->duration_prev<<" minutes"<<endl
					<<fixed<<setprecision(2)<<"Cost to Previous Station: RM "<<curr->cost_prev<<endl
					<<"Next Station: "<<curr->next->name<<endl
					<<"Distance to Next Station: "<<curr->distance_next<<" km"<<endl
					<<"Duration to Next Station: "<<curr->duration_next<<" minutes"<<endl
					<<"Cost to Next Station: RM"<<curr->cost_next<<endl
					<<endl
					<<"Chan Sow Lin to Titiwangsa line"<<endl<<endl
					<<"Previous Station: "<<curr->next->name<<endl
					<<"Distance to Previous Station: "<<curr->distance_next<<" km"<<endl
					<<"Duration to Previous Station: "<<curr->duration_next<<" minutes"<<endl
					<<fixed<<setprecision(2)<<"Cost to Previous Station: RM "<<curr->cost_next<<endl
					<<"Next Station: "<<curr->prev->name<<endl
					<<"Distance to Next Station: "<<curr->distance_prev<<" km"<<endl
					<<"Duration to Next Station: "<<curr->duration_prev<<" minutes"<<endl
					<<"Cost to Next Station: RM"<<curr->cost_prev<<endl;
				
				break; //breaking loop
			} else if (opt == 2){
				// searching routes details (ask user for 2 stations and give details to them) 
				int ans = 0;
				while(true){
					
					view_routes_cus();
						
					string depart, destination;
				
					cin.ignore();
					cout<<endl<<"Enter Depart Station Name: ";
					getline(cin, depart);
					
					while(!check_station(depart)){
				   		cout<<"Please Enter A Valid Station: ";	
				   		getline(cin, depart);
					}
					
					cout<<"Enter Destination Station: ";
					getline(cin, destination);
							
					while(!check_station(destination) || destination==depart){
						cout<<"Please Enter A valid station: ";
						getline(cin, destination);
					}
					
					time_t now = time(0);
						
					tm *ltm = localtime(&now);
					
					display_routes_adm(depart, destination, ltm);
					
					cout<<"Do you want to continue searching? [0-yes, 1-no]: ";
					
					while(true){
						//data validation
						if(cin>>ans){
							break;
						} else {
							cin.clear();
							cin.ignore();
							cout<<endl<<"Invalid Data Types! Please Try Again [0 or 1] : ";
						}
					}	
					if(ans == 1){
						return;
					}
				}
				break; //breaking out of the loops
			} else if (opt == 3){
				return;
			}
		} else {
			cin.clear();
			cin.ignore();
			cout<<"Invalid Input! Please Try Again [1-3] : ";
		}
	}
	
}

//getting answer from customer main menu*
int mainmenu_input_cus(){
	int opt;
	cout<<"Enter Your Choices: ";

	while(true){
		//data validation, helps in not crashing the program if user enter a string value rather than an integer value
		if(cin>>opt){
			if(opt == 1 || opt == 2 || opt == 3 || opt == 4 || opt == 5){
				return opt;
			} else {
				
				cin.clear();
				cin.ignore();
				cout<<"No such option! Try again [1-5] : ";
			}
		} else {
			cin.clear();
			cin.ignore();
			cout<<endl<<"Invalid Data Type! Please Try Again [1-5] : ";
		}
	}
	cin.ignore();
}

void display_mainmenu_admin(){
	cout<<endl<<endl<<"\t\t\t Admin Main Menu"<<endl
		<<"-------------------------------------------------------"<<endl
		<<"1) View All Subway Station or Routes"<<endl
		<<"2) Add New Subway Station"<<endl
		<<"3) Edit Existing Subway Station"<<endl
		<<"4) Customer Purchase History"<<endl
		<<"5) Edit or Delete Ticket information"<<endl
		<<"6) Log Out"<<endl;
}

int mainmenu_input_admin(){
	int opt;
	cout<<"Enter Your Choices [1-6] : ";
	while(true){
		//data validation
		if(cin>>opt){
			return opt;
		} else {
			cin.clear();
			cin.ignore();
			cout<<endl<<"Invalid Data Types! Please Try Again [1-6] : ";
		}
	}
	cin.ignore();
}

//function to check whether user input station is valid
bool check_station(string stat){
	curr = head;
	while(curr){
		if(curr->name == stat){
			return true;
		}
		curr = curr->next;
	}
	return false;
}

//function to display specific ticket details with tticket id
void display_ticket(string ticket_id){
	if(trans_head){

		trans_curr = trans_head;
		bool found = false;
		while(trans_curr){
			//if item found
			if(trans_curr->ticket_id.compare(ticket_id) == 0){
				found = true;
				break;
			}
			trans_curr = trans_curr->next;
		}
		cout<<trans_curr->departure_time;
		//if ticket found then display
		if(found){
			cout<<endl<<endl<<"\t\t Ticket ID : "<< trans_curr->ticket_id<<endl
				<<"-----------------------------------------------------"<<endl
				<<"Departure Location:  "<<trans_curr->depart_station<<endl
				<<"Destination Location: "<<trans_curr->destination_station<<endl
				<<"Departure Time: "<<trans_curr->departure_time<<endl
				<<"Travel Distances: "<<trans_curr->distances<<" km"<<endl
				<<"Customer Name: "<< trans_curr->cus_name<<endl
				<<"Customer Identity No: "<< trans_curr->cus_ic<<endl
				<<"Quantity: "<<trans_curr->quantity<<endl
				<<"------------------------------------------------------"<<endl
				<<fixed<<setprecision(2)<<"Total Price : RM "<<trans_curr->price<<endl;
		}
		else {
			cout<<endl<<"No Ticket Found with give ticket ID"<<endl;
		}
		
		
		
	} else{
		cout<<endl<<endl<<"No Tickets Found in the list"<<endl;
	}
}

void purchase_func(string depart,string destination,tm *ltm){
	//6.00am --> 1.00am
	// 6->1am = 19 hours, 19*60/30 = 38 trips
	//6am --> 6 *60 = 360 minutes
	//FORMAT IN MINUTES, to get back time, titi_train/60 --> if got remaider, remainder mul 60
	
	int titi_train[38] = {360,};
	int i = 1;
	while(i<39){
		titi_train[i] = titi_train[i-1] + 30;
		if(titi_train[i] == 1500){
			titi_train[i] = 60;
		}

		i++;
	}
	
	Subway *departure = new struct Subway; 
	//the depart station
	i = 0;
	departure = head;
	while(departure){
		if(departure->name == depart){
			break;
		}
		i++;
		departure= departure->next;
	}
	
	int x = 0;
	Subway *dest = head;
	while(dest){
		if(dest->name == destination){
			break;
		}
		x++;
		dest = dest->next;
	}
	
	
	int current_time = ltm->tm_hour*60 + ltm->tm_min;
	
	int next_arrival=0;
	while(next_arrival<39){
		if(titi_train[next_arrival] >= current_time){
			break;
		}
		next_arrival++;
	}
	int next_available = titi_train[next_arrival]; //next available on the titiwangsa train
	
	//if i>x meaning it's from Chan --> titiwangsa line
	// if i<x meaning it's from Titiwangsa --> Chan line]
	double price =0;
	int duration =0; 
	double total_distances = 0;
	if(i>x){
		curr = tail;
		while(curr){ //getting next available time
			if(curr->name == destination){
				break;
			}
			next_available += curr->distance_prev;
			curr = curr->prev;
		}
		
		curr = departure;
		while(curr){
			if(curr->name == destination){
				break;
			}
			price +=curr->cost_prev;
			duration +=curr->duration_prev;
			total_distances += curr->distance_prev;
			curr = curr->prev;
		}
	}else if(i<x){
		curr = head;
		while(curr){
			if(curr->name == destination){
				break;
			}
			next_available += curr->duration_next;
			curr= curr->next;
		}
		
		curr = departure;
		while(curr){
			if(curr->name == destination){
				break;
			}
			price +=curr->cost_next;
			duration +=curr->duration_next;
			total_distances += curr->distance_next;
			curr = curr->next;
		}
	}
	cout<<next_available;
	int hr = next_available/60;
	int minutes = next_available%60;

	cout<<endl<<endl<<"\tComfirm Purchase"<<endl
		<<"-------------------------------------------------------"<<endl
		<<"Departure Location: "<<depart<<endl
			  <<"Destination Location: "<<destination<<endl
			  <<"Duration: "<<duration<<" minutes"<<endl
			  <<"Travel Distances: "<<total_distances<<" km"<<endl
			  <<"Price: RM "<<price<<endl
			  <<"Nearest Arrival Time: "<<hr<<":"<<minutes<<":00"<<endl;
	
	// Do confirmation purchase, purchase function.
	char confirmation; 
	cout<<"Proceed to checkout? : [Y|N] ";
	cin>>confirmation;
	
	while(confirmation != 'Y' && confirmation != 'N'){
		cout<<"Invalid Input, Please Enter [Y|N] only: ";
		cin>>confirmation;		
	}
	
	if(confirmation == 'Y'){
		//proceed purchase		
		cout<<hr<<minutes<<endl; 
		string depart_time = to_string(hr);
		depart_time.append(":");
		depart_time.append(to_string(minutes));
		depart_time +=":00";
		string ticket_id, cus_name,  cus_ic;
		int quantity;
		cout<<endl<<"\t\t Customer Details "<<endl
			<<"-------------------------------------------------"<<endl;
			cin.ignore();
		cout<<"Enter Your name: ";
		getline(cin, cus_name);
		cout<<"Enter your Identity No (IC): ";
		getline(cin, cus_ic);
		cout<<"How Many Ticket to purchase: ";
		cin>>quantity;
		price = price * quantity;
		cout<<price;		
		//generating ticket id
			ticket_id = "T" + (to_string(top_ticket_id+1));
			top_ticket_id +=1;
		
		insertFront_trans(ticket_id, depart, destination, depart_time, cus_name, cus_ic, price, quantity, total_distances);		
		
		display_ticket(ticket_id);
		
	} else {
		//cancel transaction, return to main
		return;
	}
}

//----------------------------------------------------------------
//ADMIN LOGIN -- koee
bool login_verification(){
	
	string username, pass;
	int try_time=0;
	
	cout<<endl<<"\t\t Identity Verification "<<endl
			<<"-------------------------------------------------"<<endl;
	cin.ignore();
	
	
	// only allow 3 failure
	while(try_time<3){
		cout<<"Enter username: ";
		getline(cin, username);
		cout<<"Enter Password: ";
		getline(cin, pass);
		if(username.compare(admin_name) == 0){ //if name match
		
			if(pass.compare(password) ==0 ){ //check if password match
				cout<<endl<<"Loggin in . . ."<<endl<<endl;
				login_status = true;
				return true;
						
			} else {
				
				cout<<endl<<"Invalid Username or Password! Retry Chances: "<< (3-(try_time+1))<<endl<<endl;
				
			}
			
		} else{
			cout<<endl<<"No Such User! Retry Chances: "<< (3-(try_time+1))<<endl<<endl;
		
		}
		
		try_time +=1;
	}
	
	return false;
}


//function to display searched routes
void display_routes_adm(string depart, string destination, tm *ltm){
	int titi_train[38] = {360,};
	int i = 1;
	while(i<39){
		titi_train[i] = titi_train[i-1] + 30;
		if(titi_train[i] == 1500){
			titi_train[i] = 60;
		}

		i++;
	}
	
	Subway *departure = new struct Subway; 
	//the depart station
	i = 0;
	departure = head;
	while(departure){
		if(departure->name == depart){
			break;
		}
		i++;
		departure= departure->next;
	}
	
	int x = 0;
	Subway *dest = head;
	while(dest){
		if(dest->name == destination){
			break;
		}
		x++;
		dest = dest->next;
	}
	
	
	int current_time = ltm->tm_hour*60 + ltm->tm_min;
	
	int next_arrival=0;
	while(next_arrival<39){
		if(titi_train[next_arrival] >= current_time){
			break;
		}
		next_arrival++;
	}
	int next_available = titi_train[next_arrival]; //next available on the titiwangsa train
	
	//if i>x meaning it's from Chan --> titiwangsa line
	// if i<x meaning it's from Titiwangsa --> Chan line]
	double price =0;
	int duration =0; 
	double total_distances = 0;
	if(i>x){
		curr = tail;
		while(curr){ //getting next available time
			if(curr->name == destination){
				break;
			}
			next_available += curr->distance_prev;
			curr = curr->prev;
		}
		
		curr = departure;
		while(curr){
			if(curr->name == destination){
				break;
			}
			price +=curr->cost_prev;
			duration +=curr->duration_prev;
			total_distances += curr->distance_prev;
			curr = curr->prev;
		}
	}else if(i<x){
		curr = head;
		while(curr){
			if(curr->name == destination){
				break;
			}
			next_available += curr->duration_next;
			curr= curr->next;
		}
		
		curr = departure;
		while(curr){
			if(curr->name == destination){
				break;
			}
			price +=curr->cost_next;
			duration +=curr->duration_next;
			total_distances += curr->distance_next;
			curr = curr->next;
		}
	}
	cout<<next_available;
	int hr = next_available/60;
	int minutes = next_available%60;

	cout<<endl<<endl<<"\tRoutes Information"<<endl
		<<"-------------------------------------------------------"<<endl
		<<"Departure Location: "<<depart<<endl
			  <<"Destination Location: "<<destination<<endl
			  <<"Duration: "<<duration<<" minutes"<<endl
			  <<"Travel Distances: "<<total_distances<<" km"<<endl
			  <<"Price: RM "<<price<<endl
			  <<"Nearest Arrival Time: "<<hr<<":"<<minutes<<":00"<<endl;
			  
	return;
}

//routes options (viwe subway details, view routes details, back to main menu

void routes_options_admin(){
	
	int opt;
	//Display options
	cout<<endl<<"Options: "<<endl;
	cout<<"1) View Subway Details "<<endl
		<<"2) View Routes Details "<<endl
		<<"3) Back to Main Menu   "<<endl<<endl;
		
	cout<<"Enter Your Choices [1-3] :";
	//data validation, Prevent system crash
	while(true){
		//data validation, helps in not crashing the program if user enter a string value rather than an integer value
		if(cin>>opt){
			
			if(opt == 1 || opt == 2 || opt ==3){
				
				break;
			
			} else {
				
				cout<<endl<<"Please enter a valid option [1-3]: ";
		
			}
		
		} else {
		
			cin.clear();
			cin.ignore();
			cout<<endl<<"Invalid Data Type! Please Try Again [1-4] : ";
		
		}
	}
	
	switch(opt){
		case 1: {
			// view subway details, get input subway station name
			
			string search_station;
			
			cin.ignore();
			while(true){
				cout<<"Please Enter Subway Station Name: ";
				getline(cin, search_station);
				
				if(check_station(search_station)){
					break;
				} else {
					cout<<"No such Station! Please Try Agian!"<<endl;
				}
			}
			
			//displaying sub if subway exist
			cout<<endl<<"\t\t Station:  "<<search_station<<endl
				<<"-------------------------------------------------"<<endl
				<<"Station ID: "<<curr->id<<endl<<endl
				<<"Titiwangsa to Chan Sow Lin line"<<endl<<endl
				<<"Previous Station: "<<curr->prev->name<<endl
				<<"Distance to Previous Station: "<<curr->distance_prev<<" km"<<endl
				<<"Duration to Previous Station: "<<curr->duration_prev<<" minutes"<<endl
				<<fixed<<setprecision(2)<<"Cost to Previous Station: RM "<<curr->cost_prev<<endl
				<<"Next Station: "<<curr->next->name<<endl
				<<"Distance to Next Station: "<<curr->distance_next<<" km"<<endl
				<<"Duration to Next Station: "<<curr->duration_next<<" minutes"<<endl
				<<"Cost to Next Station: RM"<<curr->cost_next<<endl
				<<endl
				<<"Chan Sow Lin to Titiwangsa line"<<endl<<endl
				<<"Previous Station: "<<curr->next->name<<endl
				<<"Distance to Previous Station: "<<curr->distance_next<<" km"<<endl
				<<"Duration to Previous Station: "<<curr->duration_next<<" minutes"<<endl
				<<fixed<<setprecision(2)<<"Cost to Previous Station: RM "<<curr->cost_next<<endl
				<<"Next Station: "<<curr->prev->name<<endl
				<<"Distance to Next Station: "<<curr->distance_prev<<" km"<<endl
				<<"Duration to Next Station: "<<curr->duration_prev<<" minutes"<<endl
				<<"Cost to Next Station: RM"<<curr->cost_prev<<endl;
				
				
			return;
			
			break;
		
		}
		case 2: {
			// view routes details, get destination and depart stations
			
			//gettiing user input destination and depart station
			int ans = 0;
			while(true){
				
				view_routes_cus();
					
				string depart, destination;
			
				cin.ignore();
				cout<<endl<<"Enter Depart Station Name: ";
				getline(cin, depart);
				
				while(!check_station(depart)){
			   		cout<<"Please Enter A Valid Station: ";	
			   		getline(cin, depart);
				}
				
				cout<<"Enter Destination Station: ";
				getline(cin, destination);
						
				while(!check_station(destination) || destination==depart){
					cout<<"Please Enter A valid station: ";
					getline(cin, destination);
				}
				
				time_t now = time(0);
					
				tm *ltm = localtime(&now);
				
				display_routes_adm(depart, destination, ltm);
				
				cout<<"Do you want to continue searching? [0-yes, 1-no]: ";
				
				while(true){
					//data validation
					if(cin>>ans){
						break;
					} else {
						cin.clear();
						cin.ignore();
						cout<<endl<<"Invalid Data Types! Please Try Again [0 or 1] : ";
					}
				}	
				if(ans == 1){
					return;
				}
			}	
			break;
		}
		case 3:{
			//return to main menu
			return;
			break;
		}
	}
}


// Function to add new subway station

void add_sub(){
	
	int continue_add_sub;
	string name, nearby_attraction;
	int id;
	double distance_prev, cost_prev,distance_next, cost_next;
	int duration_prev, duration_next;
	int from_which_lane;
	string next_station_name;
	
	while(true){
	  	view_routes_cus();
	
		
		Subway *current = new Subway;
		
		cout<<endl;
		
		cout<<"Please enter which direction to input station [0 for "<<head->name<<" , 1 for "<<tail->name<<" ]: ";
		//data validation
		while(true){
			if(cin>>from_which_lane){
				if(from_which_lane == 0){
					current = head;
					break;
				} else if(from_which_lane == 1){
					current = tail;
					break;
				}
			}
			
			cin.clear();
			cin.ignore();
			cout<<"Invalid Input! Please Try Again: ";
		}
		id = tail->id +1;
		cin.ignore();
		cout<<"Enter New Station Name: ";
		getline(cin,name);
		//id, string name, double distance_prev, double cost_prev, int duration_prev, double distance_next, double cost_next, int duration_next,string nearby_attraction
		while(check_station(name) ){
	   		cout<<"Station Already Exist ! Try Again: ";	
			getline(cin, name);
		}
		cout<<"Place Station after which station: ";
		getline(cin, next_station_name);
		
		while(!check_station(next_station_name) || ( from_which_lane == 0 && next_station_name.compare(tail->name) == 0 )|| (from_which_lane == 1 &&next_station_name.compare(head->name) ==0 )){
	   		cout<<"Station Does NOT Exist or Station cannot be before or after titiwangsa and chan sow lin ! Try Again: ";	
			getline(cin, next_station_name);
		}
		
		while(current){
			if(current->name.compare(next_station_name) == 0){
				break;
			}
			
			if(from_which_lane == 0){
				current = current->next;
			} else {
				current = current->prev;
			}
			
		}
		
		cout<<"Enter Distance to Next station ("; 
		if(from_which_lane == 0){
			
			cout<<current->next->name;
		} else{
			cout<<current->prev->name;
		}
		
		cout<<") in KM: ";
		cin>> distance_next;
		cout<<"Enter Cost to Next station: RM ";
		cin>> cost_next;
		cout<<"Enter Duration to Next staton in minutes: ";
		cin>>duration_next;
		
		cout<<"Enter Distance to Previous station ("<<current->name<<") in KM: ";
		cin>> distance_prev;
		cout<<"Enter Cost to Previous station: RM ";
		cin>> cost_prev;
		cout<<"Enter Duration to Previous staton in minutes: ";
		cin>>duration_prev;
		
		cin.ignore();
		cout<<"Enter Nearby Attraction to Station: ";
		getline(cin, nearby_attraction);
		
		double d_temp, c_temp;
		int du_temp;
		
		if(from_which_lane == 1){
			d_temp = distance_prev;
			c_temp = cost_prev;
			du_temp = duration_prev;
			
			distance_prev = distance_next;
			cost_prev = cost_next;
			duration_prev = duration_next;
			
			distance_next = d_temp;
			cost_next = c_temp;
			duration_next = du_temp;
		}
		
		Subway *newnode = new Subway;
		newnode = createNewSubNode(id, name, distance_prev, cost_prev, duration_prev, distance_next, cost_next, duration_next, nearby_attraction);
		
		if(from_which_lane == 0){
			//ttwangsa
				newnode->prev = current;
				newnode->next = current->next;
				
				current->distance_next = distance_prev;
				current->duration_next = duration_prev;
				current->cost_next = cost_prev;
				
				current->next->distance_prev = distance_next;
				current->next->cost_prev = cost_next;
				current->next->duration_prev = duration_next;
				
				current->next->prev = newnode;
				current->next = newnode;
			
		} else{
				newnode->prev = current->prev;
				newnode->next = current;
				
				current->distance_prev = distance_next;
				current->duration_prev = duration_next;
				current->cost_prev = cost_next;
				
				current->prev->distance_next = distance_prev;
				current->prev->cost_next = cost_prev;
				current->prev->duration_next = duration_prev;
				
				current->prev->next = newnode;
				current->prev = newnode;
			

		}
		
		view_routes_cus();
		
		//asking whether user wants to continue add new subway or return to main function
		cout<<"Continue Adding New Subway? [0-YES, 1-NO]: ";
		while(true){
			//data validation
			if(cin>>continue_add_sub){
				break;
			} else {
				cin.clear();
				cin.ignore();
				cout<<endl<<"Invalid Data Types! Please Try Again [0 or 1] : ";
			}
		}
		cin.ignore();		
		if(continue_add_sub == 1){
			return;
		}	
	}
}

//function to display available subway station
void display_subs(){
	curr = head;
	
		cout<<endl<<"\t\t Stations "<<endl;
		cout<<"-----------------------------------------------------"<<endl;
	while(curr){
		cout<<curr->name<<endl;
		curr = curr->next;
	}
}

//function to search station details
void search_station(string search_station){
		
		curr = head;
		
		while(curr){
			if(curr->name.compare(search_station) == 0){
				break;
			}
			curr = curr->next;
		}
		
		//displaying sub if subway exist
		cout<<endl<<"\t\t Station:  "<<search_station<<endl
			<<"-------------------------------------------------"<<endl
			<<"Station ID: "<<curr->id<<endl<<endl;
			
			if(curr->prev){
				cout<<"Previous Station: "<<curr->prev->name<<endl
				<<"Distance to Previous Station: "<<curr->distance_prev<<" km"<<endl
				<<"Duration to Previous Station: "<<curr->duration_prev<<" minutes"<<endl
				<<fixed<<setprecision(2)<<"Cost to Previous Station: RM "<<curr->cost_prev<<endl;
			}
			
			
			if(curr->next){
				cout<<"Next Station: "<<curr->next->name<<endl
				<<"Distance to Next Station: "<<curr->distance_next<<" km"<<endl
				<<"Duration to Next Station: "<<curr->duration_next<<" minutes"<<endl
				<<"Cost to Next Station: RM"<<curr->cost_next<<endl<<endl;
			}			
			
}

//function to edit subway details
void edit_sub(){
	
	//displaying available subs
	display_subs();
	
	
	string stat_name;
			
	cin.ignore();
	while(true){
		cout<<"Please Enter Subway Station Name: ";
		getline(cin, stat_name);
			
		if(check_station(stat_name)){
			break;
		} else {
			cout<<"No such Station! Please Try Agian!"<<endl;
		}
	}
	
	//function to display station details
	search_station(stat_name);
	
	curr = head;
	
	while(curr){
		if(curr->name.compare(stat_name) == 0){
			break;
		}
		curr = curr->next;
	}
	
	cout<<endl<<"\t\t Edit Function"<<endl
		<<"------------------------------------------------------------"<<endl
		<<"1) Edit Station Name"<<endl;
		
		if(curr->next){
			cout<<"\t\t Next Station: "<<curr->next->name<<endl
				<<"2) Edit Duration to the next station"<<endl
				<<"3) Edit Cost to the next station"<<endl;
		}
		
		if(curr->prev){
			cout<<"\t\t Previous Station: "<<curr->prev->name<<endl
			<<"4) Edit Duration to the Prev Station"<<endl
			<<"5) Edit Cost to the Prev Station"<<endl
			<<"6) Back To Main Menu"<<endl;
		}
	
		
		if(stat_name.compare(head->name) == 0){
			cout<<endl<<"Enter an options [1-3 or 6]: ";
		} else if(stat_name.compare(tail->name) == 0){
			cout<<endl<<"Enter an options [1 or 4-6]: ";
		} else {
			cout<<endl<<"Enter an options [1-6]: ";
		}
	
	int opt;
	while(true){
		//data validation
		if(cin>>opt){
			
			if(stat_name.compare(head->name) == 0){
				if(opt == 1 || opt == 2 || opt == 3 || opt == 6){
					break;
				} else {
					cin.clear();
					cin.ignore();
					cout<<"Invalid Options! Try Again: ";
				}
			} else if(stat_name.compare(tail->name) == 0){
				if(opt == 1 || opt == 4 || opt == 5 || opt == 6){
					break;
				} else {
					cin.clear();
					cin.ignore();
					cout<<"Invalid Options! Try Again: ";
				}
			} else {
				
				if(opt == 1 || opt == 2 || opt == 3 || opt == 4|| opt == 5|| opt == 6){
					break;
				} else {
					cin.clear();
					cin.ignore();
					cout<<"Invalid Options! Try Again: ";
				}
			}
		} else {
			cin.clear();
			cin.ignore();
			cout<<endl<<"Invalid Data Types! Please Try Again [1-6] : ";
		}
	}
	
	
	
	switch(opt){
		case 1: {
			//to edit station name
			string new_name;
			
			while(true){
				cout<<"Enter a New Name for the current station: ";
				getline(cin, new_name);
					
				if(!check_station(new_name)){
					break;
				} else {
					cout<<"Station Name Already Exists! Please Try Again"<<endl;
				}
			}
			
			cout<<"Making Changes to Station Name . . . "<<endl;
			curr->name = new_name;
			cout<<"Station Name Have been Changed to: "<<curr->name<<endl<<endl;
			break;
		}
		case 2: {
			// to edit duration to the next station
			int duration_next;
			
			cout<<endl<<"Enter new travel time to the next station (MINUTES): ";
			while(true){
				if(cin>>duration_next){
					break;
				} else {
					cin.clear();
					cin.ignore();
					cout<<endl<<"Invalid Data Types! Please Try Again: ";
				}
			}
			
			curr = head;
			while(curr){
				if(curr->name.compare(stat_name)  == 0){
					break;
				}
				
				curr = curr->next;
			}
			
			//editing 
			curr->duration_next = duration_next;
			
			curr->next->duration_prev = duration_next;
			
			cout<<"Successfully Making Changes!"<<endl<<endl;
			
			search_station(stat_name);
			
			break;
		} 
		case 3: {
			// to edit the cost to the next station
			double cost;
			cout<<endl<<"Enter new Cost to Next Station: RM ";
			while(true){
				if(cin>>cost){
					break;
				} else {
					cin.clear();
					cin.ignore();
					cout<<endl<<"Invalid Data Types! Please Try Again: ";
				}
			}
			
			curr = head;
			while(curr){
				if(curr->name.compare(stat_name)  == 0){
					break;
				}
				
				curr = curr->next;
			}
			
			//editing 
			
			curr->cost_next = cost;
			
			curr->next->cost_prev = cost;
			
			cout<<"Successfully Making Changes!"<<endl<<endl;
			
			search_station(stat_name);
			break;
		}
		case 4: {
			//editing duration to the prev station
			int duration_prev;
			
			cout<<endl<<"Enter new travel time to the prev station (MINUTES): ";
			while(true){
				if(cin>>duration_prev){
					break;
				} else {
					cin.clear();
					cin.ignore();
					cout<<endl<<"Invalid Data Types! Please Try Again: ";
				}
			}
			
			curr = head;
			while(curr){
				if(curr->name.compare(stat_name) == 0){
					break;
				}
				
				curr = curr->next;
			}
			
			//editing 
			
			curr->duration_prev = duration_prev;
			
			curr->prev->duration_next = duration_prev;
			
			cout<<"Successfully Making Changes!"<<endl<<endl;
			
			search_station(stat_name);
			break;
		}
		case 5:{
			//editing cost to the prev station
			int cost;
			
			cout<<endl<<"Enter new COST: RM ";
			while(true){
				if(cin>>cost){
					break;
				} else {
					cin.clear();
					cin.ignore();
					cout<<endl<<"Invalid Data Types! Please Try Again: ";
				}
			}
			
			curr = head;
			while(curr){
				if(curr->name.compare(stat_name) == 0){
					break;
				}
				
				curr = curr->next;
			}
			
			//editing 
			
			curr->cost_prev= cost;
			
			curr->prev->cost_next = cost;
			
			cout<<"Successfully Making Changes!"<<endl<<endl;
			
			search_station(stat_name);
			break;
		}
		case 6:{
			return;
			break;
		}
		
	}
	
	
	//continue?
	int continue_edit;
	cout<<"Do you want to continue edit station? [1-yes, 0-no]: ";
	cin>>continue_edit;

	if(continue_edit == 1){
		edit_sub();
	} else {
		return;
	}
	
	
}


void view_all_trans(){
	
	
	if(trans_head == NULL){
		cout<<"No Transaction Yet."<<endl;
		return;
	}
	
	trans_curr = trans_head;
	
	while(trans_curr){
		cout<<endl<<"\t\t Ticket ID: "<<trans_curr->ticket_id<<endl
			<<"-----------------------------------------------------"<<endl
			<<"1) Departion Station: "<<trans_curr->depart_station<<endl
			<<"2) Destination Station: "<<trans_curr->destination_station<<endl
			<<"3) Departure TIME: "<<trans_curr->departure_time<<endl
			<<"4) Customer Name: "<<trans_curr->cus_name<<endl
			<<"5) Customer IC: "<<trans_curr->cus_ic<<endl
			<<fixed<<setprecision(2)<<"6) Price: RM "<<trans_curr->price<<endl
			<<"7) Quantity: "<<trans_curr->quantity<<endl
			<<"8) Distance: "<<trans_curr->distances<<" km"<<endl<<endl;
			
		trans_curr = trans_curr->next;
	}
	
}


void sort_transaction_name(){
	
	if(trans_head == NULL) {
		cout<<"No Transaction Available at the moment. "<<endl;
		return;
	}
	
	trans_curr = trans_head;
	Transaction *next = trans_curr->next;
	Transaction *end = new Transaction;
	end = NULL;
	
	Transaction *temp = new Transaction;
	
	while(trans_curr && end != trans_head->next){
		trans_curr = trans_head;
		next = trans_curr->next;
		while(trans_curr->next != end){
			
			//if strings is larger than next name swap
			if(trans_curr->cus_name.compare(next->cus_name) > 0){
				//string ticket_id, string depart, string destination, string depart_time, string cus_name, string cus_ic, double price, int quantity, double distances
				
				temp->ticket_id = trans_curr->ticket_id;
				temp->depart_station = trans_curr->depart_station;
				temp->cus_ic = trans_curr->cus_ic;
				temp->cus_name = trans_curr->cus_name;
				temp->departure_time = trans_curr->departure_time;
				temp->destination_station = trans_curr->destination_station;
				temp->distances = trans_curr->distances;
				temp->price = trans_curr->price;
				temp->quantity = trans_curr->quantity;
				
				trans_curr->cus_ic = next->cus_ic;
				trans_curr->cus_name = next->cus_name;
				trans_curr->departure_time = next->departure_time;
				trans_curr->depart_station = next->depart_station;
				trans_curr->destination_station = next->destination_station;
				trans_curr->distances = next->distances;
				trans_curr->price = next->price;
				trans_curr->quantity = next->quantity;
				trans_curr->ticket_id = next->ticket_id;
				
				next->cus_ic = temp->cus_ic;
				next->cus_name = temp->cus_name;
				next->departure_time = temp->departure_time;
				next->depart_station = temp->depart_station;
				next->destination_station = temp->destination_station;
				next->distances = temp->distances;
				next->price = temp->price;
				next->quantity = temp->quantity;
				next->ticket_id = temp->ticket_id;
				
			
			}
			trans_curr = next;
			next = next->next;
		}	
		
		
		end = trans_curr;
	}
	
	
	
}


// function to view all available tickets by specific customer
void search_ticket_by_name(string cus_name){
	trans_curr =trans_head;
	int i = 0;
	while(trans_curr){
		if(trans_curr->cus_name.compare(cus_name) == 0){
			cout<<endl<<"\t\t Ticket ID: "<<trans_curr->ticket_id<<endl
			<<"-----------------------------------------------------"<<endl
			<<"1) Departion Station: "<<trans_curr->depart_station<<endl
			<<"2) Destination Station: "<<trans_curr->destination_station<<endl
			<<"3) Departure TIME: "<<trans_curr->departure_time<<endl
			<<"4) Customer Name: "<<trans_curr->cus_name<<endl
			<<"5) Customer IC: "<<trans_curr->cus_ic<<endl
			<<fixed<<setprecision(2)<<"6) Price: RM "<<trans_curr->price<<endl
			<<"7) Quantity: "<<trans_curr->quantity<<endl
			<<"8) Distance: "<<trans_curr->distances<<" km"<<endl<<endl;
			i++;
		}
		trans_curr = trans_curr->next;
	}
	
	//if there is no record
	if(i==0){
		cout<<endl<<"No Record Found with the given Customer Name"<<endl;
	}

}

void search_by_tid(string tid){
	
	int i =0;
	
	
	while(i==0){
		
		trans_curr = trans_head;
		while(trans_curr){
			if(trans_curr->ticket_id.compare(tid) == 0){
				cout<<endl<<"\t\t Selected Ticket ID: "<<trans_curr->ticket_id<<endl
				<<"-----------------------------------------------------"<<endl
				<<"1) Departion Station: "<<trans_curr->depart_station<<endl
				<<"2) Destination Station: "<<trans_curr->destination_station<<endl
				<<"3) Departure TIME: "<<trans_curr->departure_time<<endl
				<<"4) Customer Name: "<<trans_curr->cus_name<<endl
				<<"5) Customer IC: "<<trans_curr->cus_ic<<endl
				<<fixed<<setprecision(2)<<"6) Price: RM "<<trans_curr->price<<endl
				<<"7) Quantity: "<<trans_curr->quantity<<endl
				<<"8) Distance: "<<trans_curr->distances<<" km"<<endl<<endl;
				i++;
				break;
			}
			trans_curr = trans_curr->next;
		}
		
		//if no record found
		if(i==0){
			cout<<endl<<"No Record Found with the given Ticket ID, Please Try Again: ";
			getline(cin,tid);
		}
		
	}
	
	
}

//function to delete ticket with tid
void delete_ticket(string tid){
	
	
	if(trans_head->ticket_id.compare(tid) == 0) {
		
		Transaction *temp = trans_head;
		//if the deleting ticket is head
		cout<<endl<<"Deleting Ticket ID: "<<tid<<" . . . "<<endl;
		
		if(trans_head->next){
			trans_head = trans_head->next;
		} else {
			trans_head = NULL;
			trans_tail = NULL; 
		}
		
		cout<<"Succesfully Deleted ticket id: "<< temp->ticket_id<<endl;
		
		delete(temp);
		
	} else {
		
		Transaction *trans_prev = new Transaction;
		trans_prev = trans_head;
		trans_curr = trans_head->next;
	
		
		
		
		while(trans_curr){
			
			if(trans_curr->ticket_id.compare(tid) == 0){
				break;
			}
			
			trans_prev = trans_curr;
			trans_curr =  trans_curr->next;
		}
		
		
		cout<<endl<<"Deleting Ticket ID: "<<trans_curr->ticket_id<<" . . . "<<endl;
		
		//if ticket is at tail
		if(trans_curr->next == NULL) {
			
			trans_prev->next = NULL;
			trans_tail = trans_prev;
			
			cout<<"Succesfully Deleted ticket id: "<< trans_curr->ticket_id<<endl;
			delete(trans_curr);
		} else {
			//if ticket is not head or tail
			trans_prev->next = trans_curr->next;
			
			cout<<"Successfully Deleted Ticket ID: "<<trans_curr->ticket_id<<endl;
			delete(trans_curr);
			
		}
		
	}
	
	
	
	
}

//function to edit ticket with tid
void edit_ticket(string tid){
	
	int i = 0;
	trans_curr = trans_head;
	while(trans_curr){
			if(trans_curr->ticket_id.compare(tid) == 0){
				i++;
				cout<<"found";
				break;
			}
			trans_curr = trans_curr->next;
	}
	
	
	int opt_5;
	
	//only can edit travel information of the ticket
	cout<<endl<<"\t\t EDIT OPTIONS"<<endl
		<<"--------------------------------------"<<endl
		<<"1) Departion Station"<<endl
		<<"2) Destination Station"<<endl
		<<"3) Price "<<endl
		<<"4) Quantity"<<endl
		<<"5) Distance"<<endl
		<<"6) Return to main menu"<<endl
		<<"Enter Option [1-7]: ";
		
	//data validation
	while(true){
		if(cin>>opt_5){
			if(opt_5 == 1 || opt_5 == 2 || opt_5 == 3 || opt_5 == 4  || opt_5 == 5  || opt_5 == 6){
				break;
			} else {
				cin.clear();
				cin.ignore();
				cout<<endl<<"No Such Option! Try Again: ";	
			}
		} else {
			cin.clear();
			cin.ignore();
			cout<<endl<<"Invalid Data Types! Please Try Again: ";
		}
	}	
	cin.ignore();
	
	switch(opt_5){
		case 1:{
			//edit departion station
			display_subs();
			
			string depart_stat;
			cout<<endl<<"Enter New Departion Station: ";
			getline(cin, depart_stat);
			
			//checking if station exist
			while(!check_station(depart_stat)){
		   		cout<<"Please Enter A Valid Station: ";	
		   		getline(cin, depart_stat);
			}
			
			//checking if it is the same station with destination station
			while(depart_stat.compare(trans_curr->destination_station) == 0){
				cout<<"Departure station and Destination Station cannot be same"<<endl;
				cout<<"Try Again: ";
				getline(cin, depart_stat);
			}
			
			trans_curr->depart_station = depart_stat;
			cout<<"Successfully Updated!"<<endl;
			search_by_tid(tid);
			break;
		}
		case 2: {
			// edit destination station
			
			display_subs();
			
			string dest;
			cout<<endl<<"Enter New Destination Station: ";
			getline(cin, dest);
			
			//checking if station exist
			while(!check_station(dest)){
		   		cout<<"Please Enter A Valid Station: ";	
		   		getline(cin, dest);
			}
			
			//checking if it is the same station with destination station
			while(dest.compare(trans_curr->depart_station) == 0){
				cout<<"Departure station and Destination Station cannot be same"<<endl;
				cout<<"Try Again: ";
				getline(cin, dest);
			}
			
			trans_curr->destination_station = dest;
			cout<<"Successfully Updated!"<<endl;
			search_by_tid(tid);
			break;
		}
		case 3: {
			// editing price
			int price;
			cout<<endl<<"Enter New Price: RM ";
			//data validation
			while(true){
				if(cin>>price){
					break;
				} else{
					cin.clear();
					cin.ignore();
					cout<<"Invalid Data Type! Please Try Again: ";
				}
			}
			
			trans_curr->price = price;
			cout<<"Successfully Updated!"<<endl;
			search_by_tid(tid);
			cin.ignore();
			break;
		}
		case 4: {
			// editing quantity or deleting ticket
			int qty;
			
			cout<<endl<<"Enter New Quantity [Enter 0 to delete ticket]: ";
			while(true){
				if(cin>>qty){
					break;
				} else{
					cin.clear();
					cin.ignore();
					cout<<"Invalid Data Type! Please Try Again: ";
				}
			}
			
			if(qty > 0){
				trans_curr->quantity = qty;
				
				//updating price as well
				trans_curr->price = trans_curr->price * qty;
				
				cout<<endl<<"Successfully Updated The quantity of the ticket and the price as well."<<endl;
				
				search_by_tid(tid);
				
			} else if (qty <= 0){
				int dlt_ticket;
				cout<<"Are you sure to delete current ticket "<<tid<<" ? [1-YES, 0-NO]: ";
				//data validation
				while(true){
					if(cin>>dlt_ticket){
						if(dlt_ticket == 1 || dlt_ticket == 0 ){
							break;
						} else {
							cin.clear();
							cin.ignore();
							cout<<endl<<"No Such Option! Try Again: ";	
						}
					} else {
						cin.clear();
						cin.ignore();
						cout<<endl<<"Invalid Data Types! Please Try Again: ";
					}
				}	
				cin.ignore();
				
				if(dlt_ticket == 1){
					delete_ticket(tid);
				} 
			} 
			
			
			break;
		}
		case 5: {
			//editing distance
			double distance;
			cout<<endl<<"Enter New Distance (km): ";
			//data validation
			while(true){
				if(cin>>distance){
					if(distance == 0){
						cin.clear();
						cin.ignore();
						cout<<"Distance cannot be less than 0! Please Try Again: ";
					} else {
						break;		
					}
				} else{
					cin.clear();
					cin.ignore();
					cout<<"Invalid Data Type! Please Try Again: ";
				}
			}
			
			trans_curr->distances = distance;
			cout<<"Successfully Updated!"<<endl;
			search_by_tid(tid);
			
			
			break;
		}
		case 6:{
			//return back to menu
			return;
			break;
		}
	}
	
							
}





int main(){
	
	int user_type = 0; // 0 for no user yet, 1 for normal cus , 2 for admin user, 3 for exiting program
	

	
	//subway linked lsit
	head = NULL;
	tail = NULL;
	// transaction linked list
	trans_head = NULL;
	trans_tail = NULL;
	
	//loading up the system with static data
	load_system();
	
	while(true){
		//identifying user type
		if(user_type ==0){
			user_type = identify_user_type();
		}
		
		
		if(user_type == 1){
			//normal customer
			display_mainmenu_cus();
			//getting answer from customer to let program knows what to do
			int ans = mainmenu_input_cus();
			
			switch(ans){
				case 1: {
					display_routes();
					break;
				}
				case 2: {
					//call function or do operation here for purchasing ticket, when adding new transaction, sort it
					//getting current time
					time_t now = time(0);
					
					tm *ltm = localtime(&now);
				
					
					cout<<endl<<endl<<"\t\t\t Available Routes"<<endl
						<<"-------------------------------------------------------"<<endl
						<<"Current Time: "
						<< ltm->tm_hour << ":"
				   		<<  ltm->tm_min << ":"
				   		<< ltm->tm_sec;
				   		view_routes_cus();cout<<endl;
				   	
				   	string depart, destination;
				   	cin.ignore();
				   	cout<<"Enter Departion Station: ";
				   	getline(cin, depart);
				   	
					while(!check_station(depart)){
				   		cout<<"Please Enter A Valid Station: ";	
				   		getline(cin, depart);
					}
					
					cout<<"Enter Destination Station: ";
					getline(cin, destination);
					
					while(!check_station(destination) || destination==depart){
						cout<<"Please Enter A valid station: ";
						getline(cin, destination);
					}
				   	purchase_func(depart, destination, ltm);
					
					break;
				}
				case 3: {
					//call function or do operation here for viewing transaction history, deleting history, searching history
					string cus_name;
								
					cout<<endl<<"\t\t Search Purchase History"<<endl
						<<"-------------------------------------------------"<<endl
						<<"Enter Customer Name: ";
						cin.ignore();
					getline(cin, cus_name);
								
					int i =0;
								
					while(i==0){
						trans_curr = trans_head;
						while(trans_curr){
							if(trans_curr->cus_name.compare(cus_name) == 0){
								i++;
								break;
							}
							trans_curr = trans_curr->next;
						}
									
					//if no record found
						if(i==0){
							cout<<endl<<"No Record Found with the given Customer name, Please Try Again [exit - to return to main menu]: ";
							getline(cin, cus_name);
						}	
						
						if(cus_name.compare("exit") == 0 ){
							break;
						}							
					}
					
					if(cus_name.compare("exit") != 0 ){
						search_ticket_by_name(cus_name);
					}				
					
					
					break;
				}
				case 4: {
					//deleting function
					cin.ignore();
					string tid;
					cout<<"Please enter Ticket ID to delete: ";
					getline(cin, tid);
								
					int j =0;
								
					while(j==0){
									
						trans_curr = trans_head;
						while(trans_curr){
							if(trans_curr->ticket_id.compare(tid) == 0){
								j++;
								break;
							}
							trans_curr = trans_curr->next;
						}
									
						//if no record found
						if(j==0){
							cout<<endl<<"No Record Found with the given Ticket ID, Please Try Again [exit to return to main menu]: ";
							getline(cin,tid);
						}
						
						if(tid.compare("exit") == 0 ){
							break;
						}							
					}
					
					if(tid.compare("exit") != 0) {
						int dlt_opt;
						cout<<"Are you sure to delete ticket? [1-YES, 0-NO] : ";
						
						while(true){
							if(cin>>dlt_opt){
								if(dlt_opt == 1 || dlt_opt ==0){
									break;
								} else {
									cin.clear();
									cin.ignore();
									cout<<"No such options! Try again: ";
								}
							} else {
								cout<<"Invalid Data Type: Try Again: ";
							}
						}
						
						delete_ticket(tid);	
					}
					
					
					break;
				}
				case 5: {
					//logging out program
					user_type = 0;
					
					continue;
					break;
				}
			}
		} else if(user_type == 2) {
			//verify admin user
			
			if(login_status){
				display_mainmenu_admin();
			}
			else if (login_verification()){
				display_mainmenu_admin();
			}	else{
				cout<<"Sorry the maximum trial of account verification has reached! Closing Program . . . "<<endl;
				exit(0);
			}
			
			//getting answer from admin to let program knows what to do
			int ans = mainmenu_input_admin();
			
			switch(ans){
				case 1: {
					// ko ee
					//display all routes basic details
					view_routes_cus();
					// options for admin to choose what to do. [view subway details, view routes details, back to main menu]
					routes_options_admin();
					
					break;
				}
				case 2: {
					//calling function to add new subway
					add_sub();
					break;
				}
				case 3: {
					//call function to edit subway information (fare, travel time, station name
					edit_sub();
					break;
				}
				case 4: {
					
					//Transactions Details
					int trans_opt;
					bool continue_t = true;
					while(continue_t){
						cout<<endl<<"\t\t Transactions"<<endl
						<<"-------------------------------------------------------"<<endl
						<<"1) View All Purchase History "<<endl
						<<"2) Sorting Transaction Based on passenger name (ascending)"<<endl
						<<"3) Search For Specific Purchase History"<<endl//delte and edit function will be inside
						<<"4) Back To Main Menu"<<endl<<endl
						<<"Enter An Option: ";
					
						while(true){
							if(cin>>trans_opt){
								if(trans_opt == 1 || trans_opt == 2 || trans_opt == 3 || trans_opt == 4){
									break;
								} else {
									cin.clear();
									cin.ignore();
									cout<<endl<<"No Such Option! Try Again: ";	
								}
							} else {
								cin.clear();
								cin.ignore();
								cout<<endl<<"Invalid Data Types! Please Try Again: ";
							}
						}
						
						switch(trans_opt){
							
							case 1: {
								view_all_trans();
								break;
							}
							case 2: {
								// sort transaction based on customer name
								sort_transaction_name();
								view_all_trans();
								break;
							}
							case 3: {
								// search specific Purchased Ticket, delete, edit
								
								// function to search ticket
								
								string cus_name;
								
								cout<<endl<<"\t\t Search Purchase History"<<endl
									<<"-------------------------------------------------"<<endl
									<<"Enter Customer Name: ";
								cin.ignore();
								getline(cin, cus_name);
								search_ticket_by_name(cus_name);
								
								break;
							}
							case 4: {
								continue_t = false;
								break;
							}
							
						}
					}
					
					break;
				}
				case 5: {
					//edit or delete specific ticket
					
					bool continue_5 = true;
					
					int opt_5;
					
					while(continue_5){
						cout<<endl<<"\t\t Search Ticket"<<endl
							<<"------------------------------------------------"<<endl
							<<"1) Search By Ticket ID"<<endl
							<<"2) Search By Customer Name"<<endl
							<<"3) Back to main Menu"<<endl
							<<"Please Enter Your Choice [1-3]: ";
						
						while(true){
							if(cin>>opt_5){
								if(opt_5 == 1 || opt_5 == 2 || opt_5 == 3 ){
									break;
								} else {
									cin.clear();
									cin.ignore();
									cout<<endl<<"No Such Option! Try Again: ";	
								}
							} else {
								cin.clear();
								cin.ignore();
								cout<<endl<<"Invalid Data Types! Please Try Again: ";
							}
						}
						
						switch(opt_5){
							
							case 1:{
								// search the specific ticket by id and ask if editing information is required
								string tid;
								cin.ignore();
								cout<<endl<<"Enter Ticket ID to search: ";
								getline(cin, tid);
								
								int i =0;
								
								while(i==0){
									
									trans_curr = trans_head;
									while(trans_curr){
										if(trans_curr->ticket_id.compare(tid) == 0){
											i++;
											break;
										}
										trans_curr = trans_curr->next;
									}
									
									//if no record found
									if(i==0){
										cout<<endl<<"No Record Found with the given Ticket ID, Please Try Again: ";
										getline(cin,tid);
									}							
								}
								
								search_by_tid(tid);
								
								int ans_edit;
								cout<<endl<<"Edit Current Ticket Infomation? [1-YES, 0-NO]: ";
								while(true){
									if(cin>>ans_edit){
										if(ans_edit == 1 || ans_edit == 0){
											break;
										} else {
											cin.clear();
											cin.ignore();
											cout<<endl<<"No Such Option! Try Again: ";	
										}
									} else {
										cin.clear();
										cin.ignore();
										cout<<endl<<"Invalid Data Types! Please Try Again: ";
									}
								}
								
								cin.ignore();
								
								if(ans_edit = 1){
									//call edit function
									edit_ticket(tid);
								}
								
								
								break;
							}
							case 2:{
								//list a list of tickets under certain customer and ask admin to selct an ticket to edit
								
								
								// function to search ticket
								
								string cus_name;
								
								cout<<endl<<"\t\t Search Purchase History"<<endl
									<<"-------------------------------------------------"<<endl
									<<"Enter Customer Name: ";
								cin.ignore();
								getline(cin, cus_name);
								
								int i =0;
								
								while(i==0){
									
									trans_curr = trans_head;
									while(trans_curr){
										if(trans_curr->cus_name.compare(cus_name) == 0){
											i++;
											break;
										}
										trans_curr = trans_curr->next;
									}
									
									//if no record found
									if(i==0){
										cout<<endl<<"No Record Found with the given Customer name, Please Try Again: ";
										getline(cin, cus_name);
									}							
								}

								
								
								
								search_ticket_by_name(cus_name);
								
								
								
								//edit ticket
								string tid;
								cout<<endl<<"Enter Ticket ID to search: ";
								getline(cin, tid);
								
								int k =0;
								
								while(k==0){
									
									trans_curr = trans_head;
									while(trans_curr){
										if(trans_curr->ticket_id.compare(tid) == 0){
											k++;
											break;
										}
										trans_curr = trans_curr->next;
									}
									
									//if no record found
									if(k==0){
										cout<<endl<<"No Record Found with the given Ticket ID, Please Try Again: ";
										getline(cin,tid);
									}							
								}
								
								search_by_tid(tid);
								
								int ans_edit;
								cout<<endl<<"Edit Current Ticket Infomation? [1-YES, 0-NO]: ";
								while(true){
									if(cin>>ans_edit){
										if(ans_edit == 1 || ans_edit == 0){
											break;
										} else {
											cin.clear();
											cin.ignore();
											cout<<endl<<"No Such Option! Try Again: ";	
										}
									} else {
										cin.clear();
										cin.ignore();
										cout<<endl<<"Invalid Data Types! Please Try Again: ";
									}
								}
								
								cin.ignore();
								
								if(ans_edit = 1){
									//call edit function
									edit_ticket(tid);
								}		
								
								break;
							}
							case 3:{
								//back to main menu
								continue_5 = false;
								break;
							}
						}
						
					}
					
					
					break;
				}
				case 6: {
					//logging out user
					user_type = 0;
					login_status = false;
					continue;
					break;
				}
			}
		} else if(user_type == 3) {
			//exit program
			return 0;
		} 
	}
	
	return 0;
	
	
}
