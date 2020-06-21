#include "Clusterizador.hpp"
#include <iostream>
#include <sstream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
using namespace std;
 
// Clusterizador::Clusterizador(){
    // this->upDiff = this->loDiff = 40;
// }
Clusterizador::Clusterizador(int lo, int up) : loDiff(lo), upDiff(up){
}

bool Clusterizador::createNewCluster(string name, Color color){
    if(this->checkIfClusterExists(name))
        return false;
    Cluster auxCluster = Cluster(name, color);
    std::pair <string, Cluster> par = std::make_pair(name, auxCluster);
    this->clusters.insert(par);
    return true;
}
bool Clusterizador::createNewCluster(string name, string hexColor){
    if(this->checkIfClusterExists(name))
        return false;
    Color color = this->convertHexToColor(hexColor);
    Cluster auxCluster = Cluster(name, color);
    std::pair <string, Cluster> par = std::make_pair(name, auxCluster);
    this->clusters.insert(par);
    return true;
}

bool Clusterizador::createNewCluster(string name, int hexColor){
    if(this->checkIfClusterExists(name))
        return false;
    Color color = this->convertHexToColor(hexColor);
    Cluster auxCluster = Cluster(name, color);
    std::pair <string, Cluster> par = std::make_pair(name, auxCluster);
    this->clusters.insert(par);
    return true;
}



bool Clusterizador::createNewCluster(string name, int b, int g, int r){
    Cluster auxCluster = Cluster(name, b, g, r);
    std::pair <string, Cluster> par = std::make_pair(name, auxCluster);
    this->clusters.insert(par);
    return true;
}

void Clusterizador::printClusters(){
    for(auto it = this->clusters.begin(); it != this->clusters.end(); ++it){
        // cout << it->first << ": " << it->second.getColor() << " color, " << it->second.getSize() << " elements." << endl;
        cout << it->first << ": " << "sei nn" << " color, " << it->second.getSize() << " elements." << endl;
    }
}

int Clusterizador::addToClusterByImage(cv::Mat image, string cluster, int x, int y){
   /* Provavelmente o método mais complicado desta classe,
    * usando o cv::floodFill, este método reconhece as cores proximas
    * a de um determinado ponto e adiciona a um cluster (desta classe) em específico
    * Nota: Este método não altera de nenhuma maneira a imagem recebida.
    */
    if(this->clusters.find(cluster) == this->clusters.end()) {
        //o método find retorna um ponteiro para o final do std::map 
        // caso não encontre nennhuma elemento com a key que está sendo procurada
        return -1;
    }
    cv::Point seed = cv::Point(x,y); 
    // https://books.google.com.br/books?id=FtCBDwAAQBAJ&pg=PA88&lpg=PA88&dq=mask+opencv+2+width+more&source=bl&ots=PxUe-T6lDu&sig=ACfU3U3WA6r4wJb9ExH_-hI_KQGnHzA4kg&hl=pt-BR&sa=X&ved=2ahUKEwjVvODxzv_pAhVqK7kGHUPhD-UQ6AEwAnoECAQQAQ#v=onepage&q=mask%20opencv%202%20width%20more&f=false
    // Page 88
    cv::Mat mask; 
    mask.create(image.rows + 2, image.cols+2, CV_8UC1);
    mask = Scalar::all(0);
    cv::Rect ccomp;
    int aux;
    int flags = CONNECTIVITY + (NEWMASKVAL << 8) + CV_FLOODFILL_FIXED_RANGE + CV_FLOODFILL_MASK_ONLY;
    int area = cv::floodFill(image, mask, seed, Scalar(255,0,0),  &ccomp, Scalar(this->loDiff, this->loDiff, this->loDiff), 
            Scalar(this->upDiff, this->upDiff, this->upDiff),flags);
    for(auto it = mask.begin<uchar>(); it != mask.end<uchar>(); ++it){
        if(*it == 255){
                // aux = this->vec3bToInt(Vec3b() );
                // aux = this->vec3bToInt(image.at<Vec3b>(20, 20));
                aux = this->vec3bToInt(image.at<Vec3b>(it.pos().y-1, it.pos().x-1)); //Gambiarra, esse it.pos().x-1 ... a mask tem 2 a mais de largura e altura que a imagem.
            this->clusters.at(cluster).addElement(aux);
        }     
    }
    return 0;
}
int Clusterizador::vec3bToInt(cv::Vec3b vec){
    return ((int)vec.val[0] << 16) | ((int)vec.val[1] << 8) | ((int)vec.val[2]); 
}

int Clusterizador::clusterizarImagem(cv::Mat* img, string cluster){
    /* Interates through image point by point.
     * If point's color is in the specified cluster,
     * changes the color of the point to the cluster's color. #TODO
     */
    if(!this->checkIfClusterExists(cluster))
        return false;
    MatIterator_<Vec3b> it, end;
    for( it = img->begin<Vec3b>(), end = img->end<Vec3b>(); it != end; ++it)
    { // https://docs.opencv.org/2.4/doc/tutorials/core/how_to_scan_images/how_to_scan_images.html#the-iterator-safe-method
        if(this->clusters.at(cluster).findElement(this->vec3bToInt(*it))){
            (*it)[0] = this->clusters.at(cluster).getColor().b;
            (*it)[1] = this->clusters.at(cluster).getColor().g;
            (*it)[2] = this->clusters.at(cluster).getColor().r;
        }
    }
    return 0;
}

