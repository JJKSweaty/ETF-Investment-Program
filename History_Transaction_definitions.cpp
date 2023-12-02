#include <cassert>
#include <iomanip>
#include <iostream>
#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//
    // Constructor
    Transaction::Transaction( std::string ticker_symbol,  unsigned int day_date,  
        unsigned int month_date,  unsigned year_date, 
        bool buy_sell_trans,  unsigned int number_shares,  
        double trans_amount )
        {
          //trans id
          if (trans_id!=assigned_trans_id){
          trans_id=assigned_trans_id;
          assigned_trans_id++;
          }
          //day
          if (day_date>0&&day_date<=31)
          {
            day=day_date;
          }
          //month
           if (month_date>0&&month_date<=12)
          {
            month=month_date;
          }
          //year
          if (year>=0){
          year=year_date;
          }
          //symbol
          symbol=ticker_symbol; 
          //If buy is false
          if (buy_sell_trans==false)
          {
            trans_type="Sell";
          }
          //if buy is true
          else if (buy_sell_trans==true)
          {
            trans_type="Buy";
          }
          if (number_shares>=0)
          {
            shares=number_shares;
          }
          if (trans_amount>=0){
            amount=trans_amount;
          }
         
          p_next=nullptr;
        
          Transaction *p_next=p_next;
          acb=0;
          acb_per_share=0;
          share_balance=0;
          cgl=0;


          
        }

    // Destructor
    //
    Transaction::~Transaction()
    {
      p_next=nullptr;
    }


    // Overloaded < operator.
    //
      bool Transaction::operator<( Transaction const &other )
      {
        // Compare transaction dates
        //if a is smaller
          if (year < other.year) {
              return true;
              //if a is bigger
          } else if (year > other.year) {
              return false;
              // If years are the same, compare months
          } else {
              //if a is smaller
              if (month < other.month) {
                  return true;
                  //if a is bigger
              } else if (month > other.month) {
                  return false;
              } else {
                  // If months are the same, compare days
                  if (day < other.day) {
                      return true;
                      //if a is bigger
                  } else if (day > other.day) {
                      return false;
                  } else {
                      // If dates are the same, compare transaction ids
                      if (trans_id < other.trans_id)
                      {
                        return true;
                      }
                      else 
                      {
                        return false;
                      }

                  }
              }
          }
      }
// GIVEN
// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
  }
