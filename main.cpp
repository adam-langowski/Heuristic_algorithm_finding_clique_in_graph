#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <sstream>
using namespace std;

class Sequence
{
	string name;
	string nucleotides;
    int sum;

public:
    vector<int> quality;
	vector<char> seq;
	vector<int> positions;
    int deleted;

    int deletions;
	// Nazwa
	void setName(string _name){
		name = _name;
	}
	string getName(){
		return name;
	}

	// Sekwencja
	void setSequence(string sequence){
		nucleotides = sequence;
	}
	void addSequence(){
		copy(nucleotides.begin(), nucleotides.end(), back_inserter(seq));
	}
	void set_new_Sequence(string sequence){
		nucleotides = sequence;
	}
	void add_new_Sequence(){
		copy(nucleotides.begin(), nucleotides.end(), back_inserter(seq));
	}
	char getSequence(int num){
		return seq[num];
	}
	int getSequenceSize(){
		return seq.size();
	}
	string getWholeSequence(){
		return nucleotides;
	}
	int get_deletions(){
        return deletions;
	}

	// prog wiarygodnosci
	void addQuality(int num){
		quality.push_back(num);
	}
	int getQuality(int num){
		return quality[num];
	}
	int getQualitySize(){
		return quality.size();
	}

	int get_deleted(){
        return deleted;
	}
};

int nucleotides_size(Sequence arr[]){    // rozmiar sekwencji
	int total = 0;
	for (int i = 0; i < 5; i++)
		total = total + arr[i].getSequenceSize();
	return total;
}

// odczyt z pliku
void loadFile(Sequence arr[]){
	ifstream in;
	string line, line2;
	string val;
	string str;

	//fasta
	in.open("fasta.txt");
	if (in.is_open()){
		for (int i = 0; i < 5; i++){
			getline(in, line);
			arr[i].setName(line);
			getline(in, line);
			arr[i].setSequence(line + line2);
			arr[i].addSequence();
			int s = arr[i].getSequenceSize();
			for (int j = 0; j < s; j++) {
                arr[i].positions.push_back(j);
			}
		}
	}
	else{
		cout << "Error - file fasta.txt not found" << endl;
		exit(0);
	}
	in.close();

	//quality
	in.open("quality.txt");
	if (in.is_open()){
		for (int i = 0; i < 5; i++){
			getline(in, line);
			getline(in, line);
			istringstream ss(line);
			int num;
			while (ss >> num){
				arr[i].addQuality(num);
			}
		}
	}
	else{
		cout << "Error - quality.txt file not found" << endl;
		exit(0);
	}
	in.close();
}

void printData(Sequence arr[]){
	for (int i = 0; i < 5; i++){
		cout << "Sequence " << i << endl;
		cout << " name: " << arr[i].getName() << endl;
		cout << " seq: " << arr[i].getWholeSequence() << endl;
		cout << " size of qual: " << arr[i].getQualitySize() << "  size of seq: " << arr[i].getSequenceSize() << endl;
		cout << " val: ";
		for (size_t j = 0; j < arr[i].getQualitySize(); j++)
			cout << arr[i].getQuality(j) << " ";
		cout << endl;
		cout << endl << endl;
	}
	cout << "Suma nukleotydow: " << nucleotides_size(arr) << endl;

}


class Vertice
{
public:
	int number;
	int startPos;
	int origin;
	bool visited;
	vector<int> qual;
	vector<char> seq;
	vector<int> next;
	char getSequence(int num)
	{
		return seq[num];
	}
	int getQuality(int num)
	{
		return qual[num];
	}
	int getOrigin()
	{
		return origin;
	}
	int getNextSize()
	{
		return next.size();
	}
	void printSequence()
	{
		for (int i = 0; i < seq.size(); i++)
			cout << seq[i];
	}
};

void printVertices(vector<Vertice> graph, int substring, Sequence arr[])
{
	vector<Vertice>::iterator it;
	int v = 0;
	int i = 0;
	for (it = graph.begin(); it != graph.end(); ++it)
	{
		cout << "vertice: " << it->number << endl;
		cout << "seq origin: " << it->origin << endl;
		cout << "start position: " << it->startPos + 1 << endl;
		//cout << "before deletion: " << arr[i].deleted<<endl;                // pos
		cout << "sequence: ";
		for (size_t k = 0; k < substring; k++)
			cout << it->seq[k];

		cout << endl << "next vertices: ";
		for (int l = 0; l < graph[v].getNextSize(); l++)
			cout << graph[v].next[l] << ", ";
		cout << endl << endl;
		v++;
		i++;
	}
}

