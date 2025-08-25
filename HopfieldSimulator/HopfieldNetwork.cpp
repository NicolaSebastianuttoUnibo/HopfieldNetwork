

#include "HopfieldNetwork.hpp"

#include <algorithm>
#include <cmath>

template <typename T> 
static auto POINTS =   MD::getMathematicalVertex<T>();

//private function


    ///localField()
template <typename neurons_type, typename matrix_type>
const matrix_type HN::HopfieldNetwork<neurons_type, matrix_type>::localField(const int index, const std::vector<neurons_type>& input){

const int num_neurons = input.size();

    if (num_neurons != W_ij.rows() || num_neurons != W_ij.cols()) {
        throw std::invalid_argument("Dimensioni incompatibili tra pattern e matrice dei pesi.");
    }

  matrix_type h_k = matrix_type(0);

    for (int j = 0; j < num_neurons; ++j) {
        h_k += W_ij(index, j) * custom_cast<matrix_type>(input[j]);
    }



    return h_k;
}


///public function
//getTraining()
template <typename neurons_type, typename matrix_type> 
     const  std::vector<matrix_type> HN::HopfieldNetwork<neurons_type,matrix_type>::getTraining() {
std::vector<matrix_type> matrix(W_ij.size());
  std::copy(W_ij.data(), W_ij.data() + W_ij.size(), matrix.begin());
        return  matrix;

   
    }
//setTraining()

template <typename neurons_type, typename matrix_type> 
    void HN::HopfieldNetwork<neurons_type,matrix_type>::setTraining(std::vector<matrix_type>& matrix){

        
const int dim =std::sqrt(matrix.size());
    if(dim*dim!=matrix.size()){
       throw std::logic_error("Dimensione non compatibile");
    }
        W_ij.resize(dim, dim);
    W_ij = Eigen::Map<Eigen::Matrix<matrix_type, Eigen::Dynamic, Eigen::Dynamic>>(
        matrix.data(), dim, dim
    );


    }

//calculateEnergy()
template <typename neurons_type, typename matrix_type>
float HN::HopfieldNetwork<neurons_type, matrix_type>::calculateEnergy(const std::vector<neurons_type>& input) {
    const int num_neurons = input.size();

    if (num_neurons != W_ij.rows() || num_neurons != W_ij.cols()) {
        throw std::invalid_argument("Dimensioni incompatibili tra pattern e matrice dei pesi.");
    }

int N=input.size();
     Eigen::Matrix<matrix_type, Eigen::Dynamic, 1> s(N); 
        for (int k = 0; k < N; ++k) {
            s[k] = custom_cast<matrix_type>(input[k]); 
        }
        Eigen::Matrix<matrix_type, 1, 1> value = s.adjoint() * W_ij * s;
     return -0.5f*static_cast<float>(std::real(value[0]));;

}



//resolvePattern() const

template <typename neurons_type, typename matrix_type>
const std::vector<neurons_type> HN::HopfieldNetwork<neurons_type, matrix_type>::resolvePattern(const std::vector<neurons_type>& array) const{
    

   std::vector<neurons_type> getVector=array;
  const int num_neurons =getVector.size();


    if (num_neurons != W_ij.rows()) {
        throw std::invalid_argument("Dimensioni pattern/matrice non compatibili.");
    }

     for (int i = 0; i < num_neurons; ++i) {
        neurons_type best_point;
        matrix_type sum=0; 
     for (int j = 0; j < num_neurons; ++j) {
     sum+=(W_ij(i,j)*custom_cast<matrix_type>(getVector[j]));

}////for j


     double min_dist_sq = std::numeric_limits<double>::max();
     
    for (const auto& candidate_point : POINTS<neurons_type>) {
     double dist_sq = std::norm(sum - custom_cast<matrix_type>(candidate_point));


    if (dist_sq < min_dist_sq) {
        min_dist_sq = dist_sq;
        best_point = candidate_point;
    }

 }//for POINTS


  getVector[i]=best_point;

}///for i

return getVector;
    
}




    template <typename neurons_type, typename matrix_type>
void HN::HopfieldNetwork<neurons_type, matrix_type>::resolvePattern(std::vector<neurons_type>& getVector, std::vector<float>& getEnergy, std::atomic<float>* status){
      float count=0;
  
  const int num_neurons =getVector.size();


    if (num_neurons != W_ij.rows()) {
        throw std::invalid_argument("Dimensioni pattern/matrice non compatibili.");
    }

    getEnergy.clear();
    getEnergy.reserve(num_neurons+1);
count++;
     getEnergy.push_back(calculateEnergy(getVector));
     for (int i = 0; i < num_neurons; ++i) {
        neurons_type best_point;
        matrix_type sum=0; 
     for (int j = 0; j < num_neurons; ++j) {
     sum+=(W_ij(i,j)*custom_cast<matrix_type>(getVector[j]));

}


     double min_dist_sq = std::numeric_limits<double>::max();
     
    for (const auto& candidate_point : POINTS<neurons_type>) {
     double dist_sq = std::norm(sum - custom_cast<matrix_type>(candidate_point));


    if (dist_sq < min_dist_sq) {
        min_dist_sq = dist_sq;
        best_point = candidate_point;
    }

 }////for POINTS




auto local_field = localField(i, getVector);

auto diff = best_point - getVector[i];
auto conjugated_diff = std::conj(diff);

float delta = -0.5f * (std::real(conjugated_diff) * std::real(local_field) -
                       std::imag(conjugated_diff) * std::imag(local_field));

    getEnergy.push_back(delta+getEnergy.back());
  getVector[i]=best_point;

   if (status) {
        if (status->load(std::memory_order_acquire) < 0.0f) {
            return;
        }
            status->store(static_cast<float>(i + 1) / num_neurons, std::memory_order_relaxed);
        }

}////for i
}
