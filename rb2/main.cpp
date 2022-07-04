#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <stdlib.h>

using namespace std;

// structura de date ce reprezinta un nod in arbore
struct Node {
	int data; // stocheaza key-ul
    string titlu;
    string descriere;
	Node *parent; // pointer la parinte
	Node *left; // pointer la nodul din stanga
	Node *right; // pointer la nodul din dreapta
	int color; // 1 -> Rosu, 0 -> Negru
};

typedef Node *NodePtr;

// clasa RBTree implementeaza operatiunile in arborele rosu negru
class RBTree {
private:
	NodePtr root;
	NodePtr TNULL;

	void initializeNULLNode(NodePtr node, NodePtr parent) {
		node->data = 0;
        node->titlu = "initializare";
        node->descriere = "initializare";
		node->parent = parent;
		node->left = nullptr;
		node->right = nullptr;
		node->color = 0;
	}

	void preOrderHelper(NodePtr node) {
		if (node != TNULL) {
			cout<<node->data<<" ";
			preOrderHelper(node->left);
			preOrderHelper(node->right);
		}
	}

	void inOrderHelper(NodePtr node) {
		if (node != TNULL) {
			inOrderHelper(node->left);
			cout<<node->data<<" ";
			inOrderHelper(node->right);
		}
	}

	void postOrderHelper(NodePtr node) {
		if (node != TNULL) {
			postOrderHelper(node->left);
			postOrderHelper(node->right);
			cout<<node->data<<" ";
		}
	}


    NodePtr searchTreeHelper(NodePtr node, int key) {
		if (node == TNULL || key == node->data) {
			return node;
		}

		if (key < node->data) {
			return searchTreeHelper(node->left, key);
		}
		return searchTreeHelper(node->right, key);
	}

	// restituie arborele modificat de operatiunea de stergere
	void fixDelete(NodePtr x) {
		NodePtr s;
		while (x != root && x->color == 0) {
			if (x == x->parent->left) {
				s = x->parent->right;
				if (s->color == 1) {
					// cazul3.1
					s->color = 0;
					x->parent->color = 1;
					leftRotate(x->parent);
					s = x->parent->right;
				}

				if (s->left->color == 0 && s->right->color == 0) {
					// cazul 3.2
					s->color = 1;
					x = x->parent;
				} else {
					if (s->right->color == 0) {
						// czul 3.3
						s->left->color = 0;
						s->color = 1;
						rightRotate(s);
						s = x->parent->right;
					}

					// cazul 3.4
					s->color = x->parent->color;
					x->parent->color = 0;
					s->right->color = 0;
					leftRotate(x->parent);
					x = root;
				}
			} else {
				s = x->parent->left;
				if (s->color == 1) {
					// cazul 3.1
					s->color = 0;
					x->parent->color = 1;
					rightRotate(x->parent);
					s = x->parent->left;
				}

				if (s->right->color == 0 && s->right->color == 0) {
					// cazul 3.2
					s->color = 1;
					x = x->parent;
				} else {
					if (s->left->color == 0) {
						// cazul 3.3
						s->right->color = 0;
						s->color = 1;
						leftRotate(s);
						s = x->parent->left;
					}

					// cazul 3.4
					s->color = x->parent->color;
					x->parent->color = 0;
					s->left->color = 0;
					rightRotate(x->parent);
					x = root;
				}
			}
		}
		x->color = 0;
	}


	void rbTransplant(NodePtr u, NodePtr v){
		if (u->parent == nullptr) {
			root = v;
		} else if (u == u->parent->left){
			u->parent->left = v;
		} else {
			u->parent->right = v;
		}
		v->parent = u->parent;
	}

	void deleteNodeHelper(NodePtr node, int key) {
		// cauta nodul continand key-ul
		NodePtr z = TNULL;
		NodePtr x, y;
		while (node != TNULL){
			if (node->data == key) {
				z = node;
			}

			if (node->data <= key) {
				node = node->right;
			} else {
				node = node->left;
			}
		}

		if (z == TNULL) {
			cout<<"Nu am putut gasi nodul cu key-ul specificat in arbore"<<endl;
			return;
		}

		y = z;
		int y_original_color = y->color;
		if (z->left == TNULL) {
			x = z->right;
			rbTransplant(z, z->right);
		} else if (z->right == TNULL) {
			x = z->left;
			rbTransplant(z, z->left);
		} else {
			y = minimum(z->right);
			y_original_color = y->color;
			x = y->right;
			if (y->parent == z) {
				x->parent = y;
			} else {
				rbTransplant(y, y->right);
				y->right = z->right;
				y->right->parent = y;
			}

			rbTransplant(z, y);
			y->left = z->left;
			y->left->parent = y;
			y->color = z->color;
		}
		delete z;
		if (y_original_color == 0){
			fixDelete(x);
		}
	}

