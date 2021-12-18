#include <iostream>
using namespace std;
template <class T, int M>
class BtreeNode{
    public:
        BtreeNode** childs;
        BtreeNode* parent;
        T* keys;
        bool isLeaf;
        int currKeys;

        BtreeNode(bool leafState){
            keys = new T[M];
            childs = new BtreeNode<T,M>*[M+1];
            //fill children with nulls
            for(int i = 0; i < M+1; i++){
                childs[i] = NULL;
            }
            parent = NULL;
            currKeys = 0;
            isLeaf = leafState;
        }

        //add key to the current node only if it's not filled(has a free space)
        void insertKey(T key){
            int lastValidPos = currKeys-1;
            //shift right all greater keys and put the key into it's proper position
            while(keys[lastValidPos] > key && lastValidPos > -1){
                keys[lastValidPos+1] = keys[lastValidPos];
                lastValidPos--;
            }
            keys[lastValidPos+1] = key;
            currKeys++;
        }

        void splitNode(){

            BtreeNode<T,M>* newNode=new BtreeNode <T,M>(isLeaf);
            //get the mid key index
            int midIdx = currKeys/2;
            if(currKeys%2==0){
                midIdx--;
            }

            T midKey = keys[midIdx];

            int i=0,j=midIdx+1;

            //split keys and child of both nodes
            while(j<currKeys){
                newNode->keys[i] = keys[j];
                newNode->currKeys++;
                newNode->childs[i] = childs[j];
                childs[j] = NULL;
                i++,j++;
            }

            newNode->childs[i] = childs[j];
            childs[j]=NULL;
            currKeys=midIdx;

            //connect new nodes to the parent
            if(parent==NULL){
                BtreeNode <T,M> *newnode2 = new BtreeNode <T,M>(false);
                newnode2->keys[0] = midKey;
                newnode2->currKeys++;
                newnode2->childs[0] = this;
                newnode2->childs[1] = newNode;
                parent = newnode2;
                newNode->parent = newnode2;
            }else{
                newNode->parent = parent;
                parent->insertKey(midKey);
                //find the position of current child to swap it
                for(i = 0; i < M+1;i++){
                    if(parent->childs[i] == this){
                        break;
                    }
                }
                BtreeNode<T,M>* first = parent->childs[i+1];
                BtreeNode<T,M>* second;
                parent->childs[i+1] = newNode;
                //add new child to it's correct position
                for(j = i+2; j < M+1; j++){
                    second = parent->childs[j];
                    parent->childs[j] = first;
                    first = second;
                }
                //if parent is full then split it
                if(parent->currKeys == M){
                    parent->splitNode();
                }
            }
        }
};

template <class T, int M>
class BTree{
    private:
        BtreeNode <T,M>* root=NULL;
        void printNode(BtreeNode<T,M>* node, string dis){
            if (node != NULL){
                printkeys(node->keys,node->currKeys);
                for(int i=0;i<M;i++){
                    if(node->childs[i] != NULL){
                        cout<<dis;
                        printNode(node->childs[i],dis+" ");
                    }
                }
            }
        }
        void printkeys(T* keys, int currKeys){
            for(int i=0;i<currKeys;i++){
                char last = '\n';
                if(i<currKeys-1){
                    last=',';
                }
                cout<<keys[i]<<last;
            }

        }

    public:

        Btree(){
            root=NULL;
        }

        void Insert(T value){
            //make a new root and insert as left most child
            if(root==NULL){
                root = new BtreeNode<T,M>(true);
                root->keys[0] = value;
                root->currKeys = 1;
            }else{
                BtreeNode <T,M> *currnode=root;
                //get a leaf node to insert at
                while(!currnode->isLeaf){
                    if(value>currnode->keys[currnode->currKeys-1]){
                        currnode=currnode->childs[currnode->currKeys];
                    }else{
                        for(int i=0;i<currnode->currKeys;i++){
                            if(currnode->keys[i]>value){
                                currnode=currnode->childs[i];
                                break;
                            }
                        }
                    }
                }
                currnode->insertKey(value);
                //if order of it is m then split it
                if(currnode->currKeys == M){
                    currnode->splitNode();
                    //to handle right root if needed
                    if(root->parent != NULL){
                        root = root->parent;
                    }
                }
            }
        }

        void Print(){
            printNode(root, " ");
        }
};

int main()
{
    // Construct a BTree of order 3, which stores int data
    BTree<int,3> t1;

    t1.Insert(1);
    t1.Insert(5);
    t1.Insert(0);
    t1.Insert(4);
    t1.Insert(3);
    t1.Insert(2);

    t1.Print(); // Should output the following on the screen:

    /*
    1,4
      0
      2,3
      5
    */

    // Construct a BTree of order 5, which stores char data
    BTree<char,5> t;

    // Look at the example in our lecture:
    t.Insert('G');
    t.Insert('I');
    t.Insert('B');
    t.Insert('J');
    t.Insert('C');
    t.Insert('A');
    t.Insert('K');
    t.Insert('E');
    t.Insert('D');
    t.Insert('S');
    t.Insert('T');
    t.Insert('R');
    t.Insert('L');
    t.Insert('F');
    t.Insert('H');
    t.Insert('M');
    t.Insert('N');
    t.Insert('P');
    t.Insert('Q');

    t.Print(); // Should output the following on the screen:

    /*
    K
      C,G
        A,B
        D,E,F
        H,I,J
      N,R
        L,M
        P,Q
        S,T
    */

    return 0;
}

