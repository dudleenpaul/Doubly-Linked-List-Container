#ifndef DL_LIST_H
#define DL_LIST_H
#include <iostream>
#include <initializer_list>

namespace cop4530 {

    template <typename T>
    class List {
        private:
          // nested Node class
          struct Node {
             T data;
             Node *prev;
             Node *next;
    
             Node(const T & d = T{}, Node *p = nullptr, Node *n = nullptr) 
                : data{d}, prev{p}, next{n} {}
             Node(T && d, Node *p = nullptr, Node *n = nullptr)
                : data{std::move(d)}, prev{p}, next{n} {}
          };
    
       public:
          //nested const_iterator class
          class const_iterator {
             public:
                const_iterator() : current{nullptr}
                {}            // default zero parameter constructor
    
                const T & operator*() const {return retrieve();} // operator*() to return element
     
                // increment/decrement operators
                const_iterator & operator++()
                {
                    current = current->next;
                    return *this;
                }
    
                const_iterator operator++(int)
                {
                    const_iterator old = *this;
                    ++ (*this);
                    return old;
                }
    
const_iterator & operator--()
                {
                    //pre dec operator
                    this->current = this->current->prev; //moving iterator to previous node, change in style
                    return *this; //return a iterator ref
                    //current = current->prev; //to move node backward 
                    //return *this
                    // TO BE FILLED
                }
    
const_iterator operator--(int)
                {
                    //post dec operator
                   //this->const_interator old = *this; //copying iterator before decrement
                   //-- (*this); //moving over to previous node
                   //return old; ////return the version that was copied before it was decremented
                   const_iterator old = *this; //copying the iterator
                    -- (*this); //moving over to previous node (by decrementin it)
                    return old; //return the version that was copied before it was decremented
                    // TO BE FILLED
                }
    
                // comparison operators
                bool operator==(const const_iterator &rhs) const
                {
                    return current == rhs.current;
                }
       
                bool operator!=(const const_iterator &rhs) const
                {
                    return !(*this == rhs);
                }
    
             protected:
                Node *current;              // pointer to node in List
    
                // retrieve the element refers to
                T & retrieve() const
                {
                    return current->data;
                }
    
                const_iterator(Node *p) : current{p}    // protected constructor
                {
                }
    
                friend class List<T>;
          };
    
          // nested iterator class
          class iterator : public const_iterator {
             public:
                iterator()
                {}
    
                T & operator*()
                {
                    return const_iterator::retrieve();
                }
    
                const T & operator*() const
                {
                    return const_iterator::operator*();
                }
    
                // increment/decrement operators
                iterator & operator++()
                {
                    this->current = this->current->next;
                    return *this;
                }
    
                iterator operator++(int)
                {
                    iterator old = *this;
                    ++(*this);
                    return old;
                }
    
iterator & operator--()
                {
                    this->current = this->current->prev;
                    return *this; //making it similar to professors
                    // TO BE FILLED
                }
    
iterator operator--(int)
                {
                    iterator old = *this;
                    -- (*this);
                    return old; //making it similar to professors just decrementing though
                    // TO BE FILLED
                }
    
             protected:
                iterator(Node *p): const_iterator{p}
                {}
    
                friend class List<T>;
          };
    
       public:
          // constructor, desctructor, copy constructor
    
          // default zero parameter constructor
          List()
          {
              init();
          }
    
          List(const List &rhs)
          {
              init();
              for(auto & x: rhs)
                  push_back(x);
          }
    
