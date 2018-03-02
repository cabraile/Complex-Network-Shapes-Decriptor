#include "../include/adjacency_matrix.cpp"

void println(std::string msg)
{
  std::cout << msg << std::endl;
  return ;
}

int main(int argc, char * argv[])
{
  cnsd::AdjacencyMatrix<double> original_mat(4, cnsd::CON_UNDIRECTED);
  cnsd::AdjacencyMatrix<bool> bin_mat;
  std::vector<std::size_t> degrees_list;
  
  cnsd::debug::enable_log(true);

  println("=> Edges Connected");
  original_mat.setWeight(0, 1, 0.6);
  original_mat.setWeight(0,2, 0.7);
  original_mat.print();

  println("=> Binarized");
  original_mat.binarize(bin_mat, 0.1, 1.0);
  bin_mat.print();

  println("=> Degrees List");
  degrees_list = cnsd::nodes_degree(bin_mat);
  for(auto degree : degrees_list)
    std::cout << degree << " ";
  println("");

  return 0;
}