////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//



	// Constructor
	//
	History::History()
  {   
    p_head=nullptr;
  }

	// Destructor.
	//
	History::~History()
  {
    while (p_head != nullptr)
    {
        Transaction *temp = p_head;
        p_head = p_head->get_next();
        delete temp;
        temp=nullptr;
    }
  }

	// Insert transaction into linked list.
	//
	void History::insert(Transaction *p_new_trans)
  {
      
      //if Head is null 
      if (p_head==nullptr)
      {
        //set value to new trans
        p_head=p_new_trans;
      }
      else 
      {
        //Current Node
        Transaction *currentNode=p_head;
        //current node is now next head
        while (currentNode->get_next()!=nullptr)
        {
          currentNode=currentNode->get_next();
        }
        currentNode->set_next(p_new_trans);
      }

  }

	// Read the transaction history from file.
	//
	void History::read_history()
  {
    //open file
    ece150::open_file();
    std::string symbol;
    unsigned int day;
    unsigned int month;
    unsigned int year;
    unsigned int shares;
    double trans_amount;
    bool trans_type;

    //while there is another entry
    while (ece150::next_trans_entry())
    {
      symbol=ece150::get_trans_symbol();
      day=ece150::get_trans_day();
      month=ece150::get_trans_month();
      year=ece150::get_trans_year();
      trans_type=ece150::get_trans_type();
      shares=ece150::get_trans_shares();
      trans_amount=ece150::get_trans_amount();
      Transaction *p_trans=new Transaction(symbol,day,month,year,trans_type,shares,trans_amount);
      History::insert(p_trans);
    }
    ece150::close_file();
  }

	// Print the transaction history.
	//
	void History::print()
  {
    p_head=get_p_head();
    Transaction*p_next=p_head;
    int counter =0;
    std::cout<<"========== BEGIN TRANSACTION HISTORY ============\n";
    while (p_next!=nullptr)
    {
      p_next->print();
      /*if (p_next->get_trans_type())
      std::cout<<counter;
      std::cout<<"\t"<<p_next->   get_symbol();
      std::cout<<"\t"<<p_next->get_day();
      std::cout<<"\t"<<p_next->get_month();
      std::cout<<"\t"<<p_next->get_year();
      std::cout<<"\t"<<p_next->get_trans_type();
      std::cout<<"\t"<<p_next->get_shares();
      std::cout<<"\t";
      std::cout<<p_next->get_amount()<<std::endl;
      counter++;*/
      p_next=p_next->get_next();
    }
    std::cout<<"\n========== END TRANSACTION HISTORY ============";
  }
	// Sort using insertion sort.
	//
	void History::sort_by_date()
  {
   Transaction* p_sorted_head = nullptr;
  Transaction* p_current = get_p_head();

    while (p_current != nullptr)
    {
        // Remove the first transaction from the unsorted list
        Transaction* p_temp = p_current;
        p_current = p_current->get_next();
        p_temp->set_next(nullptr);

        // Insert into the sorted list in the correct position
        if (p_sorted_head == nullptr || (*p_temp < *p_sorted_head))
        {
            // Insert at the beginning
            p_temp->set_next(p_sorted_head);
            p_sorted_head = p_temp;
        }
        else
        {
            // Traverse the sorted list to find the correct position
            Transaction* p_sorted_current = p_sorted_head;
            while (p_sorted_current->get_next() != nullptr && !(*p_temp < *(p_sorted_current->get_next())))
            {
                p_sorted_current = p_sorted_current->get_next();
            }

            // Insert after p_sorted_current
            p_temp->set_next(p_sorted_current->get_next());
            p_sorted_current->set_next(p_temp);
        }
    }

    // Update the head of the history with the sorted list
    p_head = p_sorted_head;
}
	// Update the acb members in the linked list.
	//
	void History::update_acb_cgl()
  {
    p_head=get_p_head();
    double acb=0,acbshare=0,cgl=0,amount=0,cgldiff=0;
    unsigned int sharebal=0;
    Transaction *tempnode=p_head;
    while (tempnode!=nullptr)
    {
      //If it is a buy trans
      if (tempnode->get_trans_type()==1)
      {
      //sum of acb
      acb=acb+tempnode->get_amount();
      //sum of all shares
      sharebal=sharebal+tempnode->get_shares();
      //calculate acb/share
      acbshare=acb/sharebal;
      tempnode->set_acb(acb);
      tempnode->set_share_balance(sharebal);
      tempnode->set_acb_per_share(acbshare);
      tempnode=tempnode->get_next();
      }
      
      //sell
      else
      {
      acbshare=acb/sharebal;
      cgldiff=(tempnode->get_shares()*acbshare);
      acb=acb-cgldiff;
      cgl=tempnode->get_amount()-cgldiff;
      tempnode->set_cgl(cgl);
      sharebal=sharebal-tempnode->get_shares();
      //sum of all shares
      //calculate acb/share
      acbshare=acb/sharebal;
      //setting values 
      tempnode->set_acb(acb);
      tempnode->set_share_balance(sharebal);
      tempnode->set_acb_per_share(acbshare);
      tempnode=tempnode->get_next();
      }
    }
  }

	// Compute the ACB, and CGL.
	//
	double History::compute_cgl(unsigned int year)
  {

    p_head=get_p_head();
    Transaction*p_next=p_head;
    double acb=0,acbshare=0,cgl=0,amount=0,cglsum=0;
    unsigned int sharebal=0, transyear=0;
    //compute cgl
    //if its a sell
    
    while(p_next!=nullptr)
    {
      transyear=p_next->get_year();
      if (transyear==year)
      {
      //if sell
      if (!(p_next->get_trans_type())){
      cgl=p_next->get_cgl();
      p_next->set_cgl(cgl);
      //cgl sum for that year
      cglsum+=cgl;
      }
      }
      else
      {
       if (!(p_next->get_trans_type())){
      cgl=p_next->get_cgl();
      p_next->set_cgl(cgl);
      }
      }
      p_next=p_next->get_next();

    }
     return cglsum;
  }
  
// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }


  