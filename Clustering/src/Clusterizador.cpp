#include "Clusterizador.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
Clusterizador::Clusterizador(){
    this->upDiff = this->loDiff = 40;
}
Clusterizador::Clusterizador(int lo, int up) : loDiff(lo), upDiff(up){
}
bool Clusterizador::createNewCluster(string name, Color color){

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
                aux = this->vec3bToInt(image.at<Vec3b>(it.pos().y, it.pos().x));
            
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
    if(this->clusters.find(cluster) == this->clusters.end()) {
           return -1;
    }
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

    
     
