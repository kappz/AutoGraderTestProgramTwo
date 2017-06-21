/*
Author: Peter O'Donohue
Creation Date: 06/12/17
Modification Date: 06/21/17
Description: This class contains several functions that allow the user to create, compare, and
manipulate DNA strands. To gain access to this class, the user must include "dnaManip.H"
in their main program header. The user will then create objects of type "DNA" that will act as
their DNA strands, further modifying these objects with the function members included below.
*/

#include <iostream>
#include <string>
#include <ostream>
#include <vector>
using namespace std;
struct Node
{
	char acid;
	Node *next;
	Node(char character = ' ', Node *ptr = nullptr)
	{
		acid = character;
		next = ptr;
	}
};

class DNA
{
public:
	DNA(); // initializes the strand to default
	DNA(string dna); // initalizes the strand to the string
	DNA(const DNA& rhs); // copy constructor
	~DNA(); // destructor
	bool operator==(const DNA& rhs) const;
	//returns true if rhs has the same acids in the same order to self
	DNA operator=(const DNA& rhs);
	//	// assigns rhs to self (with no memory leak)
	DNA operator+(const DNA& rhs) const; // returns self + rhs
	int find(const DNA & rhs);  // returns the first position of subStr in self if exists or -1
								//	int find(const DNA& subStr) const;
								// returns the first position subStr in self exists or -1
	DNA reverse() const; // returns a DNA strand reversed
	friend ostream & operator<< (ostream & out, DNA rhs); // prints the strand from begin to end
														  // the first position is 0
														  // output gattaca print ( 1 , 3 ) is att
	DNA substr(int begin, int width) const;
	//  returns the substr and from begin, returns the next width acids
	bool unitTestPlus();  // tests sum of two DNA strands of equal length
	bool unitTestPlusDiffLengths();  // tests sum of two DNA strands of unequal length
	bool unitTestSubstr();  // tests return a sustring of a strand
	bool unitTestFind();  // tests finding a substring and returning its position
	bool unitTestReverse();  // tests reverse function

private:
	Node *strand;
	int length;
};

DNA::DNA()
{
	strand = nullptr;
	length = 0;
}

DNA::DNA(string dna)
{
	/*
	PRE: an object of type DNA has been initialized with
	a parameter containing a string word in the form of a DNA strand
	POST: link list created with new memory space, each node containing a
	character of string dna, head of list assigned to obect's pointer strand
	*/
	Node *walker = nullptr;
	strand = nullptr;
	length = dna.length();
	for (int i = 0; i < length; ++i)
	{
		if (strand == nullptr)
		{
			strand = new Node(dna[i]);
			walker = strand;
		}
		else
		{
			walker->next = new Node(dna[i]);
			walker = walker->next;
		}
	}
}


DNA::DNA(const DNA& rhs)
{
	/*
	PRE: an operation requiring a copy constructor has been performed
	POST: a deep copy of the original argument has been created and
	stored in memory
	*/
	Node *oldWalker = rhs.strand;
	Node *newWalker = nullptr;
	strand = nullptr;
	while (oldWalker != nullptr)
	{
		if (strand == nullptr)
		{
			strand = new Node(oldWalker->acid);
			newWalker = strand;
		}
		else
		{
			newWalker->next = new Node(oldWalker->acid);
			newWalker = newWalker->next;
		}
		oldWalker = oldWalker->next;
	}
}


DNA::~DNA()
{
	/*
	PRE: DNA type variable left scope,
	POST: an object has been deleted from memory
	*/
	Node *deletePtr = this->strand;
	while (deletePtr != nullptr)
	{
		this->strand = strand->next;
		delete deletePtr;
		deletePtr = this->strand;
	}
}


bool DNA::operator==(const DNA& rhs) const
{
	/*
	PRE: rhs and self declared without a default value
	POST: the two strands have been checked for equality
	the function returns with true if the two strands are equal
	and returns false if otherwise.
	*/
	Node *rhsWalker = rhs.strand;
	Node *lhsWalker = this->strand;
	if (length == rhs.length)  // checks for equality in strand length
	{
		while (lhsWalker->next != nullptr)
		{
			if (lhsWalker->acid == rhsWalker->acid)
			{
				lhsWalker = lhsWalker->next;
				rhsWalker = rhsWalker->next;
			}
			else
				return false;
		}
	}
	return true;
}



DNA DNA::operator=(const DNA& rhs)