void fillEdges(vector<Vertice>& graph, int substring, int reliability, int count)
{
	int con = 0;

	for (int a = 0; a < count; a++)
	{
		for (int b = a + 1; b < count; b++)
		{
			if (graph[a].origin != graph[b].origin)
			{
				for (int i = 0; i < substring; i++)
				{

                    if (graph[a].seq[i] == graph[b].seq[i] && graph[a].qual[i] >= reliability && graph[b].qual[i] >= reliability){
						con++;
                    }
				}
				if (con == substring)
				{
					graph[a].next.push_back(b);
					graph[b].next.push_back(a);
				}
				con = 0;
			}
		}
	}
}

void visitNode(vector<Vertice> graph, int v, vector<Vertice>& temp, vector<Vertice>& best, bool visitedSeq[], bool& key)
{
	if (key == true)
		return;
	graph[v].visited = true;
	visitedSeq[graph[v].origin] = true;
	temp.push_back(graph[v]);
	if (temp.size() > best.size())
		best = temp;
	if (best.size() == 5)
	{
		key = true;
		return;
	}
	for (int i = 0; i < graph[v].getNextSize(); i++)
	{
		if (visitedSeq[graph[graph[v].next[i]].getOrigin()] == false && graph[graph[v].next[i]].visited == false)
			visitNode(graph, graph[v].next[i], temp, best, visitedSeq, key);
	}
	temp.pop_back();
	visitedSeq[graph[v].origin] = false;
}

vector<Vertice> findClique(vector<Vertice> graph)
{
	vector<Vertice> temp;
	vector<Vertice> best;
	bool visitedSeq[5] = {};
	bool key = false;
	for (int i = 0; i < graph.size(); i++)
		graph[i].visited = false;
	for (int j = 0; j < graph.size(); j++)
		visitNode(graph, j, temp, best, visitedSeq, key);

	return best;
}

int main()
{
	Sequence arr[5];
	int del_arr[5];
	int substring;
	int reliability;
	loadFile(arr);
	cout<<"Wczytane sekwencje:"<<endl<<endl;
	printData(arr);
	cout << endl;
	cout << "Wprowadz dlugosc podciagu (4-9): " << endl;
	cin >> substring;
	do{
		if(substring < 4 || substring > 9){
			cout << "Nieprawidlowa dlugosc podciagu. Wartosc calkowita musi nalezec do przedzialu (4,9): " << endl;
			cin >> substring;
		}
	} while(substring < 4 || substring > 9);

    cout << "Podaj prog wiarygodnosci (0-40): " << endl;
    cin >> reliability;
	do{
		if(reliability < 0 || reliability > 40){
			cout << "Nieprawidlowy prog wiarygodnosci. Wartosc calkowita musi nalezec do przedzialu (0,40): " << endl;
			cin >> reliability;
		}
	} while (reliability < 0 || reliability > 40);


	printData(arr);
	cout<<endl;

	vector<Vertice> graph;
	vector<int> StartPositions;

	int total = 0;
	for (int i = 0; i < 5; i++) //dla kazdej sekwencji
	{
	    int t = 0;
		for (int j = 0; j < arr[i].getSequenceSize() - substring; j++) // dla kazdego nukleotydu w sekw
		{

			Vertice* v = new Vertice;
			for (int k = 0; k < substring; k++)
			{
				v->number = total;
				v->seq.push_back(arr[i].getSequence(j + k));
				v->qual.push_back(arr[i].getQuality(j + k));
				v->origin = i;
				v->startPos = j;
			}
            graph.push_back(*v);
            total++;
            t += 1;

		}
	}
    for (int i = 0; i < 5; i++){
        cout<<del_arr[i]<<endl;
    }
	fillEdges(graph, substring, reliability, total);
	printVertices(graph, substring, arr);

	vector<Vertice> best = findClique(graph);
	cout << endl<< "Clique information: "<< endl;
    printVertices(best, substring, arr);     // znaleziona klika
    cout<< "Prog: "<< reliability<< " |  Dlugosc podciagu: "<< substring<<endl;
	cout << "Znaleziono: " << best.size() << endl;
	for (int j = 0; j < best.size(); j++)
	{
	    if(best.size() > 1){
            cout << " seq: ";
            best[j].printSequence();
            cout << " from sequence: " << best[j].origin << endl;
	    }
	    else{
            cout<<"Clique not found"<<endl;
	    }

	}
}
