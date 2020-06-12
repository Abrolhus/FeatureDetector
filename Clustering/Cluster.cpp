#include "Cluster.hpp"
void Cluster::printElements(){
    for (auto it = this->elements.begin(); it != this->elements.end(); ++it)
    {
        cout << *it << " ";
    }
    cout << endl;
}
int  Cluster::addElement(int val){
    if(this->newElementVerifier(val)){
        return this->elements.insert(val).second;
    }
    return false;
}
Cluster::Cluster(string name, string color) : name(name), color(color){
}
string Cluster::getName(){
    return this->name;
}
string Cluster::getColor(){
    return this->color;
}
void Cluster::setName(string name){
    this->name = name;
}
void Cluster::setColor(string color){
    this->color = color;
}
bool Cluster::newElementVerifier(int val){
    return true;
}
