#include "../include/cnsd.cpp"

void println(std::string msg)
{
  std::cout << msg << std::endl;
  return ;
}

int main(int argc, char * argv[])
{
  cnsd::AdjacencyMatrix<double> mat;
  mat.fromFile("files/test_mat.txt");
  cnsd::debug::enable_log(false);
  cnsd::Descriptor desc = cnsd::CNSD(mat, 3, 0.0, 1.0);
  for(double feature : desc)
    println(std::to_string(feature));
  return 0;
}