{
	/*
	PRE: rhs was declared without a default value
	self was declared with or without a default value
	POST: self contains a copy of the elements of rhs
	any data that self originally pointed to has been deleted
	*/
	Node *newWalker = nullptr;
	Node *rhsWalker = rhs.strand;
	Node *deleteWalker = this->strand;    // walks down delete strand
	Node *deletePtr = deleteWalker;  // deletes the nodes
	this->length = rhs.length;
	this->strand = new Node(rhsWalker->acid);
	newWalker = this->strand;
	rhsWalker = rhsWalker->next;
	while (deleteWalker != nullptr)  // deletes old memory space
	{
		deleteWalker = deleteWalker->next;
		delete deletePtr;
		deletePtr = deleteWalker;
	}
	while (rhsWalker != nullptr)
	{
		newWalker->next = new Node(rhsWalker->acid);
		newWalker = newWalker->next;
		rhsWalker = rhsWalker->next;
	}
	return *this;
}

DNA DNA::operator+(const DNA& rhs) const
{
	/*
	PRE: rhs and self objects were declared, at least one of them having length > 0
	POST: the two objects were searched and stored in
	a new object 'result'
	result is returned to main function
	*/
	DNA result = *this; // stores sum result
	Node *rhsWalker = rhs.strand;
	Node *resultWalker = result.strand;
	if (length != 0 && rhs.length != 0)
	{
		while (resultWalker->next != nullptr)
			resultWalker = resultWalker->next;
		while (rhsWalker != nullptr)
		{
			resultWalker->next = new Node(rhsWalker->acid);
			resultWalker = resultWalker->next;
			rhsWalker = rhsWalker->next;
		}
	}
	return result;
}

ostream& operator<<(ostream & out, DNA rhs)
{
	/*
	PRE: rhs contains strand elements
	ofstream is included in main
	POST: rhs strand elements have been printed on screen
	*/
	Node *walker = rhs.strand;
	while (walker != nullptr)
	{
		out << walker->acid << " ";
		walker = walker->next;
	}
	return out;
}

DNA DNA::reverse() const
{
	/*
	PRE: object contains strand elements
	POST: new object created w/copy of strand elements in reverse
	new object returned to main
	*/
	DNA reverseDNA;
	Node *front = nullptr;
	Node *back = nullptr;
	Node *newWalker = this->strand;
	reverseDNA.strand = new Node(newWalker->acid);
	back = reverseDNA.strand;
	newWalker = newWalker->next;
	while (newWalker != nullptr)
	{
		front = new Node(newWalker->acid);
		front->next = back;
		back = front;
		newWalker = newWalker->next;
	}
	reverseDNA.strand = back;
	return reverseDNA;
}

DNA DNA::substr(int begin, int width) const
{
	/*
	PRE: begin and width fall withing objects length
	POST: object containing substring starting at position of begin
	and ending at position width is returned to main
	*/
	DNA substr;
	Node *walker = nullptr;
	Node *search = this->strand;
	if (begin + width <= length)  // checks if withing bounds
	{
		for (int i = 0; i < begin - 1; ++i)
		{
			search = search->next;
		}
		substr.strand = new Node(search->acid);
		search = search->next;
		walker = substr.strand;
		for (int i = 0; i < width - 1; ++i)
		{
			walker->next = new Node(search->acid);
			search = search->next;
			walker = walker->next;
		}
	}
	return substr;
}

int DNA::find(const DNA & rhs)
{
	/*
	PRE: rhs contains more than one strand element
	POST: if subStr is found, function returns position of first character
	returns -1 if Substr is not found
	*/
	int count = 1;  // keeps count of string location
	int position = 0;  // stores starting position
	Node *selfWalker = this->strand;
	Node *rhsWalker = rhs.strand;
	Node *tempWalker = nullptr;
	while (selfWalker != nullptr)
	{
		if (selfWalker->acid == rhsWalker->acid)
		{
			position = count;
			tempWalker = selfWalker;
			while (tempWalker->acid == rhsWalker->acid)
			{
				tempWalker = tempWalker->next;
				rhsWalker = rhsWalker->next;
				if (rhsWalker == nullptr)
				{
					return position;
				}
			}
		}
		else
		{
			position = 0;
			++count;
			selfWalker = selfWalker->next;
			rhsWalker = rhs.strand;
		}
	}
	return -1;
}

