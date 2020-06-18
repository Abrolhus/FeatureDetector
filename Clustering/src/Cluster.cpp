#include "Cluster.hpp"
void Cluster::printElements(){
    for (auto it = this->elements.begin(); it != this->elements.end(); ++it)
    {
        cout << *it << " ";
    }
    cout << endl;
}

void Cluster::printColor(){
    cout << this->color.b << " " << this->color.g << " " << this->color.r << endl;
}

int  Cluster::addElement(int val){
    if(this->newElementVerifier(val)){
        return this->elements.insert(val).second;
    }
    return false;
}

bool Cluster::findElement(int val){
    return this->elements.find(val) != this->elements.end();
}
set<int> Cluster::getElements(){// cheap way to interate through this' elements
    return this->elements;
}

Cluster::Cluster(string name, Color color) : name(name), color(color){
}
Cluster::Cluster(string name, int b, int g, int r) : name(name){
       this->color = Color(b, g, r);
       cout << b << " " << g << " " << r << endl;
}

string Cluster::getName(){
    return this->name;
}
Color Cluster::getColor(){
    return this->color;
}
int Cluster::getSize(){
    return this->elements.size();
}
void Cluster::setName(string name){
    this->name = name;
}
void Cluster::setColor(Color color){
    this->color = color;
}
bool Cluster::newElementVerifier(int val){
    return true;
}