	// restituie arborele rosu negru
	void fixInsert(NodePtr k){
		NodePtr u;
		while (k->parent->color == 1) {
			if (k->parent == k->parent->parent->right) {
				u = k->parent->parent->left; // uncle
				if (u->color == 1) {
					// cazul 3.1
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;
				} else {
					if (k == k->parent->left) {
						// cazul 3.2.2
						k = k->parent;
						rightRotate(k);
					}
					// cazul 3.2.1
					k->parent->color = 0;
					k->parent->parent->color = 1;
					leftRotate(k->parent->parent);
				}
			} else {
				u = k->parent->parent->right; // uncle

				if (u->color == 1) {
					// oglinda-cazul 3.1
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;
				} else {
					if (k == k->parent->right) {
						// oglinda-cazul 3.2.2
						k = k->parent;
						leftRotate(k);
					}
					// oglinda-cazul 3.2.1
					k->parent->color = 0;
					k->parent->parent->color = 1;
					rightRotate(k->parent->parent);
				}
			}
			if (k == root) {
				break;
			}
		}
		root->color = 0;
	}

	void printHelper(NodePtr root, string indent, bool last) {
		// afiseaza structura arborelui pe ecran
	   	if (root != TNULL) {
		   cout << indent;
		   if (last) {
		      cout<<"R----";
		      indent += "     ";
		   } else {
		      cout<<"N----";
		      indent += "|    ";
		   }

           string sColor = root->color?"ROSU":"NEGRU";
		   cout<<root->data<<"("<<sColor<<")"<<endl;
		   printHelper(root->left, indent, false);
		   printHelper(root->right, indent, true);
		}
	}

public:
	RBTree() {
		TNULL = new Node;
		TNULL->color = 0;
		TNULL->left = nullptr;
		TNULL->right = nullptr;
		root = TNULL;
	}


	void preorder() {
		preOrderHelper(this->root);
	}


	void inorder() {
		inOrderHelper(this->root);
	}


	void postorder() {
		postOrderHelper(this->root);
	}

	// cauta in arbore dupa key-ul k
	// si returneaza nodul corespunzator
	NodePtr searchTree(int k) {
		return searchTreeHelper(this->root, k);
	}

	// cauta nodul cu cel mai mic key
	NodePtr minimum(NodePtr node) {
		while (node->left != TNULL) {
			node = node->left;
		}
		return node;
	}

	// cauta nodul cu cel mai mare key
	NodePtr maximum(NodePtr node) {
		while (node->right != TNULL) {
			node = node->right;
		}
		return node;
	}

	// cauta succesorul unui anumit nod
	NodePtr successor(NodePtr x) {
		if (x->right != TNULL) {
			return minimum(x->right);
		}

		NodePtr y = x->parent;
		while (y != TNULL && x == y->right) {
			x = y;
			y = y->parent;
		}
		return y;
	}

	// cauta predecesorul unui anumit nod
	NodePtr predecessor(NodePtr x) {
		if (x->left != TNULL) {
			return maximum(x->left);
		}

		NodePtr y = x->parent;
		while (y != TNULL && x == y->left) {
			x = y;
			y = y->parent;
		}

		return y;
	}