bool DNA::unitTestPlus()
{
	/*
	PRE: function for operator+ has been defined
	POST: the function for operator+ has resulted in a pass or fail
	control returns to main
	*/
	DNA c;
	DNA a("ACGT");
	DNA b("CGTA");
	DNA d("ACGTCGTA");
	c = a + b;
	if (c == d)
	{
		return true;
	}
	else
		return false;
}

bool DNA::unitTestPlusDiffLengths()
{
	/*
	PRE: function for overload operator+ has been defined
	POST: function for operator+ has been tested for
	two strands of unequal length
	control returns to main
	*/
	DNA a, b;
	DNA c("ACCTAG");
	DNA d("TCGA");
	DNA e("ACCTAGTCGA");
	DNA f("TCGAACCTAG");
	a = c + d;
	b = d + c;
	if (a == e && b == f)
		return true;
	else
		return false;

}

bool DNA::unitTestSubstr()
{
	/*
	PRE: substr function has been defined
	object invokes function call
	POST: substr function tested, result returned along
	with control to the main
	*/
	DNA a = "ACGAAGTCAGGGT";
	DNA b = "GGGT";
	DNA c;
	if (b == a.substr(10, 4))
		return true;
	else
		return false;

}

bool DNA::unitTestFind()
{
	/*
	PRE: function for find has been defined
	POST: find function has been tested
	result and control return to main
	*/
	int position = 0;
	DNA a("ACGT");
	DNA b("ACTTGACTAGCAACGT");
	position = b.find(a);
	if (position != -1)
		return true;
	else
		return false;
}

bool DNA::unitTestReverse()
{
	/*
	PRE: function for reverse defined
	POST: reverse tested, result and control pass
	to the main
	*/
	DNA a("GCATGGCT");
	DNA b("TCGGTACG");
	DNA c;

	c = a.reverse();
	if (c == b)
		return true;
	else
		return false;
}

int main()
{
	vector<DNA *> dnas;
	DNA empty;
	char oper;
	string word;
	cin >> oper;
	int index1, index2, strandIndex;
	int numDnas = 0;
	while (oper != 'q') {
		if (oper == 'i') { // i for inialize strands
			cin >> numDnas;

			for (int i = 0; i < numDnas; i++) {
				cin >> word;
				dnas.push_back(new DNA(word));
			}
			dnas.push_back(new DNA(""));
		}
		else if (oper == 'p') {
			for (int i = 0; i < dnas.size(); i++)
				cout << (*dnas[i]) << endl;
			cout << empty << endl;
		}
		else if (oper == 'r') { //r for rev
			for (int i = 0; i < dnas.size(); i++) {
				empty = dnas[i]->reverse();
				cout << empty << " " << *dnas[i] << endl;
			}
			cout << "checking originals:\n";
			for (int i = 0; i < numDnas; i++)
				cout << (*dnas[i]) << endl;
		}
		else  if (oper == 'f') { // f for find
			cin >> index1 >> index2;
			cout << dnas[index1]->find(*dnas[index2]);
		}
		else if (oper == 'a') {// addtion
			cin >> index1 >> index2;
			empty = *dnas[index1] + *dnas[index2];
			cout << empty << " now double components : ";
			*dnas[index1] = empty + empty;
			cout << empty << " = " << *dnas[index1] << " " << *dnas[index2] << endl;
		}
		else if (oper == 's') {//substr
			int start;
			int length;
			DNA temp("aaaa");
			cin >> strandIndex >> start >> length;
			empty = dnas[strandIndex]->substr(start, length);
			cout << "original " << *(dnas[strandIndex]) << endl;
			*dnas[strandIndex] = temp;
			cout << empty << endl;
		}
		else if (oper == 'd') {// destructor
			cin >> index1;
			dnas.erase(dnas.begin() + index1);
			{
				DNA local("gatac");
			}
		}
		else if (oper == 'e') {//equals (assignment)
			DNA temp;
			cin >> index1 >> index2;
			temp = *dnas[index2];
			*dnas[index2] = *dnas[index1];
			*dnas[index1] = temp;
		}
		else if (oper = 'E') {// equals comparison
			DNA nothing("");
			cin >> index1 >> index2;
			if (*dnas[index1] == *dnas[index2])
				cout << "same\n";
			else
				cout << "different";
			if (empty == nothing)
				cout << "same\n";
			else
				cout << "different";
		}
		else if (oper == 'c') {// clear
			dnas.clear();
		}
		cin >> oper;
	}
	return 0;
}