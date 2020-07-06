#include "Cluster.hpp"
#include <string>
void Cluster::printElements(){
    cout << "from " << endl;
}
string Cluster::getColorString(){
    return "(" + to_string(this->b()) + ", " + to_string(this->g()) + ", " + to_string(this->r()) + ")";
}

int  Cluster::addElement(int val){
    
    if(!this->newElementVerifier(val))
        return false;
    if(isInRange(val))
        return 2;
    Color color = intToColor(val);
    this->maxColor.b = (maxColor.b < color.b)? color.b : maxColor.b;
    this->maxColor.g = (maxColor.g < color.g)? color.g : maxColor.g;
    this->maxColor.r = (maxColor.r < color.r)? color.r : maxColor.r;
    this->minColor.b = (minColor.b > color.b)? color.b : minColor.b;
    this->minColor.g = (minColor.g > color.g)? color.g : minColor.g;
    this->minColor.r = (minColor.r > color.r)? color.r : minColor.r;
    return true;;
}

bool Cluster::findElement(int val){
    return this->isInRange(val);
}

Cluster::Cluster(string name, int hexColor) : name(name), color(hexColor){
           this->maxColor = Color(0,0,0);
           this->minColor = Color(255,255,255);
}

Cluster::Cluster(string name, int b, int g, int r) : name(name){
           this->color = this->bgrToInt(b,g,r);
           this->maxColor = Color(0,0,0);
           this->minColor = Color(255,255,255);
}

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

void Cluster::setName(string name){
    this->name = name;
}
void Cluster::setColor(int hexColor){
    this->color = hexColor;
}
bool Cluster::newElementVerifier(int val){
    return true;
}
int Cluster::bgrToInt(int b, int g, int r){
    return  (b << 16) | (g << 8) | (r);
}
bool Cluster::isInRange(int val){
    Color color = intToColor(val);
    if(color.b < this->minColor.b || color.b > this->maxColor.b)
        return false;
    if(color.g < this->minColor.g || color.g > this->maxColor.g)
        return false;
    if(color.r < this->minColor.r || color.r > this->maxColor.r)
        return false;
    return true;
}
Color Cluster::intToColor(int hexColor){
    Color color;
    color.b = ((hexColor >> 16) & 0xFF);
    color.g = ((hexColor >> 8) & 0xFF);
    color.r = ((hexColor) & 0xFF);
    return color;
}