          // move constructor
          List(List && rhs) : theSize{rhs.theSize}, head{rhs.head}, tail{rhs.tail}
          {
              rhs.theSize = 0;
              rhs.head = nullptr;
              rhs.tail = nullptr;
          }
    
// num elements with value of val
explicit List(int num, const T& val = T{})
          {
            init(); //initializes the list without this i got segmentation error once it hit testing other constructors
            /**for (count = 0; count < num; ++count) //loop for a number of times to make sure theres num elements
            {
                Node* newNumber = new num(val, tail->prev, tail); //prev node is the current tail's previous node and then the next node is going to be the current tail
                for (int count = 0; count < num; ++count) //loop for a number of times to make sure theres num elements, new update: think this is causing a segmentation
                tail->prev->next = newElem; //next pointer 
                tail->prev = newElem; //tail is now pointing to the newest node
                 ++theSize; increment the list's size once a new element is taken in
            }**/
            
            int count = 0;
            for (count = 0; count < num; count++) //loop to insert as many nums into the list
            push_back(val); //anything with val goes to the back of the list
              // TO BE FILLED
          }
    
// constructs with elements [start, end)
List(const_iterator start, const_iterator end)
          {
            //elements from the start of the list to the end
            init(); //putting this just to attempt to avoid segmentation fault, initializes list
            for (const_iterator itr = start; itr != end; ++itr) //loop as long as itr doesnt reach the last iterator
            {
                push_back(*itr); //while iterating through pushes new elems to the back
            }
             // TO BE FILLED
          } 
    
// constructs with a copy of each of the elements in the initalizer_list
List (std::initializer_list<T> iList)
//come back to this, might be issue for seg
          {
            init(); //putting this just to attempt to avoid segmentation fault
            for (auto itr = iList.begin(); itr != iList.end(); ++itr) //loop to initialize and iterates through the list
            {
                const auto& element = *itr; //value ref that can't be changed by theloop
                push_back(element); //elements to the back once more
            } 
            // TO BE FILLED
          }
    
          // destructor
          ~List()
          {
              clear();
              delete head;
              delete tail;
          }
    
          // copy assignment operator
          const List& operator=(const List &rhs)
          {
              if(this == &rhs)
                  return *this;
    
              List copy = rhs;
              std::swap(*this, copy);
              // The following should work too.
              //List(rhs.begin(), rhs.end());
              return *this;
          }
    
          // move assignment operator
          List & operator=(List && rhs)
          {
              std::swap(theSize, rhs.theSize);
              std::swap(head, rhs.head);
              std::swap(tail, rhs.tail);
              return *this;
          }
    
// sets list to the elements of the initializer_list
List& operator= (std::initializer_list<T> iList)
         {
            //overloads operator to assign elements to current list
            clear(); //ridding of the current existing elements
            //for (auto itr = iList.begin(); itr != iList.end(); ++itr) //causing issue
            for (const auto& element : iList) //loops through iList and pushes elements to the back
            {
                push_back(element);
            }
            return *this; //returns the newly made list
              // TO BE FILLED
          }
    
    
          // member functions
    
          int size() const {return theSize;}       // number of elements
          bool empty() const {return theSize == 0;}     // check if list is empty
    
          // delete all elements
          void clear() 
          {
              while(! empty())
                  pop_front();
          }
    
          // BONUS POINTS
// reverse the order of the elements
void reverse()
          {
            Node* reverseList = nullptr; //pointer for the new head of the reverse list
            Node* current = head;
            Node* next = nullptr; //pointer to the next node

            while (current != nullptr) //loops until it reaches the end of the list
            {
                next = current->next; //storing next node in normal list
                current->next = reverseList; //point to the previous node of the reversed list instead
                //current->prev = reverseList;
                current->prev = next; //point to the next node of the normal list
                reverseList = current; //reverse pointer being moved to current
                current = next;
            }
            tail = head; //tail is the last node of the original list (now)
            head = reverseList; //head is the newest head!
            //Node* tail = tail->prev; 
            
             //while (tail != head) //print in reverse with tail as pointer
            //{
              //  std::cout << tail->data
                //tail = tail->prev;
            //}
              //  std::cout << tail->data << std::endl;



            /**if (theSize <= 1)
            return;
            std::swap(head, tail);
             **/
            
 
              // TO BE FILLED
          }
    
          T& front(){return *begin();}             // reference to the first element
          const T& front() const{return *begin();}
          T& back(){return *(--end());}              // reference to the last element
          const T& back() const{return *(--end());} 
    
