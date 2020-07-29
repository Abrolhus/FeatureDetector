#include "Cluster.hpp"
#include <string>
void Cluster::printElements(){
    /* Prints all int values stored
     */
    for (auto it = this->elements.begin(); it != this->elements.end(); ++it)
    {
        cout << *it << " ";
    }
    cout << endl;
}

string Cluster::getColorString(){
    /* returns a BGR 
     */
    return "(" + to_string(this->b()) + ", " + to_string(this->g()) + ", " + to_string(this->r()) + ")";
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

Cluster::Cluster(string name, int hexColor) : name(name), color(hexColor){
}
Cluster::Cluster(string name, int b, int g, int r) : name(name){
           this->color = this->bgrToInt(b,g,r);
}

/// GETS ///
string Cluster::getName(){
    return this->name;
}
int Cluster::getColor(){
    return this->color;
}
int Cluster::b(){
    return ((this->color >> 16) & 0xFF);
}
int Cluster::g(){
    return ((this->color >> 8) & 0xFF);
}
int Cluster::r(){
    return ((this->color) & 0xFF);
}
int Cluster::getSize(){
    return this->elements.size();
}

/// SETS ///
void Cluster::setName(string name){
    this->name = name;
}
void Cluster::setColor(int hexColor){
    if(newElementVerifier(hexColor))
        this->color = hexColor;
}
/// PRIVATE ///
bool Cluster::newElementVerifier(int val){
    /* checks if its a valid color value,
     * from 0 to ffffff
     */
    if(0 <= val && val <= 0xFFFFFF) 
        return true;
    return false;
}
int Cluster::bgrToInt(int b, int g, int r){
    return  (b << 16) | (g << 8) | (r);
}