	// rotatie de stanga la nodul x
	void leftRotate(NodePtr x) {
		NodePtr y = x->right;
		x->right = y->left;
		if (y->left != TNULL) {
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		} else if (x == x->parent->left) {
			x->parent->left = y;
		} else {
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	}

	// rotatie de dreapta la nodul x
	void rightRotate(NodePtr x) {
		NodePtr y = x->left;
		x->left = y->right;
		if (y->right != TNULL) {
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		} else if (x == x->parent->right) {
			x->parent->right = y;
		} else {
			x->parent->left = y;
		}
		y->right = x;
		x->parent = y;
	}

	// inserarea nodului cu titlu si descriere
	void insert(int key, string titlu, string descriere) {
		NodePtr node = new Node;
		node->parent = nullptr;
		node->data = key;
        node->titlu = titlu;
        node->descriere = descriere;
		node->left = TNULL;
		node->right = TNULL;
		node->color = 1;

		NodePtr y = nullptr;
		NodePtr x = this->root;

		while (x != TNULL) {
			y = x;
			if (node->data < x->data) {
				x = x->left;
			} else {
				x = x->right;
			}
		}

		node->parent = y;
		if (y == nullptr) {
			root = node;
		} else if (node->data < y->data) {
			y->left = node;
		} else {
			y->right = node;
		}

		if (node->parent == nullptr){
			node->color = 0;
			return;
		}

		if (node->parent->parent == nullptr) {
			return;
		}

		fixInsert(node);
	}

	NodePtr getRoot(){
		return this->root;
	}

	void deleteNode(int data) {
		deleteNodeHelper(this->root, data);
	}

	void prettyPrint() {
	    if (root) {
    		printHelper(this->root, "", true);
	    }
	}

};

void insertMessageEvent(RBTree &rbt, int &id)
{
	ofstream fout("date.out", ios::app);
    string titlu, descriere;
    cout << "Introduceti titlul evenimentului:" << endl;
    getline(cin, titlu);
	fout << titlu << endl;
	fout << id << endl;
	fout.close();

    cout << "Introduceti descrierea evenimentului:" << endl;
	getline(cin, descriere);
    rbt.insert(id, titlu, descriere);
	cout << endl << endl;
    id = rand() % 1000000;
}

void findEvent(RBTree rbt)
{
	int id;
	string titlu, titlu_cautat;
	cout << "Introduceti titlul evenimentului: ";
	getline(cin, titlu);
	bool ok = false;

	ifstream fin("date.out");
	for( int i = 0; i < 10; i++)
	{
		if(!ok)
		{
			getline(fin, titlu_cautat);

			if(titlu_cautat==titlu)
			{
				string myStr;
				getline(fin, myStr);
				id = stoi(myStr);
				ok = true;
			}
			getline(fin, titlu_cautat);
		}
	}

	if(ok)
	{
		cout << endl;
		cout << "Eveniment" << endl;
		cout << "	Nod ID: " << id << endl;
		cout << "	Titlu: " << titlu << endl;
		cout << "	Descriere: ";
		cout << rbt.searchTree(id)->descriere << endl << endl << endl;
	}
	else
		cout << "Nu exista acest eveniment" << endl << endl;

	fin.close();
}

void deleteLineFile(string fisier, int &nod)
{
    string linieStearsa;
    string line;

    ifstream fin;
    fin.open(fisier);
    ofstream temp;
    temp.open("temp.txt");
    cout << "Introduceti titlul evenimentului care doriti sa fie sters: "; //input line to remove
    getline(cin, linieStearsa);
	int nr = 0;

    while (getline(fin,line))
	{

		if(line == linieStearsa)
		{
			getline(fin, line);
			nod = stoi(line);
		}
		else
			temp << line << endl;

	}

    temp.close();
    fin.close();
	remove("date.out");
	rename("temp.txt","date.out");
}

void deleteEvent(RBTree &rbt)
{
	int nr_nod;
	deleteLineFile("date.out", nr_nod);
	rbt.deleteNode(nr_nod);
	cout << "Evenimentul a fost sters." << endl << endl;
}

void displayEvents(RBTree rbt)
{
	string linie;
	ifstream fin("date.out");
	while(getline(fin, linie))
	{
		getline(fin, linie);
		int id = stoi(linie);
		// cout << endl;
		cout << "Nod ID: " << id << endl;
		cout << "Titlu: " << rbt.searchTree(id)->titlu << endl;
		cout << "Descriere: ";
		cout << rbt.searchTree(id)->descriere << endl << endl << endl;
	}
}

int main() {

	remove("date.out");

	RBTree bst;
    int id = rand() % 1000000;
    string answer;

	while(answer != "nu")
	{
		cout << "Ce actiune doriti sa faceti(pentru a inchide programul scrieti 'nu'): " << endl
		<< "1. Pentru a introduce un eveniment nou apasati tasta 1;" << endl
		<< "2. Pentru stergerea unui eveniment apasati tasta 2;" << endl
		<< "3. Pentru cautarea unui eveniment apasati tasta 3." << endl
		<< "4. Pentru afisarea tuturor evenimentelor apasati tasta 4." << endl;
		getline(cin, answer);

		if(answer == "1")
			insertMessageEvent(bst, id);
		else if(answer == "2")
			deleteEvent(bst);
		else if(answer == "3")
			findEvent(bst);
		else if(answer == "4")
			displayEvents(bst);
	}

    return 0;
}