          void push_front(const T & val){insert(begin(), val);} // insert to the beginning
          void push_front(T && val){insert(begin(), std::move(val));}      // move version of insert
          void push_back(const T & val){insert(end(), val);}  // insert to the end
          void push_back(T && val){insert(end(), std::move(val));}       // move version of insert
          void pop_front(){erase(begin());}               // delete first element
          void pop_back(){erase(--end());}                // delete last element
    
// remove all elements with value = val
void remove(const T &val)
          {
            for (auto itr = begin(); itr != end();) //iterating through list
            {
                if (*itr == val) //checking if the current element equals val
                {
                    itr = erase(itr); //erased if it does!
                }
                else 
                {
                    ++itr; //if not, element/iterator increments
                }
            }
              // TO BE FILLED
          }
    
// remove all elements for which Predicate pred
//  returns true. pred can take in a function object
template <typename PREDICATE>
void remove_if(PREDICATE pred)
          {
            for (auto itr = begin(); itr != end();) //iterating with iterators once more
            {
                if (pred(*itr)) //checking if predicate returns true for element
                {
                    itr = erase(itr); //if true, it is removed from the list
                }
                else
                {
                    ++itr; //if false increment to the next element
                }
            }
              // TO BE FILLED
          }
    
// print out all elements. ofc is deliminitor
void print(std::ostream& os, char ofc = ' ') const
          {
            auto itr = begin(); //iterating through the list
            while (itr != end())
            {
              os << *itr; //prints the most current elemet
              ++itr; //and onto the next element
              if (itr != end()) //checks if the current element is not the last one
              os << ofc; //prints if its not the last one
            }
            /**Node* current = head->next; //from first elem to the tail
            while (current != tail)
            {
                os << current->data << ofc; //status of current node is printed

                if (current != tail) //if its not the tail then print
                os << ofc;
            }
              **/ // TO BE FILLED
          }
    
          iterator begin(){return iterator(head->next);}               // iterator to first element
          const_iterator begin() const{return const_iterator(head->next);}
          iterator end(){return iterator(tail);}                 // end marker iterator
          const_iterator end() const{return const_iterator(tail);} 
    
          // insert val ahead of itr
          iterator insert(iterator itr, const T& val)
          {
              Node *p = itr.current;
              theSize ++;
              return iterator{p->prev = p->prev->next = new Node(val, p->prev, p)};
          }
    
          // move version of insert
          iterator insert(iterator itr, T && val)
          {
              Node *p = itr.current;
              theSize ++;
              return iterator{p->prev = p->prev->next = new Node(std::move(val), p->prev, p)};
          }
    
          // erase one element
          iterator erase(iterator itr)
          {
              Node *p = itr.current;
              iterator I = ++itr;
              p->prev->next = p->next;
              p->next->prev = p->prev;
              theSize --;
              delete p;
              return I;
          }
    
          // erase [start, end)
          iterator erase(iterator start, iterator end)
          {
              iterator I = start;
              while(I != end)
                  I = erase(I);
              return I;
          }
    
    
       private:
          int theSize;           // number of elements
          Node *head;            // head node
          Node *tail;            // tail node
      
          // initialization
          void init(){
              theSize = 0;
              head = new Node;
              tail = new Node;
              head->next = tail;
              tail->prev = head;
          }
    };
    
    // overloading comparison operators
    template <typename T>
    bool operator==(const List<T> & lhs, const List<T> &rhs)
    {
        if(lhs.size() != rhs.size())
            return false;
        typename List<T>::const_iterator Ir = rhs.begin();
        typename List<T>::const_iterator Il = lhs.begin();
        for(; Il != lhs.end(); Il ++)
        {
            if(*Il != *Ir)
                return false;
            Ir ++;
        }
        return true;
    }
   
    template <typename T>
    bool operator!=(const List<T> & lhs, const List<T> &rhs)
    {
        return ! (lhs == rhs);
    }
    
// overloading output operator
template <typename T>
    std::ostream & operator<<(std::ostream &os, const List<T> &l)
    {
        l.print(os); //calling print function for the list
        return os; //return the output 
        // TO BE FILLED
    }


} // end of namespace 4530

#endif

