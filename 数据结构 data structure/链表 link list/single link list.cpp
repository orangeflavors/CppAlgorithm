#include<iostream>

using namespace std;

struct Node {
    int data;       // 节点存储的数据
    Node* next;     // 指向下一个节点的指针

    Node(int val):data(val), next(NULL){} // 构造函数
};

class SingleLinkedList {
    private:
        Node* head;     // 链表的头指针
        Node* tail;     // 链表的尾指针
    
    public:
        // 构造函数
        SingleLinkedList():head(NULL), tail(NULL){}
    
        // 析构函数
        ~SingleLinkedList() {
            Node* cur = head;
            while (cur != NULL) {
                Node* next = cur->next;
                delete cur;
                cur = next;
            }
        }
    
        // 头插
        void push_front(int val) {
            Node* newNode = new Node(val);
            if (head == NULL) {
                head = tail = newNode; // 链表为空时，头尾都指向新节点
            }
            else {
                newNode->next = head;
                head = newNode;
            }
        }
    
        // 尾插
        void push_back(int val) {
            Node* newNode = new Node(val);
            if (tail == NULL) {
                head = tail = newNode; // 链表为空时，头尾都指向新节点
            }
            else {
                tail->next = newNode;
                tail = newNode;
            }
        }
        


        // 按值删除节点
        void deleteNode(int val){
            Node* cur = head;//指向当前节点
            Node* pre = NULL;//指向上一个节点
    
            while(cur != NULL){
                if(cur->data == val){
                    if(pre == NULL){//如果删除头节点
                        head = cur->next;
                        if (head == NULL){//如果删除后链表为空
                            tail = NULL;
                        }
                    }
                    else{
                        pre->next = cur->next;//链表跨过要删除的节点
                        if (cur == tail){//如果删除尾节点
                            tail = pre;
                        }
                    }
                    delete cur;
                    return;
                }

                pre = cur;
                cur = cur->next;
            }
        }

        //升序排序
        void sort(){
            if(head == NULL || head->next == NULL) return;
            head = MergeSort(head);

            Node* cur = head;
            while (cur->next != NULL){
                cur = cur->next;
            }
            tail = cur;
        }

        //归并排序
        //分解
        void Split(Node* &head, Node* &mid){
            if(head == NULL || head->next == NULL){//链表无节点或只有一个节点时
                mid = NULL;
                return;
            }

            //快慢指针
            Node* fast = head;
            Node* slow = head;
            Node* pre = NULL;
            while(fast != NULL && fast->next != NULL){
                pre = slow;
                slow = slow->next;
                fast = fast->next->next;
            }
            mid = slow;
            pre->next = NULL;
        }

        //合并
        Node* Merge(Node* head, Node* mid){
            if(head == NULL) return mid;
            if(mid == NULL) return head;

            Node* dummy = new Node(0);//哨兵节点
            Node* cur = dummy;//cur指针

            while(head != NULL && mid != NULL){
                if(head->data <= mid->data){//升序
                    cur->next = head;
                    head = head->next;
                }
                else{
                    cur->next = mid;
                    mid = mid->next;
                }
                cur = cur->next;
            }

            if(head != NULL){
                cur->next = head;
            }
            else{
                cur->next = mid;
            }
            head = dummy->next;
            delete dummy;
            return head;
        }

        //归并
        Node* MergeSort(Node* head){
            if(head == NULL || head->next == NULL) return head;
            Node* mid;
            Split(head, mid);
            head = MergeSort(head);
            mid = MergeSort(mid);
            return Merge(head, mid);
        }
        
        //计算链表长度
        int size(){
            if(head == NULL) return 0;
            Node* cur = head;
            int cnt = 1;
            while(cur->next != NULL){
                cnt++;
                cur = cur->next;
            }
            return cnt;
        }

        //查找it位置的值
        int search(int it){
            Node* cur = head;
            int cnt = 0;
            while(cur != NULL){
                if(cnt == it){
                    return (cur->data);
                }
                cur = cur->next;
                cnt++;
            }
            return -1;
        }

        //反转链表
        void reverse(){
            tail = head;//反转后尾变头
            Node* pre = NULL;
            Node* cur = head;
            
            while(cur != NULL){
                Node* nxt = cur->next;
                cur->next = pre;
                pre = cur;
                cur = nxt;
            }
            head = pre;//反转后的头节点
        }

        // 遍历链表并打印
        void print(){
            Node* cur = head;
            while (cur != NULL) {
                cout << cur->data << " ";
                cur = cur->next;
            }
            cout << endl;
        }
};

int main() {
    SingleLinkedList list;

    // 测试头插
    list.push_front(3);
    list.push_front(2);
    list.push_front(1);
    cout << "After push_front: ";
    list.print(); // 输出: 1 2 3 

    // 测试尾插
    list.push_back(4);
    list.push_back(5);
    cout << "After push_back: ";
    list.print(); // 输出: 1 2 3 4 5 

    // 测试删除节点
    list.deleteNode(3);
    cout << "After deleteNode(3): ";
    list.print(); // 输出: 1 2 4 5 

    // 测试排序
    list.push_front(10);
    list.push_front(5);
    cout << "Before sort: ";
    list.print(); // 输出: 5 10 1 2 4 5 
    list.sort();
    cout << "After sort: ";
    list.print(); // 输出: 1 2 4 5 5 10 

    // 测试反转
    list.reverse();
    cout << "After reverse: ";
    list.print(); // 输出: 10 5 5 4 2 1 

    // 测试查找
    cout << "Element at index 2: " << list.search(2) << endl; // 输出: 5

    // 测试链表长度
    cout << "List size: " << list.size() << endl; // 输出: 6

    return 0;
}