// Online C++ compiler to run C++ program online
#include <iostream>

//begin, end
template<typename T>
class Node{
    private:
        T data;
        
    public:
        Node* next;
        
        Node(T d): data(d){
            next=nullptr;
        }
        void print() const{
            std::cout << data;
        }
        const T& get_data() const{
            return data;
        }
        T& get_data() {
            return data;
        }
};

template<typename T>
class LinkedListIterator{
    private:
        Node<T>* current;
    public:
        LinkedListIterator(Node<T>* cur): current(cur){
            
        }
        T& operator*() const{
            return current->get_data();
        }
        void operator++ (){
            if(current!=nullptr){
                current=current->next;
            }
        }
        bool operator==(const LinkedListIterator<T>& rhs) const {
            return current == rhs.current;
        }
        
        bool operator!=(const LinkedListIterator<T>& rhs) const {
            return current != rhs.current;
        }
};

template<typename T>
class LinkedList{
    private:
        Node<T>* head;
        Node<T>* current;
        size_t m_size;
    public:
        LinkedList(){
            head=nullptr;
            current=head;
        }
        void append(T data){
            if (head == nullptr) {
                head = new Node<T>(data);
                current = head;
            } else {
                current->next = new Node<T>(data);
                current = current->next;
            }
            m_size++;
        }
        size_t size() const{
            return m_size;
        }
        LinkedListIterator<T> begin(){
            LinkedListIterator<T> it = LinkedListIterator<T>(head);
            return it;
        }
        LinkedListIterator<T> end(){
            LinkedListIterator<T> it = LinkedListIterator<T>(nullptr);
            return it;
        }
};

int main() {
    
    LinkedList<int> list;
    list.append(10);
    list.append(30);
    list.append(20);
    
    for(auto it = list.begin();it!=list.end();++it){
        std::cout << *it << ",";
    }

    return 0;
}