int Clusterizador::clusterizarImagem(cv::Mat* img){
    /* Para cada elemento da imagem, procura por este em cada um dos "clusters".
     * Se encontrar substitui o valor de cor deste pelo valor do cluster em questão
     */
    MatIterator_<Vec3b> it, end;
    //TODO: find a better way to interate through image and clusterize it. 
    //Right now it n^2 * log(n).
    for( it = img->begin<Vec3b>(), end = img->end<Vec3b>(); it != end; ++it) 
        //TODO: check if this interator method is faster than the old for(i){for(j)} way
        // https://docs.opencv.org/2.4/doc/tutorials/core/how_to_scan_images/how_to_scan_images.html
    {
        for(auto clust = this->clusters.begin(); clust != this->clusters.end(); ++clust)
        { // https://docs.opencv.org/2.4/doc/tutorials/core/how_to_scan_images/how_to_scan_images.html#the-iterator-safe-method
            if(clust->second.findElement(this->vec3bToInt(*it))){
                (*it)[0] = clust->second.getColor().b;
                (*it)[1] = clust->second.getColor().g;
                (*it)[2] = clust->second.getColor().r;
            }
        }
    }
    return 0;
}

int Clusterizador::addToClusterViaFile(string file, string cluster){
     int c;
    cout<< "Loading file..." << endl;
    string line;
    ifstream inFile(file.c_str(), ios::in);
    if (inFile)
    {
        while (getline(inFile, line))
        {
            //cout << line << '\n';
            istringstream actualColor(line);
            actualColor >> c;
            //cout << c << endl;
            this->clusters.at(cluster).addElement(c);
            actualColor.str("");
        }
        inFile.close();
        return 0;
    }
    else
        cout << "could not open file, creating a new one..." << endl;
    return -1;
} 
int Clusterizador::saveClusterToFile(string file, string cluster){ 
    if(!this->checkIfClusterExists(cluster))
        return -1;
    cout<< "Loading file..." << endl;
    string line;
    ifstream inFile(file.c_str(), ios::in);
    int c;
    std::set<int> fileValues;
    std::set<int> clusterValues = this->clusters.at(cluster).getElements();
    if (inFile)
    {
        while (getline(inFile, line))
        {
            istringstream actualColor(line);
            actualColor >> c;
            fileValues.insert(c);
            actualColor.str("");
            // if(this->clusters.at(cluster).findElement(c)){
        }
        inFile.close();
    } else {
        cout << "could not open file, creating a new one..." << endl;
    }
    ofstream outFile(file.c_str(), ios::out);
    stringstream ss;
    if(outFile){
        for(auto it = clusterValues.begin(); it != clusterValues.end(); ++it){
            if(fileValues.find(*it) == fileValues.end()){
                ss << *it << endl;
            }
        }
        outFile << ss.str();
        outFile.close();
    } else {
        cout << "cant open" << endl;
    }
    return 0;
}
int Clusterizador::saveAllClustersToFile(string filePrefix){
    for(auto it = this->clusters.begin(); it != this->clusters.end(); ++it){
        this->saveClusterToFile(filePrefix + it->first + ".txt", it->first);
    }
    return 0;
}

int Clusterizador::saveAllClustersToFile(){
    for(auto it = this->clusters.begin(); it != this->clusters.end(); ++it){
        this->saveClusterToFile(it->first + ".txt", it->first);
    }
    return 0;
}

bool Clusterizador::checkIfClusterExists(string cluster){
    return this->clusters.find(cluster) != this->clusters.end();
}

vector<string> Clusterizador::getClusterNames(){
    vector<string> aux;
    for(auto it = this->clusters.begin(); it != this->clusters.end(); ++it)
        aux.push_back(it->first);
    return aux;
}

Color Clusterizador::convertHexToColor(int hexValue){
    Color color;
    color.r = ((hexValue >> 16) & 0xFF);  // Extract the RR byte
    color.g = ((hexValue >> 8) & 0xFF);   // Extract the GG byte
    color.b = ((hexValue) & 0xFF);        // Extract the BB byte

  return color; 
}
Color Clusterizador::convertHexToColor(string color){
    // https://stackoverflow.com/questions/1070497/c-convert-hex-string-to-signed-integer
    cout << stoi(color.c_str(), 0, 16) << " uai" << endl;
    return convertHexToColor(stoi(color.c_str(), 0, 16));
    // int x;
// 
    // std::stringstream ss;
    // ss << std::hex << color;
    // ss >> x;
    // cout << x << endl;
    // return convertHexToColor(x);
// 
}
